#pragma once
#include "OrtoSerw.h"

/*
Klasa InfoComputer s�u�y do przechowywania
pude�ka do wy�wietlania z podstawowymi
informacjami (patrz: ni�ej zmienne).

Zmienne poni�sze (extern) musz� zosta� zainicjalizowae przed
u�yciem konstruktora klasy
*/
extern std::string ip_serwer,
	name_serwer,
	port;

class InfoComputer
{
	sfg::Box::Ptr main_box;
public:
	sfg::Box::Ptr GetBox() { return main_box; }
	InfoComputer(void);
	~InfoComputer(void);
};

