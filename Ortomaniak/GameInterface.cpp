#include "GameInterface.h"


GameInterface::GameInterface(unsigned int win_h):
	statsgame(Iddle),
	gra(win_h),
	serwer_game(false)
{
	LoadFont(IDR_MASZYNAAEG,font,45);
	wait_to_click.SetFont(font);
	wait_to_click.SetSize(45);
	wait_to_click.SetPosition(1920.f/2.f+65,1080.f/2.f);
	wait_to_click.SetText("Naciœnij dowolny klawisz\n   aby rozpocz¹æ grê!");
	SetCenterText(wait_to_click);
}


GameInterface::~GameInterface(void)
{
}


int GameInterface::MouseDown(sf::Vector2f pos)
{
	switch(statsgame)
	{
	case Wait_for_accept_school:
		school.MouseDown(pos);
		break;
	case User_Config_Game:
		orto.MouseDown(pos);
		break;
	case Overview:
	case Final_Result:
		listbadodp.AnyKeyPress();
		break;
	}
	return 0;
}

void GameInterface::MouseUp()
{
	switch(statsgame)
	{
	case User_Config_Game:
			orto.MouseUp();
			break;
	}
}

int GameInterface::MouseMove(Vector2f pos)
{
	switch(statsgame)
	{
	case Wait_for_accept_school:
		school.MouseMove(pos);
		break;
	case User_Config_Game:
		orto.MouseMove(pos);
		break;
	}
	return 0;
}

int GameInterface::PressKey(unsigned int numkey)
{
	switch(statsgame)
	{
	case Wait_for_user_start_game:
		statsgame=GamePlay;
		counter_words=0;
		//new_odp=true;
		break;
	case GamePlay:
		if(numkey==sf::Key::Left)
			gra.KeyPressed(Game::joy_left);
		else if(numkey==sf::Key::Right)
			gra.KeyPressed(Game::joy_right);
		else if(numkey==sf::Key::Down)
			gra.KeyPressed(Game::joy_down);
		break;
	case Overview:
	case Final_Result:
		listbadodp.AnyKeyPress();
		break;
	}
	return 0;
}

int GameInterface::Loop()
{
	if(serwer_game)
	{
		if(komputer.Interpret()!=0)return ErrorExitGame();
		if(!komputer.GetConnected()) exit(1);
		if(statsgame>=Wait_for_school_list)
		{
			if(!komputer.GetUser())
			{
				ShowErr(NET_ERROR_UNEXPECTED,"GameInterface::Loop()",0,__LINE__);
				return ErrorExitGame();
			}
			if(komputer.GetUser()->Interpret()!=0)return ErrorExitGame();
			if(komputer.GetUser()->GetLogoutSuccess() && statsgame<Final_Result)return ErrorExitGame();
		}
	}
	switch(statsgame)
	{
	case Wait_for_school_list:
		if(komputer.GetUser()->GetListSchoolSuccess())
		{
			waitmsg.SetShow(false);
			school.SetSchool(komputer.GetUser()->GetNextSchoolName());
			school.ShowArrow( komputer.GetUser()->GetCountSchool() > 1 );
			statsgame=Wait_for_accept_school;
		}
		break;

	case Wait_for_accept_school:
		switch(school.GetWork())
		{
		case SchoolAccept::SCHOOLACCEPTEVENT_GETNEXTSCHOOL:
			school.SetSchool( komputer.GetUser()->GetNextSchoolName() );
			break;
		case SchoolAccept::SCHOOLACCEPTEVENT_GETPREVSCHOOL:
			school.SetSchool( komputer.GetUser()->GetPrevSchoolName());
			break;
		case SchoolAccept::SCHOOLACCEPTEVENT_ACCEPTSCHOOL:
			school.StopSelect();
			komputer.GetUser()->SendLogin();
			statsgame=Wait_for_login;
			waitmsg.SetText("Trwa logowanie...");
			waitmsg.SetShow(true);
			break;
		case SchoolAccept::SCHOOLACCEPTEVENT_DENYSCHOOL:
			school.StopSelect();
			KoniecGry();
			return CONF_MSG_SHOW_MENU;
		}
		break;

	case Wait_for_login:
		if(komputer.GetUser()->GetLoginSuccess())
		{
			waitmsg.SetText("Oczekiwanie na rozpoczêcie konkursu");
			stats.SetStateKonkurs((BYTE(*)[2])komputer.GetUser()->GetStats());
			statsgame=Wait_for_serwer_start_game;
			komputer.GetUser()->SendWaitToStartKonkurs();
		}
		break;
		
	case Wait_for_serwer_start_game:
		if(komputer.GetUser()->GetStartKonkursSuccess())
		{
			lista.SetActiveType( komputer.GetUser()->GetConfActiveTypes() );
			gra.SetMaxTime( komputer.GetUser()->GetConfMaxTime() );
			max_words= komputer.GetUser()->GetConfMaxWord();
			statsgame=Wait_for_user_start_game;
			waitmsg.SetShow(false);

			if(stats.NastepnaProba()!=-1)
			{
				gra.ResetGame();
				statsgame=Wait_for_user_start_game;
			}
			else
			{
				BYTE dane[3][2];
				stats.GetStateKonkurs(dane);
				listbadodp.InitializeFinalResults(dane);
				komputer.GetUser()->SendLogout();
				statsgame=Final_Result;
			}
		}

	case User_Config_Game:
		if(orto.GetInitialize())
		{
			if(orto.GetButtonAccept())
			{
				lista.SetActiveType(orto.GetActiveTypes());
				gra.SetMaxTime(orto.GetMaxTime());
				max_words=orto.GetMaxWord();
				statsgame=Wait_for_user_start_game;
				stats.Reset();
			}
			else 
			{
				statsgame=Iddle;
				return CONF_MSG_SHOW_MENU;
			}
		}
		break;

	case GamePlay:
		switch(gra.GetWork())
		{
			case gra.wait_for_word:
			{
				new_odp=true;
				//nadawanie nowego slowka
				if(counter_words>=max_words||gra.GetCountBadOdp()>9)
				{
					if(gra.GetCountBadOdp()>9)
						if(gra.FullRed())
						{
							new_odp=false;
								return 0;
						}
					sf::Sleep(0.5f);
					listbadodp.InitializeBadList(gra.GetListBadWord(),counter_words);
					if(serwer_game)
					{
						stats.GetStateKonkurs( (BYTE(*)[2])komputer.GetUser()->GetStats() );   //GetStateKonkurs copy stats to buffer (to User*)
						komputer.GetUser()->SendStats();
					}
					statsgame=Overview;
					return 0;
				}
				Word* nowe_slowko=lista.GetRandomWord();
				if(nowe_slowko==NULL)
				{
					sf::Sleep(0.5f);
					listbadodp.InitializeBadList(gra.GetListBadWord(),counter_words);
					if(serwer_game)
					{
						stats.GetStateKonkurs( (BYTE(*)[2])komputer.GetUser()->GetStats() );   //GetStateKonkurs copy stats to buffer (to User*)
						komputer.GetUser()->SendStats();
					}
					statsgame=Overview;
					return 0;
				}
				counter_words++;
				gra.NoweSlowko(nowe_slowko);
				break;
			}
			case gra.fly_word:break;
			case gra.good_odp:
			{
				if(new_odp)
				{
					face.god_odp();
					stats.GodAnswer();
					new_odp=false;
				}
			}
			case gra.bad_odp:
			{
				if(new_odp)
				{
					face.bad_odp();
					stats.BadAnswer();
					new_odp=false;
				}
			}
		}
		break;


	case Overview:
		if(!serwer_game)
		{
			if(listbadodp.Loop())
			{
				KoniecGry();
				return CONF_MSG_SHOW_MENU;
			}
		}
		else
		{
			if(listbadodp.Loop())
			{
				if(komputer.GetUser()->GetStatsUploadSuccess())
				{
					if(stats.NastepnaProba()!=-1)
					{
						gra.ResetGame();
						statsgame=Wait_for_user_start_game;
					}
					else
					{
						BYTE dane[3][2];
						stats.GetStateKonkurs(dane);
						listbadodp.InitializeFinalResults(dane);
						komputer.GetUser()->SendLogout();
						statsgame=Final_Result;
					}
				}
				else
				{
					waitmsg.SetText("Trwa wysy³anie wyniku...");
					waitmsg.SetShow(true);
					statsgame=Wait_for_upload_stats;
				}
			}
		}
		break;
	case Wait_for_upload_stats:
		if(komputer.GetUser()->GetStatsUploadSuccess())
		{
			waitmsg.SetShow(false);
			if(stats.NastepnaProba()!=-1)
			{
				gra.ResetGame();
				statsgame=Wait_for_user_start_game;
			}
			else
			{
				BYTE dane[3][2];
				stats.GetStateKonkurs(dane);
				listbadodp.InitializeFinalResults(dane);
				komputer.GetUser()->SendLogout();
				statsgame=Final_Result;
			}
		}
		break;
	case Final_Result:
		if(listbadodp.Loop())
		{
			waitmsg.SetText("Trwa wylogowywanie...");
			if(!komputer.GetUser()->GetLogoutSuccess())waitmsg.SetShow(true);
			statsgame=Wait_for_logout;
		}
		break;
	case Wait_for_logout://it will never happen
		if(komputer.GetUser()->GetLogoutSuccess())
		{
			KoniecGry();
			waitmsg.SetShow(false);
			return CONF_MSG_SHOW_MENU;
		}
	}
	return 0;
}

int GameInterface::StartGame( bool _serwer_game, string firstname, string surname)
{
	if(!(serwer_game=_serwer_game))
	{
		orto.Deinitialize();
		orto.ShowWindowTypes(true);
		statsgame=User_Config_Game;
	}
	else
	{
		stats.SetKonkursType(true);
		komputer.SetUser(new User(firstname,surname));
		school.StartSelect(firstname,surname);
		statsgame=Wait_for_school_list;
		waitmsg.SetShow(true);
		waitmsg.SetText("Trwa pobieranie listy szkó³...");
	}
	return 0;
}

int GameInterface::KoniecGry()
{
	gra.ResetGame();
	new_odp=true;
	statsgame=Iddle;
	serwer_game=false;
	stats.SetKonkursType(false);
	stats.Clear();
	lista.Reset();
	if(serwer_game)
	{
		if(komputer.GetUser())
		{
			delete komputer.GetUser();
			komputer.SetUser(0);
		}
	}
	return 0;
}

int GameInterface::Draw(RenderWindow&wnd)
{
	face.Draw(wnd);
	stats.Draw(wnd);
	switch(statsgame)
	{
	case Iddle:break;
	case Wait_for_school_list:
		school.Draw(wnd);
		waitmsg.Draw(wnd);
		break;
	case Wait_for_accept_school:
		school.Draw(wnd);
		break;
	case User_Config_Game:
		break;
	case Wait_for_user_start_game:
		gra.Draw(wnd);
		wnd.Draw(wait_to_click);
		break;
	case GamePlay:
		gra.Draw(wnd);
		break;
	case Overview:
	case Final_Result:
		listbadodp.Draw(wnd);
		break;

	}
	orto.Draw(wnd);
	waitmsg.Draw(wnd);
	return 0;
}