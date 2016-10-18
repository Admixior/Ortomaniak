#pragma once
#include <string>
#define BYTE unsigned char


struct NewSchoolDB	//use in new school
{
	std::string name_school;	//name_school with enter
};

struct NewUserDB
{
	std::string imie,
		nazwisko;
	unsigned __int32 sID;
};

struct EditUserDB
{
	std::string imie,
		nazwisko;
	unsigned __int32 sID,
		uID;
	BYTE stats[3];		
};

struct ShortUserInfo		//don't touch
{
	unsigned int id_school,	//sID
		id_user;			//local_id
	bool login;				//now is connect
};

struct ShortUserPublicInfo
{
	unsigned int sID,	//sID
		uID;			//uID
	bool login;				//now is connect
};


struct ID_Name
{
	std::string name;
	unsigned __int32 id;
};

struct ChangeCompStatus
{
	unsigned __int32 cID;
	unsigned int status;
};

#define MESSAGEBOX_YES		0x01
#define MESSAGEBOX_NO		0x02
#define MESSAGEBOX_OK		0x04

typedef void (*_callbackSimple)(int);
struct MessageBoxStruct
{
	std::wstring msg;
	unsigned int flag;
	_callbackSimple callback;
};
