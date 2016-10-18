#pragma once
#include "OrtoSerw.h"
#include <fstream>
/*
Klasa Logger s�u�y do wy�wietlania aktualnych
log�w i r�wnoleg�ym zapisywaniu ich na dysk twardy
*/
class Logger
{
	//wy�wietlanie
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

