#pragma once
#include "orto.h"
#include "Darkness.h"

class SchoolAccept
{
public:
	enum Work
	{
		SCHOOLACCEPTEVENT_NOSHOW,
		SCHOOLACCEPTEVENT_WAIT,
		SCHOOLACCEPTEVENT_GETNEXTSCHOOL,
		SCHOOLACCEPTEVENT_GETPREVSCHOOL,
		SCHOOLACCEPTEVENT_ACCEPTSCHOOL,
		SCHOOLACCEPTEVENT_DENYSCHOOL
	};
	SchoolAccept();
	~SchoolAccept(void);
	void StartSelect(std::string imie, std::string nazwisko);
	void StopSelect(){dark.SetBlank(false);}
	void MouseDown(sf::Vector2f pos);
	void MouseMove(sf::Vector2f pos);
	void SetSchool(std::string parameter);
	void ShowArrow(bool _show){show_arrow=_show;}
	Work GetWork(){return wid;}
	int Draw(sf::RenderWindow& wnd);

private:
	sf::Font font;

	sf::String imie,
		nazwisko,
		nazwa_szkoly,
		akceptuj,
		odrzuc;

	sf::Image podstawa_img,
		agrafka_img,
		left_button_img,
		right_button_img;

	sf::Sprite podstawa,
		agrafka,
		left_button,
		right_button;

	sf::FloatRect small_accept, big_accept,
		small_cancel, big_cancel;
	Darkness dark;
	BYTE mouseover;	//myszka na przycisku odrzuæ lub akceptuj
	Work wid;

	bool show_arrow;
};

