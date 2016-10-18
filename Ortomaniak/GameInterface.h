#pragma once
#include "Game.h"
#include "ListWord.h"
#include "face.h"
#include "Statystyki.h"
#include "OrtoConf.h"
#include "ListBadOdp.h"
#include "SchoolAccept.h"
#include "WaitForSerwer.h"

class GameInterface
{
	//interface
	Statystyki stats;
	Face face;
	ListWord lista;
	OrtoConf orto;
	Game gra;
	ListBadOdp listbadodp;
	SchoolAccept school;
	WaitForSerwer waitmsg;

	//own-interface
	String wait_to_click;
	Font font;

	//sterowanie gr¹
	enum
	{
		Iddle,
		Wait_for_school_list,
		Wait_for_accept_school,
		Wait_for_login,
		Wait_for_serwer_start_game,
		User_Config_Game,
		Wait_for_user_start_game,
		GamePlay,
		Overview,
		Wait_for_upload_stats,
		Final_Result,
		Wait_for_logout
	}statsgame;

	bool serwer_game;
	unsigned int counter_words,max_words;
	bool new_odp;
public:
	int StartGame( bool serwer_game, string firstname="", string surname="");
	int GetWork(){ return statsgame;}
	int Loop();
	int KoniecGry();
	int Draw(RenderWindow&wnd);
	////---   I N P U T  ---////
	void MouseUp();
	int MouseDown(Vector2f pos);
	int MouseMove(Vector2f pos);
	int PressKey(unsigned int numkey);
	////--- /END/ INPUT  ---////

	////-- ERRORGAME --
	int ErrorExitGame()
	{
		waitmsg.SetShow(false);
		KoniecGry();
		return CONF_MSG_SHOW_MENU;
	}
	GameInterface(unsigned int windows_height);
	~GameInterface(void);
};

