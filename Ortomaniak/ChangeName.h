#pragma once
#include "Darkness.h"
using namespace sf;


class ChangeName
{
	Darkness dark;
	Font font_imie,font_nazwisko;
	Image podstawa_img;
	Vector2f pkt,img_size;
	Sprite podstawa;
	String imie
		,nazwisko;
	std::string imie_text
		,nazwisko_text;
	enum what_i_do
	{
		idle,
		firstname,
		surname
	};
	what_i_do wid;
	

	Clock timer;
	bool success_entered;
	bool cursor;
public:
	bool Initialize();
	bool Deinitialize();
	int AddChar(char c);
	int DeleteChar();
	int Click(float x, float y);
	int Draw(RenderWindow& wnd);
	int GetName(string& firstname,string& nazwisko);
	int Work(){return wid;}
	bool GetSuccessEntered()
	{
		if(!success_entered)return false;
		success_entered=false;
		return true;
	}
	int TabOrEnterPressed();
	int ChangeCursor();
	ChangeName(void);
	~ChangeName(void);
	int DeleteCursor(void);
};

