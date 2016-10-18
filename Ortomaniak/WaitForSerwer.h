#pragma once
#include "orto.h"
#include "Darkness.h"

class WaitForSerwer
{
	sf::String text_prosze_czekac,
		text_wiadomosc;

	sf::Image loading_img;
	sf::Sprite loading;
	sf::Clock timer;
	sf::Font font;
	Darkness dark;
	void Rotate();
	bool show;
public:
	void SetText(std::string wiadomosc);
	int Draw(sf::RenderWindow& wnd);
	void SetShow(bool _show)
	{
		dark.SetBlank(show=_show);
	}
	WaitForSerwer(void);
	~WaitForSerwer(void);
};

