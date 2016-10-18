#include "AddUserWindow.h"
#include "DataBase.h"

AddUserWindow::AddUserWindow(int _id_window):	
	Window(_id_window)
{
	//set title
	window->SetTitle( L"Dodawanie ucznia" );

	//create box
	main_box = sfg::Box::Create(sfg::Box::VERTICAL,10.f);
	edit_box = sfg::Box::Create(sfg::Box::VERTICAL,5.f);
	button_box = sfg::Box::Create(sfg::Box::HORIZONTAL,20.f);

	//create buttons & connect with signals & add to button box
	button_ok=sfg::Button::Create("Dodaj");
	button_ok->SetRequisition(STANDARD_BUTTON_SIZE);
	button_ok->GetSignal( sfg::Widget::OnLeftClick ).Connect( &AddUserWindow::ClickAdd, this );

	button_cancel=sfg::Button::Create("Anuluj");
	button_cancel->SetRequisition(STANDARD_BUTTON_SIZE);
	button_cancel->GetSignal( sfg::Widget::OnLeftClick ).Connect( &AddUserWindow::ClickCancel, this );

	button_box->Pack(button_ok);
	button_box->Pack(button_cancel);

	//create & add Entry
	//*---------------------
	sfg::Box::Ptr box_vertical= sfg::Box::Create(sfg::Box::VERTICAL, 5.f),
		box_horizontal= sfg::Box::Create(sfg::Box::HORIZONTAL, 5.f);
	
	//---------------------------------
	box_vertical->Pack(sfg::Label::Create(L"Imiê:"));
	box_vertical->Pack(sfg::Label::Create(L"Nazwisko:"));
	box_horizontal->Pack(box_vertical);
	//---------------------------------
	box_vertical= sfg::Box::Create(sfg::Box::VERTICAL, 5.f);
	user_firstname=sfg::Entry::Create();
	user_firstname->SetRequisition(sf::Vector2f(150.f,0.f));
	box_vertical->Pack(user_firstname);

	user_surname=sfg::Entry::Create();
	user_surname->SetRequisition(sf::Vector2f(150.f,0.f));
	box_vertical->Pack(user_surname);
	box_horizontal->Pack(box_vertical);
	//---------------------------------
	edit_box->Pack(box_horizontal);
	//---------------------------------
	std::vector<ID_Name> schools;
	PublicDB.GetListOfSchool(schools);

	school_combobox=sfg::ComboBox::Create();
	school_combobox->AppendItem( L"Wybierz szko³ê" );
	school_combobox->SelectItem(0);
	for(unsigned int i=0; i<schools.size(); ++i)
	{
		school_combobox->AppendItem( stringToWstring(schools[i].name) );
		sID_list.push_back( schools[i].id );
	}

	edit_box->Pack( school_combobox );
	
	//adding to main_box
	main_box->Pack(edit_box);
	main_box->Pack(button_box);

	//and place in window
	window->Add(main_box);
}


AddUserWindow::~AddUserWindow(void)
{
}

void AddUserWindow::ClickAdd()
{
	if( user_firstname->GetText().getSize() && user_surname->GetText().getSize() && school_combobox->GetSelectedItem() )
	{
		SendEvent();
		EraseWindow();
	}
	else
	{
		ShowError("Nie poda³eœ pe³nych danych");
	}
}

void AddUserWindow::ClickCancel()
{
	EraseWindow();
}

void AddUserWindow::SendEvent()
{
	EventMem* em= new EventMem;
	em->type= EVENT_CREATE_USER_DB;
	em->data= new NewUserDB;
	reinterpret_cast<NewUserDB*>(em->data)->imie= wstringToString(user_firstname->GetText());
	reinterpret_cast<NewUserDB*>(em->data)->nazwisko= wstringToString(user_surname->GetText());
	reinterpret_cast<NewUserDB*>(em->data)->sID=sID_list[ school_combobox->GetSelectedItem()-1 ];
	event.push(em);
}

