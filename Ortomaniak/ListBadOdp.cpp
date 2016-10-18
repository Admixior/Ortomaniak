#include "ListBadOdp.h"


ListBadOdp::ListBadOdp(void)
{
	LoadFont(LISTBADODP_FONT,czcionka,LISTBADODP_FONTSIZE);
	
	kartka_img.LoadFromMemory(zaladuj(IDB_KARTKA,L"PNG"),rozmiar(IDB_KARTKA,L"PNG"));
	kartka.SetImage(kartka_img);
	kartka.SetPosition(LISTBADODP_KARTKA_POS);
	//---
	lista_blednych_slowek.resize(7);
	for(float i=0 ; i<7 ; ++i)
	{
		lista_blednych_slowek[i].Settings(
			LISTBADODP_POSSTART_X +  LISTBADODP_POSMOVE_X * i,
			LISTBADODP_POSSTART_Y +  LISTBADODP_POSMOVE_Y * i,
			czcionka,
			LISTBADODP_FONTSIZE,
			LISTBADODP_NACHYLENIE
			);
	}

	linia = 0;
	timer.Reset();

	//---
	kolor=sf::Color(255, LISTBADODP_CONTINUETEXT_COLORMAX ,LISTBADODP_CONTINUETEXT_COLORMAX );
	txt_to_continue.SetPosition( LISTBADODP_CONTINUETEXT_POS );
	txt_to_continue.SetSize( LISTBADODP_CONTINUETEXT_SIZE );
	txt_to_continue.SetText( LISTBADODP_CONTINUETEXT );
	txt_to_continue.SetRotation( LISTBADODP_NACHYLENIE );
	txt_to_continue.SetFont( czcionka );
	darker=true;

	end=false;
}


ListBadOdp::~ListBadOdp(void)
{
}


void ListBadOdp::InitializeBadList(std::vector<sf::String>& badlist, unsigned int use_words)
{
	for(int i=0;i<lista_blednych_slowek.size();++i)
	{
		lista_blednych_slowek[i].SetText("");
	}
	std::string first_line="Rozwi¹za³eœ "+std::to_string((unsigned long long int) use_words-badlist.size())
		+" z "+std::to_string((unsigned long long int) use_words)+" s³ówek";
	lista_blednych_slowek[0].SetText(first_line);
	if(badlist.size())
	{
		lista_blednych_slowek[1].SetText("S³ówka b³êdnie rozwi¹zane :");
		for(int i=0 ; 2*i<badlist.size() ; ++i)
		{
			lista_blednych_slowek[i+2].SetText("   "+
					(std::string)badlist[i*2].GetText() +
					(2*i+1<badlist.size()	?	(std::string)" ,  "+(std::string)badlist[2*i+1].GetText():"")
				);
		}
	}
	else
	{
		lista_blednych_slowek[1].SetText("Wszystkie s³ówka zosta³y poprawnie");
		lista_blednych_slowek[2].SetText("rozwi¹zane!");
		lista_blednych_slowek[3].SetText("Gratulacje!");
	}
	end=false;
	darker=true;
	kolor=sf::Color(255, LISTBADODP_CONTINUETEXT_COLORMAX, LISTBADODP_CONTINUETEXT_COLORMAX );
	
	linia = 0;
	timer.Reset();
}

void ListBadOdp::InitializeFinalResults(BYTE stats[3][2])
{
	for(int i=0;i<lista_blednych_slowek.size();++i)
	{
		lista_blednych_slowek[i].SetText("");
	}
	lista_blednych_slowek[0].SetText("      Podsumowanie wyników dla");
	lista_blednych_slowek[1].SetText("              kolejnych prób :");
#define ADD_STATS(x,y) (stats[x][y]<10?" ":"")+std::to_string((long long)stats[x][y])
	lista_blednych_slowek[3].SetText((std::string)"   Poprawnie :      "+ADD_STATS(0,0)+"      "+ADD_STATS(1,0)+"      "+ADD_STATS(2,0));
	lista_blednych_slowek[4].SetText((std::string)"   B³êdnie :          "+ADD_STATS(0,1)+"      "+ADD_STATS(1,1)+"      "+ADD_STATS(2,1));
	lista_blednych_slowek[6].SetText("  Dziêkujemy za udzia³ w konkursie!");
#undef ADD_STATS
	end=false;
	darker=true;
	kolor=sf::Color(255, LISTBADODP_CONTINUETEXT_COLORMAX, LISTBADODP_CONTINUETEXT_COLORMAX );
	
	linia = 0;
	timer.Reset();
}

void ListBadOdp::Draw(sf::RenderWindow& wnd)
{
	wnd.Draw(kartka);
	for(unsigned int i=0 ; lista_blednych_slowek.size()>i ; ++i)
	{
		lista_blednych_slowek[i].Draw(wnd);
	}
	if( linia >= lista_blednych_slowek.size() )		wnd.Draw(txt_to_continue);
}

bool ListBadOdp::Loop()
{
	if( linia < lista_blednych_slowek.size() )
	{
		if( timer.GetElapsedTime() >= LISTBADODP_TIMENEXTCHAR )
		{
			if( lista_blednych_slowek[linia].NextChar() ) //dodajemy znak
				linia++;		//jeœli wszystkie znaki dodane
			timer.Reset();
		}
	}
	else if( LISTBADODP_CONTINUETEXT_TIMENEXTCOLOR < timer.GetElapsedTime() )
	{
		if(darker)
		{
			if( (kolor.b=kolor.g-=LISTBADODP_CONTINUETEXT_JMP_COLOR) <= LISTBADODP_CONTINUETEXT_COLORMIN )
				darker=false;
			txt_to_continue.SetColor(kolor);
		}
		else
		{
			if( (kolor.b=kolor.g+=LISTBADODP_CONTINUETEXT_JMP_COLOR) >= LISTBADODP_CONTINUETEXT_COLORMAX )
				darker=true;
			txt_to_continue.SetColor(kolor);
		}
		timer.Reset();
	}

	return end;
}


void ListBadOdp::AnyKeyPress()
{
	if( linia >= lista_blednych_slowek.size() ) end= true;
	else end=false;
}