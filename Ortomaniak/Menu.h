#pragma once
#include "orto.h"
////////////////////////////////////////////////
///************* < B U T T O N > ************///
////////////////////////////////////////////////

class Button
{
	bool mouseover
		,enable;
	sf::Font font;
	sf::String text;
	sf::FloatRect rect;
	bool update_rect;
	int bID;

	Button();
public:
	Button(std::string _text, unsigned int size_font,sf::Vector2f centerpos,int button_id, short int RCid_font);
	void SetEnable(bool enable);
	bool SetMouseOver(bool _enable);
	int GetClicked(sf::Vector2f& pos);
	int MouseMove(sf::Vector2f& pos);
	bool in_rect(sf::Vector2f& pos);
	int buttonMove(float x,float y);
	int Draw(sf::RenderWindow& wnd);
};


namespace std_menu
{
	extern Button button_1;
	extern Button button_2;
	extern Button button_3;
	extern Button button_4;
};

//
class Menu
{
	std::vector<Button*> przyciski;
	sf::Image up_img;
	sf::Image down_img;
	sf::Sprite up;
	sf::Sprite down;

	float sin_pos,
		hide_pos,
		show_pos,
		last_pos;
	bool show;

	sf::Clock timer;


	int MoveMenu(void);
public:
	Menu(float show_pos);
	~Menu(void);

	int ShowMenu(bool _show)
	{
		for( unsigned int i=0; przyciski.size()>i ;++i)
			przyciski[i]->SetMouseOver(false);
		if(show!=_show)
			timer.Reset();
		return show=_show;
	}
	bool GetShowMenu(){return sin_pos>=1.f;}
	bool GetHideMenu(){return last_pos==hide_pos;}

	int MouseClick(sf::Vector2f& pos);
	int MouseMove(sf::Vector2f& pos);

	int AddButton(Button* button);
	int ClearButton(void){	przyciski.clear();	return 0;  }
	Button* GetButtonById(unsigned int id)
	{
		if(przyciski.size()>id)return przyciski[id];
		else return 0;
	}
	int Draw(sf::RenderWindow& wnd);
};

