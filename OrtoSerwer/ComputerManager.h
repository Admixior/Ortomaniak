#pragma once
#include "OrtoSerw.h"

/*
Klasa ComputerManager s³u¿y do przegl¹dania aktywnych po³¹czeñ,
od³¹czania komputerów, od³¹czania u¿ytkowników zwi¹zanych z komputerem,
zezwalania na konkurs
*/


class ComputerManager
{
	struct anonymous
	{
		struct computer
		{
			unsigned __int32 cID;
			sfg::CheckButton::Ptr check;
			sfg::Label::Ptr nazwa_kompa,
				uzytkownik,
				status;
			sfg::Box::Ptr main_box;

			//***********************************
			void CreateComputer(unsigned __int32 cID, std::string& name);
			void ConnectedUser(std::string& username);
			void SetStatusUser(int status);
			bool GetChecked();
		};
		//**************************************************
		anonymous();
		~anonymous();
		sfg::Box::Ptr main_box;

		sfg::Label::Ptr static_title;
		sfg::ScrolledWindow::Ptr window_with_computer;
		sfg::Box::Ptr box_with_computer;

		std::vector< computer* > computer_list;
		//-- manage --
		void NewComputerConnected(unsigned __int32 cID, std::string& name);
		void ConnectedUser(unsigned __int32 cID, std::string& username);
		void SetStatusUser(unsigned __int32 cID, int status);
		void DisconnectComputer(unsigned __int32 cID);

		//---- 
		void CreateListWithCheckedComputers(std::vector< unsigned __int32 >& new_computer_list);//the name speaks for itself :)
		int FindLocalComputerID(unsigned __int32 cID);
	}MiniDB;

	sfg::Box::Ptr main_box;
public:
	ComputerManager(void);
	~ComputerManager(void);
	void LogoutUser();
	void DisconnectComputer();
	void StartGame();
	int HandlingEvent( EventMem& zdarzenie );
	sfg::Box::Ptr GetHandleToBox(){return main_box;}
};

