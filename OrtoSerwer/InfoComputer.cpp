#include "InfoComputer.h"


InfoComputer::InfoComputer(void):
	main_box( sfg::Box::Create(sfg::Box::HORIZONTAL,5.f) )
{
	sfg::Label::Ptr info;	//ogólna zmienna na teksty
	
	
	//--------------------------------
	sfg::Alignment::Ptr alignment = sfg::Alignment::Create();
	sfg::Box::Ptr box_h(sfg::Box::Create(sfg::Box::HORIZONTAL, 5.f));
	//Dodanie tekstu "Nazwa komputera:"
	info=sfg::Label::Create("Nazwa komputera:");
	info->SetId("Label#INFO_nazwa_komputera_txt");
	box_h->Pack(info);

	//Dodanie w³aœciwej nazwy komputera
	info=sfg::Label::Create(stringToWstring(name_serwer));
	info->SetId("Label#INFO_compter_name");
	box_h->Pack(info,false);

	alignment->Add(box_h);
	alignment->SetScale( sf::Vector2f( .0f, .0f ) );
	alignment->SetAlignment( sf::Vector2f( 0.f, 0.f ) );
	main_box->Pack(alignment);


	//-------------------------------
	alignment = sfg::Alignment::Create();
	box_h=sfg::Box::Create(sfg::Box::HORIZONTAL, 5.f);

	//Dodanie tekstu "Adres IP:"
	info=sfg::Label::Create("Adres IP:");
	info->SetId("Label#INFO_adres_ip_txt");
	box_h->Pack(info);

	//Dodanie w³aœciwego adresu ip
	info=sfg::Label::Create(stringToWstring(ip_serwer));
	info->SetId("Label#INFO_address_ip");
	box_h->Pack(info,false);
	
	alignment->Add(box_h);
	alignment->SetScale( sf::Vector2f( .0f, .0f ) );
	alignment->SetAlignment( sf::Vector2f( 0.5f, 0.5f ) );
	main_box->Pack(alignment);

	//-------------------------------
	alignment = sfg::Alignment::Create();
	box_h=sfg::Box::Create(sfg::Box::HORIZONTAL, 5.f);

	//Dodanie tekstu "Port:"
	info=sfg::Label::Create("Port:");
	info->SetId("Label#INFO_port_txt");
	box_h->Pack(info);

	//Dodanie w³aœciwego numeru portu
	info=sfg::Label::Create(stringToWstring(port));
	info->SetId("Label#INFO_port");
	box_h->Pack(info, false);

	alignment->Add(box_h);
	alignment->SetScale( sf::Vector2f( .0f, .0f ) );
	alignment->SetAlignment( sf::Vector2f( 1.f, 1.f ) );
	main_box->Pack(alignment);


	sfg::Context::Get().GetEngine().SetProperty( "Label#INFO_computer_name", "FontColor", sf::Color(255,114,0) );
	sfg::Context::Get().GetEngine().SetProperty( "Label#INFO_address_ip", "FontColor", sf::Color(255,114,0) );
	sfg::Context::Get().GetEngine().SetProperty( "Label#INFO_port", "FontColor", sf::Color(255,114,0) );
}


InfoComputer::~InfoComputer(void)
{
}
