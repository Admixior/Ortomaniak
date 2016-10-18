#pragma once
#include "Window.h"

class AddSchoolWindow: public Window
{
	sfg::Box::Ptr main_box,
		edit_box,
		button_box;

	sfg::Button::Ptr button_ok,
		button_cancel;

	sfg::Entry::Ptr school_name;
	
public:
	AddSchoolWindow(int _id_window);
	~AddSchoolWindow(void);

	void ClickAdd();
	void ClickCancel();

	void SendEvent();
};

