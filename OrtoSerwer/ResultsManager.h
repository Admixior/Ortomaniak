#pragma once
#include "DataBase.h"

class ResultsManager
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
				unsigned __int32 uID;	//use this to localize the user
				BYTE stats[3];
				//graphics variables
				sfg::Box::Ptr main_box;
				sfg::Label::Ptr Gid,	//uID
					Gimie,
					Gnazwisko,
					Gwyniki[3];

				void CreateUser(unsigned __int32 uID ,std::string& imie, std::string& nazwisko, BYTE stats[3]);
				void ChangeName( std::string& imie, std::string& nazwisko);
				void ChangeStats(BYTE stats[3]);
			};
			//------------------------

			unsigned __int32 sID;	//use this to localize the school
			
			//graphics variables
			sfg::Box::Ptr main_box;
			sfg::Label::Ptr Gid, //sID
				Gnazwa_szkoly,
				Gwyniki[3];		//summa summarum

			std::vector< User* > all_user;

			void CreateSchool(unsigned __int32 sID, std::string& nazwa_szkoly );
			void ChangeSchoolName( std::string& nazwa_szkoly );
			void AddUser( unsigned __int32 uID,std::string& imie, std::string& nazwisko, BYTE stats[3]);
			void DeleteUser ( unsigned __int32 uID);
			void ChangeUserStats(unsigned __int32 uID, BYTE stats[3]);
			void ChangeUserName(unsigned __int32 uID, std::string& imie, std::string& nazwisko);
			void RefreshStats();
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
		anonymous();
		~anonymous()
		{
			for( unsigned int i=0; i < all_school.size(); ++i)
				delete all_school[i];
			all_school.clear();
		}

		sfg::Box::Ptr main_box;

		sfg::Label::Ptr static_title;
		sfg::ScrolledWindow::Ptr window_with_stats;
		sfg::Box::Ptr box_with_stats;

		std::vector< School* > all_school;
		
		//---single manage
		//school
		void AddSchool( unsigned __int32 sID, std::string& nazwa_szkoly);
		void ChangeSchoolName( unsigned __int32 sID, std::string& nazwa_szkoly);
		void DeleteSchool( unsigned __int32 sID );
		//user
		void AddUser(unsigned __int32 sID,  unsigned __int32 uID,std::string& imie, std::string& nazwisko, BYTE stats[3]);
		void ChangeUserName(unsigned __int32 sID, unsigned __int32 uID, std::string& imie, std::string& nazwisko);
		void DeleteUser (unsigned __int32 sID, unsigned __int32 uID);
		void ChangeUserStats(unsigned __int32 sID, unsigned __int32 uID, BYTE stats[3]);

		//---multi manage
		//void AddAllSchool( std::vector< ShortSchoolInfo >& schools );
		//void AddAllUser( std::vector< LongInfoUser >& users );
		
		//------
		int FindSchoolLocalId(unsigned __int32 sID);
		/////////////////////////////////////////////////////////////////////////////////
	}MiniDB;

	sfg::Box::Ptr main_box;

public:
	ResultsManager(void);
	~ResultsManager(void);
	int HandlingEvent( EventMem& zdarzenie );
	sfg::Box::Ptr GetHandleToBox(){return main_box;}
	void ZapiszListe();
	void WczytajListe();
};

