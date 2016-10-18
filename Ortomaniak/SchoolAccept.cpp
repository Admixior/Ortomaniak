#include "SchoolAccept.h"


SchoolAccept::SchoolAccept():
	mouseover(0),
	wid(SCHOOLACCEPTEVENT_NOSHOW),
	show_arrow(false)
{
	//load font
	LoadFont(SA_DEFAULT_FONT,font,SA_FONT_FIRSTANDSURNAME_SIZE);

	//font text
	imie.SetFont(font);
	nazwisko.SetFont(font);
	nazwa_szkoly.SetFont(font);
	akceptuj.SetFont(font);
	odrzuc.SetFont(font);
	//size text
	imie.SetSize(SA_FONT_FIRSTANDSURNAME_SIZE);
	nazwisko.SetSize(SA_FONT_FIRSTANDSURNAME_SIZE);
	nazwa_szkoly.SetSize(SA_FONT_SCHOOLSIZE);
	nazwa_szkoly.SetScale(1.f,1.3f);
	akceptuj.SetSize(SA_FONTBTN_SIZE+5);		//WARNING: This is a large size button
	odrzuc.SetSize(SA_FONTBTN_SIZE+5);			//			here too.  See down (10lines)
	
	//set static text
	akceptuj.SetText("Akceptuj");
	odrzuc.SetText("Odrzuc");

	//get the rect on screen
	big_accept= akceptuj.GetRect();
	big_cancel= odrzuc.GetRect();

	akceptuj.SetSize(SA_FONTBTN_SIZE);		//The normal size
	odrzuc.SetSize(SA_FONTBTN_SIZE);	

	small_accept= akceptuj.GetRect();
	small_cancel= odrzuc.GetRect();

	//position text
	imie.SetPosition(SA_FIRSTNAME_POS);
	nazwisko.SetPosition(SA_SURNAME_POS);
	nazwa_szkoly.SetPosition(SA_SCHOOL_CENTER);
	akceptuj.SetPosition(SA_ACCEPTTEXT_POS);
	odrzuc.SetPosition(SA_DENYTEXT_POS);

	imie.SetColor(sf::Color::Black);
	nazwisko.SetColor(sf::Color::Black);
	nazwa_szkoly.SetColor(sf::Color::Black);
	akceptuj.SetColor(sf::Color::Black);
	odrzuc.SetColor(sf::Color::Black);

	//rotation text
	akceptuj.SetRotation(SA_TEXT_ROTATE);
	odrzuc.SetRotation(SA_TEXT_ROTATE);


	//center the text
	SetCenterText(akceptuj);
	SetCenterText(odrzuc);

	//load img
	podstawa_img.LoadFromMemory(zaladuj(IDB_SCHOOLACCEPT,L"PNG"),rozmiar(IDB_SCHOOLACCEPT,L"PNG"));
	agrafka_img.LoadFromMemory(zaladuj(IDB_SCHOOLACCEPT_AGRAFKA,L"PNG"),rozmiar(IDB_SCHOOLACCEPT_AGRAFKA,L"PNG"));
	left_button_img.LoadFromMemory(zaladuj(IDB_SA_LEFT,L"PNG"),rozmiar(IDB_SA_LEFT,L"PNG"));
	right_button_img.LoadFromMemory(zaladuj(IDB_SA_RIGHT,L"PNG"),rozmiar(IDB_SA_RIGHT,L"PNG"));

	podstawa.SetImage(podstawa_img);
	agrafka.SetImage(agrafka_img);
	left_button.SetImage(left_button_img);
	right_button.SetImage(right_button_img);

	podstawa.SetPosition(SA_MAIN_IMG_POS);
	agrafka.SetPosition(SA_AGR_IMG_POS);
	left_button.SetPosition(SA_PREVSCHL_BTNPOS);
	right_button.SetPosition(SA_NEXTSCHL_BTNPOS);
	dark.SetBlank(false);
}


SchoolAccept::~SchoolAccept(void)
{
}

void SchoolAccept::StartSelect(std::string imie_t, std::string nazwisko_t)
{
	imie.SetText(imie_t);
	nazwisko.SetText(nazwisko_t);
	nazwa_szkoly.SetText("");

	//poprawienie przycisków
	akceptuj.SetSize(SA_FONTBTN_SIZE);
	odrzuc.SetSize(SA_FONTBTN_SIZE);
	SetCenterText(akceptuj);
	SetCenterText(odrzuc);

	mouseover=0;

	wid=SCHOOLACCEPTEVENT_WAIT;
	dark.SetBlank(true);
}

void SchoolAccept::MouseDown(sf::Vector2f pos)
{
	if(	show_arrow && 
		left_button.GetSubRect().Contains(
		pos.x-left_button.GetPosition().x,
		pos.y-left_button.GetPosition().y
		))
		wid=SCHOOLACCEPTEVENT_GETPREVSCHOOL;
	else if( show_arrow &&
		right_button.GetSubRect().Contains(
		pos.x-right_button.GetPosition().x,
		pos.y-right_button.GetPosition().y
		))
		wid= SCHOOLACCEPTEVENT_GETNEXTSCHOOL;
	else if(mouseover&0x01 )
	{
		pos= akceptuj.TransformToLocal(pos);
		if(big_accept.Contains(pos.x,pos.y))
			wid = SCHOOLACCEPTEVENT_ACCEPTSCHOOL;
	}
	else if(mouseover&0x02 )
	{
		pos= odrzuc.TransformToLocal(pos);
		if(big_cancel.Contains(pos.x,pos.y))
			wid = SCHOOLACCEPTEVENT_DENYSCHOOL;
	}
}


void SchoolAccept::MouseMove(sf::Vector2f pos)
{
	//zmniejszanie czcionki po zjechaniu
	if(mouseover&0x01)
	{
		sf::Vector2f pos_cache = akceptuj.TransformToLocal( pos);
		if(big_accept.Contains(pos_cache.x,pos_cache.y))
			return;
		akceptuj.SetSize(SA_FONTBTN_SIZE);
		SetCenterText(akceptuj);
		mouseover&=~0x01;
	}
	if(mouseover&0x02)
	{
		sf::Vector2f pos_cache = odrzuc.TransformToLocal( pos);
		if(big_cancel.Contains(pos_cache.x,pos_cache.y))
			return;
		odrzuc.SetSize(SA_FONTBTN_SIZE);
		SetCenterText(odrzuc);
		mouseover&=~0x02;
	}

	//zwiêkszanie najechaniu
	sf::Vector2f pos_cache= akceptuj.TransformToLocal(pos);
	if(small_accept.Contains(pos_cache.x, pos_cache.y) && !(mouseover&0x01))
	{
		akceptuj.SetSize(SA_FONTBTN_SIZE+5);
		SetCenterText(akceptuj);
		mouseover|=0x01;
		return;
	}
	
	pos_cache= odrzuc.TransformToLocal(pos);
	if(small_cancel.Contains(pos_cache.x,pos_cache.y) && !(mouseover&0x02))
	{
		odrzuc.SetSize(SA_FONTBTN_SIZE+5);
		SetCenterText(odrzuc);
		mouseover|=0x02;
	}
}


void SchoolAccept::SetSchool(std::string parameter)
{
	nazwa_szkoly.SetText(parameter);
	wid=SCHOOLACCEPTEVENT_WAIT;
}


int SchoolAccept::Draw(sf::RenderWindow& wnd)
{
	if(wid)
	{
		dark.Draw(wnd);
		wnd.Draw(podstawa);
		wnd.Draw(imie);
		wnd.Draw(nazwisko);
		wnd.Draw(nazwa_szkoly);
		if(show_arrow)
		{
			wnd.Draw(left_button);
			wnd.Draw(right_button);
		}
		wnd.Draw(akceptuj);
		wnd.Draw(odrzuc);
		wnd.Draw(agrafka);
	}
	else return dark.Draw(wnd);
	return 0;
}
