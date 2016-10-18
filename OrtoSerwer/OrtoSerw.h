#pragma once
#include <SFML\Window.hpp>
#include <SFGUI\SFGUI.hpp>
#include <WinSock2.h>
#include <queue>

#include "Structures.h"
#include "..\Ortomaniak\NetConf.h"

#define BYTE unsigned char

void ShowError(std::string message, unsigned int errocode=0);

template< class abc >unsigned int FindNullPointer( std::vector<abc*>& obj)
{
	for(unsigned int i=0; i<obj.size() ;++i)
	{
		if( obj[i]==0 )return i;
	}
	return -1;
}

struct EventMem
{
	unsigned __int32 type;
	void* data;
};


std::wstring stringToWstring(const std::string& txt);
std::string wstringToString(const std::wstring& txt);

class User;

struct ComputerConn
{
	SOCKET sd;
	unsigned __int32 uID;
	unsigned __int64 hash;
	std::string computer_name,
		ip_computer,
		port_computer;
	NetPacket* in_packet,* out_packet;
	
	enum
	{
		computer_login= 1,
		wait_to_start = 2
	};
	int flag;
};

int Connect(void*);

extern bool serwer_on;
extern std::vector<ComputerConn*> computer_connection;


//////  EVENTS ARE HERE  //////
extern std::queue< EventMem* > event;

static const wchar_t* computer_status_text[]=
{
	L"N/D",
	L"Zalogowany",
	L"Oczekuje na start",
	L"Trwa gra",
	L"Zakoñczono"
};

enum computerstatus
{
	N_D,
	login,
	wait_for_start,
	play_game,
	finish_part_of_game
};

#define TEXT_TITLE_RESULTS_MANAGER	L"Wyniki ze wszystkich szkó³:"
#define TEXT_TITLE_USER_MANAGER		L"Lista uczniów:"
#define TEXT_TITLE_SCHOOL_MANAGER	L"Lista szkó³:"
#define TEXT_TITLE_COMPUTER_MANAGER	L"Lista sktywnych komputerów:"

#define STANDARD_BUTTON_SIZE		 sf::Vector2f(30.f,0.f)
//Kolejka zdarzeñ ulatwia zrozumienie kodu, sprawia ¿e jest prostszy i ³atwiej dodawaæ nowe elementy
///////////***********///////////***********////////***********
//////////------------    E V E N T S -------------////////////
// GUI EVENT
#define EVENT_CREATE_USER				0x01	//LongInfoUser: sID, uID, imie, nazwisko, wyniki
#define EVENT_CHANGE_USER_NAME			0x02	//LongInfoUser: sID, uID, imie, nazwisko
#define EVENT_CHANGE_STATS				0x03	//LongInfoUser: sID, uID, wyniki
#define EVENT_DELETE_USER				0x04	//LongInfoUser: sID, uID

#define EVENT_CREATE_SCHOOL				0x05	//ShortSchoolInfo: sID, name
#define EVENT_CHANGE_SCHOOL_NAME		0x06	//ShortSchoolInfo: sID, name
#define EVENT_DELETE_SCHOOL				0x07	//ShortSchoolInfo: sID

#define EVENT_USER_LOGIN				0x08	//ShortUserPublicInfo: sID, uID, hasLogged

#define EVENT_COMPUTER_USER_LOGIN		0x09	//ID_Name: msg, cID
#define EVENT_COMPUTER_CHANGE_STATUS	0x0A	//ChangeCompStatus: cID, statusID
#define EVENT_COMPUTER_LOGIN			0x0B	//ID_Name: msg, cID
#define EVENT_COMPUTER_LOGOUT			0x0C	//*int ComputerID

#define EVENT_GUI_EVENT_END				0x10
// DATABASE EVENT
#define EVENT_CREATE_USER_DB			0x11	//NewUserDB
#define EVENT_CHANGE_USER_DB			0x12	//EditUserDB
#define EVENT_DELETE_USER_DB			0x13	//*int uID

#define EVENT_CREATE_SCHOOL_DB			0x14	//NewSchoolDB
#define EVENT_CHANGE_SCHOOL_NAME_DB		0x15	//EditSchoolDB
#define EVENT_DELETE_SCHOOL_DB			0x16	//*int sID

//#define EVENT_USER_LOGIN_DB				0x17	//it doesn't make sense

// SYSTEM EVENT
#define EVENT_DESTROY_WINDOW			0x0f1	//*int id_window
#define EVENT_SHOW_ADDSCHOOLWINDOW		0x0f2	//nothing(null)
#define EVENT_SHOW_ADDUSERWINDOW		0x0f3	//nothing(null)
#define EVENT_SHOW_EDITSCHOOLWINDOW		0x0f4	//*int sID
#define EVENT_SHOW_EDITUSERWINDOW		0x0f5	//*int uID
#define EVENT_SHOW_MESSAGEBOX			0x0f6	//MessageBoxStruct

// NET-WORK TASK
#define EVENT_DISCONNECT_COMPUTER		0x0fa	//*int ComputerID
#define EVENT_LOGOUT_USER				0x0fb	//*int ComputerID
#define EVENT_START_GAME				0x0fc	//null (all active users accept automatic)

// ONE NETWORK EVENT
#define EVENT_NEWCOMPUTER_CONN			0x0fd	//*int ComputerID
#define EVENT_NEWCOMPUTER_DISCONN		0x0fe	//*int ComputerID (danger: object in computer_onnection doesn't exist)
#define EVENT_MESSAGECOME				0x0ff	//*int ComputerID
