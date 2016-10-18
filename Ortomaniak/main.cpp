//*********************************
//Projekt: Ortomaniak
// Autorzy: Adrian i £ukasz Jeleñ

#include <iostream>

#include "interfejs.h"
#include "ChangeName.h"
#include "Menu.h"
#include "GameInterface.h"
#include "Autorzy.h"
#include "SFML\Graphics\View.hpp"

using namespace sf;

int main(int ilosc,char** parametr,char**)
{
	if(ilosc==4)	//je¿eli gra jest w trybie konkurs
	{
		if( komputer.SetConnOption(inet_addr(parametr[2]),atoi(parametr[1])) == -1 )
			return -1;
		if( komputer.LoginComputer(parametr[3]) )
		{
			cout<<"\nNie mozna sie zalogowac";
			return -1;
		}
		cout<<"\nTrwa logowanie";
		while(!komputer.GetConnected())
		{
			/*if(!komputer.GetConnected())
			{
				cout<<"Komputer zostal rozlaczony";
				return 1;
			}*/
			if(komputer.Interpret())
			{
				cout<<"Blad logowania";
				return 1;
			}
			::Sleep(100);
		}
	}
	Interfejs intf(true, komputer.GetConnected()? komputer.GetEditionText(): (string)"   I I   ");
	ChangeName chgnm;
	Autorzy autorzy;
	Menu menu(POS_MENU_Y);
	menu.AddButton(&std_menu::button_1);
	menu.AddButton(&std_menu::button_2);
	menu.AddButton(&std_menu::button_3);
	menu.AddButton(&std_menu::button_4);
	menu.GetButtonById(0)->SetEnable(komputer.GetConnected());
	//-----------------------------
	RECT rec;
	GetWindowRect(GetDesktopWindow(),&rec);
	RenderWindow wnd(sf::VideoMode( rec.right, rec.bottom, 32 ), "OrtoManiak",sf::Style::Fullscreen);
	wnd.UseVerticalSync(true);
	WindowIsOpen=true;
	
	GameInterface game(wnd.GetHeight());
	//-----
	View kamera(FloatRect(0,0,1920,1080));
	wnd.SetView(kamera);
	//-----------------------------
	Event zdarzenie;
	int button=-1;

	menu.ShowMenu(true);
	while(wnd.IsOpened())
	{
		while(wnd.GetEvent(zdarzenie))
		{
			if(zdarzenie.Type==zdarzenie.Closed)
				wnd.Close();
			/////////////////////////   PRZYCISK MYSZY WCISNIÊTY  /////////////////////
			else if(zdarzenie.Type==zdarzenie.MouseButtonPressed)
			{
				if(autorzy.GetShow())
				{
					autorzy.Show(false);
					menu.ShowMenu(true);
					continue;		//nastêpny komunikat
				}
				sf::Vector2f MousePos = wnd.ConvertCoords(zdarzenie.MouseButton.X, zdarzenie.MouseButton.Y);
				if(game.GetWork())game.MouseDown(MousePos);
				else if(!chgnm.Click(MousePos.x,MousePos.y))
					if(menu.GetShowMenu())
					{
						if((button=menu.MouseClick(MousePos))!=-1)
							menu.ShowMenu(false);
					}
			}
			else if(zdarzenie.Type==zdarzenie.MouseButtonReleased)
			{
				if(game.GetWork())game.MouseUp();
			}
			//////////////////////////////   RUCH MYSZK¥  /////////////////////////////
			else if(zdarzenie.Type==zdarzenie.MouseMoved)
			{
				sf::Vector2f MousePos = wnd.ConvertCoords(zdarzenie.MouseMove.X, zdarzenie.MouseMove.Y);
				if(chgnm.Work());
				else if(menu.GetShowMenu())	menu.MouseMove(MousePos);
				else if(game.GetWork())	game.MouseMove(MousePos);
			}
			////////////////////////////   TEXT WPROWADZONY ///////////////////////////
			else if(zdarzenie.Type==zdarzenie.TextEntered)
			{
				chgnm.AddChar(ANSItoASCI(zdarzenie.Text.Unicode));
			}
			////////////////////////    INNY KLAWISZ NACISNIETY   ////////////////////
			else if(zdarzenie.Type==zdarzenie.KeyPressed)
			{
				if(autorzy.GetShow())
				{
					autorzy.Show(false);
					menu.ShowMenu(true);
					continue;	//nastêpny komunikat
				}
				if(chgnm.Work())
				{
					if(zdarzenie.Key.Code==sf::Key::Return||
						zdarzenie.Key.Code==sf::Key::Tab)chgnm.TabOrEnterPressed();
					if(zdarzenie.Key.Code==sf::Key::Back)chgnm.DeleteChar();
					if(zdarzenie.Key.Code==sf::Key::Escape)chgnm.Deinitialize();
				}
				else if(game.GetWork())
				{
					game.PressKey(zdarzenie.Key.Code);
				}
			}
		}

		////////////////////////////  KOMUNIKATY  PRZETWORZONE  ///////////////////////
		//////  NOWE IMIE
		
		static bool rozpocznij_konkurs=false;
		if(chgnm.GetSuccessEntered())
		{
			string imie,nazwisko;
			chgnm.GetName(imie,nazwisko);
			intf.SetName(imie,nazwisko);
			if(rozpocznij_konkurs)button=0;
		}
		////  PRZYCISK W MENU NACISNIÊTY
		if(button!=-1&&menu.GetHideMenu())
		{
			switch(button)
			{
			case 0:
				if(komputer.GetConnected())
				{
					string imie,nazwisko;
					chgnm.GetName(imie,nazwisko);
					if(imie=="Imiê" || nazwisko=="Nazwisko")
					{
						chgnm.Initialize();
						rozpocznij_konkurs=true;
					}
					else game.StartGame(true,imie,nazwisko);
				}
				break;
			case 1:
				game.StartGame(false);
				break;
			case 2:
				autorzy.Show(true);
				break;
			case 3:
				komputer.Logout();
				while(komputer.GetConnected())
				{
					if(komputer.Interpret())break;
					sf::Sleep(0.1f);
				}
				wnd.Close();
			}
			button=-1;
		}
		
		if(game.Loop())menu.ShowMenu(true);
		////////////////////////////////   RYSOWANIE   ///////////////////////////////
		wnd.Clear();
		intf.DrawTo(wnd);
		menu.Draw(wnd);
		game.Draw(wnd);
		chgnm.Draw(wnd);
		autorzy.Draw(wnd);
		wnd.Display();
	}
	return 0;
}