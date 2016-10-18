#pragma once
#include "orto.h"
#include "Darkness.h"

class Autorzy
{
	Darkness dark;
	sf::Image img;
	sf::Sprite sprite;
	BYTE alfa;
	bool show;
public:
	Autorzy(void);
	~Autorzy(void);
	void Draw(sf::RenderWindow& wnd);
	void Show(bool _show)	{ dark.SetBlank( (show=_show) ); }
	bool GetShow(){ return show; }
};

