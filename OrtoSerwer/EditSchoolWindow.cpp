#include "EditSchoolWindow.h"
#include "DataBase.h"

EditSchoolWindow::EditSchoolWindow(int _id_window, int _sID):	
	Window(_id_window),
	sID(_sID)
{
	//set title
	window->SetTitle(L"Edytowanie szko³y" );

	//create box
	main_box = sfg::Box::Create(sfg::Box::VERTICAL,10.f);
	edit_box = sfg::Box::Create(sfg::Box::VERTICAL);
	button_box = sfg::Box::Create(sfg::Box::HORIZONTAL,20.f);

	//create buttons & connect with signals & add to button box
	button_ok=sfg::Button::Create("Zapisz");
	button_ok->SetRequisition(STANDARD_BUTTON_SIZE);
	button_ok->GetSignal( sfg::Widget::OnLeftClick ).Connect( &EditSchoolWindow::ClickAdd, this );

	button_cancel=sfg::Button::Create("Anuluj");
	button_cancel->SetRequisition(STANDARD_BUTTON_SIZE);
	button_cancel->GetSignal( sfg::Widget::OnLeftClick ).Connect( &EditSchoolWindow::ClickCancel, this );

	button_box->Pack(button_ok);
	button_box->Pack(button_cancel);

	//create & add Entry
	ID_Name save;
	PublicDB.GetInfoAboutSchool(sID, save);
	school_name=sfg::Entry::Create(stringToWstring(save.name));
	school_name->SetRequisition(sf::Vector2f(250.f,0.f));

	edit_box->Pack(school_name);

	edit_box->Pack(sfg::Label::Create(((std::wstring)L"Id szko³y: ")+std::to_wstring((long long)sID)));

	//adding to main_box
	main_box->Pack(edit_box);
	main_box->Pack(button_box);

	//and place in window
	window->Add(main_box);
}


EditSchoolWindow::~EditSchoolWindow(void)
{
}

void EditSchoolWindow::ClickAdd()
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

void EditSchoolWindow::ClickCancel()
{
	EraseWindow();
}

void EditSchoolWindow::SendEvent()
{
	EventMem* em= new EventMem;
	em->type= EVENT_CHANGE_SCHOOL_NAME_DB;
	em->data= new ID_Name;
	reinterpret_cast<ID_Name*>(em->data)->name=wstringToString(school_name->GetText());
	reinterpret_cast<ID_Name*>(em->data)->id=sID;
	event.push(em);
}

