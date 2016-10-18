#pragma once
#include "OrtoSerw.h"

/*
Klasa User:
Cz�� bazy danych zawieraj�ca informacje o userach.
Przechowuje tylko imi� i nazwisko oraz ile punkt�w ma ta osoba.

Udost�pnia sprawdzenie zgodno�ci imienie i nazwiska z baz�,
zmian� danych, oraz ustawieniu nowej ilo�ci punkt�w zgodnie z
zasad�. A tak�e pobieranie danych o u�ytkowniku.

Zasada jest jedna - nie mo�na zmieni� isitniej�cych ju� punkt�w

*/

class User
{
public:
	User(std::string imie, std::string nazwisko);
	~User(void);

	// return true if name and surname is equal
	bool CheckName(const std::string& imie, const std::string& nazwisko);

	// set new name imie and nazwisko
	void SetNewName(const std::string& imie, const std::string& nazwisko);

	// set stats if succes return true
	bool SetStats(BYTE wyniki[3]);

	// copy the stats to wyniki
	bool GetStats(BYTE wyniki[3]);

	// copy the firstname and surname
	bool GetName(std::string& imie, std::string& nazwisko);
private:
	std::string firstname,
		surname;
	BYTE wyniki[3];
};

