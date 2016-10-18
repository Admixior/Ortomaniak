#pragma once

#include "NetConf.h"
#include <vector>

using namespace std;

//funkcja pokazuj¹ca b³êdy
int ShowErr(int numErr,std::string who,int exnum=0,int linenum=0);

class User
{
	struct School
	{
		string name;
		__int32 sID;
	};
	// ------ LOCAL -------
	string imie,
		nazwisko;

	vector<School*> lista_szkol;
	BYTE stats[3][2];

	struct 
	{
		unsigned short activetype_words;
		unsigned int max_time_for_word;
		unsigned int max_word;
	}config;
	// ------ NETWORK ------
	__int32 uID;	//identyfikacja u¿ytkownika (osoby fizycznej)
	__int64 hash;	//tajny kod s³u¿¹cy do sprawdzania autentycznoœci danych
	__int32 sID;	//numer aktualnie wybranej szko³y

	__int8 status;
	enum list_status
	{
		list_school=0x01,
		logged=0x02,
		start_konkursu=0x04,
		set_stats=0x08,
		exit_game=0x10,
		error=0x20
	};


	NetPacket* pack_send;
	NetPacket* pack_recv;

	friend int Sender(void* comp_void);
public:

	User(string _imie,string _nazwisko);
	~User(void);

	//wybór szko³y
	string GetNextSchoolName()
	{
		if((++sID)>=lista_szkol.size())sID=0;
		return lista_szkol[sID]->name;
	}
	string GetPrevSchoolName()
	{
		if((--sID)<0)sID=lista_szkol.size()-1;
		return lista_szkol[sID]->name;
	}
	int GetCountSchool(){return lista_szkol.size();}

	//pobieranie ustawieñ gry
	unsigned short	GetConfActiveTypes(){return config.activetype_words; }
	unsigned int	GetConfMaxTime(){return config.max_time_for_word; }
	unsigned int	GetConfMaxWord(){return config.max_word; }

	//wyniki
	BYTE* GetStats() {  return &stats[0][0];  }

	//funkcje do serwera
	int Interpret();

	// wysy³anie
	int SendGetListSchoolFromSerwer();
	int SendLogin();
	int SendWaitToStartKonkurs();
	int SendStats();
	int SendLogout();

	// sprawdzanie statusu - true jeœli prawdziwe
	int GetListSchoolSuccess(){return status&list_school;}
	int GetLoginSuccess(){return status&logged;}
	int GetStartKonkursSuccess(){return status&start_konkursu;}
	int GetStatsUploadSuccess(){return status&set_stats;}
	int GetLogoutSuccess(){return status&exit_game;}
};