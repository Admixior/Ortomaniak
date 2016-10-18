#include <WinSock2.h>
#include "SerwerConnect.h"


//thread do komunikacji z serwerem - definicja
int Sender(void* comp_void)
{
	SerwerConnect* comp=reinterpret_cast<SerwerConnect*>(comp_void);
	SOCKET sd=SOCKET_ERROR;
	WSADATA wsd;
	memset(&wsd,0,sizeof(wsd));
	NetPacket* pack=NULL;
	timeval timewait={0,250000};

	try
	{
		WSASetLastError(0);
		if(WSAStartup(MAKEWORD(2,2),&wsd))throw NET_ERROR_WSA_STARTUP;
		if(sd!=SOCKET_ERROR)closesocket(sd);
		if(comp->addres.sin_family!=AF_INET)throw NET_ERROR_NO_ADDRESS_WRITTEN;
		sd=socket(AF_INET,SOCK_STREAM,0);
		if(sd==SOCKET_ERROR)throw NET_ERROR_CREATE_SOCKET;
		if(connect(sd,(sockaddr*)&comp->addres,sizeof(comp->addres)))
			throw NET_ERROR_CONNECT_SERWER;

		//wysy³anie sta³ej liczby potwierdzaj¹cej prawid³owe po³aczenie
		SendValue(sd,(char*)&VerificationNumber,sizeof(VerificationNumber));
		
		while(comp->active_thread)
		{
			Sleep(10);
			///// --- wysy³anie danych ----- /////
			if(comp->GetUser())
				if(pack=comp->GetUser()->pack_send)
				{
					comp->GetUser()->pack_send=NULL;
					SendPacket(sd,*pack);
					delete pack;
					pack=NULL;
				}
			if(pack=comp->pack_send)
			{
				comp->pack_send=NULL;
				SendPacket(sd,*pack);
				delete pack;
				pack=NULL;
			}

			///// --- odbieranie danych ----- /////
			fd_set grupa_read;
			FD_ZERO(&grupa_read);
			FD_SET(sd,&grupa_read);
			switch(select(0,&grupa_read,0,0,&timewait))
			{
			case 0:					continue;
			case SOCKET_ERROR:		throw NET_ERROR_SELECT_FAIL;
			case 1:					break;
			default:				throw NET_ERROR_SELECT_UNEXPECTED_RETURN;
			}
			pack=new NetPacket;
			RecvPacket(sd,*pack);
			switch(pack->nadawca)
			{

			case NET_MSG_COMPUTER:
				while(comp->pack_recv)Sleep(1);
					comp->pack_recv=pack;
				break;

			case NET_MSG_USER:
				while(comp->GetUser()->pack_recv)Sleep(1);
				comp->GetUser()->pack_recv=pack;
				break;

			default:
				throw NET_ERROR_BAD_DESTINATION;
			}
		}
	}
	catch(NetErrType err)
	{
		if(pack)
		{
			delete pack;
			pack=NULL;
		}
		ShowErr(err,"Sender()",WSAGetLastError());
		ShowErr(PLEASE_RESTART_PROGRAM,"Sender()");
	}
	closesocket(sd);
	WSACleanup();
	comp->connected=false;
	return 0;
}

