#pragma once
#include "orto.h"

using namespace sf;
class Interfejs
{
	bool edycja_konkursowa;
	Image main_img,edition_img,blank_img;
	Sprite main,edition,blank;
	Font font_wersja, font_imie, font_nazwisko;
	String edition_text,imie_text,nazwisko_text;
	
public:
	Interfejs(bool konkurs,std::string wersja_konkursu);
	~Interfejs(void);
	int SetName(std::string& imie, std::string& nazwisko);
	int DrawTo(RenderWindow& wnd);
};

