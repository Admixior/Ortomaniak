#pragma once
#include "OrtoSerw.h"
#include <fstream>
/*
Klasa Logger s³u¿y do wyœwietlania aktualnych
logów i równoleg³ym zapisywaniu ich na dysk twardy
*/
class Logger
{
	//wyœwietlanie
	sfg::Box::Ptr main_box;
	sfg::Box::Ptr log_box;

	//zapisywanie
	std::ofstream file_log;
public:
	void PushLog(std::string new_log);
	sfg::Box::Ptr GetBox(){ return main_box;}
	Logger(void);
	~Logger(void);
};

