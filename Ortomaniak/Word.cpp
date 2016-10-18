#include "Word.h"


Word::Word(string _word,string good,string bad,unsigned int _id):id(_id)
{
	int x=strcmp(good.c_str(),bad.c_str());
	word=_word;
	switch(x)
	{
	case 0:
		MessageBoxA(0,"B³ednie napisany plik ze s³ówkami",0,0);
		throw "Te same dane w s³ówkach";
	case 1:
		left=bad;
		right=good;
		god_odp=false;
		break;
	case -1:
		left=good;
		right=bad;
		god_odp=true;
		break;
	}
	last_odp=false;
	last_odp_exist=false;
}


Word::~Word(void)
{
}

string Word::GetLeftName()
{
	if(!left.size())return "Razem";
	if(left==" ")return "Osobno";
	return left;
}

string Word::GetRightName()
{
	if(!right.size())return "Razem";
	if(right==" ")return "Osobno";
	return right;
}

string Word::GetWord(bool _left)
{
	string slowo=word;
	last_odp_exist=true;
	int pos=slowo.find('?');
	slowo.erase(pos,1);
	if(last_odp=_left)slowo.insert(pos,left);
	else slowo.insert(pos,right);
	return slowo;
}

string Word::GetTrueWord()
{
	string slowo=word;
	int pos=slowo.find('?');
	slowo.erase(pos,1);
	if(god_odp)slowo.insert(pos,left);
	else slowo.insert(pos,right);
	return slowo;
}

string Word::GetWord()
{
	return word;
}

bool Word::GetOdp()
{
	if(last_odp_exist)
		if(god_odp==last_odp) return true;
	return false;
}