#include "UserManager.h"


sf::Image connected_image,
		disconnected_image;

UserManager::UserManager(void)
{
	connected_image.loadFromFile("data\\tick.png");
	disconnected_image.loadFromFile("data\\cross.png");
	main_box=sfg::Box::Create(sfg::Box::VERTICAL, 5.f);
	main_box->Pack(MiniDB.main_box);
	
	//add buttons
	sfg::Box::Ptr button_box( sfg::Box::Create(sfg::Box::HORIZONTAL, 15.f));
	{
		sfg::Button::Ptr button(sfg::Button::Create( L"Dodaj ucznia" ));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &UserManager::AddUser, this );
		button_box->Pack(button);
	}
	{
		sfg::Button::Ptr button(sfg::Button::Create( L"Zmieñ dane ucznia" ));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &UserManager::ChangeUser, this );
		button_box->Pack(button);
	}
	{
		sfg::Button::Ptr button(sfg::Button::Create( L"Usuñ ucznia" ));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &UserManager::DeleteUser, this );
		button_box->Pack(button);
	}
	main_box->Pack( button_box,false );
}


UserManager::~UserManager(void)
{
}

//-------------------------------------------------------------//
//***********************    U S E R   ************************//
void UserManager::anonymous::School::User::CreateUser(unsigned __int32 _uID ,std::string& imie, std::string& nazwisko)
{
	Gcheck=sfg::CheckButton::Create( std::to_string( (long long int) (uID=_uID) ));
	Gimie=sfg::Label::Create( stringToWstring("   "+imie) );
	Gnazwisko=sfg::Label::Create( stringToWstring(nazwisko) );
	Gconnected=sfg::Image::Create(disconnected_image);

	//pakowanie
	main_box=sfg::Box::Create(sfg::Box::HORIZONTAL, 3.f);
	main_box->Pack(Gcheck, false);
	main_box->Pack(Gimie, false);
	main_box->Pack(sfg::Label::Create(L"  "),false);
	main_box->Pack(Gnazwisko, false);

	
	sfg::Alignment::Ptr alignment = sfg::Alignment::Create();
	sfg::Box::Ptr box(sfg::Box::Create());

	box->Pack(sfg::Label::Create(L"Po³¹czony:"),false);
	box->Pack(Gconnected,false);

	alignment->Add(box);
	alignment->SetScale( sf::Vector2f( .0f, .0f ) );
	alignment->SetAlignment( sf::Vector2f( 1.f, 1.f ) );

	main_box->Pack(alignment);
}

void UserManager::anonymous::School::User::ChangeName( std::string& imie, std::string& nazwisko)
{
	Gimie->SetText(stringToWstring(imie));
	Gnazwisko->SetText(stringToWstring(nazwisko));
}

void UserManager::anonymous::School::User::ChangeConnected(bool connect)
{
	if(connect)Gconnected->SetImage(connected_image);
	else Gconnected->SetImage(disconnected_image);
}

bool UserManager::anonymous::School::User::GetChecked()
{
	return Gcheck->IsActive();	
}

//-------------------------------------------------------------//
//********************   S C H O O L   ************************//
void UserManager::anonymous::School::CreateSchool(unsigned __int32 _sID, std::string& _nazwa_szkoly )
{
	sID=_sID;

	button_toggle=sfg::ToggleButton::Create( stringToWstring(_nazwa_szkoly) );
	button_toggle->GetSignal( sfg::ToggleButton::OnToggle ).Connect( &UserManager::anonymous::School::ToggleButtonChange, this );
	main_box=sfg::Box::Create(sfg::Box::VERTICAL, 5.f);
	main_box->Pack(button_toggle);
	
	user_box=sfg::Box::Create(sfg::Box::VERTICAL, 3.f);

	main_box->Pack(user_box);
	user_box->Show(false);

	button_toggle->SetActive(true);
}

void UserManager::anonymous::School::ChangeSchoolName( std::string& nazwa_szkoly )
{
	button_toggle->SetLabel(stringToWstring(nazwa_szkoly));
}

//-----
void UserManager::anonymous::School::AddUser( unsigned __int32 uID, std::string& imie, std::string& nazwisko)
{
	all_user.push_back(new User());
	(*(all_user.end()-1))->CreateUser(uID, imie, nazwisko);
	user_box->Pack((*(all_user.end()-1))->main_box);
}

void UserManager::anonymous::School::DeleteUser( unsigned __int32 uID)
{
	int local_id=FindUserLocalId(uID);
	if(local_id==-1)return;	//very fatal error
	user_box->Remove(all_user[local_id]->main_box);
	delete all_user[local_id];
	all_user.erase(all_user.begin()+local_id);
}

void UserManager::anonymous::School::ChangeUserLogged(unsigned __int32 uID, bool login)
{
	int local_id=FindUserLocalId(uID);
	if(local_id==-1)return;	//very fatal error
	all_user[local_id]->ChangeConnected(login);
}

void UserManager::anonymous::School::ChangeUserName(unsigned __int32 uID, std::string& imie, std::string& nazwisko)
{
	int local_id=FindUserLocalId(uID);
	if(local_id==-1)return;	//very fatal error
	all_user[local_id]->ChangeName(imie,nazwisko);
}

int UserManager::anonymous::School::FindUserLocalId(unsigned __int32 uID)
{
	for(unsigned int i=0; i<all_user.size(); ++i)
		if(all_user[i]->uID==uID)return i;
	return -1;
}

void UserManager::anonymous::School::PushSelectedUseruID( std::vector< unsigned __int32 >& ret)
{
	for( unsigned int i=0; i<all_user.size(); ++i)
	{
		if(all_user[i]->GetChecked())
			ret.push_back(all_user[i]->uID);
	}
}

void UserManager::anonymous::School::ToggleButtonChange()
{
	if(button_toggle->IsActive() )
		user_box->Show(true);
	else user_box->Show(false);
}

//-------------------------------------------------------------//
//********************   M I N I  D B   ***********************//

UserManager::anonymous::anonymous()
{
	//title
	static_title=sfg::Label::Create( TEXT_TITLE_USER_MANAGER );
	//scrolled box with school
	box_with_user=sfg::Box::Create(sfg::Box::VERTICAL, 2.f);
	box_with_user->SetRequisition(sf::Vector2f(480.f, 0.f));
	window_with_user=sfg::ScrolledWindow::Create();
	window_with_user->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_ALWAYS );
	window_with_user->AddWithViewport(box_with_user);
	window_with_user->SetRequisition( sf::Vector2f( 500.f, 100.f ) );
	
	//pack
	main_box=sfg::Box::Create(sfg::Box::VERTICAL);
	main_box->Pack(static_title,false);
	main_box->Pack(window_with_user);
}


void UserManager::anonymous::AddSchool( unsigned __int32 sID, std::string& nazwa_szkoly)
{
	all_school.push_back(new School() );
	(*(all_school.end()-1))->CreateSchool( sID, nazwa_szkoly );
	box_with_user->Pack( (*(all_school.end()-1))->main_box);
}


void UserManager::anonymous::ChangeSchoolName( unsigned __int32 sID, std::string& nazwa_szkoly)
{
	int local_id=FindSchoolLocalId(sID);
	if( local_id==-1) return;
	all_school[local_id]->ChangeSchoolName(nazwa_szkoly);
}


void UserManager::anonymous::DeleteSchool( unsigned __int32 sID )
{
	int local_id=FindSchoolLocalId(sID);
	if( local_id==-1) return;
	box_with_user->Remove( all_school[local_id]->main_box );
	delete all_school[local_id];
	all_school.erase(all_school.begin()+local_id);
}


//--------

void UserManager::anonymous::AddUser(unsigned __int32 sID,  unsigned __int32 uID,std::string& imie, std::string& nazwisko)
{
	int local_id=FindSchoolLocalId(sID);
	if( local_id==-1) return;
	all_school[local_id]->AddUser(uID, imie, nazwisko);
}


void UserManager::anonymous::ChangeUserName(unsigned __int32 sID, unsigned __int32 uID, std::string& imie, std::string& nazwisko)
{
	int local_id=FindSchoolLocalId(sID);
	if( local_id==-1) return;
	all_school[local_id]->ChangeUserName(uID, imie, nazwisko);
}


void UserManager::anonymous::DeleteUser(unsigned __int32 sID, unsigned __int32 uID)
{
	int local_id=FindSchoolLocalId(sID);
	if( local_id==-1) return;
	all_school[local_id]->DeleteUser(uID);
}


void UserManager::anonymous::ChangeUserHasLogged(unsigned __int32 sID, unsigned __int32 uID, bool logged )
{
	int local_id=FindSchoolLocalId(sID);
	if( local_id==-1) return;
	all_school[local_id]->ChangeUserLogged(uID, logged);
}

void UserManager::anonymous::GetAllSelectedUseruID( std::vector< unsigned __int32 >& ret)
{
	for( unsigned __int32 i=0; i<all_school.size(); ++i)
		all_school[i]->PushSelectedUseruID(ret);
}
//--------
int UserManager::anonymous::FindSchoolLocalId(unsigned __int32 sID)
{
	for( unsigned int i=0; i<all_school.size(); ++i)
		if(all_school[i]->sID==sID)return i;
	return -1;
}


//*************************************************************
//----------------------    M A I N    ------------------------

int UserManager::HandlingEvent( EventMem& zdarzenie )
{
	switch(zdarzenie.type)
	{

	case EVENT_CREATE_USER:
		{
			LongInfoUser* info = (LongInfoUser*)zdarzenie.data;
			MiniDB.AddUser( info->sID, info->uID, info->imie, 
				info->nazwisko);
		}
		break;

	case EVENT_CHANGE_USER_NAME:
		{
			LongInfoUser* info = (LongInfoUser*)zdarzenie.data;
			MiniDB.ChangeUserName( info->sID, info->uID, info->imie, info->nazwisko);
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

	case EVENT_USER_LOGIN:
		{
			ShortUserPublicInfo* info = (ShortUserPublicInfo*)zdarzenie.data;
			MiniDB.ChangeUserHasLogged( info->sID, info->uID, info->login);
		}
		break;
	}

	return 0;
}

void UserManager::AddUser()
{
	EventMem* em=new EventMem;
	em->data=0;
	em->type=EVENT_SHOW_ADDUSERWINDOW;
	event.push(em);
}

void UserManager::ChangeUser()
{
	std::vector<unsigned __int32> users;
	MiniDB.GetAllSelectedUseruID(users);
	if(users.size()>1)
	{
		ShowError("Nie mo¿na edytowaæ wielu osób naraz");
		return;
	}
	if(users.size()==0)
	{
		ShowError("Nie wybra³eœ osób");
		return;
	}
	EventMem* em=new EventMem;
	em->data=new int(users[0]);
	em->type=EVENT_SHOW_EDITUSERWINDOW;
	event.push(em);
}

void UserManager::DeleteUser()
{
	std::vector<unsigned __int32> users;
	MiniDB.GetAllSelectedUseruID(users);
	if(users.size()==0)
	{
		ShowError("Nie wybra³eœ osób");
		return;
	}
	for( unsigned int i=0; i<users.size(); ++i)
	{
		EventMem* em=new EventMem;
		em->data=new int(users[i]);
		em->type = EVENT_DELETE_USER_DB;
		event.push(em);
	}
}

