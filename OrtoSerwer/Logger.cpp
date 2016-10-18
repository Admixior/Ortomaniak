#include "Logger.h"


Logger::Logger(void)
{
	//GUI
	log_box=sfg::Box::Create(sfg::Box::VERTICAL);	//create a log_box
	main_box= sfg::Box::Create();
	sfg::ScrolledWindow::Ptr scrolled = sfg::ScrolledWindow::Create();	//create scrolled box
	scrolled->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );	//set settings
	scrolled->AddWithViewport( log_box );		//add the log_box to scrolled box
	scrolled->SetRequisition( sf::Vector2f( 500.f, 150.f ) );	// other settings

	//and push it to main box
	main_box->Pack(scrolled);

	//FILE
	file_log.open("logs.txt");
}


Logger::~Logger(void)
{
}

void Logger::PushLog(std::string new_log)
{
	file_log<<new_log<<"\n";
	//ficzer: dodaæ usuwanie logów je¿eli bêdzie zbyt du¿o
	sfg::Box::Ptr box=sfg::Box::Create();
	box->Pack(sfg::Label::Create(stringToWstring(new_log)),false);
	log_box->Pack(box);
}

