#pragma once
#include "OrtoSerw.h"

//********************************
// Klasa bazowa Window - s�u�aca
// jak baza do wszystkich okienek
// dzi�ki dziedziczeniu

class Window
{
protected:
	int id_window;					//identyfikacja okna
	sfg::Window::Ptr window;		//g��wne okno
	void EraseWindow();				//usuwanie tego okna
public:
	Window(int _id_school);
	virtual ~Window(void);							//wirtualny destruktor
	sfg::Window::Ptr GetWindow(){return window;}	//na potrzeby pulpitu aplikacji
};

