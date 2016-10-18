#pragma once
#include "orto.h"
class Darkness
{
	sf::Shape blank;
	unsigned char alfa_stop,alfa_act;
	bool NextBlankValue();
public:
	int SetBlank(bool alfa)
	{
		if(alfa) alfa_stop=200;
		else alfa_stop=0;
		return alfa_act;
	}
	int Draw(sf::RenderWindow& wnd);
	Darkness(void);
	~Darkness(void);
};

