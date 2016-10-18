#include "ListWord.h"
#include <fstream>
bool exist_obj_in_vec(vector<unsigned int>& wec,unsigned int val)
{
	for(vector<unsigned int>::iterator chck=wec.begin(); chck!=wec.end(); chck++)
	{
		if(*chck==val)return true;
	}
	return false;
}
ListWord::ListWord(void)
{
	sf::Randomizer::SetSeed(GetTickCount());
	////// --  £adowanie wszyskich s³ówek do wektora  -- /////
	const char* File=zaladuj(IDR_SLOWKA,L"Slowka");
	ifstream buff;
	buff.open("data\\slowka.txt");
	if( !buff.good() )
	{
		::ShowErr(LOCAL_LOAD_LISTWORD,"ListWord");
		exit(0);
	}
	//buff.write((char*)File,rozmiar(IDR_SLOWKA,L"Slowka"));
	size_t rozmiar=rozmiar(IDR_SLOWKA,L"Slowka");
	string line;
	unsigned int n=-1,i=0;
	while(true)
	{
		getline(buff,line);
		if(!line.size())break;
		else if(line[0]=='#')break;
		else if(line[0]=='@')
		{
			lista.push_back(vector<Word>());
			++n;
		}
		else
		{
			size_t pos,pos2;
			string word=line.substr(0,pos=line.find_first_of('\t'));
			pos++;
			string good=line.substr(pos,(pos2=line.find_first_of('\t',pos))-pos);
			string bad=line.substr(pos2+1);
			lista[n].push_back(Word(word,good,bad,i++));
		}
	}
	//---
	ActiveType=0x0007;
	type_uses=0;
}


ListWord::~ListWord(void)
{
}

Word* ListWord::GetRandomWord()
{
	size_t type_start=GetType();
	size_t type=type_start;
	unsigned int random_start=sf::Randomizer::Random(0,lista[type].size()-1);
	while(true)
	{
		unsigned int random=random_start;
		while(true)
		{
			if(!exist_obj_in_vec(wykorzystane,lista[type_start][random].GetId()))
			{
				wykorzystane.push_back(lista[type_start][random].GetId());
				return &lista[type_start][random];
			}
			if(random==0)random=lista[type_start].size();
			random--;
			if(random==random_start)break;
		}
		type=GetType();
		if(type==type_start)break;
		random_start=sf::Randomizer::Random(0,lista[type].size()-1);
	}
	return 0;
}

int ListWord::SetActiveType(__int16 new_type)
{
	if(!new_type)return -1;
	ActiveType=new_type;
	Reset();
	return 0;
}
unsigned __int16 ListWord::GetType()
{
	__int16 type=0x0001;
	unsigned __int16 n=0;
	while((!(type&ActiveType)) ||	(type&type_uses))
	{
		n++;
		type<<=1;
		if(n>=lista.size())
		{
			type_uses=0;
			type=0x0001;
			n=0;
		}
	}
	type_uses|=type;
	return n;
}