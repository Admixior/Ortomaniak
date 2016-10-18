#include "EditUserWindow.h"
#include "DataBase.h"
#include <sstream>
EditUserWindow::EditUserWindow(int _id_window, unsigned __int32 _uID):	
	Window(_id_window),
	uID(_uID)
{
	//set title
	window->SetTitle( L"Zmienianie danych ucznia" );

	//create box
	main_box = sfg::Box::Create(sfg::Box::VERTICAL,10.f);
	edit_box = sfg::Box::Create(sfg::Box::VERTICAL);
	button_box = sfg::Box::Create(sfg::Box::HORIZONTAL,20.f);

	//create buttons & connect with signals & add to button box
	button_ok=sfg::Button::Create("Zapisz");
	button_ok->SetRequisition(STANDARD_BUTTON_SIZE);
	button_ok->GetSignal( sfg::Widget::OnLeftClick ).Connect( &EditUserWindow::ClickAdd, this );

	button_cancel=sfg::Button::Create("Anuluj");
	button_cancel->SetRequisition(STANDARD_BUTTON_SIZE);
	button_cancel->GetSignal( sfg::Widget::OnLeftClick ).Connect( &EditUserWindow::ClickCancel, this );

	button_box->Pack(button_ok);
	button_box->Pack(button_cancel);

	//?
	LongInfoUser save;
	PublicDB.GetFullInfoAboutUser(uID, save);

	//*---------------------
	sfg::Box::Ptr box_vertical= sfg::Box::Create(sfg::Box::VERTICAL, 5.f),
		box_horizontal= sfg::Box::Create(sfg::Box::HORIZONTAL, 5.f);
	
	//---------------------------------
	box_vertical->Pack(sfg::Label::Create(L"Imiê:"));
	box_vertical->Pack(sfg::Label::Create(L"Nazwisko:"));
	box_horizontal->Pack(box_vertical);
	//---------------------------------
	box_vertical= sfg::Box::Create(sfg::Box::VERTICAL, 5.f);
	user_firstname=sfg::Entry::Create(stringToWstring(save.imie));
	user_firstname->SetRequisition(sf::Vector2f(150.f,0.f));
	box_vertical->Pack(user_firstname);

	user_surname=sfg::Entry::Create(stringToWstring(save.nazwisko));
	user_surname->SetRequisition(sf::Vector2f(150.f,0.f));
	box_vertical->Pack(user_surname);
	box_horizontal->Pack(box_vertical);
	//---------------------------------
	edit_box->Pack(box_horizontal);
	//---------------------------------

	for(int i=0; i<3; ++i)
	{
		if(save.wyniki[i]!=255)proba[i]=sfg::Entry::Create( std::to_string( (long long)save.wyniki[i] ) );
		else proba[i]=sfg::Entry::Create("-");
		sfg::Box::Ptr box=sfg::Box::Create(sfg::Box::HORIZONTAL, 5.f);
		proba[i]->SetRequisition(sf::Vector2f(20.f,0.f));
		box->Pack(sfg::Label::Create((std::string)"Proba "+std::to_string((long long)i)+(std::string)":"));
		box->Pack(proba[i]);
		edit_box->Pack(box);
	}
	

	/*std::vector<ID_Name> schools;
	PublicDB.GetListOfSchool(schools);

	school_combobox=sfg::ComboBox::Create();
	school_combobox->AppendItem( L"Wybierz szko³ê" );
	for(unsigned int i=0; i<schools.size(); ++i)
	{
		school_combobox->AppendItem( stringToWstring(schools[i].name) );
		sID_list.push_back( schools[i].id );
		if(schools[i].id==save.sID) school_combobox->SelectItem(i+1);
	}

	edit_box->Pack( school_combobox );*/

	//adding to main_box
	main_box->Pack(edit_box);
	main_box->Pack(button_box);

	//and place in window
	window->Add(main_box);
}


EditUserWindow::~EditUserWindow(void)
{
}

void EditUserWindow::ClickAdd()
{
	if( user_firstname->GetText().getSize() && user_surname->GetText().getSize() /*&& school_combobox->GetSelectedItem()*/ )
	{
		SendEvent();
		EraseWindow();
	}
	else
	{
		ShowError("Nie poda³eœ pe³nych danych");
	}
}

void EditUserWindow::ClickCancel()
{
	EraseWindow();
}

void EditUserWindow::SendEvent()
{
	//komentarze oznaczaj¹ braki implementacyjne w dalszej czêœci programu
	EventMem* em= new EventMem;
	em->type= EVENT_CHANGE_USER_DB;
	em->data= new EditUserDB;
	reinterpret_cast<EditUserDB*>(em->data)->imie = wstringToString(user_firstname->GetText());
	reinterpret_cast<EditUserDB*>(em->data)->nazwisko = wstringToString(user_surname->GetText());
	//reinterpret_cast<EditUserDB*>(em->data)->sID = sID_list[ school_combobox->GetSelectedItem()-1 ];
	reinterpret_cast<EditUserDB*>(em->data)->uID = uID;

	for( int i=0; i<3; ++i)
	{
		std::stringstream trans;
		trans<<(std::string)proba[i]->GetText();
		unsigned short int x;
		if(((std::string)"0123456879").find_first_of(trans.peek())==std::string::npos)  x=-1;
		else trans>>x;	reinterpret_cast<EditUserDB*>(em->data)->stats[i]=x;
	}

	event.push(em);
}

