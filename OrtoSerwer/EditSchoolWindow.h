#pragma once
#include "Window.h"

class EditSchoolWindow: public Window
{
	sfg::Box::Ptr main_box,
		edit_box,
		button_box;

	sfg::Button::Ptr button_ok,
		button_cancel;

	sfg::Entry::Ptr school_name;
	
	unsigned __int32 sID;

public:
	EditSchoolWindow(int _id_window, int _sID);
	~EditSchoolWindow(void);

	void ClickAdd();
	void ClickCancel();

	void SendEvent();
};

