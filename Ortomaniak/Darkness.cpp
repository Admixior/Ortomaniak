#include "Darkness.h"

using namespace sf;
Darkness::Darkness(void)
{
	blank.AddPoint(0,0,Color(0,0,0,0));
	blank.AddPoint(1920,0,Color(0,0,0,0));
	blank.AddPoint(1920,1080,Color(0,0,0,0));
	blank.AddPoint(0,1080,Color(0,0,0,0));
	blank.EnableOutline(false);
	alfa_act=0;
	alfa_stop=0;
}


Darkness::~Darkness(void)
{
}

bool Darkness::NextBlankValue()
{
	if(alfa_act!=alfa_stop)
	{
		if(alfa_act>alfa_stop)
		{
			if((alfa_act-alfa_stop)<=10)alfa_act=alfa_stop;
			else alfa_act-=10;
		}
		else
		{
			if((alfa_stop-alfa_act)<=10)alfa_act=alfa_stop;
			else alfa_act+=10;
		}
		for(int i=0;i<4;i++)blank.SetPointColor(i,Color(0,0,0,alfa_act));
	}
	return static_cast<bool>(alfa_act);
}

int Darkness::Draw(RenderWindow& wnd)
{
	if(!NextBlankValue())
		return -1;
	wnd.Draw(blank);
	return 0;
}