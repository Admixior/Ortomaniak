#pragma once
#include "DataBase.h"

class SchoolManager
{
	struct anonymous
	{
		struct School
		{
			unsigned __int32 sID;
			unsigned int ilosc_uczniow;

			sfg::Box::Ptr main_box;
			sfg::CheckButton::Ptr checkbox;
			sfg::Label::Ptr nazwa_szkoly,
				ilosc_osob;

			void CreateSchool(unsigned __int32 sID, std::string& school_name);
			// Get info
			bool GetChecked();

			//-- Change info --
			void AddUser();
			void DeleteUser();
			//edit school
			void ChangeNameSchool(std::string& school_name);
		};
		anonymous();
		~anonymous()
		{
			for( unsigned int i=0; i < all_school.size(); ++i)
				delete all_school[i];
			all_school.clear();
		}

		sfg::Box::Ptr main_box;

		sfg::Label::Ptr static_title;
		sfg::ScrolledWindow::Ptr window_with_school;
		sfg::Box::Ptr box_with_school;

		std::vector< School* > all_school;
		//manage user
		void AddUser( unsigned __int32 sID );
		void DeleteUser( unsigned __int32 sID );
		//manage school
		void AddSchool( unsigned __int32 sID, std::string& school_name );
		void ChangeNameSchool( unsigned __int32 sID, std::string& school_name );
		void DeleteSchool( unsigned __int32 sID);
		// get school to edit
		void GetAllCheckedSchool( std::vector< unsigned __int32 >& ret );
	
		int FindSchoolLocalId(unsigned __int32 sID);
	}MiniDB;

	sfg::Box::Ptr main_box;
public:
	SchoolManager(void);
	~SchoolManager(void);
	int HandlingEvent( EventMem& zdarzenie );
	sfg::Box::Ptr GetHandleToBox(){return main_box;}
	void AddSchool();
	void ChangeSchool();
	void DeleteSchool();
};

