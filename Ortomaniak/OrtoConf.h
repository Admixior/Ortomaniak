#pragma once
#include "Darkness.h"
#include <sstream>
using namespace sf;
using namespace std;

class OrtoConf
{
	unsigned __int16 ActiveType;
	bool initialize,button_accept;

	//******************   K L A S A  -  S U W A K *****************
	class Switcher
	{
		Image suwak_img;
		Sprite suwak;
		struct			//dane aktualnego zaznaczenia
		{
			Font font;						//czcionka
			string tekst_pre,
				tekst_post;					//txt do tworzenia
			String txt;						//txt - do wyœwietlania
			unsigned int cur_sel,			//aktualne zaznaczenie
						element_by_step;	//iloœæ pkt na krok
			void RefreshText(unsigned int value)
			{
				ostringstream str;
				str<<tekst_pre<<value<<tekst_post;
				txt.SetText(str.str());
			}
		}select;
		struct			//---- - dane w  punktach - ----
		{
			unsigned int min_point,		//minimalna liczba pkt
				max_step_count;			//maksymalna iloœæ krokow
		}human;
		struct			//---- - dane w pixelach - ----
		{
			float pos_start		//max wychylenie w lewo
			,pos_end			//max w prawo
			,pos_y				//const pos na osi y
			,move_step;			//iloœæ pixeli na jeden krok
		}pixel;
		
		bool clicked;
		Switcher();
	public:
		Switcher(
			//pixel
			float _pos_start,
			float _pos_end,
			float _pos_y,

			//human
			unsigned int _min_point,
			unsigned int _max_step_count,
			
			//select
			string _tekst_pre,
			string _tekst_post,
			unsigned int _cur_sel,
			unsigned int _element_by_step,
			Vector2f pos
			);


		bool GetActive(){return clicked;}
		int MoveMouse(Vector2f pos);
		int MouseClicked(Vector2f pos);
		void MouseReleased(){clicked=false;}

		unsigned int GetValue()
			{return select.cur_sel*select.element_by_step+human.min_point;}
		unsigned int SetValue(unsigned int x);

		void Draw(RenderWindow& wnd)
		{
			wnd.Draw(suwak);
			wnd.Draw(select.txt);
		}
	}
	ilosc_slowek, czas_na_slowko;
	//******************  K O N I E C - K L A S A  -  S U W A K *****************
	bool render;
	Darkness dark;
	Image tlo_img, on_img, off_img;
	Sprite tlo;
	Sprite button[9];
	Vector2f size_onoff;

	Font font;
	String accept
		,cancel;

	BYTE mousemov_chck;
	BYTE mousepress_chck;
public:
	unsigned __int16 GetActiveTypes(){return ActiveType;}
	unsigned int GetMaxWord(){return ilosc_slowek.GetValue();}
	unsigned int GetMaxTime(){return czas_na_slowko.GetValue();}
	bool GetInitialize(){return initialize;}
	bool GetRender(){return render;}

	void Deinitialize();
	void SetTypes(unsigned __int16 newtypes,float newtime, unsigned int maxwords)
	{
		ActiveType=newtypes;
		czas_na_slowko.SetValue(newtime);
		ilosc_slowek.SetValue(maxwords);
		initialize=true;
		button_accept=true;
	}
	int Draw(RenderWindow& wnd);
	void ShowWindowTypes(bool show)
	{
		dark.SetBlank(render=show);
	}
	void MouseMove(Vector2f pos);
	int MouseDown(Vector2f pos);
	void MouseUp();
	bool  GetButtonAccept(){return button_accept;}
	OrtoConf(void);
	~OrtoConf(void);
};

