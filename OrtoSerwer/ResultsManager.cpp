#include "ResultsManager.h"
#include <fstream>

ResultsManager::ResultsManager(void)
{
	main_box=sfg::Box::Create(sfg::Box::VERTICAL, 5.f);
	main_box->Pack(MiniDB.main_box);

	//add buttons
	sfg::Box::Ptr button_box( sfg::Box::Create(sfg::Box::HORIZONTAL, 15.f));
	{
		sfg::Button::Ptr button(sfg::Button::Create( L"Zapisz wyniki" ));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &ResultsManager::ZapiszListe, this );
		button_box->Pack(button);
	}
	{
		sfg::Button::Ptr button(sfg::Button::Create( L"Wczytaj listê osób" ));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &ResultsManager::WczytajListe, this );
		button_box->Pack(button);
	}
	/*{
		sfg::Button::Ptr button(sfg::Button::Create( L"Szybka kopia bezpieczeñstwa" ));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &ResultsManager::Backup, this );
		button_box->Pack(button);
	}*/
	main_box->Pack( button_box ,false);
}


ResultsManager::~ResultsManager(void)
{
	
}


//-------------------------------------------------------------//
//***********************    U S E R   ************************//
void ResultsManager::anonymous::School::User::CreateUser(unsigned __int32 _uID ,std::string& imie, std::string& nazwisko, BYTE _stats[3])
{
	Gid=sfg::Label::Create( L"    Uczeñ: "+std::to_wstring( (long long int) (uID=_uID) ));
	Gimie=sfg::Label::Create( stringToWstring(imie ));
	Gnazwisko=sfg::Label::Create(stringToWstring( nazwisko ) );
	for(int i=0; i<3; ++i)
	{
		if((stats[i]=_stats[i])!=255)	Gwyniki[i]=sfg::Label::Create( std::to_string((long long)stats[i]) );
		else Gwyniki[i]=sfg::Label::Create("-");
	}
	
	//pakowanie
	main_box=sfg::Box::Create(sfg::Box::HORIZONTAL, 7.f);
	main_box->Pack(Gid, false);
	main_box->Pack(Gimie, false);
	main_box->Pack(Gnazwisko, false);

	sfg::Alignment::Ptr alignment = sfg::Alignment::Create();
	sfg::Box::Ptr box(sfg::Box::Create());

	box->Pack(sfg::Label::Create(L"   Wyniki:"));
	for(int i=0; i<3; ++i)
	{
		box->Pack(sfg::Label::Create("   "));
		box->Pack( Gwyniki[i] );
	}

	alignment->Add(box);
	alignment->SetScale( sf::Vector2f( .0f, .0f ) );
	alignment->SetAlignment( sf::Vector2f( 1.f, 1.f ) );

	main_box->Pack(alignment);
}

void ResultsManager::anonymous::School::User::ChangeName( std::string& imie, std::string& nazwisko)
{
	Gimie->SetText(stringToWstring(imie));
	Gnazwisko->SetText(stringToWstring(nazwisko));
}

void ResultsManager::anonymous::School::User::ChangeStats(BYTE _stats[3])
{
	for(int i=0; i<3; ++i)
		Gwyniki[i]->SetText( (stats[i]=_stats[i])==255?"-":std::to_string((long long)stats[i]) );
}


//-------------------------------------------------------------//
//********************   S C H O O L   ************************//
void ResultsManager::anonymous::School::CreateSchool(unsigned __int32 _sID, std::string& _nazwa_szkoly )
{
	Gid=sfg::Label::Create( L"Szko³a ID: "+std::to_wstring((long long) (sID=_sID) ));
	Gnazwa_szkoly=sfg::Label::Create( stringToWstring(_nazwa_szkoly) );
	for( int i=0; i<3; ++i)
	{
		Gwyniki[i]=sfg::Label::Create( "-" );
	}

	//pakowanie
	sfg::Box::Ptr box=sfg::Box::Create( sfg::Box::HORIZONTAL , 5.f);
	box->Pack( Gid , false);
	box->Pack( Gnazwa_szkoly ,false);

	
	sfg::Alignment::Ptr alignment = sfg::Alignment::Create();
	sfg::Box::Ptr box_alg(sfg::Box::Create());

	box_alg->Pack(sfg::Label::Create(L"   Wyniki:"));
	for(int i=0; i<3; ++i)
	{
		box_alg->Pack(sfg::Label::Create("   "));
		box_alg->Pack( Gwyniki[i] );
	}

	alignment->Add(box_alg);
	alignment->SetScale( sf::Vector2f( .0f, .0f ) );
	alignment->SetAlignment( sf::Vector2f( 1.f, 1.f ) );

	box->Pack(alignment);

	main_box=sfg::Box::Create(sfg::Box::VERTICAL,5.f);
	main_box->Pack(box,false);
}

void ResultsManager::anonymous::School::ChangeSchoolName( std::string& nazwa_szkoly )
{
	Gnazwa_szkoly->SetText(stringToWstring(nazwa_szkoly) );
}

void ResultsManager::anonymous::School::AddUser( unsigned __int32 uID,std::string& imie, std::string& nazwisko, BYTE stats[3])
{
	all_user.push_back(new User());
	(*(all_user.end()-1))->CreateUser(uID, imie, nazwisko, stats);
	main_box->Pack((*(all_user.end()-1))->main_box,false);
	RefreshStats();
}

void ResultsManager::anonymous::School::DeleteUser ( unsigned __int32 uID)
{
	int local_id=FindUserLocalId(uID);
	if(local_id==-1)return;	//very fatal error
	main_box->Remove(all_user[local_id]->main_box);
	delete all_user[local_id];
	all_user.erase(all_user.begin()+local_id);
	RefreshStats();
}

void ResultsManager::anonymous::School::ChangeUserStats(unsigned __int32 uID, BYTE stats[3])
{
	int local_id=FindUserLocalId(uID);
	if(local_id==-1)return;	//very fatal error
	all_user[local_id]->ChangeStats(stats);
	RefreshStats();
}

void ResultsManager::anonymous::School::ChangeUserName(unsigned __int32 uID, std::string& imie, std::string& nazwisko)
{
	int local_id=FindUserLocalId(uID);
	if(local_id==-1)return;	//very fatal error
	all_user[local_id]->ChangeName(imie,nazwisko);
}

int ResultsManager::anonymous::School::FindUserLocalId(unsigned __int32 uID)
{
	for(unsigned int i=0; i<all_user.size(); ++i)
		if(all_user[i]->uID==uID)return i;
	return -1;
}

void ResultsManager::anonymous::School::RefreshStats()
{
	long int new_stats[3]={-1,-1,-1};

	for( unsigned int i=0; i<3; ++i )
	{
		for( unsigned int m=0; m<all_user.size(); ++m )
		{
			int x=all_user[m]->stats[i];
			if(x!=255)
			{
				if(new_stats[i]==-1)++new_stats[i];
				new_stats[i]+=x;
			}
		}
	}

	for( unsigned int i=0; i<3; ++i )
	{
		if(new_stats[i]!=-1)		Gwyniki[i]->SetText( std::to_string((long long)new_stats[i]) );
		else Gwyniki[i]->SetText("-");
	}
}


//-------------------------------------------------------------//
//********************   M I N I  D B   ***********************//

ResultsManager::anonymous::anonymous()
{
	//title
	static_title=sfg::Label::Create( TEXT_TITLE_RESULTS_MANAGER );
	//scrolled box with school
	box_with_stats=sfg::Box::Create(sfg::Box::VERTICAL, 2.f);
	box_with_stats->SetRequisition(sf::Vector2f(480.f, 0.f));
	window_with_stats=sfg::ScrolledWindow::Create();
	window_with_stats->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
	window_with_stats->AddWithViewport(box_with_stats);
	window_with_stats->SetRequisition( sf::Vector2f( 500.f, 100.f ) );
	//pack
	main_box=sfg::Box::Create(sfg::Box::VERTICAL, 3.f);
	main_box->Pack(static_title,false);
	main_box->Pack(window_with_stats);
}


void ResultsManager::anonymous::AddSchool( unsigned __int32 sID, std::string& nazwa_szkoly)
{
	all_school.push_back(new School());
	(*(all_school.end()-1))->CreateSchool(sID, nazwa_szkoly);
	box_with_stats->Pack( (*(all_school.end()-1))->main_box);
}


void ResultsManager::anonymous::ChangeSchoolName( unsigned __int32 sID, std::string& nazwa_szkoly)
{
	int local_id=FindSchoolLocalId(sID);
	if( local_id==-1) return;
	all_school[local_id]->ChangeSchoolName(nazwa_szkoly);
}


void ResultsManager::anonymous::DeleteSchool( unsigned __int32 sID )
{
	int local_id=FindSchoolLocalId(sID);
	if( local_id==-1) return;
	box_with_stats->Remove( all_school[local_id]->main_box );
	delete all_school[local_id];
	all_school.erase(all_school.begin()+local_id);
}


//--------

void ResultsManager::anonymous::AddUser(unsigned __int32 sID,  unsigned __int32 uID,std::string& imie, std::string& nazwisko, BYTE stats[3])
{
	int local_id=FindSchoolLocalId(sID);
	if( local_id==-1) return;
	all_school[local_id]->AddUser(uID, imie, nazwisko, stats);
}


void ResultsManager::anonymous::ChangeUserName(unsigned __int32 sID, unsigned __int32 uID, std::string& imie, std::string& nazwisko)
{
	int local_id=FindSchoolLocalId(sID);
	if( local_id==-1) return;
	all_school[local_id]->ChangeUserName(uID, imie, nazwisko);
}


void ResultsManager::anonymous::DeleteUser(unsigned __int32 sID, unsigned __int32 uID)
{
	int local_id=FindSchoolLocalId(sID);
	if( local_id==-1) return;
	all_school[local_id]->DeleteUser(uID);
}


void ResultsManager::anonymous::ChangeUserStats(unsigned __int32 sID, unsigned __int32 uID, BYTE stats[3])
{
	int local_id=FindSchoolLocalId(sID);
	if( local_id==-1) return;
	all_school[local_id]->ChangeUserStats(uID, stats);
}


//--------
int ResultsManager::anonymous::FindSchoolLocalId(unsigned __int32 sID)
{
	for( unsigned int i=0; i<all_school.size(); ++i)
		if(all_school[i]->sID==sID)return i;
	return -1;
}


//*************************************************************
//----------------------    M A I N    ------------------------

int ResultsManager::HandlingEvent( EventMem& zdarzenie )
{
	switch(zdarzenie.type)
	{

	case EVENT_CREATE_USER:
		{
			LongInfoUser* info = (LongInfoUser*)zdarzenie.data;
			MiniDB.AddUser( info->sID, info->uID, info->imie, 
				info->nazwisko, info->wyniki);
		}
		break;

	case EVENT_CHANGE_USER_NAME:
		{
			LongInfoUser* info = (LongInfoUser*)zdarzenie.data;
			MiniDB.ChangeUserName( info->sID, info->uID, info->imie, info->nazwisko);
		}
		//break; :p

	case EVENT_CHANGE_STATS:
		{
			LongInfoUser* info = (LongInfoUser*)zdarzenie.data;
			MiniDB.ChangeUserStats( info->sID, info->uID, info->wyniki);
		}
		break;

	case EVENT_DELETE_USER:
		{
			LongInfoUser* info = (LongInfoUser*)zdarzenie.data;
			MiniDB.DeleteUser( info->sID, info->uID);
		}
		break;

	case EVENT_CREATE_SCHOOL:
		{
			ID_Name* info = (ID_Name*)zdarzenie.data;
			MiniDB.AddSchool( info->id, info->name );
		}
		break;

	case EVENT_CHANGE_SCHOOL_NAME:
		{
			ID_Name* info = (ID_Name*)zdarzenie.data;
			MiniDB.ChangeSchoolName(info->id, info->name);
		}
		break;

	case EVENT_DELETE_SCHOOL:
		{
			ID_Name* info = (ID_Name*)zdarzenie.data;
			MiniDB.DeleteSchool( info->id );
		}
		break;

	}

	return 0;
}

void ResultsManager::ZapiszListe()
{
	std::ofstream outfile("wyniki.txt");
	if(outfile.bad())
	{
		ShowError("Nie mo¿na otworzyæ pliku aby zapisaæ wyniki. Zamknij inne programy korzystaj¹ce z tego pliku.", GetLastError());
		return;
	}
	SYSTEMTIME syst;
	GetLocalTime(&syst);
	outfile<<";************************************\n"
			 ";Lista wyników uczniów w konkursie.  \n"
			 ";Utworzono:"+std::to_string((long long) syst.wDay)+"."+std::to_string((long long) syst.wMonth)+"."+std::to_string((long long) syst.wYear)+" "+std::to_string((long long) syst.wHour)+":"+std::to_string((long long) syst.wMinute)+":"+std::to_string((long long) syst.wSecond)+"\n\n";

	std::vector<ID_Name> lista_szkol;
	PublicDB.GetListOfSchool(lista_szkol);
	for(size_t x=0; x < lista_szkol.size(); ++x)
	{
		std::vector<LongInfoUser> user_info;
		if(PublicDB.GetFullInfoUserFromSchool(lista_szkol[x].id,user_info)==-1)
		{
			ShowError("Wyst¹pi³ b³¹d wewnêtrzny",2000);
			continue;
		}

		outfile<<"#"<<lista_szkol[x].name<<"\n";

#define WRITE_RESULT(x,y) ((user_info[(x)].wyniki[(y)]==255)?((std::string)"-"):(std::to_string((long long)user_info[(x)].wyniki[(y)])))

		for(size_t i=0; i < user_info.size(); ++i)
			outfile<<user_info[i].imie<<" "<<user_info[i].nazwisko<<"\t"
				<<WRITE_RESULT(i,0)<<"\t"<<WRITE_RESULT(i,1)<<"\t"<<WRITE_RESULT(i,2)<<"\n";

#undef WRITE_RESULT

		outfile<<"\n\n";
	}

	//so this is the end
	outfile.close();

	ShowError("Zapisano do pliku \"wyniki.txt\"!");
}

std::string GetNextLine(std::ifstream &file_in, unsigned int& line_number)
{
	std::string ret;
	do
	{
		getline(file_in, ret);
		if(file_in.eof())return "";
	}while((!ret.size()) || (ret[0]==';') || (ret.find_first_of("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789")==std::string::npos));
	return ret;
}


static inline void trim(std::string &s)
{
	if(!s.size())return;
	//don't change sequence (if you don't know what you do)
	//erase from end
	s.erase(s.find_last_not_of(" \n\r\t")+1);//if string has only spaces "find_last_not_of" return -1 and we add 1
	if(!s.size())return;
	//erase from start (in string must be at least one normal character)
	s.erase(0, s.find_first_not_of(" \n\r\t"));
}

void ResultsManager::WczytajListe()
{
	std::ifstream file_in("wyniki.txt");
	if(!file_in.good())
	{
		ShowError("Nie mo¿na otworzyæ pliku \"wyniki.txt\".",1);
		return;
	}
	unsigned int line_number=0;
	std::string line=GetNextLine(file_in, line_number);
	if(!line.size())
	{
		ShowError("B³edny plik \"wyniki.txt\".\nPlik jest pusty.",2);
		return;
	}
	if(line[0]!='#')
	{
		ShowError("B³edny plik wyniki.txt.\nPlik powinien siê zaczynaæ od nazwy szko³y.",3);
		return;
	}
	std::vector<ID_Name> school;
	PublicDB.GetListOfSchool(school);
	for( unsigned int i = 0; i < school.size(); ++i)
	{
		ID_Name* send=new ID_Name;
		
		if(!PublicDB.DeleteSchool(school[i].id))
		{
			ShowError("Nie mo¿na usun¹æ szko³y: " + school[i].name + ".\nPrawdopodobnie u¿ytkownik jest zalogowany");
			continue;
		}
		send->id=school[i].id;
		EventMem* em=new EventMem;
		em->type= EVENT_DELETE_SCHOOL;
		em->data=send;
		event.push(em);
	}
	
	unsigned __int32 last_sID=-1;
	do
	{
		if(line[0]=='#')
		{
			//przygotowanie
			line.erase(0,1);
			trim(line);
			
			if(line.size()==0)//whops
			{
				line="Nienazwany #" + std::to_string((long long)last_sID);
				ShowError("Uwaga! W pliku wystepuje nienazwana szko³a.\nOznaczona jako: "+line,0);
			}

			//dodajemy do bazy
			last_sID=PublicDB.AddSchool(line);
			if(last_sID==-1)
			{
				ShowError("W pliku wystepuj¹ szko³y o tych samych nazwach:"+line+"\nZosta³y one po³¹czone.");
				last_sID = PublicDB.FindSchool(line);
			}
			
			//wysy³amy komunikat
			ID_Name* send=new ID_Name;
			send->id=last_sID;
			send->name=line;

			EventMem* em=new EventMem;
			em->type= EVENT_CREATE_SCHOOL;
			em->data=send;
			event.push(em);
		}
		else
		{
			std::string imie, nazwisko;

			unsigned int x = line.find(" ");
			if(x==std::string::npos)goto err;//shit line
			imie = line.substr(0,x);
			trim(imie);
			line.erase(0,x+1);
			if(!imie.size()|!line.size())goto err;//shit line

			x = line.find("\t");
			nazwisko = line.substr(0,x);
			trim(nazwisko);
			if(x!=std::string::npos)line.erase(0,x+1);
			else line.clear();
			if(!nazwisko.size())goto err;//shit line

			//dodawanie usera
			unsigned int id=PublicDB.AddUser(imie, nazwisko, last_sID);
			if( id == -1)goto err;//it's not god think
			
			LongInfoUser* send=new LongInfoUser;
			PublicDB.GetFullInfoAboutUser(id,*send);

			EventMem* em=new EventMem;
			em->type= EVENT_CREATE_USER;
			em->data=send;
			event.push(em);

			//------------
			if(line.size())
			{
				BYTE tab[3];
				std::string simple_number;
				for(int i=0 ; i<3 && line.size() ; ++i)
				{
					x=line.find("\t");
					simple_number = line.substr(0,x);
					trim(simple_number);
					tab[i]=0xff;
					if(simple_number.size() && simple_number[0]!='-')
					{
						try {  tab[i]=std::stoi(simple_number);  }   catch(...){}
					}
					line.erase(0,x+1);
				}	
				
				if(PublicDB.SetStats(id, tab) == false)
				{
					ID_Name name_school;
					PublicDB.GetInfoAboutSchool(send->sID, name_school);
					ShowError("Nie mo¿na ustawiæ punktacji!\nOsoba:"+imie+" "+nazwisko+"Szko³a:"+name_school.name,0);
					goto next_line;
				}
				//wys³anie komunikatu zmieniaj¹cego dane  w GUI
				EventMem* em= new EventMem;
				em->type = EVENT_CHANGE_STATS;
				em->data = new LongInfoUser;
				PublicDB.GetFullInfoAboutUser(id, *(LongInfoUser*)em->data );
				event.push(em);
			}
		}
		goto next_line;
	err:
		ShowError("Linia "+std::to_string((long long)line_number)+": "+line+"\nZosta³a odrzucona.",0);
next_line:
		line=GetNextLine(file_in, line_number);
	}while(line.size());

	file_in.close();
}
