#pragma once
#include <WinSock2.h>
#include <sstream>
#include <string>

//pomocnicze makro
#define BYTE unsigned char

//funkcja która siê przyda
std::string& change_char_for_std(std::string& x);

//numer weryfikacyjny
extern const __int32 VerificationNumber;


//----kto nadaje komunikat----
#define NET_MSG_COMPUTER	0x01
#define NET_MSG_USER		0x02
#define NET_MSG_SERWER		0x03

//---------typy paczek--------

//--komputer--
#define NET_COMTYPE_REG		0x01
#define NET_COMTYPE_UNREG	0x02

//---user---
#define NET_USERTYPE_GET_LIST_SCHOOL	0x01
#define NET_USERTYPE_LOGIN				0x02
#define NET_USERTYPE_LOGOUT				0x03
#define NET_USERTYPE_SETSTATE			0x04
#define NET_USERTYPE_START_KONKURS		0x05


//---------Typy b³êdów--------

enum NetErrType
{
	//--b³êdy spowodowane dzia³alnoœci¹ administratora/programu---///	
	 NET_SUCCESS
	,NET_ERROR_NOT_CONNECT
	,NET_ERROR_USER_NOT_CONNECT
	,NET_ERROR_NEED_MSG
	,NET_WARNING_NOT_NEED_MSG
	,NET_ERROR_BAD_ID
	,NET_ERROR_BAD_KEY
	,NET_ERROR_BAD_COMPUTER_NAME
	,NET_ERROR_COMP_ALREADY_LOGIN
	,NET_ERROR_COMP_ALREADY_LOGIN_USER
	,NET_ERROR_BAD_DESTINATION
	,NET_ERROR_BAD_TYPE_MSG
	,NET_ERROR_INCOMPLETE_PACK
	,NET_WARNING_TOOLARGE_PACK
	,NET_ERROR_UNEXPECTED
	,NET_ERROR_NO_ADDRESS_WRITTEN
	,NET_ERROR_CONNECT_SERWER
	,NET_ERROR_WRITE_ADDRESS


	//--b³êdy spowodowane dzia³alnoœci¹ u¿ytkownika programu
	,NET_ERROR_USER_ALREADY_LOGIN
	,NET_ERROR_USER_NOT_EXIST
	

	//--b³êdy spowodowane przez biblioteki/komputer/brak zasobów/itp...--//
	,NET_ERROR_CREATE_THREAD
	,NET_ERROR_CREATE_SOCKET
	,NET_ERROR_WSA_STARTUP
	,NET_ERROR_SELECT_FAIL
	,NET_ERROR_SELECT_UNEXPECTED_RETURN
	,NET_ERROR_RECV_OR_SEND_FUNCTION_FAIL
	,PLEASE_RESTART_PROGRAM
	,LOCAL_LOAD_LISTWORD
};




const char* ErrText(BYTE NumErr);


class NetPacket
{
public:
	unsigned __int32 id;
	unsigned __int64 key;
	unsigned __int64 time;
	BYTE nadawca
		,typ;
	unsigned __int32 err;
	unsigned __int32 size;
	BYTE* msg;

	//konstruktor zeruj¹cy strukturê
	NetPacket():	id(0),key(0),time(0),nadawca(0),typ(0),err(0),size(0),msg(0)
		{}


	~NetPacket()
	{
		if(msg)
		{
			delete[] msg;
			msg=0;
			size=0;
		}
	}
};

//-- -- Funkcja odpowiadaj¹ca za wysy³anie okreœlonej iloœci bajtów
int SendValue(SOCKET sd,char* value, unsigned __int32 size);
int ReceiveValue(SOCKET sd,char*  value,unsigned __int32 size);

int SendPacket(SOCKET sd, NetPacket& to_send);

int RecvPacket(SOCKET sd, NetPacket& dest);

__int64 GenerateKey(__int64 HASH, __int64 TIME);

NetPacket* CreatePacket(
	__int32 id,
	__int64 hash,
	BYTE nadawca,
	BYTE typ,
	__int32 err,
	__int32 size,
	BYTE* msg
	);

