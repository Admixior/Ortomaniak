#include "Game.h"


Game::Game(unsigned int win_height):h_win(win_height)
{
	podstawa_img.LoadFromMemory(zaladuj(IDB_PODEST,L"PNG"),rozmiar(IDB_PODEST,L"PNG"));
	belka_img.LoadFromMemory(zaladuj(IDB_BELKA,L"PNG"),rozmiar(IDB_BELKA,L"PNG"));
	podstawa.SetImage(podstawa_img);
	podstawa.SetPosition(GAME_PODSTAWA);
	podstawa_red.SetImage(podstawa_img);
	podstawa_red.SetPosition(GAME_PODSTAWA);
	podstawa_red.SetColor(Color::Red);
	belka.SetImage(belka_img);
	belka.SetPosition(GAME_BELKA);
	//----------
	my_word=NULL;
	LoadFont(GAME_FONT,font,static_cast<unsigned int>(GAME_FONT_SIZE_WORD));
	slowko.SetFont(font);
	left.SetFont(font);
	right.SetFont(font);
	slowko.SetSize(GAME_FONT_SIZE_WORD);
	left.SetSize(GAME_FONT_SIZE_ODP);
	right.SetSize(GAME_FONT_SIZE_ODP);
	
	slowko.SetColor(Color::Black);
	left.SetColor(Color::Black);
	right.SetColor(Color::Black);
	
	left.SetPosition(GAME_LEFT_POINT_ODP);
	right.SetPosition(GAME_RIGHT_POINT_ODP);
	//----------
	wid=wait_for_word;
	last_good_odp=-1;
	ResetGame();
}


Game::~Game(void)
{
}


int Game::InitializeWord()
{
	if(my_word==0)return -1;
	slowko.SetText(my_word->GetWord());
	//h_word=slowko.GetRect().GetHeight();
	slowko.SetCenter(slowko.GetRect().GetWidth()/2.f,h_word=GAME_SPACE_WORD);
	slowko.SetPosition(GAME_SLOWKA_OS,position_word_h=(GAME_SLOWKA_START+slowko.GetRect().GetHeight()));
	
	left.SetText(my_word->GetLeftName());
	right.SetText(my_word->GetRightName());
	right.SetCenter(right.GetRect().GetWidth(),0.f);

	return 0;
}

int Game::NextRedPosition()
{
	if(wid!=bad_odp)return 0;
	unsigned int add=static_cast<unsigned int>(timer.GetElapsedTime()/GAME_RED_FIELD_SPEED*static_cast<float>(h_word));

	if(add>=h_word)
	{
		red_position_last=red_position=red_position_last-h_word;
		wid=wait_for_word;
		return 0;
	}
	red_position=red_position_last-add;
	return 0;
}

int Game::MoveWord()
{
	if(wid!=fly_word)
	{
		if(wid==good_odp)
		{
			if(position_word_h==red_position+h_word)
			{
				slowko.SetPosition(GAME_SLOWKA_OS,position_word_h);
				wid=wait_for_word;
				return 0;
			}
			unsigned int add=timer.GetElapsedTime()/max_time*(GAME_SLOWKA_END-GAME_SLOWKA_START);
			position_word_h=red_position+add;
			slowko.SetPosition(GAME_SLOWKA_OS,position_word_h);
			if(position_word_h>=(red_position+h_word))
			{
				position_word_h=(red_position+h_word);
				slowko.SetPosition(GAME_SLOWKA_OS,position_word_h);
				wid=wait_for_word;
			}
			return 0;
		}
		else return 0;
	}
	unsigned int add=timer.GetElapsedTime()/max_time*(GAME_SLOWKA_END-GAME_SLOWKA_START);
	position_word_h=GAME_SLOWKA_START+add;

	if(position_word_h>=red_position)CheckOdp();
	slowko.SetPosition(GAME_SLOWKA_OS,position_word_h);
	return 0;
}

int Game::NoweSlowko(Word* new_word)
{
	my_word=new_word;
	my_word->Reset();
	wid=fly_word;
	timer.Reset();
	return InitializeWord();
}

int Game::Draw(RenderWindow& wnd)
{
	MoveWord();
	NextRedPosition();
	::glEnable( GL_SCISSOR_TEST );
	::glScissor( 0,h_win*(1080.f - GAME_SLOWKA_END)/1080.f /*915*/ , wnd.GetWidth(), h_win*(GAME_SLOWKA_END-GAME_SLOWKA_START)/1080.f );
	wnd.Draw(podstawa);
	wnd.Draw(slowko);
	::glScissor( 0,h_win*(1080.f - GAME_SLOWKA_END)/1080.f /*915*/ , wnd.GetWidth(), h_win*(GAME_SLOWKA_END-red_position)/1080.f );
	wnd.Draw(podstawa_red);
	for(size_t i=0;i<bad_word.size();i++)wnd.Draw(bad_word[i]);
	::glDisable( GL_SCISSOR_TEST );
	wnd.Draw(belka);
	wnd.Draw(left);
	wnd.Draw(right);
	return 0;
}

int Game::ResetGame()
{
	bad_word.clear();
	red_position=red_position_last=GAME_SLOWKA_END;
	last_good_odp=-1;
	slowko.SetText("");
	left.SetText("");
	right.SetText("");
	full_red=false;
	return 0;
}

int Game::KeyPressed(ButtonJoy button)
{
	if(wid!=fly_word)return -1;
	if(my_word==0)return -1;
	switch(button)
	{
	case joy_down:
		if(CheckOdp())
		{
			position_word_h+=h_word;
		}
		break;
	case joy_right:
		slowko.SetText(my_word->GetWord(false));
		break;
	case joy_left:
		slowko.SetText(my_word->GetWord(true));
		break;
	}
	slowko.SetCenter(slowko.GetRect().GetWidth()/2.f,h_word);
	return 0;
}

bool Game::CheckOdp()
{
	position_word_h=red_position;
	slowko.SetPosition(GAME_SLOWKA_OS,position_word_h);
	last_good_odp=my_word->GetOdp();
	if(last_good_odp)wid=good_odp;
	else
	{
		slowko.SetText(my_word->GetTrueWord());
		slowko.SetCenter(slowko.GetRect().GetWidth()/2.f,h_word);
		wid=bad_odp;
		bad_word.push_back(slowko);
	}
	timer.Reset();
	return last_good_odp;
}


bool Game::FullRed()
{
	if(full_red)return false;
	wid=bad_odp;
	red_position+=h_word;
	timer.Reset();
	full_red=true;
	return true;
}
