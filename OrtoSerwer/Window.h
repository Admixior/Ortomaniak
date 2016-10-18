#pragma once
#include "OrtoSerw.h"

//********************************
// Klasa bazowa Window - s³u¿aca
// jak baza do wszystkich okienek
// dziêki dziedziczeniu

class Window
{
protected:
	int id_window;					//identyfikacja okna
	sfg::Window::Ptr window;		//g³ówne okno
	void EraseWindow();				//usuwanie tego okna
public:
	Window(int _id_school);
	virtual ~Window(void);							//wirtualny destruktor
	sfg::Window::Ptr GetWindow(){return window;}	//na potrzeby pulpitu aplikacji
};

