#pragma once
#include "Window.h"

class EditUserWindow: public Window
{
	sfg::Box::Ptr main_box,
		edit_box,
		button_box;

	sfg::Button::Ptr button_ok,
		button_cancel;

	sfg::Entry::Ptr user_firstname,
		user_surname,
		proba[3];
	
	//sfg::ComboBox::Ptr school_combobox;

	std::vector<unsigned __int32> sID_list; //to fast transport local_id to sID

	unsigned __int32 uID;
public:
	EditUserWindow(int id_window, unsigned __int32 uID);
	~EditUserWindow(void);
	
	void ClickAdd();
	void ClickCancel();

	void SendEvent();
};

