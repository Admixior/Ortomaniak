#include "DataBase.h"

// Define object (see: deklaration in header)
DataBase PublicDB;
__Config config;
std::string logo_name;


DataBase::DataBase(void)
{
}


DataBase::~DataBase(void)
{
	for( unsigned int i=0; i<school.size(); ++i)
	{
		if(school[i])
		{
			delete school[i];
			school[i]=0;
		}
	}
	school.clear();
	userinfo.clear();
}


/////////-------- School manage ---------//////////
// add new school and return school ID
unsigned __int32 DataBase::AddSchool(std::string school_name)
{
	if( FindSchool(school_name) != -1 )	return -1;
	unsigned int pos=FindNullPointer(school);
	School* new_school=new School(school_name);
	if(pos==-1)
	{
		school.push_back(new_school);
		return school.size()-1;
	}
	else 
	{
		school[pos]=new_school;
		return pos;
	}
}

// delete school if possible else return false
bool DataBase::DeleteSchool(unsigned __int32 sID)
{
	if(school.size() <= sID)	return false;	//nie ma tyle szkó³ (napewno nie istnieje)
	if(school[sID]==0)			return false;	//szko³a o tym sID nie istnieje

	//pobieranie uID wszystkich osobników ze szko³y
	std::vector<unsigned __int32> all_uID;
	FindAllUsersFromSchool(sID,all_uID);
	
	//sprawdzanie czy ktoœ nie jest zalogowany
	for( unsigned int i=0; i < all_uID.size() ;++i)
	{
		if( userinfo[i].login )	return false;
	}
	
	delete school[sID];	//usuniêcie szko³y i jej u¿yszkodników
	school[sID]=0;
	//usuniêcie z cache-u info o userach
	for( unsigned int i=0; i < all_uID.size() ;++i)
	{
		userinfo[all_uID[i]].login=false;	//w³aœciwie nie potrzebne
		userinfo[all_uID[i]].id_school=-1;
		userinfo[all_uID[i]].id_user=-1;
	}
	
	return true; //poprawnie zrobiono
}

// edit school name
bool DataBase::EditSchool(unsigned __int32 sID, std::string new_school_name)
{
	if(school.size() <= sID)	return false;
	if(school[sID]==0)			return false;
	//czy nazwa jest zajêta
	if(FindSchool(new_school_name)!=-1)
	{
		//jeœli nazwa jest taka sama...
		if(school[sID]->CheckName(new_school_name))	return true;
		else return false;//taka szkole istnieje
	}
	//mo¿na zmieniaæ
	school[sID]->SetNameSchool(new_school_name);
	return true;
}

// find school at this name
unsigned __int32 DataBase::FindSchool(std::string school_name)
{
	for( unsigned int i=0; i< school.size() ;++i)
	{
		if(school[i])
			if(school[i]->CheckName(school_name))return i;
	}
	return -1;
}

// put the ShortSchoolInfo to the ret and return total number of school
int DataBase::GetListOfSchool(std::vector<ID_Name>& ret)
{
	ID_Name obj;
	for( unsigned int i=0; i< school.size() ;++i)
	{
		if(school[i])
		{
			obj.id=i;
			obj.name=school[i]->GetSchoolName(false);
			ret.push_back(obj);
		}
	}
	return ret.size();
}


// put the LongInfoUser to the ret and return total number of users register in school
int DataBase::GetFullInfoUserFromSchool(unsigned __int32 sID, std::vector<LongInfoUser> &ret)
{
	if( sID >= school.size() )	return -1;
	if( school[sID]==0 )		return -1;

	std::vector<unsigned __int32> list_id;
	FindAllUsersFromSchool(sID, list_id);
	ret.resize(list_id.size());
	for( unsigned int i=0; i<list_id.size(); ++i)
		GetFullInfoAboutUser( list_id[i], ret[i] );
	return ret.size();
}

// return full info about user with uID
bool DataBase::GetFullInfoAboutUser(unsigned __int32 uID, LongInfoUser& ret)
{
	if( userinfo.size() <= uID ) return false;
	if( userinfo[uID].id_school==-1 || userinfo[uID].id_user==-1 ) return false;
	if( school[ userinfo[uID].id_school ]->GetFullInfoAboutUser( userinfo[uID].id_user, ret ) == false) return false;
	ret.uID = uID;
	ret.login = userinfo[ uID ].login;
	ret.sID = userinfo[uID].id_school;
	return true;
}


bool DataBase::GetInfoAboutSchool(unsigned __int32 sID, ID_Name& ret)
{
	if( school.size() <= sID ) return false;
	if( school[sID] == 0) return false;
	ret.name= school[sID]->GetSchoolName(false);
	ret.id=sID;
	return true;
}

/////////--------- User manage ----------//////////
// add new user to school and return is uID new user
unsigned __int32 DataBase::AddUser(std::string imie, std::string nazwisko, unsigned __int32 sID)
{
	if( sID >= school.size() )	return -1;
	if( school[sID]==0 )		return -1;

	unsigned __int32 local_id=school[sID]->AddUser(imie,nazwisko);
	if(local_id==-1) return false;
	unsigned int uID=FindEmptyuID();
	if(uID==-1)
	{
		ShortUserInfo new_user={sID, local_id, false};
		userinfo.push_back(new_user);
		return userinfo.size()-1;
	}
	else
	{
		userinfo[uID].id_school=sID;
		userinfo[uID].id_user=local_id;
		userinfo[uID].login=false;
		return uID;
	}
}

// delete user from school if possible - return true if success
bool DataBase::DeleteUsers(unsigned __int32 uID)
{
	bool ret=false;
	// Check all possible error
	if( uID>=userinfo.size() )	return false;		//  user not exist
	if( userinfo[uID].id_school==-1 || userinfo[uID].id_user==-1 ) goto exit;	//user was deleted

	// create help variables
	unsigned __int32 sID = userinfo[uID].id_school,
		local_id=userinfo[uID].id_user;

	if( sID >= school.size() )	goto exit;	//school user not exist //(never should be)
	if( school[sID] == 0 )	goto exit;	//school was deleted

	// maybe it is connect
	if( userinfo[uID].login == true )	return false;
	// O.K. we must delete user
	ret=school[sID]->DeleteUser(userinfo[uID].id_user);
exit:
	userinfo[uID].id_school=-1;
	userinfo[uID].id_user=-1;
	userinfo[uID].login=false;
	return ret;
}
	
// edit name user and return true if success
bool DataBase::EditUser(unsigned __int32 uID, std::string imie, std::string nazwisko, BYTE stats[3], __int32)
{
	if( uID>=userinfo.size() )	return false;		//  user not exist
	if( userinfo[uID].id_school==-1 || userinfo[uID].id_user==-1 ) return false;	//user was deleted

	// create help variables
	unsigned __int32 sID = userinfo[uID].id_school,
		local_id=userinfo[uID].id_user;

	if( sID >= school.size() )	return false;	//school user not exist //(never should be)
	if( school[sID] == 0 )		return false;	//school was deleted
	if(!school[sID]->SetNameUser(imie, nazwisko, local_id))return false;
	return school[sID]->SetStatsUser(stats, local_id);
}

	
/////////-------- Network manage --------//////////
// save to ret school avaible for this user
void DataBase::GetSchoolListForUser(std::string imie, std::string nazwisko, std::vector<ID_Name>& ret)
{
	for(unsigned int sID=0; sID<school.size(); ++sID)
	{
		if(school[sID])
		{
			unsigned int local_id=school[sID]->FindUser(imie,nazwisko);
			if(local_id==-1)continue;

			ID_Name new_school;
			new_school.id=sID;
			new_school.name=school[sID]->GetSchoolName(true);
			ret.push_back(new_school);
		}
	}
}

// Login and return user ID -   uID == (-1) if can't login
unsigned __int32 DataBase::LoginUser(std::string imie, std::string nazwisko, unsigned __int32 sID)
{
	if(sID>=school.size())return -1;
	if(school[sID]==0)return -1;
	// get local_id user
	int local_id=school[sID]->FindUser(imie, nazwisko);
	if(local_id == -1)return -1;

	// get public id user
	unsigned __int32 uID= FindUseruID(sID, local_id);
	if(uID==-1)	return -1;	//fatal error
	userinfo[uID].login=true;
	return uID;
}
	
// Set new stats if can - else return false
bool DataBase::SetStats(unsigned __int32 uID, BYTE wyniki[3])
{
	if(uID >= userinfo.size() )	return false;
	
	unsigned int sID= userinfo[uID].id_school,
		local_id= userinfo[uID].id_user;

	if(sID==-1 || local_id == -1)		return false;	//user zostal usuniety (hmm... you should be early logout)
	//if(userinfo[uID].login == false)	return false;	//yyy.. are you cheating?
	
	return school[sID]->SetStatsUser(wyniki, local_id);
}

// Logout user and return true if all success
bool DataBase::Logout(unsigned __int32 uID)
{
	if(uID >= userinfo.size() )	return false;

	if(userinfo[uID].login == false)	return false;	//yyy.. are you cheating?
	userinfo[uID].login = false;
	return true;
}

/////////-------- Local manage --------//////////
// save to ret all uID where school id is equal
void DataBase::FindAllUsersFromSchool(unsigned __int32 sID, std::vector<unsigned __int32>& ret)
{
	for( unsigned int i=0; i<userinfo.size(); ++i)
	{
		if(userinfo[i].id_school==sID)  ret.push_back(i);
	}
}

// return uID user where local_id and school_id is equal if no object return (-1)
unsigned __int32 DataBase::FindUseruID(unsigned int sID, unsigned int local_id)
{
	for( unsigned int i=0; i<userinfo.size(); ++i)
	{
		if(userinfo[i].id_school==sID &&
			userinfo[i].id_user==local_id)
			return i;
	}
	return -1;
}

// find first empty uID and return, if no empty uID return (-1)
unsigned __int32 DataBase::FindEmptyuID()
{
	for( unsigned int i=0; i<userinfo.size(); ++i)
	{
		if(userinfo[i].id_school==-1 || userinfo[i].id_user==-1)
			return i;
	}
	return -1;
}


void DataBase::ClearBase()
{
	for( unsigned int i=0; i<school.size(); ++i)
	{
		if(school[i])
		{
			delete school[i];
			school[i]=0;
		}
	}
	school.clear();
	userinfo.clear();
}