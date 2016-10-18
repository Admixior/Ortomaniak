#pragma once
#include <cstdlib>
//#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>

//---- czêœci sk³adowe okna ----//
#include "UserManager.h"
#include "SchoolManager.h"
#include "ResultsManager.h"
#include "ComputerManager.h"
#include "InfoComputer.h"
#include "Logger.h"
#include "Window.h"

class SerwerUI: public Window
{
public:
	void SaveLogs(std::string mem){logger.PushLog(mem);}
	void HandlingEvent(EventMem*);
	SerwerUI(int id_window);
	~SerwerUI(void);

private:

	////************************
	UserManager usermgr;
	SchoolManager schlmgr;
	ResultsManager resmgr;
	ComputerManager compmgr;
	////************************

	InfoComputer infocomp;
	Logger logger;
};
