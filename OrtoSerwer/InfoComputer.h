#pragma once
#include "OrtoSerw.h"

/*
Klasa InfoComputer s³u¿y do przechowywania
pude³ka do wyœwietlania z podstawowymi
informacjami (patrz: ni¿ej zmienne).

Zmienne poni¿sze (extern) musz¹ zostaæ zainicjalizowae przed
u¿yciem konstruktora klasy
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

