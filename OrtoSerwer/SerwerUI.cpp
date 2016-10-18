#include "SerwerUI.h"


SerwerUI::SerwerUI(int id_window):
	Window(id_window)
{
	
	sfg::Notebook::Ptr zakladki = sfg::Notebook::Create();
	zakladki->AppendPage(usermgr.GetHandleToBox(),sfg::Label::Create(L"Uczniowie"));
	zakladki->AppendPage(schlmgr.GetHandleToBox(),sfg::Label::Create(L"Szko³y"));
	zakladki->AppendPage(resmgr.GetHandleToBox(),sfg::Label::Create(L"Wyniki"));
	zakladki->AppendPage(compmgr.GetHandleToBox(),sfg::Label::Create(L"Komputery"));
	zakladki->SetScrollable( false );
	zakladki->SetRequisition( sf::Vector2f( 500.f, 300.f ) );

	sfg::Box::Ptr main_box(sfg::Box::Create(sfg::Box::VERTICAL,5.f));
	main_box->Pack(zakladki);

	sfg::Separator::Ptr sep=sfg::Separator::Create();
	sep->SetRequisition(sf::Vector2f(0.f,15.f));

	main_box->Pack(sep,false);
	main_box->Pack(infocomp.GetBox(),false);
	main_box->Pack(logger.GetBox(),false);
	window->Add(main_box);
	window->SetTitle(L"Panel sterowania");
}


SerwerUI::~SerwerUI(void)
{
}

void SerwerUI::HandlingEvent(EventMem* em)
{
	usermgr.HandlingEvent(*em);
	schlmgr.HandlingEvent(*em);
	resmgr.HandlingEvent(*em);
	compmgr.HandlingEvent(*em);
}

