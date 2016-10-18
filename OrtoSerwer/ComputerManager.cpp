#include "ComputerManager.h"


ComputerManager::ComputerManager(void)
{
	main_box=sfg::Box::Create(sfg::Box::VERTICAL, 5.f);
	main_box->Pack(MiniDB.main_box);

	//add buttons
	sfg::Box::Ptr button_box( sfg::Box::Create(sfg::Box::HORIZONTAL, 15.f));
	{
		sfg::Button::Ptr button(sfg::Button::Create( L"Rozpocznij grê" ));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &ComputerManager::StartGame, this );
		button_box->Pack(button);
	}
	{
		sfg::Button::Ptr button(sfg::Button::Create( L"Wyloguj u¿ytkownika" ));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &ComputerManager::LogoutUser, this );
		button_box->Pack(button);
	}
	{
		sfg::Button::Ptr button(sfg::Button::Create( L"Roz³¹cz komputer" ));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &ComputerManager::DisconnectComputer, this );
		button_box->Pack(button);
	}
	main_box->Pack( button_box ,false);
}


ComputerManager::~ComputerManager(void)
{
}

void ComputerManager::anonymous::computer::CreateComputer(unsigned __int32 _cID, std::string& name)
{
	cID=_cID;

	check = sfg::CheckButton::Create( std::to_string((long long) _cID));
	nazwa_kompa = sfg::Label::Create(L"   "+stringToWstring(name));
	uzytkownik = sfg::Label::Create("   Niezalogowany");
	status = sfg::Label::Create( (std::wstring)L"Status: "+(std::wstring)computer_status_text[N_D] );
	
	main_box = sfg::Box::Create(sfg::Box::HORIZONTAL, 7.f);
	
	main_box->Pack(check, false);
	main_box->Pack(nazwa_kompa,false);
	main_box->Pack(uzytkownik,false);

	sfg::Alignment::Ptr alignment = sfg::Alignment::Create();
	alignment->Add(status);
	alignment->SetScale( sf::Vector2f( .0f, .0f ) );
	alignment->SetAlignment( sf::Vector2f( 1.f, 1.f ) );

	main_box->Pack(alignment);
}

void ComputerManager::anonymous::computer::ConnectedUser(std::string& username)
{
	uzytkownik->SetText( (std::wstring)L"Uzytkownik: " + stringToWstring(username) );
	status->SetText( computer_status_text[ login ] );
}

void ComputerManager::anonymous::computer::SetStatusUser(int _status)
{
	status->SetText( (std::wstring)L"status: " + (std::wstring)computer_status_text[_status] );
	if(_status==0)
	{
		uzytkownik->SetText("   Niezalogowany");
	}
}

bool ComputerManager::anonymous::computer::GetChecked()
{
	bool tmp=check->IsActive();
	check->SetActive(false);
	return tmp;
}


//***************************************

ComputerManager::anonymous::anonymous()
{
	//title
	static_title=sfg::Label::Create( TEXT_TITLE_COMPUTER_MANAGER );
	//scrolled box with school
	box_with_computer=sfg::Box::Create(sfg::Box::VERTICAL, 2.f);
	box_with_computer->SetRequisition(sf::Vector2f(480.f, 0.f));
	window_with_computer=sfg::ScrolledWindow::Create();
	window_with_computer->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
	window_with_computer->AddWithViewport(box_with_computer);
	window_with_computer->SetRequisition( sf::Vector2f( 500.f, 100.f ) );
	//pack
	main_box=sfg::Box::Create(sfg::Box::VERTICAL, 3.f);
	main_box->Pack(static_title,false);
	main_box->Pack(window_with_computer);
}

ComputerManager::anonymous::~anonymous()
{
	for(unsigned long int i=0; i < computer_list.size(); ++i)
	{
		if(computer_list[i])
		{
			delete computer_list[i];
			computer_list[i]=0;
		}
	}
}

void ComputerManager::anonymous::NewComputerConnected(unsigned __int32 cID, std::string& name)
{
	unsigned int pos = FindNullPointer(computer_list);
	if(pos == -1)
	{
		pos = computer_list.size();
		computer_list.push_back(0);
	}

	computer* wsk = computer_list[pos] = new computer;
	wsk->CreateComputer(cID, name);
	box_with_computer->Pack( wsk->main_box );
}

void ComputerManager::anonymous::ConnectedUser(unsigned __int32 cID, std::string& username)
{
	unsigned int local = FindLocalComputerID( cID );
	if(local == -1)return;	//some error
	if(computer_list[local] == 0) return; //it's big fault
	computer_list[local]->ConnectedUser(username);
}

void ComputerManager::anonymous::SetStatusUser(unsigned __int32 cID, int status)
{
	unsigned int local = FindLocalComputerID( cID );
	if(local == -1)return;	//some error
	if(computer_list[local] == 0) return; //it's big fault
	computer_list[local]->SetStatusUser( status );
}

void ComputerManager::anonymous::DisconnectComputer(unsigned __int32 cID)
{
	unsigned int local = FindLocalComputerID( cID );
	if(local == -1)return;	//some error
	if(computer_list[local] == 0) return; //it's big fault
	box_with_computer->Remove(computer_list[local]->main_box);
	delete computer_list[local];
	computer_list[local]=0;
}

void ComputerManager::anonymous::CreateListWithCheckedComputers(std::vector< unsigned __int32 >& new_computer_list)
{
	for( unsigned long int i=0; i< computer_list.size(); ++i)
	{
		if(computer_list[i])
		{
			if(computer_list[i]->GetChecked())
				new_computer_list.push_back( computer_list[i]->cID);
		}
	}
}

int ComputerManager::anonymous::FindLocalComputerID(unsigned __int32 cID)
{
	for( unsigned long int i=0; i< computer_list.size(); ++i)
	{
		if(computer_list[i])
		{
			if(computer_list[i]->cID == cID)
				return i;
		}
	}
	return -1;
}

int ComputerManager::HandlingEvent( EventMem& zdarzenie )
{
	switch(zdarzenie.type)
	{
	case EVENT_COMPUTER_USER_LOGIN:
		{
			ID_Name* data = (ID_Name*)zdarzenie.data;
			MiniDB.ConnectedUser(data->id, data->name);
		}
		break;
	case EVENT_COMPUTER_CHANGE_STATUS:
		{
			ChangeCompStatus* data = (ChangeCompStatus*)zdarzenie.data;
			MiniDB.SetStatusUser(data->cID, data->status);
		}
		break;
	case EVENT_COMPUTER_LOGIN:
		{
			ID_Name* data = (ID_Name*)zdarzenie.data;
			MiniDB.NewComputerConnected(data->id, data->name);
		}
		break;
	case EVENT_COMPUTER_LOGOUT:
		{
			unsigned __int32 cID= *(int*)zdarzenie.data;
			MiniDB.DisconnectComputer(cID);
		}
		break;
	}
	return 0;
}

void ComputerManager::LogoutUser()
{
	std::vector<unsigned __int32> lista;
	MiniDB.CreateListWithCheckedComputers( lista );
	
	if(lista.size() == 0)
	{
		ShowError("Nie wybra³eœ komputerów");
	}

	for( unsigned int i=0; i< lista.size() ; ++i)
	{
		EventMem* em = new EventMem;
		em->type = EVENT_LOGOUT_USER;
		em->data = new int(lista[i]);
		event.push(em);
	}
}

void ComputerManager::DisconnectComputer()
{
	std::vector<unsigned __int32> lista;
	MiniDB.CreateListWithCheckedComputers( lista );
	
	if(lista.size() == 0)
	{
		ShowError("Nie wybra³eœ komputerów");
	}

	for( unsigned int i=0; i< lista.size() ; ++i)
	{
		EventMem* em = new EventMem;
		em->type = EVENT_DISCONNECT_COMPUTER;
		em->data = new int(lista[i]);
		event.push(em);
	}
}

void ComputerManager::StartGame()
{
	std::vector<unsigned __int32> lista;
	MiniDB.CreateListWithCheckedComputers( lista );
	
	if(lista.size() == 0)
	{
		ShowError("Nie wybra³eœ komputerów");
	}

	for( unsigned int i=0; i< lista.size() ; ++i)
	{
		EventMem* em = new EventMem;
			em->type = EVENT_START_GAME;
			em->data = 0;
			event.push(em);
	}
}
