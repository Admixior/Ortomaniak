#pragma once
#include "orto.h"
#include <string>
using namespace std;
class Word
{
	string word;
	string left,
		right;
	bool god_odp;
	bool last_odp;
	bool last_odp_exist;
	unsigned int id;
public:
	string GetLeftName();
	string GetRightName();
	string GetWord(bool left);
	string GetWord();
	string GetTrueWord();
	bool GetOdp();
	unsigned int GetId(){return id;}
	void Reset(){last_odp_exist=false;}
	Word(string word,string good,string bad,unsigned int id);
	~Word(void);
};

