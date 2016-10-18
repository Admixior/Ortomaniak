#pragma once
#include <vector>
#include "Word.h"

using namespace std;
class ListWord
{
	vector<vector<Word>> lista;
	vector<unsigned int> wykorzystane;
	unsigned __int16 ActiveType;
	unsigned __int16 type_uses;
	unsigned __int16 GetType();
public:
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
	Word* GetRandomWord();
	void Reset()
	{
		type_uses=0;
		wykorzystane.clear();
	}
	int SetActiveType(__int16 new_type);

	ListWord(void);
	~ListWord(void);
};

