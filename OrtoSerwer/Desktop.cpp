#include "Desktop.h"

inline bool ComputerExist(unsigned int cID)
{
	if(cID >= computer_connection.size() ) return false;
	if(!computer_connection[cID]) return false;
	return true;
}

inline bool ComputerConnected(unsigned int cID)
{
	if(computer_connection[cID]->flag & ComputerConn::computer_login) return true;
	return false;
}

inline bool ComputerExistAndConn(unsigned int cID)
{
	if(!ComputerExist(cID))return false;
	if(!ComputerConnected(cID))return false;
	return true;
}

Desktop::Desktop(void)
{
}


Desktop::~Desktop(void)
{
}

int Desktop::RunServer()
{
	serwer_win=new SerwerUI( windows.size() );
	m_desktop.Add(serwer_win->GetWindow());
	m_desktop.LoadThemeFromFile("data\\style.theme");

	background_texture.loadFromFile("data\\bgn.jpg");
	
	background.setTexture(background_texture);

	RECT rect={0,0,0,0};
	GetWindowRect(GetDesktopWindow(), &rect);
	if(rect.right <= 1100) rect.right=1100;
	if(rect.bottom <= 800) rect.bottom=800;

	rect.right-=100;
	rect.bottom-=100;

	sf::RenderWindow render_window( sf::VideoMode( rect.right, rect.bottom ), "Ortomaniak" );

	if(background_texture.getSize().x && background_texture.getSize().y)
		background.setScale((float)(rect.right)/background_texture.getSize().x,
			(float)(rect.bottom)/background_texture.getSize().y);

	render_window.resetGLStates();
	render_window.setFramerateLimit(30);
	res.x=static_cast<float>( render_window.getSize().x ),
	res.y=static_cast<float>( render_window.getSize().y );

	CenterWindow(serwer_win);
	serwer_win->GetWindow()->SetPosition(
		sf::Vector2f(
			30.f,
			serwer_win->GetWindow()->GetAbsolutePosition().y
			)
		);
	window_is_open = true;
	sf::Event ev;
	sf::Clock timer;
	timer.restart();
	while( render_window.isOpen() ) {
		while( render_window.pollEvent( ev ) ) {
			if(ev.type == sf::Event::Closed)
				render_window.close();
			else if(ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
				m_desktop.LoadThemeFromFile("data\\stye.theme");
			else if( ev.type == sf::Event::Resized )
			{
				m_desktop.UpdateViewRect(
					sf::FloatRect(
						0,
						0,
						res.x=static_cast<float>( render_window.getSize().x ),
						res.y=static_cast<float>( render_window.getSize().y )
					)
				);
			}
			else {
				m_desktop.HandleEvent( ev );
			}
		}
		LoopEvents();
		if(timer.getElapsedTime().asMilliseconds()>50)
		{
			m_desktop.Update( timer.getElapsedTime().asSeconds() );
			timer.restart();
		}
		render_window.clear();
		render_window.draw(background);
		m_sfgui.Display( render_window );
		render_window.display();
	}
	//erase all window
	m_desktop.RemoveAll();
	for( unsigned int i=0; i<windows.size(); ++i)
	{
		delete windows[i];
	}
	windows.clear();
	delete serwer_win;
	return 0;
}


int Desktop::LoopEvents()
{
	while(event.size())
	{
		EventMem* ev = event.front();
		event.pop();
		if(ev)
		{
			try
			{
				int tmp;
				if(ev->type <= EVENT_GUI_EVENT_END)			serwer_win->HandlingEvent(ev);
				else if(ev->type <= EVENT_DELETE_SCHOOL_DB)	{if(tmp=BaseEvent(ev))throw tmp;}
				else if(ev->type <= EVENT_SHOW_MESSAGEBOX)	{if(tmp=SystemEvent(ev))throw tmp;}
				else if(ev->type <= EVENT_START_GAME)		{if(tmp=NetWorkTask(ev))throw tmp;}
				else /*if(ev->type == EVENT_MESSAGECOME)*/	{if(tmp=NetWorkEvent(ev))throw tmp;}
				/*else	ShowError("Nie rozpoznano rodzaju komunikatu!");*/
			}
			catch(int tmp)
			{
				serwer_win->SaveLogs("Fatalny b³¹d"+std::to_string((long long)tmp));
			}
			//delete event
			if(ev->data)delete ev->data;
			delete ev;
		}
		else	ShowError("B³¹d! Komunkat jest nader pusty!");

	}
	return 1;
}

int Desktop::BaseEvent(EventMem* ev)
{
	switch(ev->type)
	{
	case EVENT_CREATE_USER_DB:
		{
			if(!ev->data)return 1;
			NewUserDB* save=(NewUserDB*)ev->data;

			LongInfoUser* send=new LongInfoUser;

			unsigned int id=PublicDB.AddUser(save->imie, save->nazwisko, save->sID);
			if( id == -1)return 2;
			if(!PublicDB.GetFullInfoAboutUser(id,*send))return 3;

			EventMem* em=new EventMem;
			em->type= EVENT_CREATE_USER;
			em->data=send;
			event.push(em);
			break;
		}
	case EVENT_CHANGE_USER_DB:
		{
			if(!ev->data)return 4;
			EditUserDB* save=(EditUserDB*)ev->data;
			
			LongInfoUser* send=new LongInfoUser;

			if(!PublicDB.EditUser(save->uID, save->imie, save->nazwisko, save->stats, save->sID))
			{
				ShowError("Nie mo¿na edytowaæ u¿ytkownika.\nPrawdopodobnie jest zalogowany lub\nnie istnieje");
				return 0;
			}
			if(!PublicDB.GetFullInfoAboutUser(save->uID,*send))return 5;

			EventMem* em=new EventMem;
			em->type= EVENT_CHANGE_USER_NAME;
			em->data=send;
			event.push(em);
			break;
		}
	case EVENT_DELETE_USER_DB:
		{
			if(!ev->data)return 6;
			unsigned int* save=(unsigned int*)ev->data;

			LongInfoUser* send=new LongInfoUser;
			
			if(!PublicDB.GetFullInfoAboutUser(*save, *send))return 7;
			if(!PublicDB.DeleteUsers(*save))
			{
				ShowError("Nie mo¿na usun¹æ u¿ytkownika.\nPrawdopodobnie jest zalogowany lub\nnie istnieje");
				return 0;
			}

			EventMem* em=new EventMem;
			em->type= EVENT_DELETE_USER;
			em->data=send;
			event.push(em);
			break;
		}
	case EVENT_CREATE_SCHOOL_DB:
		{
			if(!ev->data)return 8;
			NewSchoolDB* save=(NewSchoolDB*)ev->data;

			ID_Name* send=new ID_Name;
			unsigned __int32 sID=PublicDB.AddSchool(save->name_school);
			if(sID==-1)
			{
				ShowError("Nie mo¿na dodaæ szko³y.\nTaka szko³a ju¿ istnieje.");
				return 0;
			}
			
			send->id=sID;
			send->name=save->name_school;
			EventMem* em=new EventMem;
			em->type= EVENT_CREATE_SCHOOL;
			em->data=send;
			event.push(em);
			break;
		}
	case EVENT_CHANGE_SCHOOL_NAME_DB:
		{
			if(!ev->data)return 10;
			ID_Name* save=(ID_Name*)ev->data;

			ID_Name* send=new ID_Name;

			if(!PublicDB.EditSchool(save->id, save->name))
			{
				ShowError("Nie mo¿na edytowaæ szko³y.\nTaka szko³a ju¿ istnieje.");
				return 0;
			}
			
			if(!PublicDB.GetInfoAboutSchool(save->id, *send))return 11;
			EventMem* em=new EventMem;
			em->type= EVENT_CHANGE_SCHOOL_NAME;
			em->data=send;
			event.push(em);
			break;
		}
	case EVENT_DELETE_SCHOOL_DB:
		{
			if(!ev->data)return 12;
			unsigned int* save=(unsigned int*)ev->data;

			ID_Name* send=new ID_Name;

			if(!PublicDB.DeleteSchool(*save))
			{
				ShowError("Nie mo¿na usun¹æ szko³y");
				return 0;
			}
			send->id=*save;
			EventMem* em=new EventMem;
			em->type= EVENT_DELETE_SCHOOL;
			em->data=send;
			event.push(em);
			break;
		}
	}
	return 0;
}


int Desktop::SystemEvent(EventMem* ev)
{
	switch(ev->type)
	{
	case EVENT_DESTROY_WINDOW:
		{
			//checking data
			if(!ev->data)return 100;
			unsigned int* id_window=(unsigned int*)ev->data;
			if( windows.size() <= *id_window )return 101;
			if( windows[*id_window] == 0 ) return 102;

			//remove window
			m_desktop.Remove(windows[*id_window]->GetWindow());
			delete windows[*id_window];
			windows[*id_window]=0;
			break;
		}
		
	case EVENT_SHOW_ADDSCHOOLWINDOW:
		{
			int id_window=FindNullPointer(windows);
			if(id_window==-1)
			{
				id_window=windows.size();
				windows.push_back(0);
			}
			windows[id_window]=new AddSchoolWindow(id_window);
			m_desktop.Add(windows[id_window]->GetWindow());
			CenterWindow(windows[id_window]);
			break;
		}

	case EVENT_SHOW_ADDUSERWINDOW:
		{
			int id_window=FindNullPointer(windows);
			if(id_window==-1)
			{
				id_window=windows.size();
				windows.push_back(0);
			}
			windows[id_window]=new AddUserWindow(id_window);
			m_desktop.Add(windows[id_window]->GetWindow());
			CenterWindow(windows[id_window]);
			break;
		}

	case EVENT_SHOW_EDITSCHOOLWINDOW:
		{
			//checking data
			if(!ev->data)return 103;
			int* sID = (int*)ev->data;

			int id_window=FindNullPointer(windows);
			if(id_window==-1)
			{
				id_window=windows.size();
				windows.push_back(0);
			}

			windows[id_window]=new EditSchoolWindow(id_window,*sID);
			m_desktop.Add(windows[id_window]->GetWindow());
			CenterWindow(windows[id_window]);
			break;
		}

	case EVENT_SHOW_EDITUSERWINDOW:
		{
			//checking data
			if(!ev->data)return 104;
			int* uID = (int*)ev->data;

			int id_window=FindNullPointer(windows);
			if(id_window==-1)
			{
				id_window=windows.size();
				windows.push_back(0);
			}

			windows[id_window]=new EditUserWindow(id_window,*uID);
			m_desktop.Add(windows[id_window]->GetWindow());
			CenterWindow(windows[id_window]);
			break;
		}

	case EVENT_SHOW_MESSAGEBOX:
		{
			//checking data
			if(!ev->data)return 105;
			MessageBoxStruct* save = (MessageBoxStruct*)ev->data;

			int id_window=FindNullPointer(windows);
			if(id_window==-1)
			{
				id_window=windows.size();
				windows.push_back(0);
			}

			windows[id_window]=new SimpleMessageBox(id_window, save->msg, save->flag, save->callback);
			m_desktop.Add(windows[id_window]->GetWindow());
			CenterWindow(windows[id_window]);
			break;
		}
	}
	return 0;
}


int Desktop::NetWorkEvent(EventMem* ev)
{
	unsigned __int32 cID= *(int*)ev->data;
	if(!ComputerExist(cID) && ev->type!=EVENT_NEWCOMPUTER_DISCONN)
	{
		ShowError("Przysz³a wiadomoœæ od komputera-widmo");
		return 1000;
	}
	
	switch(ev->type)
	{
	case EVENT_NEWCOMPUTER_CONN:	serwer_win->SaveLogs("Nowy komputer nr."+std::to_string((long long)cID)+ " inicjuje po³¹czenie");	break;
	case EVENT_NEWCOMPUTER_DISCONN:	serwer_win->SaveLogs("Komputer nr. "+std::to_string((long long)cID)+ " od³¹czy³ siê");				break;
	case EVENT_MESSAGECOME:
		{
			NetPacket* msg = computer_connection[cID]->in_packet;
			computer_connection[cID]->in_packet=0;

			//ok let's do it
			if(!msg)
			{
				ShowError("Osz maj gat!\nNie ma komunikatu z internetu a powinien byæ");
				return 1001;
			}

			//----------
			if(msg->nadawca==NET_MSG_COMPUTER)
			{
				if(msg->typ==NET_COMTYPE_REG)	return RejestracjaKompa(msg, cID);
				else							return WyrejestrujKompa(msg, cID);
			}//wiadomoœæ do komputera
			else
			{
				if(msg->typ==NET_USERTYPE_GET_LIST_SCHOOL)	return PobierzListeSzkol(msg, cID);
				if(msg->typ==NET_USERTYPE_LOGIN)			return ZalogujUzytkownika(msg, cID);
				if(msg->typ==NET_USERTYPE_LOGOUT)			return WylogujUzytkownika(msg, cID);
				if(msg->typ==NET_USERTYPE_SETSTATE)			return ZmienWyniki(msg, cID);
				if(msg->typ==NET_USERTYPE_START_KONKURS)	return StartKonkursu(msg, cID);

			}//wiadomoœæ od usera
			//-----------
			delete msg;
			msg=0;

		}//obs³uga wiadomoœci messagecome

	}//switch rodzaj wiadomoœci ev->type

	return 0;
}


int Desktop::NetWorkTask(EventMem* ev)
{
	switch(ev->type)
	{
	case EVENT_DISCONNECT_COMPUTER:
		{
			if(!ev->data)return 2000;
			unsigned int cID =  *(unsigned int*)(ev->data);
			if(cID >= computer_connection.size() ) return 2001;
			if(!computer_connection[cID]) return 2002;
			if(!(computer_connection[cID]->flag & ComputerConn::computer_login))return 2003; 
			
			//--  wylogowanie uzytkownika --
			int uID= computer_connection[cID]->uID;
			if(uID != -1)
			{
				//pobranie informacji
				LongInfoUser user;
				PublicDB.GetFullInfoAboutUser(uID, user);
				PublicDB.Logout(uID);

				//wys³anie zdarzenia
				EventMem* em=new EventMem;
				em->type = EVENT_USER_LOGIN;

				ShortUserPublicInfo* info=new ShortUserPublicInfo;
				info->login=false;
				info->sID=user.sID;
				info->uID=user.uID;

				em->data=info;
				event.push(em);

				//zmienienie danych po³¹czenia
				computer_connection[cID]->uID=-1;
				computer_connection[cID]->flag &= ~ComputerConn::wait_to_start;

				//zalogowanie informacji
				serwer_win->SaveLogs("U¿ytkownik nr. "+std::to_string((long long)user.uID)+", "+user.imie+" "+user.nazwisko+" zosta³ wylogowany");
			}

			//----------  usuniecie komputera  ----------
			computer_connection[cID]->flag &= ~ComputerConn::computer_login;
			//--wys³anie infa
			EventMem* em = new EventMem;
			em->type = EVENT_COMPUTER_LOGOUT;
			em->data = new int(cID);
					
			event.push(em);

			//--wys³anie infa
			em = new EventMem;
			em->type = EVENT_COMPUTER_LOGOUT;
			em->data = new int(cID);
					
			event.push(em);

			//----------------------------
			serwer_win->SaveLogs("Komputer nr. "+std::to_string((long long)cID) +", "+ computer_connection[cID]->computer_name+ " zosta³ wyrejestrowany");
			if(computer_connection[cID]->uID)
			{
				computer_connection[cID]->uID=-1;
			}
			computer_connection[cID]->out_packet=CreatePacket(0,0,NET_MSG_COMPUTER, NET_COMTYPE_UNREG, 0,0,0);
		}
		break;

	case EVENT_LOGOUT_USER:
		{
			if(!ev->data)return 2004;
			unsigned int cID =  *(unsigned int*)(ev->data);
			if(cID >= computer_connection.size() ) return 2005;
			if(!computer_connection[cID]) return 2006;
			if(!(computer_connection[cID]->flag & ComputerConn::computer_login))return 2007; 
			int uID= computer_connection[cID]->uID;
			if(uID == -1) return 2008;

			//pobranie informacji
			LongInfoUser user;
			PublicDB.GetFullInfoAboutUser(uID, user);
			PublicDB.Logout(uID);

			//wys³anie zdarzenia
			EventMem* em=new EventMem;
			em->type = EVENT_USER_LOGIN;

			ShortUserPublicInfo* info=new ShortUserPublicInfo;
			info->login=false;
			info->sID=user.sID;
			info->uID=user.uID;

			em->data=info;
			event.push(em);
			//---
			em = new EventMem;
			em->type = EVENT_COMPUTER_CHANGE_STATUS;

			ChangeCompStatus* info2 = new ChangeCompStatus;
			info2->cID = cID;
			info2->status=0;

			em->data = info2;
			event.push(em);
			//---------

			//zmienienie danych po³¹czenia
			computer_connection[cID]->uID=-1;
			computer_connection[cID]->flag &= ~ComputerConn::wait_to_start;
			//wys³anie paczki
			computer_connection[cID]->out_packet=CreatePacket(0,0,NET_MSG_USER, NET_USERTYPE_LOGOUT, 0,0,0);

			//zalogowanie informacji
			serwer_win->SaveLogs("U¿ytkownik nr. "+std::to_string((long long)user.uID)+", "+user.imie+" "+user.nazwisko+" zosta³ wylogowany");
		}
		break;

	case EVENT_START_GAME:
		{
			for( unsigned int i = 0; i< computer_connection.size(); ++i)
			{
				if(computer_connection[i] && (computer_connection[i]->flag & ComputerConn::wait_to_start))
				{
					if(computer_connection[i]->uID != -1)	//if i make some mistake
					{
						computer_connection[i]->out_packet=CreatePacket(0,0,NET_MSG_USER, NET_USERTYPE_START_KONKURS, 0,sizeof(config), (unsigned char*)new __Config(config));
						
						// wysy³anie info do zak³adki komputery
						EventMem* em = new EventMem;
						em->type = EVENT_COMPUTER_CHANGE_STATUS;

						ChangeCompStatus* info = new ChangeCompStatus;
						info->cID = i;
						info->status = play_game;

						em->data=info;
						event.push(em);
					}

					computer_connection[i]->flag &= ~ComputerConn::wait_to_start;
				}
			}
			break;
		}
	}
	return 0;
}


void Desktop::CenterWindow(Window* win)
{
	sf::Vector2f final_pos;
	final_pos.x=res.x/2.f;
	final_pos.y=res.y/2.f;
	final_pos.x-= win->GetWindow()->GetClientRect().width/2.f;
	final_pos.y-= win->GetWindow()->GetClientRect().height/2.f;
	win->GetWindow()->SetPosition(final_pos);
}


//////////////////////////////////////////////////////////////////////////////////////
// poboczne funkcje

int Desktop::RejestracjaKompa(NetPacket* msg, unsigned int cID)
{
	//przygotowanie
	if(msg->size > 30 || msg->size == 0)
	{
		computer_connection[cID]->out_packet = 
			CreatePacket(0,0,NET_MSG_COMPUTER, NET_COMTYPE_REG, 
			NET_ERROR_BAD_COMPUTER_NAME, 0, 0);
		serwer_win->SaveLogs("Ktoœ próbuje zarejestrowaæ kompa o nazwie zbyt d³ugiej lub bez niej (max. 30)");
		return 0;
	}
	if(ComputerConnected(cID))
	{
		computer_connection[cID]->out_packet = 
			CreatePacket(0,0,NET_MSG_COMPUTER, NET_COMTYPE_REG, 
			NET_ERROR_COMP_ALREADY_LOGIN, 0, 0);
		return 1100;
	}
	std::string komp;
	komp.resize(msg->size);
	memcpy(&komp[0],(char*)msg->msg, msg->size);
	serwer_win->SaveLogs("Po³¹czono z: "+komp+" - "+
		computer_connection[cID]->ip_computer+":"+
		computer_connection[cID]->port_computer);
	

	computer_connection[cID]->hash=rand();
	computer_connection[cID]->computer_name=komp;
	computer_connection[cID]->flag |= ComputerConn::computer_login;
	
	//odpowiedŸ
	char* string=new char[12+logo_name.size()+1];	//cID, hash, sizeof(logoname)+null
	char* rac_mem=string;	//random_acces memory
	memcpy(rac_mem, &cID, sizeof(cID));
	rac_mem+=sizeof(size_t);

	memcpy(rac_mem, &(computer_connection[cID]->hash) ,sizeof(__int64));
	rac_mem+=sizeof(__int64);

	memcpy(rac_mem, logo_name.c_str(), logo_name.size()+1);

	computer_connection[cID]->out_packet = 
		CreatePacket(0,0,NET_MSG_COMPUTER, NET_COMTYPE_REG, 
		0, 12+logo_name.size()+1, (unsigned char*)string);
	
	// logowanie
	serwer_win->SaveLogs("Komputer nr. "+std::to_string((long long)cID) +", "+ computer_connection[cID]->computer_name+ " zarejestrowa³ siê");

	//poinformowanie gui o zaistnia³ej sytuacji
	EventMem* em = new EventMem;
	em->type = EVENT_COMPUTER_LOGIN;

	ID_Name* info= new ID_Name;
	info->id = cID;
	info->name = komp;

	em->data = info;
	event.push(em);
	return 0;
}


int Desktop::WyrejestrujKompa(NetPacket* msg, unsigned int cID)
{
	if(!ComputerConnected(cID))
	{
		computer_connection[cID]->out_packet = 
			CreatePacket(0,0,NET_MSG_COMPUTER, NET_COMTYPE_UNREG, NET_ERROR_NOT_CONNECT,0,0);
		return 1101;
	}

	//--  wylogowanie uzytkownika --
	int uID= computer_connection[cID]->uID;
	if(uID != -1)
	{
		//pobranie informacji
		LongInfoUser user;
		PublicDB.GetFullInfoAboutUser(uID, user);
		PublicDB.Logout(uID);

		//wys³anie zdarzenia
		EventMem* em=new EventMem;
		em->type = EVENT_USER_LOGIN;

		ShortUserPublicInfo* info=new ShortUserPublicInfo;
		info->login=false;
		info->sID=user.sID;
		info->uID=user.uID;

		em->data=info;
		event.push(em);

		//zmienienie danych po³¹czenia
		computer_connection[cID]->uID=-1;
		computer_connection[cID]->flag &= ~ComputerConn::wait_to_start;

		//zalogowanie informacji
		serwer_win->SaveLogs("U¿ytkownik nr. "+std::to_string((long long)user.uID)+", "+user.imie+" "+user.nazwisko+" zosta³ wylogowany");
	}

	//--wys³anie infa
	EventMem* em = new EventMem;
	em->type = EVENT_COMPUTER_LOGOUT;
	em->data = new int(cID);
					
	event.push(em);

	//----------------------------
	serwer_win->SaveLogs("Komputer nr. "+std::to_string((long long)cID) +", " 
							+ computer_connection[cID]->computer_name+ " wyrejestrowa³ siê");
	if(computer_connection[cID]->uID)
	{
		computer_connection[cID]->uID=-1;
	}
	
	computer_connection[cID]->out_packet = 
		CreatePacket(0,0,NET_MSG_COMPUTER, NET_COMTYPE_UNREG, 0,0,0);
	
	// ------- usuniecie kompa --------
	computer_connection[cID]->flag &=	~ComputerConn::computer_login;
	return 0;
}

int Desktop::PobierzListeSzkol(NetPacket* msg, unsigned int cID)
{
	if(!ComputerConnected(cID)||msg->msg==0)
	{
		computer_connection[cID]->out_packet = 
			CreatePacket(0, 0, NET_MSG_USER, NET_USERTYPE_GET_LIST_SCHOOL, NET_ERROR_NOT_CONNECT, 0, 0);
		if(msg->msg==0)return 1300;
		return 1102;
	}
	//przygotowanie
	std::string imie,nazwisko;
	int help;
	memcpy(&help, msg->msg, 4);
	if(help==0)
	{
		computer_connection[cID]->out_packet=CreatePacket(0,0,NET_MSG_USER, NET_USERTYPE_GET_LIST_SCHOOL,NET_ERROR_USER_NOT_EXIST,0,0);
		return 0;
	}
	imie.resize(help);
	memcpy(&imie[0], msg->msg+4, help);
	memcpy(&help, msg->msg+4+help, 4);
	if(help==0)
	{
		computer_connection[cID]->out_packet=CreatePacket(0,0,NET_MSG_USER, NET_USERTYPE_GET_LIST_SCHOOL,NET_ERROR_USER_NOT_EXIST,0,0);
		return 0;
	}
	nazwisko.resize(help);
	memcpy(&nazwisko[0], msg->msg+4+imie.size()+4, help);

	std::vector< ID_Name >info;
	PublicDB.GetSchoolListForUser(imie, nazwisko, info);
	if(info.size()==0)
	{
		computer_connection[cID]->out_packet=CreatePacket(0,0,NET_MSG_USER, NET_USERTYPE_GET_LIST_SCHOOL,NET_ERROR_USER_NOT_EXIST,0,0);
		return 0;
	}
	help=info.size();
	std::vector<char> msgv;
	int size=4;
	msgv.resize(4);
	memcpy(&msgv[0], &help, 4);
	for(unsigned int i=0; i<info.size(); ++i)
	{
		size+=4;
		msgv.resize(size);
		help=info[i].name.size();
		memcpy(&msgv[size-4], &help, 4);
		size+=help;
		msgv.resize(size);
		memcpy(&msgv[size-help], info[i].name.data(), help);
		//sid
		size+=4;
		msgv.resize(size);
		help=info[i].id;
		memcpy(&msgv[size-4], &help, 4);
	}
	unsigned char* wsk=new unsigned char[msgv.size()];
	memcpy(wsk, msgv.data(), msgv.size());
	computer_connection[cID]->out_packet=CreatePacket(0,0,NET_MSG_USER, NET_USERTYPE_GET_LIST_SCHOOL,0,msgv.size(), wsk);
	
	//zalogowanie
	serwer_win->SaveLogs("Komputer nr. "+std::to_string((long long)cID)+", "+computer_connection[cID]->computer_name+" pobra³ listê szkó³");
	return 0;
}

int Desktop::ZalogujUzytkownika(NetPacket* msg, unsigned int cID)
{
	if(!ComputerConnected(cID)||msg->msg==0)
	{
		computer_connection[cID]->out_packet = 
			CreatePacket(0, 0, NET_MSG_USER, NET_USERTYPE_LOGIN, NET_ERROR_NOT_CONNECT, 0, 0);
		if(msg->msg==0)return 1301;
		return 1103;
	}

	std::string imie, nazwisko;
	unsigned __int32 sID,  uID;
	int size;
	memcpy(&size, msg->msg, 4);
	imie.resize(size);
	memcpy(&imie[0], msg->msg+4, size);
	memcpy(&size, msg->msg+4+size, 4);
	nazwisko.resize(size);
	memcpy(&nazwisko[0], msg->msg+8+imie.size(),size);
	memcpy(&sID, msg->msg+8+imie.size()+nazwisko.size(),4);
						
	if(computer_connection[cID]->uID!=-1)//ju¿ jest zalogowany
	{
		computer_connection[cID]->out_packet=CreatePacket(0,0,NET_MSG_USER, NET_USERTYPE_LOGIN, NET_ERROR_COMP_ALREADY_LOGIN_USER,0,0);
		return 1104;
	}
	uID=PublicDB.LoginUser(imie,nazwisko, sID);
	if(uID==-1)
	{
		computer_connection[cID]->out_packet=CreatePacket(0,0,NET_MSG_USER, NET_USERTYPE_LOGIN, NET_ERROR_USER_NOT_EXIST,0,0);
		return 1105;
	}
	computer_connection[cID]->uID=uID;
	char* string=new char[18];
	char* rac_msg= string;

	memcpy(rac_msg, &uID, sizeof(uID));
	rac_msg+=sizeof(uID);

	memcpy(rac_msg, &(computer_connection[cID]->hash), sizeof(computer_connection[cID]->hash));
	rac_msg+=sizeof(computer_connection[cID]->hash);

	LongInfoUser user;
	PublicDB.GetFullInfoAboutUser(uID, user);

	BYTE stats[3][2];

	for(int i=0; i<3; ++i)
	{
		stats[i][0] = user.wyniki[i];
		if(stats[i][0]==255)  stats[i][1]=255;
		else stats[i][1] = config.max_word - stats[i][0];	//obliczanie niepoprawnych wyników
		if(stats[i][1]>10 && stats[i][0]!=255)	stats[i][1] = 10;
	}

	memcpy(rac_msg, stats, 6);

	computer_connection[cID]->out_packet=CreatePacket(0,0,NET_MSG_USER, NET_USERTYPE_LOGIN, 0,18, (unsigned char*)string);
	//---
	EventMem* em=new EventMem;
	em->type = EVENT_USER_LOGIN;

	ShortUserPublicInfo* info=new ShortUserPublicInfo;
	info->login=true;
	info->sID=user.sID;
	info->uID=user.uID;

	em->data=info;
	event.push(em);
	//---
	em=new EventMem;
	em->type = EVENT_COMPUTER_USER_LOGIN;

	ID_Name* info2= new ID_Name;
	info2->name = user.imie+"  "+user.nazwisko;
	info2->id = cID;

	em->data = info2;
	event.push(em);
	//zalogowanie info
	serwer_win->SaveLogs("U¿ytkownik nr. "+std::to_string((long long)uID)+", "+user.imie+" "+user.nazwisko+" zalogowa³ siê");
	return 0;
}

int Desktop::WylogujUzytkownika(NetPacket* msg, unsigned int cID)
{
	if(!ComputerConnected(cID))
	{
		computer_connection[cID]->out_packet = 
			CreatePacket(0, 0, NET_MSG_USER, NET_USERTYPE_LOGOUT, NET_ERROR_NOT_CONNECT, 0, 0);
		return 1106;
	}

	unsigned __int32 uID = computer_connection[cID]->uID;
	if(uID == -1)
	{
		computer_connection[cID]->out_packet = 
			CreatePacket(0, 0, NET_MSG_USER, NET_USERTYPE_LOGOUT, NET_ERROR_USER_NOT_CONNECT, 0, 0);
		return 1107;
	}
	
	if(!PublicDB.Logout(uID))
	{
		computer_connection[cID]->out_packet = 
			CreatePacket(0, 0, NET_MSG_USER, NET_USERTYPE_LOGOUT, NET_ERROR_USER_NOT_CONNECT, 0, 0);
		return 1108;
	}
	//pobranie informacji
	LongInfoUser user;
	PublicDB.GetFullInfoAboutUser(uID, user);

	//wys³anie zdarzenia
	EventMem* em=new EventMem;
	em->type = EVENT_USER_LOGIN;

	ShortUserPublicInfo* info=new ShortUserPublicInfo;
	info->login=false;
	info->sID=user.sID;
	info->uID=user.uID;

	em->data=info;
	event.push(em);
	//---
	em = new EventMem;
	em->type = EVENT_COMPUTER_CHANGE_STATUS;

	ChangeCompStatus* info2 = new ChangeCompStatus;
	info2->cID = cID;
	info2->status=0;

	em->data = info2;
	event.push(em);
	//-------
	//zmienienie danych po³¹czenia
	computer_connection[cID]->uID=-1;
	computer_connection[cID]->out_packet = CreatePacket(0, 0, NET_MSG_USER, NET_USERTYPE_LOGOUT, 0, 0, 0);

	//zalogowanie informacji
	serwer_win->SaveLogs("U¿ytkownik nr. "+std::to_string((long long)user.uID)+", "+user.imie+" "+user.nazwisko+" wylogowa³ siê");
	return 0;
}

int Desktop::ZmienWyniki(NetPacket* msg, unsigned int cID)
{
	if(!ComputerConnected(cID)||msg->msg==0)
	{
		computer_connection[cID]->out_packet = 
			CreatePacket(0, 0, NET_MSG_USER, NET_USERTYPE_SETSTATE, NET_ERROR_NOT_CONNECT, 0, 0);
		if(msg->msg==0)return 1301;
		return 1109;
	}

	unsigned __int32 uID = computer_connection[cID]->uID;
	if(uID == -1)
	{
		computer_connection[cID]->out_packet = 
			CreatePacket(0, 0, NET_MSG_USER, NET_USERTYPE_SETSTATE, NET_ERROR_USER_NOT_CONNECT, 0, 0);
		return 1110;
	}

	BYTE tab[3];
	BYTE *stab=msg->msg;
	for(int i=0;i<3;++i)
	{
		tab[i]=stab[i*2];	//odcedzenie danych (tylko dobre wyniki)
	}	
				
	if(PublicDB.SetStats(uID, tab) == false)
	{
		computer_connection[cID]->out_packet = CreatePacket(0,0,NET_MSG_USER, NET_USERTYPE_SETSTATE, NET_ERROR_UNEXPECTED,0,0);
		return 1111;
	}
	//wys³anie komunikatu zmieniaj¹cego dane  w GUI
	EventMem* em= new EventMem;
	em->type = EVENT_CHANGE_STATS;
	em->data = new LongInfoUser;
	PublicDB.GetFullInfoAboutUser(uID, *(LongInfoUser*)em->data );
	event.push(em);

	computer_connection[cID]->out_packet = CreatePacket(0,0,NET_MSG_USER, NET_USERTYPE_SETSTATE, 0,0,0);

	//zalogowanie informacji
	serwer_win->SaveLogs("U¿ytkownik nr. "+std::to_string((long long)uID)+", "
		+((LongInfoUser*)em->data)->imie+" "+((LongInfoUser*)em->data)->nazwisko+" zmieni³ wyniki");
	return 0;
}

int Desktop::StartKonkursu(NetPacket* msg, unsigned int cID)
{
	if(!ComputerConnected(cID))
	{
		computer_connection[cID]->out_packet = 
			CreatePacket(0, 0, NET_MSG_USER, NET_USERTYPE_SETSTATE, NET_ERROR_NOT_CONNECT, 0, 0);
		return 1112;
	}

	unsigned __int32 uID = computer_connection[cID]->uID;
	if(uID == -1)
	{
		computer_connection[cID]->out_packet = 
			CreatePacket(0, 0, NET_MSG_USER, NET_USERTYPE_SETSTATE, NET_ERROR_USER_NOT_CONNECT, 0, 0);
		return 1113;
	}

	//pobranie informacji
	LongInfoUser user;
	if(!PublicDB.GetFullInfoAboutUser(uID, user))
	{
		computer_connection[cID]->out_packet = 
			CreatePacket(0, 0, NET_MSG_USER, NET_USERTYPE_SETSTATE, NET_ERROR_UNEXPECTED, 0, 0);
		return 1114;
	}
	//ok
	computer_connection[cID]->flag|=ComputerConn::wait_to_start;

	//zalogowanie informacji
	serwer_win->SaveLogs("U¿ytkownik nr. "+std::to_string((long long)uID)+", "	+user.imie+" "+user.nazwisko+" prosi o start konkursu");
					
	EventMem* em = new EventMem;
	em->type = EVENT_COMPUTER_CHANGE_STATUS;

	ChangeCompStatus* info = new ChangeCompStatus;
	info->cID = cID;
	info->status = wait_for_start;

	em->data=info;
	event.push(em);
	return 0;
}