#include "AddSchoolWindow.h"


AddSchoolWindow::AddSchoolWindow(int _id_window):	
	Window(_id_window)
{
	//set title
	window->SetTitle( L"Dodawanie szko³y" );

	//create box
	main_box = sfg::Box::Create(sfg::Box::VERTICAL,10.f);
	edit_box = sfg::Box::Create(sfg::Box::VERTICAL,3.f);
	button_box = sfg::Box::Create(sfg::Box::HORIZONTAL,20.f);

	//create buttons & connect with signals & add to button box
	button_ok=sfg::Button::Create("Dodaj");
	button_ok->SetRequisition(STANDARD_BUTTON_SIZE);
	button_ok->GetSignal( sfg::Widget::OnLeftClick ).Connect( &AddSchoolWindow::ClickAdd, this );

	button_cancel=sfg::Button::Create("Anuluj");
	button_cancel->SetRequisition(STANDARD_BUTTON_SIZE);
	button_cancel->GetSignal( sfg::Widget::OnLeftClick ).Connect( &AddSchoolWindow::ClickCancel, this );

	button_box->Pack(button_ok);
	button_box->Pack(button_cancel);

	//create & add Entry
	school_name=sfg::Entry::Create();
	school_name->SetRequisition(sf::Vector2f(250.f,0.f));

	edit_box->Pack(sfg::Label::Create(L"Nazwa szko³y:"));
	edit_box->Pack(school_name);

	//adding to main_box
	main_box->Pack(edit_box);
	main_box->Pack(button_box);

	//and place in window
	window->Add(main_box);
}


AddSchoolWindow::~AddSchoolWindow(void)
{
}

void AddSchoolWindow::ClickAdd()
{
	if( school_name->GetText().getSize() )
	{
		SendEvent();
		EraseWindow();
	}
	else
	{
		ShowError("Nie poda³eœ nazwy szko³y");
	}
}

void AddSchoolWindow::ClickCancel()
{
	EraseWindow();
}


void AddSchoolWindow::SendEvent()
{
	EventMem* em= new EventMem;
	em->type= EVENT_CREATE_SCHOOL_DB;
	em->data= new NewSchoolDB;
	reinterpret_cast<NewSchoolDB*>(em->data)->name_school= wstringToString(school_name->GetText());
	event.push(em);
}

