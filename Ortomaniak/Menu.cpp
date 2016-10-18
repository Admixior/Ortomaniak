#include "menu.h"


namespace std_menu
{
	Button button_1(MENU_1_TEXT,MENU_1_SIZE,sf::Vector2f(MENU_1_CEN_POINT),1,MENU_FONT);
	Button button_2(MENU_2_TEXT,MENU_2_SIZE,sf::Vector2f(MENU_2_CEN_POINT),2,MENU_FONT);
	Button button_3(MENU_3_TEXT,MENU_3_SIZE,sf::Vector2f(MENU_3_CEN_POINT),3,MENU_FONT);
	Button button_4(MENU_4_TEXT,MENU_4_SIZE,sf::Vector2f(MENU_4_CEN_POINT),4,MENU_FONT);
};


Button::Button(std::string _text, unsigned int size_font,sf::Vector2f centerpos,int button_id, short int RCid_font)
{
	bID=button_id;

	LoadFont(RCid_font,font,size_font);
	text.SetFont(font);
	text.SetSize(static_cast<float>(size_font));
	text.SetPosition(centerpos);
	text.SetText(_text);
	SetCenterText(text);

	update_rect=true;
	mouseover=true;
	SetEnable(true);
	SetMouseOver(false);
}

bool Button::SetMouseOver(bool _mouseover)
{
	if(!enable)return false;
	if(mouseover==_mouseover)return 0;
	mouseover=_mouseover;
	if(mouseover)text.SetColor(MENU_COLOR_BTNMOUSEOVER);
	else text.SetColor(MENU_COLOR_BTNACTIVE);
	return mouseover;
}

int Button::GetClicked(sf::Vector2f& pos)
{
	if(!enable)return false;
	return in_rect(pos);
}

int Button::MouseMove(sf::Vector2f& pos)
{
	if(!enable)return false;
	return SetMouseOver(in_rect(pos));
}

bool Button::in_rect(sf::Vector2f& pos)
{
	if(!enable)return false;
	if(update_rect)
	{
		update_rect=false;
		rect=text.GetRect();
	}
	if(
		pos.x<rect.Left		||
		pos.x>rect.Right	||
		pos.y<rect.Top		||
		pos.y>rect.Bottom
		)
		return false;
	return true;
}

int Button::buttonMove(float x,float y)
{
	text.Move(x,y);
	update_rect=true;
	return 0;
}

int Button::Draw(sf::RenderWindow& wnd)
{
	wnd.Draw(text);
	return 0;
}

void Button::SetEnable(bool _enable)
{
	if(enable=_enable)
	{
		text.SetColor(MENU_COLOR_BTNACTIVE);;
	}
	else
	{
		text.SetColor(MENU_COLOR_BTNDEACTIVE);
	}
}


/////////////////////////////////////////////////////////////
//*******************   M  E  N  U   **********************//
Menu::Menu(float _show_pos):show_pos(_show_pos)
{
	down_img.LoadFromMemory(zaladuj(IDB_MENU_BELKA,L"PNG"),rozmiar(IDB_MENU_BELKA,L"PNG"));
	up_img.LoadFromMemory(zaladuj(IDB_MENU_TRAWA,L"PNG"),rozmiar(IDB_MENU_TRAWA,L"PNG"));
	down.SetImage(down_img);
	up.SetImage(up_img);
	down.SetPosition(POS_MENU_X,last_pos=hide_pos=-static_cast<float>(down_img.GetHeight()));
	up.SetPosition(POS_MENU_X,last_pos);

	sin_pos=0;
	show_pos=POS_MENU_Y;
}


Menu::~Menu(void)
{
}

#include <cmath>
int Menu::MoveMenu(void)
{
	float wspolczynnik=3.1415926535f*5.f/8.f;
	if(show)
	{
		if(sin_pos>=1.f)return 0;
		sin_pos=timer.GetElapsedTime()/TIME_SHOW_MENU;
		if(sin_pos>1.f)sin_pos=1.f;
	}
	else
	{
		if(sin_pos<=0.f)return 0;
		sin_pos=(TIME_HIDE_MENU-timer.GetElapsedTime())/TIME_HIDE_MENU;
		if(sin_pos<0.f)sin_pos=0.f;
	}
	float move_full=-(sinf(wspolczynnik*sin_pos)*(hide_pos-show_pos));
	float  move=move_full-last_pos+hide_pos;
	last_pos+=move;
	up.Move(0,move);
	down.Move(0,move);
	for(size_t i=0;i<przyciski.size();++i)
		przyciski[i]->buttonMove(0,move);
	return 0;
}


int Menu::MouseClick(sf::Vector2f& pos)
{
	for(size_t i=0;i<przyciski.size();i++)
		if(przyciski[i]->GetClicked(pos))
				return i;
	return -1;
}


int Menu::MouseMove(sf::Vector2f& pos)
{
	for(size_t i=0;i<przyciski.size();i++)
		if(przyciski[i]->MouseMove(pos))return i;
	return -1;
}


int Menu::AddButton(Button* button)
{
	button->buttonMove(0,last_pos-POS_MENU_SHOW);
	przyciski.push_back(button);
	return przyciski.size()-1;
}


int Menu::Draw(sf::RenderWindow& wnd)
{
	MoveMenu();
	wnd.Draw(down);
	for(size_t i=0;i<przyciski.size();i++)
		przyciski[i]->Draw(wnd);
	wnd.Draw(up);
	return 0;
}
