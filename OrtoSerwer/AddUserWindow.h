#pragma once
#include "Window.h"

class AddUserWindow: public Window
{
	sfg::Box::Ptr main_box,
		edit_box,
		button_box;

	sfg::Button::Ptr button_ok,
		button_cancel;

	sfg::Entry::Ptr user_firstname,
		user_surname;
	
	sfg::ComboBox::Ptr school_combobox;

	std::vector<unsigned __int32> sID_list; //to fast transport local_id to sID
	
	
public:
	AddUserWindow(int _id_window);
	~AddUserWindow(void);

	void ClickAdd();
	void ClickCancel();

	void SendEvent();
};

