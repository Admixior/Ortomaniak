#pragma once
#include <vector>
#include "User.h"


struct LongInfoUser
{
	unsigned __int32 uID,	//public id
		sID,
		local_id;	
	std::string imie,
		nazwisko;
	BYTE wyniki[3];
	bool login;
};


class School
{
public:
	School(std::string school_name);
	~School(void);

	//////////-------- user manage --------//////////
	// add user and return local position user
	unsigned __int32 AddUser(std::string imie, std::string nazwisko);

	// delete user 
	bool DeleteUser(unsigned __int32 uID);

	// edit user and return true if success
	bool SetNameUser(std::string imie, std::string nazwisko, unsigned int local_id);

	// set stats user and return true if done
	bool SetStatsUser(BYTE stats[3], unsigned int local_id);

	// find user and return local_id if exist or (-1) if fail
	unsigned int FindUser(std::string imie, std::string nazwisko);

	// put the ret all user from school (LongInfoUser) and return total number of user in school
	//unsigned int GetUserList( std::vector<LongInfoUser> &ret);

	// return false if fail, save LongInfoUser
	bool GetFullInfoAboutUser(unsigned __int32 local_id, LongInfoUser& save);
	
	/////////-------- local manage --------//////////
	// check name school if equal return true
	bool CheckName(std::string _school_name);

	// set new name school(without new lines)
	void SetNameSchool(std::string new_name_school);

	// return school name with new lines if true
	std::string GetSchoolName(bool with_new_lines);
private:
	std::string school_name;
	std::vector<User*> users;	//cz³onkowie danej szko³y
};

