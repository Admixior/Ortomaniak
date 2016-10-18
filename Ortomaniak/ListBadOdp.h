#pragma once
#include "orto.h"

class ListBadOdp
{
	// ----  T£O -----
	sf::Sprite kartka;
	sf::Image kartka_img;

	// ---- LINIE TEKSTU -----
	class linia
	{
		sf::String text_show;
		std::string text_original,
					text_actual;
	public:
		void SetText(std::string txt)
		{
			text_original=txt;
			text_show.SetText(text_actual="");
		}
		void Settings(float x, float y, sf::Font& font, float size, float nachylenie)
		{
			text_show.SetFont(font);
			text_show.SetSize(size);
			text_show.SetPosition(x,y);
			text_show.SetRotation(nachylenie);
			text_show.SetColor(sf::Color::Black);
		}
		bool NextChar()
		{
			if( text_actual.size()==text_original.size() )	return true;	//koniec tekstu
			//dodajemy nastepny znak i wyswietlamy na ekran
			text_show.SetText(  text_actual += text_original[ text_actual.size() ]  ); 
			return false;				//to jeszcze nie koniec :]
		}
		void Draw(sf::RenderWindow& wnd) { wnd.Draw(text_show); }
	};

	std::vector<linia> lista_blednych_slowek;
	sf::Font czcionka;
	unsigned int linia; //aktualnie pojawiana linia

	// ----- Napis  "Naciœnij aby kontynuowaæ" -----
	sf::Color kolor;
	sf::String txt_to_continue;
	bool darker;	//czy kolor tekstu ma stawac sie jasniejszy czy ciemniejszy

	sf::Clock timer;	//odmierzanie czasu do nastêpnej literki, póŸniej do zmiany koloru $txt_to_continue
	
	bool end;
public:
	ListBadOdp(void);
	~ListBadOdp(void);
	void InitializeBadList(std::vector<sf::String>& badlist, unsigned int use_words);
	void ListBadOdp::InitializeFinalResults(BYTE stats[3][2]);
	void Draw(sf::RenderWindow& wnd);
	void AnyKeyPress();
	bool Loop();		//zwraca true jeœli klasa zakoñczy³a pracê
};

