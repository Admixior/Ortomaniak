#include "SchoolManager.h"


SchoolManager::SchoolManager(void)
{
	main_box=sfg::Box::Create(sfg::Box::VERTICAL, 5.f);
	main_box->Pack(MiniDB.main_box);

	//add buttons
	sfg::Box::Ptr button_box( sfg::Box::Create(sfg::Box::HORIZONTAL, 15.f));
	{
		sfg::Button::Ptr button(sfg::Button::Create( L"Dodaj szko³ê" ));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &SchoolManager::AddSchool, this );
		button_box->Pack(button);
	}
	{
		sfg::Button::Ptr button(sfg::Button::Create( L"Zmieñ dane szko³y" ));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &SchoolManager::ChangeSchool, this );
		button_box->Pack(button);
	}
	{
		sfg::Button::Ptr button(sfg::Button::Create( L"Usuñ szko³ê" ));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &SchoolManager::DeleteSchool, this );
		button_box->Pack(button);
	}
	main_box->Pack( button_box ,false);
}


SchoolManager::~SchoolManager(void)
{
}

void SchoolManager::anonymous::School::CreateSchool(unsigned __int32 _sID, std::string& school_name)
{
	checkbox=sfg::CheckButton::Create(std::to_string((long long)(sID=_sID)));
	nazwa_szkoly=sfg::Label::Create(stringToWstring(school_name));
	ilosc_osob=sfg::Label::Create(std::to_string((long long)(ilosc_uczniow=0)));

	main_box=sfg::Box::Create(sfg::Box::HORIZONTAL,10.f);
	main_box->Pack(checkbox,false);
	main_box->Pack(nazwa_szkoly,false);

	sfg::Alignment::Ptr alignment = sfg::Alignment::Create();
	sfg::Box::Ptr box(sfg::Box::Create());
	
	box->Pack(sfg::Label::Create(L"    Iloœæ osób:"), false);
	box->Pack(ilosc_osob,false);

	alignment->Add(box);
	alignment->SetScale( sf::Vector2f( .0f, .0f ) );
	alignment->SetAlignment( sf::Vector2f( 1.f, 1.f ) );

	main_box->Pack(alignment);
}

bool SchoolManager::anonymous::School::GetChecked()
{
	return checkbox->IsActive();
}

void SchoolManager::anonymous::School::AddUser()
{
	ilosc_osob->SetText(std::to_string((long long)++ilosc_uczniow));
}

void SchoolManager::anonymous::School::DeleteUser()
{
	ilosc_osob->SetText(std::to_string((long long)--ilosc_uczniow));
}

void SchoolManager::anonymous::School::ChangeNameSchool(std::string& school_name)
{
	nazwa_szkoly->SetText(stringToWstring(school_name));
}

//************************************************
SchoolManager::anonymous::anonymous()
{
	//title
	static_title=sfg::Label::Create( TEXT_TITLE_SCHOOL_MANAGER );
	//scrolled box with school
	box_with_school=sfg::Box::Create(sfg::Box::VERTICAL, 2.f);
	box_with_school->SetRequisition(sf::Vector2f(480.f, 0.f));
	window_with_school=sfg::ScrolledWindow::Create();
	window_with_school->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
	window_with_school->AddWithViewport(box_with_school);
	window_with_school->SetRequisition( sf::Vector2f( 500.f, 100.f ) );
	//pack
	main_box=sfg::Box::Create(sfg::Box::VERTICAL);
	main_box->Pack(static_title,false);
	main_box->Pack(window_with_school);
}

void SchoolManager::anonymous::AddUser( unsigned __int32 sID )
{
	int local_id=FindSchoolLocalId(sID);
	if(local_id==-1) return;	//fatal error
	all_school[local_id]->AddUser();
}

void SchoolManager::anonymous::DeleteUser( unsigned __int32 sID )
{
	int local_id=FindSchoolLocalId(sID);
	if(local_id==-1) return;	//fatal error
	all_school[local_id]->DeleteUser();
}

void SchoolManager::anonymous::AddSchool( unsigned __int32 sID, std::string& school_name )
{
	all_school.push_back(new School());
	(*(all_school.end()-1))->CreateSchool(sID, school_name);
	box_with_school->Pack( (*(all_school.end()-1))->main_box,false );
}

void SchoolManager::anonymous::ChangeNameSchool( unsigned __int32 sID, std::string& school_name )
{
	int local_id=FindSchoolLocalId(sID);
	if(local_id==-1) return;	//fatal error
	all_school[local_id]->ChangeNameSchool(school_name);
}


void SchoolManager::anonymous::DeleteSchool( unsigned __int32 sID )
{
	int local_id=FindSchoolLocalId(sID);
	if(local_id==-1) return;	//fatal error
	box_with_school->Remove(all_school[local_id]->main_box);
	delete all_school[local_id];
	all_school.erase(all_school.begin()+local_id);
}

void SchoolManager::anonymous::GetAllCheckedSchool( std::vector< unsigned __int32 >& ret )
{
	for( unsigned int i=0; i<all_school.size(); ++i)
	{
		if(all_school[i]->GetChecked())
			ret.push_back(all_school[i]->sID);
	}
}

int SchoolManager::anonymous::FindSchoolLocalId(unsigned __int32 sID)
{
	for( unsigned int i=0; i<all_school.size(); ++i)
		if(all_school[i]->sID==sID)return i;
	return -1;
}

int SchoolManager::HandlingEvent( EventMem& zdarzenie )
{
	switch(zdarzenie.type)
	{

	case EVENT_CREATE_USER:
		{
			LongInfoUser* info = (LongInfoUser*)zdarzenie.data;
			MiniDB.AddUser( info->sID );
		}
		break;

	case EVENT_DELETE_USER:
		{
			LongInfoUser* info = (LongInfoUser*)zdarzenie.data;
			MiniDB.DeleteUser( info->sID);
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
			MiniDB.ChangeNameSchool(info->id, info->name);
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

void SchoolManager::AddSchool()
{
	EventMem* em=new EventMem;
	em->type=EVENT_SHOW_ADDSCHOOLWINDOW;
	em->data=0;
	event.push(em);
}

void SchoolManager::ChangeSchool()
{
	std::vector<unsigned __int32> schools;
	MiniDB.GetAllCheckedSchool(schools);
	if(schools.size()>1)
	{
		ShowError("Nie mo¿na edytowaæ wiele szkó³ naraz");
		return;
	}
	if(schools.size()==0)
	{
		ShowError("Nie wybra³eœ szko³y");
		return;
	}
	EventMem* em= new EventMem;
	em->type= EVENT_SHOW_EDITSCHOOLWINDOW;
	em->data= new int (schools[0]);
	event.push(em);
}

void SchoolManager::DeleteSchool()
{
	std::vector<unsigned __int32> schools;
	MiniDB.GetAllCheckedSchool(schools);

	if(schools.size()==0)
	{
		ShowError("Nie wybra³eœ szko³y");
		return;
	}

	for( unsigned int i=0; i<schools.size(); ++i)
	{
		EventMem* em=new EventMem;
		em->data=new int(schools[i]);
		em->type = EVENT_DELETE_SCHOOL_DB;
		event.push(em);
	}
}

