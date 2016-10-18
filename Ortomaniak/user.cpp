#include "user.h"


User::User(string _imie,string _nazwisko)
{
	status=0;
	uID=0;
	hash=0;
	sID=-1;
	imie=_imie;
	nazwisko=_nazwisko;
	pack_send=0;
	pack_recv=0;
	SendGetListSchoolFromSerwer();
}


User::~User(void)
{
	for(unsigned __int32 i=0;i<lista_szkol.size();i++)
	{
		if(lista_szkol[i])
			delete lista_szkol[i];
	}
	lista_szkol.clear();

	if(pack_send)delete pack_send;
	if(pack_recv)delete pack_recv;
}


int User::SendGetListSchoolFromSerwer()
{
	if(GetLoginSuccess())
		return -1;
	status&=~list_school;
	__int32 size_msg=4+imie.size()+4+nazwisko.size();

	//wiadomoœæ do serwera
	BYTE* const msg=new BYTE[size_msg];		//wskaŸnik na pocz¹tek
	BYTE* rac_msg=msg;					//random access message
	
	//skopiowanie imienia
	//rozmiar
	DWORD size=imie.size();
	memcpy(rac_msg,&size,sizeof(size));
	rac_msg+=sizeof(size);
	//text
	memcpy(rac_msg,imie.data(),size);
	rac_msg+=size;
	//skopiowanie nazwiska
	//rozmiar
	size=nazwisko.size();
	memcpy(rac_msg,&size,sizeof(size));
	rac_msg+=sizeof(size);
	//text
	memcpy(rac_msg,nazwisko.data(),size);
	//rac_msg+=size;
	
	while(pack_send)::Sleep(10);
	pack_send=CreatePacket(uID,hash,NET_MSG_USER,NET_USERTYPE_GET_LIST_SCHOOL,0,size_msg,msg);
	return pack_send==0;
}

int User::SendLogin()
{
	if(GetLoginSuccess())return -1;
	status&=~logged;

	__int32 size_msg=4+imie.size()+4+nazwisko.size()+4;
	//wiadomoœæ do serwera
	BYTE* const msg=new BYTE[size_msg];		//wskaŸnik na pocz¹tek
	BYTE* rac=msg;							//random access msg
	
	//****  kopiowanie imienia ****//
	//rozmiar
	size_t rozmiar=imie.size();
	memcpy(rac,&rozmiar,sizeof(rozmiar));
	rac+=sizeof(rozmiar);
	//text
	memcpy(rac,imie.data(),rozmiar);
	rac+=rozmiar;

	//**** kopiowanie nazwiska ****//
	//rozmiar
	rozmiar=nazwisko.size();
	memcpy(rac,&rozmiar,sizeof(rozmiar));
	rac+=sizeof(rozmiar);
	//text
	memcpy(rac,nazwisko.data(),rozmiar);
	rac+=rozmiar;

	//**** kopiowanie sID ****//
	memcpy(rac,&lista_szkol[sID]->sID,sizeof(lista_szkol[sID]->sID));

	while(pack_send)::Sleep(10);
	pack_send=CreatePacket(uID,hash,NET_MSG_USER,NET_USERTYPE_LOGIN,0,size_msg,msg);
	return pack_send==0;
}


int User::SendWaitToStartKonkurs()
{
	if(!GetLoginSuccess())return -1;
	while(pack_send)::Sleep(10);
	pack_send=CreatePacket(uID,hash,NET_MSG_USER,NET_USERTYPE_START_KONKURS,0,0,0);
	return pack_send==0;
}


int User::SendStats()
{
	if(!GetLoginSuccess())return -1;
	status&=~set_stats;
	__int32 size_msg=6;

	//wiadomoœæ do serwera
	BYTE* const msg=new BYTE[size_msg];		//wskaŸnik na pocz¹tek
	memcpy(msg,stats,size_msg);
	
	while(pack_send)::Sleep(10);
	pack_send=CreatePacket(uID,hash,NET_MSG_USER,NET_USERTYPE_SETSTATE,0,size_msg,msg);
	return pack_send==0;
}

int User::SendLogout()
{
	if(!GetLoginSuccess())return -1;
	status&=~exit_game;
	while(pack_send)::Sleep(10);
	pack_send=CreatePacket(uID,hash,NET_MSG_USER,NET_USERTYPE_LOGOUT,0,0,0);
	return pack_send==0;
}

int User::Interpret()
{
	if(!pack_recv) return 0;
	NetPacket* pack=pack_recv;
	pack_recv=NULL;
	if(pack->err)
	{
		ShowErr(pack->err,"SER->CLI::User::Interpret()");
	}
	else switch(pack->typ)
	{

		////************* POBIERANIE LISTY SZKO£ DOSTÊPNYCH DLA DANEGO USERA *************////
	case NET_USERTYPE_GET_LIST_SCHOOL:
		{
			if(!pack->size)
			{
				ShowErr(NET_ERROR_NEED_MSG,"User::Interpret()");
				return -1;
			}
			unsigned __int32 ilosc=0,read=0,max_read=pack->size;
			BYTE* rac=pack->msg;//random acces message
		
			if(max_read<sizeof(ilosc))
			{
				ShowErr(NET_ERROR_INCOMPLETE_PACK,"User::Interpret()");
				return -1;
			}
			memcpy(&ilosc,rac,sizeof(ilosc));
			rac+=read=sizeof(ilosc);
			if(!ilosc)
			{
				ShowErr(NET_ERROR_UNEXPECTED,(string)"User::Interpret()"+__FILE__,0,__LINE__);
				return -1;
			}

			School* szkola;
			for(unsigned __int32 i=0,size_name;i<ilosc;i++)
			{
				///// ********  dodawanie nowej szkoly ******* /////
				szkola=new School;
				//// -----  odczyt rozmiaru nazwy ----- ////
				if(max_read<(read+=sizeof(size_name)))
				{
					ShowErr(NET_ERROR_INCOMPLETE_PACK,"User::Interpret()");
					delete szkola;
					return -1;
				}
				memcpy(&size_name,rac,sizeof(size_name));
				rac+=sizeof(size_name);

				//// ----- odczyt nazwy ----- ////
				if(!size_name)		//je¿eli nazwy nie ma - error
				{
					ShowErr(NET_ERROR_UNEXPECTED,(string)"User::Interpret()"+__FILE__,0,__LINE__);
					delete szkola;
					return -1;
				}

				if(max_read<(read+=size_name))		//je¿eli za krótka paczka - error
				{
					ShowErr(NET_ERROR_INCOMPLETE_PACK,"User::Interpret()");
					delete szkola;
					return -1;
				}
			
				szkola->name.resize(size_name);				//rezerwacja
				memcpy(&(szkola->name[0]),rac,size_name);	//kopiowanie
				rac+=size_name;

				//// ----  ustawianie sID(school id) szkole ---- ////
				if(max_read<(read+=sizeof(szkola->sID)))		//ehh.. za ma³a paczka?
				{
					ShowErr(NET_ERROR_INCOMPLETE_PACK,"User::Interpret()");
					delete szkola;
					return -1;
				}
				memcpy(&szkola->sID,rac,sizeof(szkola->sID));
				rac+=sizeof(szkola->sID);
			
				//jeœli wszystko ok
				lista_szkol.push_back(szkola);
			}
			if(read!=max_read)
				ShowErr(NET_WARNING_TOOLARGE_PACK,"User::Interpret()");
			status|=list_school;
			break;
		}


		////*********  LOGOWANIE UDANE - POBIERANIE ID, HASH-u I WYNIKÓW  **********////
	case NET_USERTYPE_LOGIN:
		{
			if(!pack->size)
			{
				ShowErr(NET_ERROR_NEED_MSG,"User::Interpret()");
				return -1;
			}
			unsigned __int32 ilosc=0,read=0,max_read=pack->size;
			BYTE* rac=pack->msg;//random access message
			if(pack->size<(sizeof(uID)+sizeof(hash)+6))
			{
				ShowErr(NET_ERROR_INCOMPLETE_PACK,"User::Interpret()");
				return -1;
			}
			if(pack->size>(sizeof(uID)+sizeof(hash)+6))
				ShowErr(NET_WARNING_TOOLARGE_PACK,"User::Interpret()");
			//-- get id --//
			memcpy(&uID,rac,sizeof(uID));
			rac+=sizeof(uID);
			//-- get hash --//
			memcpy(&hash,rac,sizeof(hash));
			rac+=sizeof(hash);
			//-- get stats --//
			memcpy(stats,rac,6);
			status|=logged;
			break;
		}
		

	////**********  ROZPOCZÊCIE KONKURSU (GRY)  **********////
	case NET_USERTYPE_START_KONKURS:
		{
			if(!pack->size)
			{
				ShowErr(NET_ERROR_NEED_MSG,"User::Interpret()");
				return -1;
			}
			if(pack->size != sizeof(config))
			{
				ShowErr(NET_ERROR_INCOMPLETE_PACK,"User::Interpret()");
			}

			memcpy(&config, pack->msg, sizeof(config));

			status|=start_konkursu;
			break;
		}
	////**********  POMYŒLNE USTAWIENIE LICZBY PUNKTÓW **********////
	case NET_USERTYPE_SETSTATE:
		{
			if(pack->size)
			{
				ShowErr(NET_WARNING_NOT_NEED_MSG,"User::Interpret()",0,__LINE__);
				return -1;
			}
			status|=set_stats;
			break;
		}

	////************ ZAKOÑCZENIE GRY ***********////
	case NET_USERTYPE_LOGOUT:
		{
			if(pack->size)
			{
				ShowErr(NET_WARNING_NOT_NEED_MSG,"User::Interpret()",0,__LINE__);
				return -1;
			}
			status|=exit_game;
			break;
		}
	////********** NIEOKREŒLONA KOMENDA - ERROR **********////
	default:
		ShowErr(NET_ERROR_BAD_TYPE_MSG,"SerwerConnect::Interpret()");
	}
	int err=pack->err;
	delete pack;
	return err;
}

