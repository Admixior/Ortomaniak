#pragma once
#include "Window.h"

class SimpleMessageBox: public  Window
{
	_callbackSimple callback;
public:
	SimpleMessageBox(int id_window, std::wstring message, int flag, _callbackSimple callback);
	~SimpleMessageBox(void);
	void ClickYes();
	void ClickNo();
	void ClickOK();
};

