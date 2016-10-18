#include <iostream>
#include "Desktop.h"

bool GetNextLine(std::ifstream& file, std::string& str)
{
	while(file.good())
	{
		std::getline(file, str);
		if(str.size())
			if(str.find_first_of("#")==std::string::npos)
				if(str.find_first_not_of(" \t\n")!=std::string::npos)
					return true;
	}
	return false;
}

int main()
{
	srand(GetTickCount());	//initialize
	
	config.activetype_words=0;
	config.max_time_for_word=-1;
	config.max_word=-1;

	/*enum TypeWords
	{
		u_o				=0x0001,
		rz_z			=0x0002,
		ch_h			=0x0004,
		om_a_e			=0x0008,
		j_i				=0x0010,
		nie				=0x0020,
		by				=0x0040,
		duza_mala_lit	=0x0080,
		dla_dzieci		=0x0100
	};*/

	std::ifstream file_in("konfiguracja.txt");
	if(!file_in.good())
	{
		std::cout<<"Nie mozna otworzyc pliku konfiguracyjnego!";
		std::cin.get();
		return 0;
	}

	if(!GetNextLine(file_in,logo_name))
	{
		std::cout<<"Plik konfiguracyjny nie zawiera wszystkich informacji!";
		std::cin.get();
		return 0;
	}

	//---  ilo�� s��wek
	{
	std::stringstream str_to_int;
	std::string line;

	if(!GetNextLine(file_in,line))
	{
		std::cout<<"Plik konfiguracyjny nie zawiera wszystkich informacji!";
		std::cin.get();
		return 0;
	}

	str_to_int.str("");
	str_to_int<<line;
	str_to_int>>config.max_word;
	if(config.max_word==-1)
	{
		std::cout<<"Plik konfiguracyjny zawiera bledne dane!";
		std::cin.get();
		return 0;
	}
	}
	//---  czas na s��wko
	{
	std::stringstream str_to_int;
	std::string line;

	if(!GetNextLine(file_in,line))
	{
		std::cout<<"Plik konfiguracyjny nie zawiera wszystkich informacji!";
		std::cin.get();
		return 0;
	}

	str_to_int<<line;
	str_to_int>>config.max_time_for_word;
	if(config.max_time_for_word==-1)
	{
		std::cout<<"Plik konfiguracyjny zawiera bledne dane!";
		std::cin.get();
		return 0;
	}

	}
	//--- typy s��wek
	for(int i=0; i < 9; ++i)
	{
		std::stringstream str_to_int;
		std::string line;

		if(!GetNextLine(file_in,line))
		{
			std::cout<<"Plik konfiguracyjny nie zawiera wszystkich informacji!";
			std::cin.get();
			return 0;
		}

		str_to_int<<line;
		str_to_int>>line;	//pomini�cie bia�ych znak�w
		if(line.size()==0)
		{
			std::cout<<"Plik konfiguracyjny zawiera bledne dane!";
			std::cin.get();
			return 0;
		}

		if(line[0]=='T' || line[0]=='t')
		{
			config.activetype_words|= (1<<i);
		}
	}

	if(config.activetype_words==0)
	{
		std::cout<<"Nie wybrano rodzaju s��wek!";
		std::cin.get();
		return 0;
	}

	std::ofstream test("wyniki.txt",std::ios::app|std::ios::in);
	if(test.bad())
	{
		std::cout<<"Nie mo�na otworzy� pliku: wyniki.txt! \nZamknij wszystkie programy korzystaj�ce z tego pliku \nlub uruchom program z wy�szymi uprawnieniami.";
		std::cin.get();
		return 0;
	}
	test.close();

	CreateThread(0,0,(LPTHREAD_START_ROUTINE) Connect, 0, 0, 0);
	while(!serwer_on)Sleep(1);


	Desktop interface;
	interface.RunServer();
}