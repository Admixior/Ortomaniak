#include "Autorzy.h"


Autorzy::Autorzy(void)
{
	img.LoadFromMemory(zaladuj(IDB_AUTORZY,L"PNG"),rozmiar(IDB_AUTORZY,L"PNG"));
	sprite.SetImage(img);
	sprite.SetPosition(AUTORZY_POS_IMG);
	sprite.SetColor(sf::Color(255,255,255,alfa=0));
	dark.SetBlank(false);
	show=false;
}


Autorzy::~Autorzy(void)
{
}

void Autorzy::Draw(sf::RenderWindow& wnd)
{
	if(alfa||show)
	{
		if(show)
		{
			if(alfa!=255)
			{
				if(alfa>=245)alfa=255;
				else alfa+=10;
			}
		}
		else
		{
			if(alfa!=0)
			{
				if(alfa<=10)alfa=0;
				else alfa-=10;
			}
		}
		sprite.SetColor(sf::Color(255,255,255,alfa));
		dark.Draw(wnd);
		wnd.Draw(sprite);
	}
}