#include "SimpleMessageBox.h"


SimpleMessageBox::SimpleMessageBox(int id_window, std::wstring message, int flag, _callbackSimple _callback):
	callback(_callback),
	Window(id_window)
{
	sfg::Box::Ptr main_box(sfg::Box::Create(sfg::Box::VERTICAL, 10.f)),
		button_box(sfg::Box::Create(sfg::Box::HORIZONTAL, 15.f));
	
	sfg::Label::Ptr label=sfg::Label::Create( message );
	main_box->Pack( label);
	
	if( flag & MESSAGEBOX_YES )
	{
		sfg::Button::Ptr button(sfg::Button::Create("Tak"));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &SimpleMessageBox::ClickYes, this );
		button->SetRequisition( STANDARD_BUTTON_SIZE );
		button_box->Pack(button);
	}
	
	if( flag & MESSAGEBOX_NO )
	{
		sfg::Button::Ptr button(sfg::Button::Create("Nie"));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &SimpleMessageBox::ClickNo, this );
		button->SetRequisition( STANDARD_BUTTON_SIZE );
		button_box->Pack(button);
	}
	
	if( flag & MESSAGEBOX_OK )
	{
		sfg::Button::Ptr button(sfg::Button::Create("OK"));
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &SimpleMessageBox::ClickOK, this );
		button->SetRequisition( STANDARD_BUTTON_SIZE );
		button_box->Pack(button);
	}
	
	main_box->Pack(button_box);
	window->SetTitle(L"Uwaga!");
	window->Add(main_box);
}


SimpleMessageBox::~SimpleMessageBox(void)
{
}


void SimpleMessageBox::ClickYes()
{
	if(callback)	callback(MESSAGEBOX_YES);
	EraseWindow();
}

void SimpleMessageBox::ClickNo()
{
	if(callback)	callback(MESSAGEBOX_NO);
	EraseWindow();
}

void SimpleMessageBox::ClickOK()
{
	if(callback)	callback(MESSAGEBOX_OK);
	EraseWindow();
}

