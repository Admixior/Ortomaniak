#include "School.h"


School::School(std::string _school_name)
{
	school_name=_school_name;
}


School::~School(void)
{
	//delete all users
	for( unsigned int i=0; i < users.size() ; ++i)
	{
		if(users[i])	delete users[i];
		users[i]=0;
	}
}

unsigned __int32 School::AddUser(std::string imie, std::string nazwisko)
{
	if( FindUser(imie,nazwisko) != -1 )	return -1;
	unsigned int pos=FindNullPointer(users);
	User* new_user=new User(imie,nazwisko);
	if(pos==-1)
	{
		users.push_back(new_user);
		return users.size()-1;
	}
	else 
	{
		users[pos]=new_user;
		return pos;
	}
}


bool School::DeleteUser(unsigned __int32 local_id)
{
	if( local_id >= users.size() )	return false;
	if( users[local_id]==0 )		return false;
	delete users[local_id];
	users[local_id]=0;
	return true;
}


bool School::SetNameUser(std::string imie, std::string nazwisko, unsigned int local_id)
{
	if( local_id >= users.size() )	return false;
	if( users[local_id]==0 )		return false;
	if( FindUser(imie,nazwisko) != -1 )
	{
		// je¿eli imie jest takie samo to zwracamy true
		if( users[local_id]->CheckName(imie,nazwisko) )	return true;
		else return false;
	}
	users[local_id]->SetNewName(imie,nazwisko);
	return true;
}


bool School::SetStatsUser(BYTE stats[3], unsigned int local_id)
{
	if( local_id >= users.size() )	return false;
	if( users[local_id]==0 )		return false;
	return users[local_id]->SetStats(stats);
}


unsigned int School::FindUser(std::string imie, std::string nazwisko)
{
	for( unsigned int i=0; i < users.size() ; ++i)
	{
		if( users[i])
			if( users[i]->CheckName(imie,nazwisko) )
				return i;
	}
	return -1;
}


bool School::CheckName(std::string _school_name)
{
	if( school_name == _school_name )	return true;
	else return false;
}


void School::SetNameSchool(std::string new_school_name)
{
	school_name=new_school_name;
}


std::string School::GetSchoolName(bool new_lines)
{
	if(new_lines==false) return school_name;
	//else
#define MAX_COUNT_CHARS 25

	std::string line, school_name=this->school_name;

	//line 1
	int x= school_name.find_last_of(' ',MAX_COUNT_CHARS);
	if(	
		x==std::string::npos ||
		school_name.size() <= MAX_COUNT_CHARS
		)
	{
		if(school_name.size() <= MAX_COUNT_CHARS)
			x=school_name.size();
		else x=MAX_COUNT_CHARS;
	}
	line=school_name.substr(0,x);
	school_name.erase(school_name.begin(), school_name.begin()+x);
	if(!school_name.size()) return line;
	if(school_name[0]==' ')	school_name.erase(school_name.begin());
	line+='\n';

	//line 2
	if(!school_name.size()) return line;
	x= school_name.find_last_of(' ',MAX_COUNT_CHARS);
	if(	
		x==std::string::npos   ||
		school_name.size() <= MAX_COUNT_CHARS
		)
	{
		if(school_name.size() <= MAX_COUNT_CHARS)
			x=school_name.size();
		else x=MAX_COUNT_CHARS;
	}
	line+=school_name.substr(0,x);
	school_name.erase(school_name.begin(), school_name.begin()+x);
	if(!school_name.size()) return line;
	if(school_name[0]==' ')	school_name.erase(school_name.begin());
	line+='\n';

	//line 3
	if( !school_name.size() ) return line;
	if( school_name.size() <= MAX_COUNT_CHARS )
	{
		line+=school_name;
	}
	else
	{
		school_name.erase( school_name.begin() + MAX_COUNT_CHARS-2, school_name.end() );
		school_name+="...";
		line+=school_name;
	}

	return line;

#undef MAX_COUNT_CHARS
}

// put the ret all user from school (LongInfoUser) and return total number of user in school
//unsigned int School::GetUserList( std::vector<LongInfoUser> &ret)
//{
//	LongInfoUser obj;
//	for(unsigned int i=0; i<users.size(); ++i)
//	{
//		if(users[i])
//		{
//			users[i]->GetName(obj.imie, obj.nazwisko);
//			users[i]->GetStats(obj.wyniki);
//			obj.local_id=i;
//			ret.push_back(obj);
//		}
//	}
//	return ret.size();
//}


// return long info about user with uID
bool School::GetFullInfoAboutUser(unsigned __int32 local_id, LongInfoUser& ret)
{
	if( users.size() <= local_id )	return false;
	if( users[local_id]==0 )			return false;
	users[local_id]->GetName(ret.imie, ret.nazwisko);
	users[local_id]->GetStats(ret.wyniki);
	ret.local_id=local_id;
	return true;
}