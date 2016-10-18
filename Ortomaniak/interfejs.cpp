#include "interfejs.h"


Interfejs::Interfejs(bool konkurs,std::string wersja_konkursu)
{
	edycja_konkursowa=konkurs;
	main_img.LoadFromMemory(zaladuj(IDB_LAYOUT,L"BMP"),rozmiar(IDB_LAYOUT,L"BMP"));
	main.SetImage(main_img);
	if(konkurs)		//rysowanie edycji
	{
		//obrazek
		edition_img.LoadFromMemory(zaladuj(IDB_EDYCJA_KONKURSU,L"PNG"),rozmiar(IDB_EDYCJA_KONKURSU,L"PNG"));
		edition.SetPosition(SPACE_TO_WRITE_VERSION);
		edition.SetImage(edition_img);
		//text
		LoadFont(FONT_EDITION,font_wersja,VERSION_TEXT_WEIGHT,wersja_konkursu);
		edition_text.SetFont(font_wersja);
		edition_text.SetColor(Color(0,0,0));
		edition_text.SetPosition(VERSION_TEXT_CENTER_POINT);
		edition_text.SetRotation(VERSION_TEXT_ANGLE);
		edition_text.SetText(wersja_konkursu);
		MakeSmaller(edition_text,VERSION_TEXT_WEIGHT,VERSION_TEXT_RIGHT_POINT);
	}

	//zaladowanie czcionek
	LoadFont(FONT_NAME,font_imie,FIRST_NAME_WEIGHT);
	LoadFont(FONT_NAME,font_nazwisko,SURNAME_WEIGHT);
	//i ustawienie
	imie_text.SetFont(font_imie);
	nazwisko_text.SetFont(font_nazwisko);
	//koloru
	imie_text.SetColor(Color(0,0,0));
	nazwisko_text.SetColor(Color(0,0,0));
	//obrócenie
	imie_text.SetRotation(ANGLE_NAME);
	nazwisko_text.SetRotation(ANGLE_NAME);
	//ustawienie pozycji
	imie_text.SetPosition(FIRST_NAME_CENTER_POINT);
	nazwisko_text.SetPosition(SURNAME_CENTER_POINT);
	//zmniejszanie
	SetName((string)"Imiê",(string)"Nazwisko");
}


Interfejs::~Interfejs(void)
{
}


int Interfejs::SetName(std::string& imie, std::string& nazwisko)
{
	//ustawienie textu i rozmiaru
	imie_text.SetText(imie);
	nazwisko_text.SetText(nazwisko);

	//ustawienie rozmiaru czcionki
	MakeSmaller(imie_text,FIRST_NAME_WEIGHT,FIRST_NAME_RIGHT_POINT);
	MakeSmaller(nazwisko_text,SURNAME_WEIGHT,SURNAME_RIGHT_POINT);

	return 0;
}

int Interfejs::DrawTo(RenderWindow& wnd)
{
	wnd.Draw(main);
	wnd.Draw(imie_text);
	wnd.Draw(nazwisko_text);
	if(edycja_konkursowa)
	{
		wnd.Draw(edition);
		wnd.Draw(edition_text);
	}
	return 0;
}