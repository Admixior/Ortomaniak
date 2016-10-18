#pragma once
#include "OrtoSerw.h"

/*
Klasa User:
Czêœæ bazy danych zawieraj¹ca informacje o userach.
Przechowuje tylko imiê i nazwisko oraz ile punktów ma ta osoba.

Udostêpnia sprawdzenie zgodnoœci imienie i nazwiska z baz¹,
zmianê danych, oraz ustawieniu nowej iloœci punktów zgodnie z
zasad¹. A tak¿e pobieranie danych o u¿ytkowniku.

Zasada jest jedna - nie mo¿na zmieniæ isitniej¹cych ju¿ punktów

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

