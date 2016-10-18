#pragma once
#include "SerwerUI.h"
#include "AddSchoolWindow.h"
#include "AddUserWindow.h"
#include "EditSchoolWindow.h"
#include "EditUserWindow.h"
#include "SimpleMessageBox.h"
/************************************************
GUIDesktop - G³ówna klasa obs³uguj¹ca komunikaty
daj¹ca ¿ycie oknu, ³¹cz¹ca bazê danych internet
i interfejs graficzny w jedn¹ logiczn¹ (?)(!)
(mam nadzieje) ca³oœæ .........................
************************************************/

extern bool window_is_open;

class Desktop
{
	std::vector<Window*> windows;

	sfg::SFGUI m_sfgui;
	sfg::Desktop m_desktop;

	sf::Vector2f res;
	sf::Texture background_texture;
	sf::Sprite background;

	SerwerUI* serwer_win;
	//	Dyspozycja komunikatów
	int LoopEvents();

	int BaseEvent(EventMem*);
	int SystemEvent(EventMem*);
	int NetWorkTask(EventMem*);
	int NetWorkEvent(EventMem*);

	void CenterWindow(Window* win);

	//poboczne funkcje
	int RejestracjaKompa(NetPacket* msg, unsigned int cID);
	int WyrejestrujKompa(NetPacket* msg, unsigned int cID);
	int PobierzListeSzkol(NetPacket* msg, unsigned int cID);
	int ZalogujUzytkownika(NetPacket* msg, unsigned int cID);
	int WylogujUzytkownika(NetPacket* msg, unsigned int cID);
	int ZmienWyniki(NetPacket* msg, unsigned int cID);
	int StartKonkursu(NetPacket* msg, unsigned int cID);
public:
	Desktop(void);
	~Desktop(void);

	int RunServer();
};
