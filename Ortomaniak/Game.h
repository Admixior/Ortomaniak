#pragma once
#include"orto.h"
#include "Word.h"
using namespace sf;
class Game
{
	public:
	enum what_i_do
	{
		wait_for_word,
		fly_word,
		good_odp,
		bad_odp
	};
	enum ButtonJoy
	{
		joy_down,
		joy_left,
		joy_right
	};
	int NoweSlowko(Word* new_word);
	int Draw(RenderWindow& wnd);
	int ResetGame();
	void SetMaxTime(float _max_time){max_time=_max_time;}
	what_i_do GetWork(){ return wid;  }
	Game(unsigned int win_height);
	int GetCountBadOdp(){ return bad_word.size(); }
	~Game(void);
	int KeyPressed(ButtonJoy button);
	vector<String>& GetListBadWord(){ return bad_word; }
	bool Game::FullRed();
private:
	Image podstawa_img,
		belka_img;
	Sprite podstawa,
		podstawa_red,
		belka;

	vector<String> bad_word;
	Word* my_word;
	Font font;
	String slowko
		,left
		,right;

	unsigned int h_word;	//wysokoœæ s³ówka
	float position_word_h;
	unsigned int red_position,red_position_last,h_win;
	Clock timer;
	float max_time;
	bool last_good_odp;
	bool full_red;

	what_i_do wid;				//co aktualnie robie
	int InitializeWord();		//ustawia tekst lewego, takst i pozycje prawego i slowka, h_word
	int NextRedPosition();
	int MoveWord();
	bool CheckOdp();
};

