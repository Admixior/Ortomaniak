#include "User.h"


User::User(std::string imie, std::string nazwisko)
{
	firstname=imie,
	surname=nazwisko;
	wyniki[0]=-1;
	wyniki[1]=-1;
	wyniki[2]=-1;
}


User::~User(void)
{
}

bool User::CheckName(const std::string& imie, const std::string& nazwisko)
{
	if( nazwisko==surname && imie==firstname ) return true;
	else	return false;
}

void User::SetNewName(const std::string& imie, const std::string& nazwisko)
{
	firstname=imie;
	surname=nazwisko;
}

bool User::SetStats(BYTE stats[3])
{
	for(int i=0; i<3 ; ++i)
	{
		if(wyniki[i]!=255)
			if(wyniki[i]!=stats[i])
				return false;
	}
	for(int i=0; i<3 ; ++i)
	{
		wyniki[i]=stats[i];
	}
	return true;
}


// copy the stats to wyniki
bool User::GetStats(BYTE stats[3])
{
	for(int i=0; i<3; ++i)
		stats[i]=wyniki[i];
	return true;
}

// copy the firstname and surname
bool User::GetName(std::string& imie, std::string& nazwisko)
{
	imie=firstname;
	nazwisko=surname;
	return true;
}