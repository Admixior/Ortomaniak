#pragma once
#include "user.h"

using namespace std;

//bezpieczeñstwo
bool CheckSerwerAuth(NetPacket* packet,__int64 hash);

//thread
int Sender(void* comp_void);

class SerwerConnect
{
	User* user;
	sockaddr_in addres;
	bool connected;
	HANDLE thread;
	//do rejestracji komputera
	__int32 cID;	//identyfikacja komputera (ustalana po wys³aniu nazwy komputera na serwer)
	__int64 hash;	//suma z której jest generowana kontrolna liczba
	string comp_name;	//nazwa komputera
	NetPacket* pack_send;
	NetPacket* pack_recv;
	bool active_thread; //informacja dla threada czy ma kontynuowaæ prace
	string edition_text;
	friend int Sender(void* comp_void);
	
	int StartThread();
	int StopThread();
public:
	SerwerConnect(void);
	~SerwerConnect(){ StopThread(); }
	int SetConnOption(__int32 _ip,__int16 _port);
	bool GetConnected(){return connected;}
	int LoginComputer(string computer);
	int Logout();
	int Interpret();
	int SetUser(User* nowy){user=nowy; return 0;}
	User* GetUser(){return user;}
	std::string& GetEditionText(){return edition_text;}
};

