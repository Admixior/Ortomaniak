#pragma once
#include "School.h"

struct __Config
{
	unsigned short activetype_words;
	unsigned int max_time_for_word;
	unsigned int max_word;
};
extern __Config config;
extern std::string logo_name;

class DataBase;

//  object for all
extern DataBase PublicDB;

class DataBase
{
public:
	DataBase(void);
	~DataBase(void);

	/////////-------- School manage ---------//////////
	// add new school and return school ID
	unsigned __int32 AddSchool(std::string school_name);

	// delete school if possible else return false
	bool DeleteSchool(unsigned __int32 sID);

	// edit school name
	bool EditSchool(unsigned __int32 sID, std::string new_school_name);

	// find school at this name, return sID or return (-1) if not exist
	unsigned __int32 FindSchool(std::string school_name);

	// put the ShortSchoolInfo to the ret and return total number of school
	int GetListOfSchool(std::vector<ID_Name>& ret);

	// put the LongInfoUser to the ret and return total number of users register in school
	// return -1 if fail
	int GetFullInfoUserFromSchool(unsigned __int32 sID, std::vector<LongInfoUser> &ret);
	
	// return false if fail, save LongInfoUser
	bool GetFullInfoAboutUser(unsigned __int32 uID, LongInfoUser& save);

	// return return true if school exist and success save to ret
	bool GetInfoAboutSchool(unsigned __int32 sID, ID_Name& ret);

	/////////--------- User manage ----------//////////
	// add new user to school and return uID if success or -
	unsigned __int32 AddUser(std::string imie, std::string nazwisko, unsigned __int32 sID);

	// delete user from school if possible - return true if success
	bool DeleteUsers(unsigned __int32 uID);

	// edit name user and return true if success
	bool EditUser(unsigned __int32 uID, std::string imie, std::string nazwisko, BYTE stats[3], __int32 sID);

	
	/////////-------- Network manage --------//////////
	// save to ret school avaible for this user
	void GetSchoolListForUser(std::string imie, std::string nazwisko, std::vector<ID_Name>& ret);

	// Login and return user ID -   uID == (-1) if can't login
	unsigned __int32 LoginUser(std::string imie, std::string nazwisko, unsigned __int32 sID);
	
	// Set new stats if can else return false
	bool SetStats(unsigned __int32 uID, BYTE wyniki[3]);

	// Logout user and return true if success
	bool Logout(unsigned __int32 uID);

	
	/////////-------- Local manage --------//////////
	// save to ret all uID where school id is equal
	void FindAllUsersFromSchool(unsigned __int32 sID, std::vector<unsigned __int32>& ret);

	// return uID user where local_id and school_id is equal if no object return (-1)
	unsigned __int32 FindUseruID(unsigned int sID, unsigned int local_id);

	// find first empty uID and return, if no empty uID return (-1)
	unsigned __int32 FindEmptyuID();

	void ClearBase();
private:
	std::vector<ShortUserInfo>	userinfo;
	std::vector<School* >		school;
};

