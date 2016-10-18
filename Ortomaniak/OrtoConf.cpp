#include "OrtoConf.h"

//**************  SWITCHER  -  SUWAK  ******************
OrtoConf::Switcher::Switcher(
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
			Vector2f pos_txt
			):clicked(false)
	{
		//human
		human.min_point=_min_point;
		human.max_step_count=_max_step_count;

		//select
		select.tekst_pre	= _tekst_pre;
		select.tekst_post	= _tekst_post;
		select.cur_sel		= _cur_sel;
		select.element_by_step=_element_by_step;
		LoadFont(CONF_SCROLL_FONT,select.font,CONF_SCROLL_TEXTSIZE);
		select.txt.SetSize(CONF_SCROLL_TEXTSIZE);
		select.txt.SetFont(select.font);
		select.txt.SetPosition(pos_txt);
		select.txt.SetColor(Color::Black);
		select.RefreshText(GetValue());
		select.txt.SetCenter(select.txt.GetRect().GetWidth()/2.f,select.txt.GetRect().GetHeight()/2.f);


		//pixel
		pixel.pos_start=_pos_start;
		pixel.pos_end=_pos_end;
		pixel.pos_y=_pos_y;
		pixel.move_step=(pixel.pos_end-pixel.pos_start)/(float)_max_step_count;

		//wewnêtrzne
		suwak_img.LoadFromMemory(zaladuj(IDB_SUWAK,L"PNG"),rozmiar(IDB_SUWAK,L"PNG"));
		suwak.SetImage(suwak_img);
		suwak.SetPosition(_pos_start+pixel.move_step*_cur_sel,_pos_y);
		suwak.SetCenter(suwak_img.GetWidth()/2.f,suwak_img.GetHeight()/2.f);
	}

int OrtoConf::Switcher::MoveMouse(Vector2f pos)
{
	if(!clicked)return 0;
	if(pos.x>pixel.pos_end)pos.x=pixel.pos_end;
	if(pos.x<pixel.pos_start)pos.x=pixel.pos_start;
	suwak.SetPosition(pos.x,pixel.pos_y);
	unsigned int difference_pos=pos.x-pixel.pos_start;
	unsigned int sel=difference_pos/pixel.move_step;
	if(select.cur_sel!=sel)
	{
		select.cur_sel=sel;
		select.RefreshText(GetValue());
	}
	return 1;
}

int OrtoConf::Switcher::MouseClicked(Vector2f Mpos)
{
	IntRect rect=suwak.GetSubRect();
	Mpos-=suwak.GetPosition()-suwak.GetCenter();
	if(rect.Contains(Mpos.x,Mpos.y))	//Check Rect 
	{
		Vector2f point(Mpos.x,Mpos.y);
		if(suwak_img.GetPixel(point.x,point.y).a!=0)		//Check Real click
			{clicked=true; return 1;}
	}
	return 0;
}

unsigned int OrtoConf::Switcher::SetValue(unsigned int x)
{
	x-=human.min_point;
	select.cur_sel=x/select.element_by_step;
	select.RefreshText(GetValue());
	return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////  O R T O C O N F --- O R T O C O N F  --  O R T O C A N F ////////////////
//******************************************************************************************//
OrtoConf::OrtoConf(void):
///////  czas na slowko - suwak  ///////
	czas_na_slowko(
		//pixel
		CONF_SCROLL_LEFT,
		CONF_SCROLL_RIGHT,
		CONF_TIME_POS_Y,

		//human
		MIN_TIME_STEP,
		MAX_TIME_STEP,

		//select
		"Czas na s³ówko: ",
		"",
		DEFAULT_TIME_VALUE,
		TIME_STEP,
		Vector2f(CONF_SCROLL_TEXTPOS_1)
		),
///////  ilosc slowek - suwak  ///////
	ilosc_slowek(
		//pixel
		CONF_SCROLL_LEFT,
		CONF_SCROLL_RIGHT,
		CONF_WORD_POS_Y,

		//human
		MIN_WORD_STEP,
		MAX_WORD_STEP,

		//select
		"Iloœæ s³ówek: ",
		"",
		DEFAULT_WORD_VALUE,
		WORD_STEP,
		Vector2f(CONF_SCROLL_TEXTPOS_2)
		)
///////// -- w³aœciwy konstruktor --  /////////
{
	ActiveType=0x0007;
	initialize=false;
	/////  O B R A Z K I  --  L O A D & S E T  /////
	render=false;

	tlo_img.LoadFromMemory(zaladuj(IDB_ORTOCONF_LAYOUT,L"PNG"),rozmiar(IDB_ORTOCONF_LAYOUT,L"PNG"));
	on_img.LoadFromMemory(zaladuj(IDB_ORTOCONF_ON,L"PNG"),rozmiar(IDB_ORTOCONF_ON,L"PNG"));
	off_img.LoadFromMemory(zaladuj(IDB_ORTOCONF_OFF,L"PNG"),rozmiar(IDB_ORTOCONF_OFF,L"PNG"));
	size_onoff.x=on_img.GetWidth();
	size_onoff.y=on_img.GetHeight();

	tlo.SetImage(tlo_img);
	tlo.SetPosition(CONF_LAYOUT_POS);
	for(unsigned int i=0;i<9;i++)
	{
		if((1<<i)&ActiveType)button[i].SetImage(on_img);
		else button[i].SetImage(off_img);
		button[i].SetPosition(CONF_POS_ONOFF[i]);
	}

	/////  T E K S T  --  S E T T I N G S /////
	LoadFont(CONF_FONT_TYPE,font,CONF_ACCEPT_SIZE_SMALL);

	accept.SetFont(font);
	accept.SetSize(CONF_ACCEPT_SIZE_SMALL);
	accept.SetPosition(CONF_ACCEPT_POS);
	accept.SetColor(Color::Black);
	accept.SetText("Akceptuj");
	SetCenterText(accept);

	cancel.SetFont(font);
	cancel.SetSize(CONF_EXIT_SIZE_SMALL);
	cancel.SetPosition(CONF_EXIT_POS);
	cancel.SetColor(Color::Black);
	cancel.SetText("WyjdŸ");
	SetCenterText(cancel);
	
	mousemov_chck=0;
	mousepress_chck=0;
	button_accept=false;
}


OrtoConf::~OrtoConf(void)
{
}

int OrtoConf::Draw(RenderWindow& wnd)
{
	if(dark.Draw(wnd)==-1)return 0;
	if(!render)return 0;
	wnd.Draw(tlo);
	
	for(unsigned int i=0;i<9;i++)wnd.Draw(button[i]);

	wnd.Draw(accept);
	wnd.Draw(cancel);
	ilosc_slowek.Draw(wnd);
	czas_na_slowko.Draw(wnd);
	return 0;
}

void OrtoConf::MouseMove(Vector2f pos)
{
	if(ilosc_slowek.MoveMouse(pos))return;
	if(czas_na_slowko.MoveMouse(pos))return;
	switch(mousemov_chck)
	{
	case 1:
		if(!accept.GetRect().Contains(pos.x,pos.y))
		{
			accept.SetSize(CONF_ACCEPT_SIZE_SMALL);
			SetCenterText(accept);
			mousemov_chck=0;
		}
		else return;
		break;
	case 2:
		if(!cancel.GetRect().Contains(pos.x,pos.y))
		{
			cancel.SetSize(CONF_EXIT_SIZE_SMALL);
			SetCenterText(cancel);
			mousemov_chck=0;
		}
		else return;
		break;
	}
	if(accept.GetRect().Contains(pos.x,pos.y))
	{
		accept.SetSize(CONF_ACCEPT_SIZE_BIG);
		SetCenterText(accept);
		mousemov_chck=1;
		return;
	}
	if(cancel.GetRect().Contains(pos.x,pos.y))
	{
		cancel.SetSize(CONF_EXIT_SIZE_BIG);
		SetCenterText(cancel);
		mousemov_chck=2;
		return;
	}
}

int OrtoConf::MouseDown(Vector2f pos)
{
	int i=0;
	for(;i<9;i++)
	{
		if(pos.x<CONF_POS_ONOFF[i].x)continue;
		if(pos.y<CONF_POS_ONOFF[i].y)continue;
		if(pos.x>CONF_POS_ONOFF[i].x+size_onoff.x)continue;
		if(pos.y>CONF_POS_ONOFF[i].y+size_onoff.y)continue;
		break;
	}
	if(i<9)//was pressed
	{
		int x=1<<i;
		if(ActiveType&x)
		{
			ActiveType&=~x;
			button[i].SetImage(off_img);
		}
		else
		{
			ActiveType|=x;
			button[i].SetImage(on_img);
		}
		return 0;
	}
	/////  cos inne naciœniête...
	if(accept.GetRect().Contains(pos.x,pos.y))
	{
		initialize=true;
		button_accept=true;
		ShowWindowTypes(false);
		return 0;
	}
	if(cancel.GetRect().Contains(pos.x,pos.y))
	{
		initialize=true;
		button_accept=false;
		ShowWindowTypes(false);
		return 0;
	}

	/////  mo¿e suwak
	if(ilosc_slowek.MouseClicked(pos))return 0;
	if(czas_na_slowko.MouseClicked(pos))return 0;
	//nic ciekawego
	return 0;
}

void OrtoConf::MouseUp()
{
	ilosc_slowek.MouseReleased();
	czas_na_slowko.MouseReleased();
	if(mousepress_chck)return;
	mousepress_chck=0;
	return;
}


void OrtoConf::Deinitialize()
{
	initialize=false;
	button_accept=false;
	//ustawienie na ma³¹ czcionkê przycisku 
	//akceptuj...
	accept.SetSize(CONF_ACCEPT_SIZE_SMALL);
	SetCenterText(accept);
	//i wyjdŸ
	cancel.SetSize(CONF_EXIT_SIZE_SMALL);
	SetCenterText(cancel);
	mousemov_chck=0;

	dark.SetBlank(false);
}