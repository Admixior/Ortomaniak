#pragma once
#include "DataBase.h"


class UserManager
{
	/////////////////////////////////////////////////////////////////////////////////
	struct anonymous
	{
		//**************************************************
		struct School
		{
			//------------------------
			struct User
			{
				unsigned __int32 uID;	// use this to localize user

				sfg::CheckButton::Ptr Gcheck;
				sfg::Label::Ptr Gimie,
					Gnazwisko;
				sfg::Image::Ptr Gconnected;
				sfg::Box::Ptr main_box;

				void CreateUser(unsigned __int32 uID, std::string& imie, std::string& nazwisko); //use this before all
				void ChangeName(std::string& imie, std::string& nazwisko);
				bool GetChecked();
				void ChangeConnected(bool connect);
			};
			//------------------------

			unsigned __int32 sID;

			sfg::Box::Ptr main_box;
			sfg::Box::Ptr user_box;
			sfg::ToggleButton::Ptr button_toggle;
			std::vector< User* > all_user;

			void CreateSchool(unsigned __int32 sID, std::string& schoolname);
			void ChangeSchoolName( std::string& schoolname );

			void AddUser( unsigned __int32 uID,std::string& imie, std::string& nazwisko);
			void ChangeUserName(unsigned __int32 uID, std::string& imie, std::string& nazwisko);
			void DeleteUser ( unsigned __int32 uID);
			void ChangeUserLogged(unsigned __int32 uID, bool login);	//true if user has logged

			void PushSelectedUseruID( std::vector< unsigned __int32 >& ret);
			void ToggleButtonChange();

			// this function uses function on the top
			int  FindUserLocalId(unsigned __int32 uID);	//return -1 if uID doesn't belong to this school
			~School()
			{
				for( unsigned int i=0; i < all_user.size(); ++i)
					delete all_user[i];
				all_user.clear();
			}
		};
		//**************************************************

		sfg::Box::Ptr main_box;

		sfg::Label::Ptr static_title;
		sfg::ScrolledWindow::Ptr window_with_user;
		sfg::Box::Ptr box_with_user;

		std::vector< School* > all_school;
		
		//---single manage
		//school
		void AddSchool( unsigned __int32 sID, std::string& nazwa_szkoly);
		void ChangeSchoolName( unsigned __int32 sID, std::string& nazwa_szkoly);
		void DeleteSchool( unsigned __int32 sID );
		//user
		void AddUser(unsigned __int32 sID,  unsigned __int32 uID, std::string& imie, std::string& nazwisko);
		void ChangeUserName(unsigned __int32 sID, unsigned __int32 uID, std::string& imie, std::string& nazwisko);
		void DeleteUser(unsigned __int32 sID, unsigned __int32 uID);
		void ChangeUserHasLogged(unsigned __int32 sID, unsigned __int32 uID, bool login);	//true if user has logged
		void GetAllSelectedUseruID( std::vector< unsigned __int32 >& ret);
		//multi manage
		//void AddAllSchool( std::vector< ShortSchoolInfo >& schools );
		//void AddAllUser( std::vector< LongInfoUser >& users );

		int FindSchoolLocalId(unsigned __int32 sID);

		anonymous();
		~anonymous()
		{
			for( unsigned int i=0; i < all_school.size(); ++i)
				delete all_school[i];
			all_school.clear();
		}
		/////////////////////////////////////////////////////////////////////////////////
	}MiniDB;
	sfg::Box::Ptr main_box;
public:
	UserManager(void);
	~UserManager(void);
	int HandlingEvent( EventMem& zdarzenie );
	sfg::Box::Ptr GetHandleToBox(){return main_box;}
	void AddUser();
	void DeleteUser();
	void ChangeUser();
};

