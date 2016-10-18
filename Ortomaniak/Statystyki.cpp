#include "Statystyki.h"


Statystyki::Statystyki():konkurs(false)
{
	wyniki_con.wyniki_txt[0][0].SetPosition(PROBA_1_PLUS);
	wyniki_con.wyniki_txt[1][0].SetPosition(PROBA_2_PLUS);
	wyniki_con.wyniki_txt[2][0].SetPosition(PROBA_3_PLUS);
	wyniki_con.wyniki_txt[0][1].SetPosition(PROBA_1_MINUS);
	wyniki_con.wyniki_txt[1][1].SetPosition(PROBA_2_MINUS);
	wyniki_con.wyniki_txt[2][1].SetPosition(PROBA_3_MINUS);
	wyniki_tren.wyniki_txt[0].SetPosition(TRENING_PLUS);
	wyniki_tren.wyniki_txt[1].SetPosition(TRENING_MINUS);
	LoadFont(FONT_STATS,czcionka,FONT_STATS_WEIGHT);

	for(int n=0;n<3;n++)
		for(int m=0;m<2;m++)
		{
			wyniki_con.wyniki_txt[n][m].SetFont(czcionka);
			wyniki_con.wyniki_txt[n][m].SetSize(FONT_STATS_WEIGHT);
			wyniki_con.wyniki_txt[n][m].SetRotation(STATS_ANGLE);
			if(m)wyniki_con.wyniki_txt[n][m].SetColor(PROBA_COLOR_MINUS);
			else wyniki_con.wyniki_txt[n][m].SetColor(PROBA_COLOR_PLUS);
		}
	for(int m=0;m<2;m++)
	{
		wyniki_tren.wyniki_txt[m].SetFont(czcionka);
		wyniki_tren.wyniki_txt[m].SetSize(FONT_STATS_WEIGHT);
		wyniki_tren.wyniki_txt[m].SetRotation(STATS_ANGLE);
		if(m)wyniki_tren.wyniki_txt[m].SetColor(PROBA_COLOR_MINUS);
		else wyniki_tren.wyniki_txt[m].SetColor(PROBA_COLOR_PLUS);
	}

	//----------------
	wyniki_con.proba.SetFont(czcionka);
	wyniki_con.proba.SetSize(FONT_PROBA_TEXT_WEIGHT);
	wyniki_con.proba.SetRotation(STATS_ANGLE);
	wyniki_con.proba.SetColor(Color(0,0,0));
	wyniki_con.proba.SetPosition(TEXT_PROBA_CENTER_POINT);

	wyniki_tren.proba.SetFont(czcionka);
	wyniki_tren.proba.SetSize(FONT_PROBA_TEXT_WEIGHT);
	wyniki_tren.proba.SetRotation(STATS_ANGLE);
	wyniki_tren.proba.SetColor(Color(0,0,0));
	wyniki_tren.proba.SetPosition(TEXT_PROBA_CENTER_POINT);

	wyniki_con.proba.SetText(TEXT_STATS_KONKURS);
	SetCenterText(wyniki_con.proba);
	wyniki_tren.proba.SetText(TEXT_STATS_STD);
	SetCenterText(wyniki_tren.proba);
	Clear();
}


Statystyki::~Statystyki(void)
{
}

int Statystyki::Reset()
{
	wyniki_con.nr_proby=-1;

	if(konkurs) return 0;
	/*for(int n=0;n<3;n++)
		for(int m=0;m<2;m++)
		{
			wyniki_con.wyniki[n][m]=0;
			wyniki_con.wyniki_txt[n][m].SetText("0");
			SetCenterText(wyniki_con.wyniki_txt[n][m]);
		}*/

	for(int m=0;m<2;m++)
	{
		wyniki_tren.wyniki[m]=0;
		wyniki_tren.wyniki_txt[m].SetText("0");
		SetCenterText(wyniki_tren.wyniki_txt[m]);
	}
	return 0;
}


int Statystyki::SetStateKonkurs(BYTE stats[3][2])
{
	if(!konkurs)return -1;
	stringstream ia;
	for(int n=0;n<3;n++)
		for(int m=0;m<2;m++)
		{
			if((wyniki_con.wyniki[n][m]=stats[n][m])!=255)
			{
				ia.str("");
				ia<<(wyniki_con.wyniki[n][m]=stats[n][m]);
				wyniki_con.wyniki_txt[n][m].SetText(ia.str());
				SetCenterText(wyniki_con.wyniki_txt[n][m]);
			}
		}
	return 0;
}

int Statystyki::GetStateKonkurs(BYTE stats[3][2])
{
	if(!konkurs)return -1;
	for(int n=0;n<3;n++)
		for(int m=0;m<2;m++)
			stats[n][m]=wyniki_con.wyniki[n][m];
	return 0;
}

int Statystyki::Draw(RenderWindow& wnd)
{
	if(konkurs)
	{
		for(int n=0;n<3;n++)
			for(int m=0;m<2;m++)
				wnd.Draw(wyniki_con.wyniki_txt[n][m]);
		wnd.Draw(wyniki_con.proba);
	}
	else
	{
		for(int m=0;m<2;m++)
			wnd.Draw(wyniki_tren.wyniki_txt[m]);
		wnd.Draw(wyniki_tren.proba);
	}
	return 0;
}

int Statystyki::GodAnswer()
{
	stringstream ia;
	if(konkurs)
	{
		if(wyniki_con.nr_proby==-1)return -1;
		ia<<++wyniki_con.wyniki[wyniki_con.nr_proby][0];
		wyniki_con.wyniki_txt[wyniki_con.nr_proby][0].SetText(ia.str());
		SetCenterText(wyniki_con.wyniki_txt[wyniki_con.nr_proby][0]);
	}
	else
	{
		ia<<++wyniki_tren.wyniki[0];
		wyniki_tren.wyniki_txt[0].SetText(ia.str());
		SetCenterText(wyniki_tren.wyniki_txt[0]);
	}
	return 0;
}
int Statystyki::BadAnswer()
{
	stringstream ia;
	if(konkurs)
	{
		if(wyniki_con.nr_proby==-1)return -1;
		ia<<++wyniki_con.wyniki[wyniki_con.nr_proby][1];
		wyniki_con.wyniki_txt[wyniki_con.nr_proby][1].SetText(ia.str());
		SetCenterText(wyniki_con.wyniki_txt[wyniki_con.nr_proby][1]);
	}
	else
	{
		ia<<++wyniki_tren.wyniki[1];
		wyniki_tren.wyniki_txt[1].SetText(ia.str());
		SetCenterText(wyniki_tren.wyniki_txt[1]);
	}
	return 0;
}


int Statystyki::NastepnaProba()
{
	if(!konkurs)return -1;
	int last=wyniki_con.nr_proby;
	while(last<3)
	{
		if(wyniki_con.wyniki[last][0]==255||
			wyniki_con.wyniki[last][1]==255)
		{
			for(int m=0;m<2;m++)
			{
				wyniki_con.wyniki[last][m]=0;
				wyniki_con.wyniki_txt[last][m].SetText("0");
				SetCenterText(wyniki_con.wyniki_txt[last][m]);
			}
			return wyniki_con.nr_proby=last;
		}
		++last;
	}
	return wyniki_con.nr_proby=-1;
}


int Statystyki::SetKonkursType(bool _konkurs)
{
	konkurs=_konkurs;
	Reset();
	return 0;
}

int Statystyki::Clear()
{
	for(int n=0;n<3;n++)
		for(int m=0;m<2;m++)
		{
			wyniki_con.wyniki[n][m]=-1;
			wyniki_con.wyniki_txt[n][m].SetText("==");
			SetCenterText(wyniki_con.wyniki_txt[n][m]);
		}

	for(int m=0;m<2;m++)
	{
		wyniki_tren.wyniki[m]=-1;
		wyniki_tren.wyniki_txt[m].SetText("==");
		SetCenterText(wyniki_tren.wyniki_txt[m]);
	}
	return 0;
}