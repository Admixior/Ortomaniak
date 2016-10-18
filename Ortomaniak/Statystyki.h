#pragma once
#include "orto.h"
#include <sstream>
using namespace sf;
class Statystyki
{
	bool konkurs;
	Font czcionka;
	//-- dla konkursu
	struct konkurs_stats
	{
		int nr_proby;
		int wyniki[3][2];
		String wyniki_txt[3][2];
		String proba;
	};
	//-- dla treningu
	struct trening_stats
	{
		int wyniki[2];
		String wyniki_txt[2];
		String proba;
	};

	konkurs_stats wyniki_con;
	trening_stats wyniki_tren;
public:
	int SetKonkursType(bool _konkurs);
	int GodAnswer();
	int BadAnswer();
	int Clear();

	int NastepnaProba();

	int SetStateKonkurs(BYTE stats[3][2]);
	int GetStateKonkurs(BYTE stats[3][2]);

	int Reset();

	int Draw(RenderWindow& wnd);
	
	Statystyki();
	~Statystyki(void);
};

