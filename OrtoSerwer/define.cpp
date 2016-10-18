#include <iostream>
#include <fstream>
#include "DataBase.h"
#include "OrtoSerw.h"
#include "..\Ortomaniak\NetConf.h"
//////  This function is **** but necessary
std::string& change_char_for_std(std::string& x)
{
	if(!x.size())return x;
	unsigned int size=x.size(), act=0;
	while(act<size)
	{
		if(x[act]==' ')x[act]='\xA8';
		else if(x[act]=='Í')x[act]='\xA9';
		else if(x[act]=='”')x[act]='\xE0';
		else if(x[act]=='Û')x[act]='\xA2';
		else if(x[act]=='•')x[act]='\xA4';
		else if(x[act]=='π')x[act]='\xA5';
		else if(x[act]=='å')x[act]='\x97';
		else if(x[act]=='ú')x[act]='\x98';
		else if(x[act]=='£')x[act]='\x9D';
		else if(x[act]=='≥')x[act]='\x88';
		else if(x[act]=='Ø')x[act]='\xBD';
		else if(x[act]=='ø')x[act]='\xBE';
		else if(x[act]=='è')x[act]='\x8D';
		else if(x[act]=='ü')x[act]='\xAB';
		else if(x[act]=='∆')x[act]='\x8F';
		else if(x[act]=='Ê')x[act]='\x86';
		else if(x[act]=='—')x[act]='\xE3';
		else if(x[act]=='Ò')x[act]='\xE4';
		act++;
	}
	return x;
}

bool window_is_open=false;


void ShowError(std::string msg, unsigned int errorcode)
{
	if(window_is_open)
	{
		EventMem* em = new EventMem;
		em->type=EVENT_SHOW_MESSAGEBOX;
		em->data= new MessageBoxStruct;
		reinterpret_cast<MessageBoxStruct*>(em->data)->flag=MESSAGEBOX_OK;
		reinterpret_cast<MessageBoxStruct*>(em->data)->callback=0;
		if(errorcode) msg+="\nErrorCode: "+std::to_string((long long)errorcode);
		reinterpret_cast<MessageBoxStruct*>(em->data)->msg=stringToWstring(msg);
		event.push(em);
	}
	else
	{
		std::string blad="\n\nB≥πd: ";
		std::cout<<change_char_for_std(blad)<<change_char_for_std(msg);
	}
}

std::queue< EventMem* > event;
//////*************************************\\\\\\\
//|||             N E T W O R K             ||||\\
//////////////////////////////////////////////////

std::vector<ComputerConn*> computer_connection;		//wszystkie poloczenia
std::string ip_serwer,
	name_serwer,
	port;		//s≥uøy do wyúwietlania w GUI

bool serwer_on=false;

int Connect(void*)
{
	WSADATA wsaData;
	SOCKET serwer_sd, client_sd;
	if(WSAStartup( 0x0202, & wsaData )!=NO_ERROR)
	{
		ShowError("Nie mozna zainicjowac biblioteki",WSAGetLastError());
		return 1;
	}
	if(wsaData.wVersion!=0x0202)
	{
		ShowError("Wersja zainicjowanej biblioteki Winsock2 jest inna niz 2.2.\nMoze to spowodowac bledy");
	}
	///////----- POBIERANIE INFORMACJI DLA GUI -------
	{
		char tab[MAX_COMPUTERNAME_LENGTH +1];
		DWORD size= MAX_COMPUTERNAME_LENGTH +1;
		if(GetComputerNameA(tab,&size)==0)
		{
			ShowError("Nie mozna pobrac nazwy uzytkownika.");
			return 1;
		}
		tab[size]=0;

		HOSTENT* host= gethostbyname(tab);
		if(host==0)return 0;
		name_serwer=host->h_name;
		ip_serwer=inet_ntoa(static_cast<in_addr>(*(in_addr*)(void*)(host->h_addr_list[0])));
		if(ip_serwer=="127.0.0.1" || ip_serwer.size()==0)
		{
			ip_serwer+="(sprawdü po≥πczenie z routerem)";
		}
		port="6000";
	}

	serwer_on=true;
	////-----  TWORZENIE POLOCZENIA Z SERWEREM  ---------
	
	if((serwer_sd = socket( AF_INET, SOCK_STREAM, 0 ))==INVALID_SOCKET )
	{
		ShowError("Nie mozna utworzyc gniazda", WSAGetLastError());
		return 1;
	}

	sockaddr_in adres_serwer = {AF_INET, htons(6000), INADDR_ANY},
		adres_klienta;

	if( bind( serwer_sd, (sockaddr*)&adres_serwer, sizeof(adres_serwer)) )	ShowError("bind():");
	listen(serwer_sd, SOMAXCONN);
	
	//****************************************************//
	//// ----   REALIZACJA POLOCZENIA SIECIOWEGO  ----- ////
	fd_set grupa_read;
	timeval timeout={0,0};
	while(serwer_on)
	{
		Sleep(100);
		{
			FD_ZERO(&grupa_read);
			FD_SET(serwer_sd,&grupa_read);
			int i=select(0,&grupa_read,0,0,&timeout);
			if(i==SOCKET_ERROR) throw NET_ERROR_SELECT_FAIL;
			//  nowe po≥πczenie jest dostÍpne
			else if(i==1)
			{
				//odbieranie po≥πczzenia
				int iAddrSize = sizeof(adres_klienta);
				client_sd = accept(serwer_sd, (sockaddr *)&adres_klienta, &iAddrSize);       

				if(client_sd==INVALID_SOCKET)
				{
					ShowError("accept():");return 0;
				}

				//sprawdzanie czy to jest program do dobrego uzytku
				int ver;
				recv(client_sd,(char*)&ver, sizeof(ver), 0);//future: naleøy zmieniÊ z limitem cna funkcje ReadValue z limitem czasu
				if(ver!= 0x4D7272B5)
				{
					closesocket(client_sd);
					return 0;
				}

				ComputerConn* comp=new ComputerConn;
				comp->ip_computer=inet_ntoa(adres_klienta.sin_addr);
				comp->port_computer=std::to_string((long long)ntohs(adres_klienta.sin_port));
				comp->uID=-1;
				comp->computer_name="";
				comp->sd=client_sd;
				comp->out_packet=0;
				comp->in_packet=0;
				comp->flag=0;

				int x=FindNullPointer(computer_connection);
				if(x==-1)
				{
					x=computer_connection.size();
					computer_connection.push_back(comp);
				}
				else
				{
					computer_connection[x]=comp;
				}

				EventMem* em=new EventMem;
				em->type=EVENT_NEWCOMPUTER_CONN;
				em->data= new int(x);
				event.push(em);
			}
		}



		//////////////////////////////////////////////////
		//  ----  -- -  Po≥πczenia klienckie  --- - - - //
		for(unsigned int i= 0; i<computer_connection.size(); ++i)
		{
			if(!computer_connection[i]) continue;	//jeúli po≥πczenie nie istnieje to pomiÒ

			//  -- - -- wysy≥anie danych jeúli sπ ---- - //
			if( computer_connection[i]->out_packet )
			{
				//let's send
				try
				{
					SendPacket(computer_connection[i]->sd, *computer_connection[i]->out_packet);
					delete computer_connection[i]->out_packet;
					computer_connection[i]->out_packet=0;
				}
				catch(...)
				{
					//delete packet before delete computer
					if(!computer_connection[i]->out_packet)
						delete computer_connection[i]->out_packet;
					
					if(!computer_connection[i]->in_packet)
						delete computer_connection[i]->in_packet;

					if( computer_connection[i]->flag & ComputerConn::computer_login)//generowanie fa≥szywego komunikatu
					{
						ShowError("Komputer nr. "+std::to_string((long long)i)+" zerwa≥ po≥πczenie");
						computer_connection[i]->in_packet=new NetPacket;
						computer_connection[i]->in_packet->err=0;
						computer_connection[i]->in_packet->id;
						computer_connection[i]->in_packet->msg=0;
						computer_connection[i]->in_packet->nadawca=NET_MSG_COMPUTER;
						computer_connection[i]->in_packet->typ= NET_COMTYPE_UNREG;
						computer_connection[i]->in_packet->size=0;
						// computer was disconnect
						EventMem* em=new EventMem;
						em->type=EVENT_MESSAGECOME;
						em->data= new int(i);
						event.push(em);

						while(computer_connection[i]->in_packet)Sleep(1);
						while(computer_connection[i]->flag & ComputerConn::computer_login)Sleep(1);
					}
				
					if(!computer_connection[i]->out_packet)
						delete computer_connection[i]->out_packet;

					// computer was disconnect
					EventMem* em=new EventMem;
					em->type=EVENT_NEWCOMPUTER_DISCONN;
					em->data= new int(i);
					event.push(em);

					delete computer_connection[i];
					computer_connection[i]=0;
				}
			}

			////  - --  odbieranie danych - - - -- ////
			FD_ZERO(&grupa_read);
			FD_SET(computer_connection[i]->sd,	&grupa_read);
			int x=select(0,&grupa_read,0,0,&timeout);
			if(x == SOCKET_ERROR )	throw NET_ERROR_SELECT_FAIL;//what the error
			if(x == 0) continue; //nothing
			
			//let's receive
			try
			{
				NetPacket *pack=new NetPacket;
				RecvPacket(computer_connection[i]->sd, *pack);
				computer_connection[i]->in_packet=pack;
				
				//put informtion to event queue about come message
				EventMem* em=new EventMem;
				em->type=EVENT_MESSAGECOME;
				em->data= new int(i);
				event.push(em);
			}
			catch(...)
			{
				if(!computer_connection[i]->out_packet)
					delete computer_connection[i]->out_packet;
				
				if(!computer_connection[i]->in_packet)
					delete computer_connection[i]->in_packet;
				
				if( computer_connection[i]->flag & ComputerConn::computer_login)//generowanie fa≥szywego komunikatu
				{
					ShowError("Komputer nr. "+std::to_string((long long)i)+" zerwa≥ po≥πczenie");
					computer_connection[i]->in_packet=new NetPacket;
					computer_connection[i]->in_packet->err=0;
					computer_connection[i]->in_packet->id;
					computer_connection[i]->in_packet->msg=0;
					computer_connection[i]->in_packet->nadawca=NET_MSG_COMPUTER;
					computer_connection[i]->in_packet->typ= NET_COMTYPE_UNREG;
					computer_connection[i]->in_packet->size=0;
					
					EventMem* em=new EventMem;
					em->type=EVENT_MESSAGECOME;
					em->data= new int(i);
					event.push(em);

					while(computer_connection[i]->in_packet)Sleep(1);
					while(computer_connection[i]->flag & ComputerConn::computer_login)Sleep(1);
				}
				
				if(!computer_connection[i]->out_packet)
					delete computer_connection[i]->out_packet;

				// computer was disconnect
				EventMem* em=new EventMem;
				em->type=EVENT_NEWCOMPUTER_DISCONN;
				em->data= new int(i);
				event.push(em);

				delete computer_connection[i];
				computer_connection[i]=0;
			}

		}// checking all computer connection to send and receive msg

	}//main loop

	for(unsigned int i=0; computer_connection.size()> i;  ++i)
	{
		if(computer_connection[i])
		{
			if(!computer_connection[i]->out_packet)
				delete computer_connection[i]->out_packet;
				
			if(!computer_connection[i]->in_packet)
				delete computer_connection[i]->in_packet;

			closesocket(computer_connection[i]->sd);
			delete computer_connection[i];
			computer_connection[i]=0;
		}
	}
	closesocket(client_sd);
	closesocket(serwer_sd);
	WSACleanup();
	return 0;
}


char ANSItoASCII(wchar_t c)
{
	switch(c)
	{
	case L' ':return ' ';
	case L'Í':return 'Í';
	case L'”':return '”';
	case L'Û':return 'Û';
	case L'•':return '•';
	case L'π':return 'π';
	case L'å':return 'å';
	case L'ú':return 'ú';
	case L'£':return '£';
	case L'≥':return '≥';
	case L'Ø':return 'Ø';
	case L'ø':return 'ø';
	case L'è':return 'è';
	case L'ü':return 'ü';
	case L'∆':return '∆';
	case L'Ê':return 'Ê';
	case L'—':return '—';
	case L'Ò':return 'Ò';
	default:return c;
	}
}

wchar_t ASCIItoANSI(char c)
{
	switch(c)
	{
	case ' ':return L' ';
	case 'Í':return L'Í';
	case '”':return L'”';
	case 'Û':return L'Û';
	case '•':return L'•';
	case 'π':return L'π';
	case 'å':return L'å';
	case 'ú':return L'ú';
	case '£':return L'£';
	case '≥':return L'≥';
	case 'Ø':return L'Ø';
	case 'ø':return L'ø';
	case 'è':return L'è';
	case 'ü':return L'ü';
	case '∆':return L'∆';
	case 'Ê':return L'Ê';
	case '—':return L'—';
	case 'Ò':return L'Ò';
	default:return c;
	}
}

std::wstring stringToWstring(const std::string& txt)
{
	std::wstring ret;
	for(unsigned int i=0; i<txt.size(); ++i)
	{
		ret+=ASCIItoANSI(txt[i]);
	}
	return ret;
}

std::string wstringToString(const std::wstring& txt)
{
	std::string ret;
	for(unsigned int i=0; i<txt.size(); ++i)
	{
		ret+=ANSItoASCII(txt[i]);
	}
	return ret;
}

