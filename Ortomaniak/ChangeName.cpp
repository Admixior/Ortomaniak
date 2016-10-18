#include "ChangeName.h"


ChangeName::ChangeName(void):
	pkt(CHG_NAME_POSITION)
	,success_entered(false)
	,wid(idle)
{
	//---podstawa textu---
	podstawa_img.LoadFromMemory(zaladuj(IDB_CHG_PODSTAWA,L"PNG"),rozmiar(IDB_CHG_PODSTAWA,L"PNG"));
	podstawa.SetImage(podstawa_img);
	podstawa.SetPosition(CHG_NAME_POSITION);
	img_size=podstawa.GetSize();
	//---txt---
	//zaladowanie czcionek
	LoadFont(FONT_NAME,font_imie,FIRST_NAME_WEIGHT);
	LoadFont(FONT_NAME,font_nazwisko,SURNAME_WEIGHT);
	//i ustawienie
	imie.SetFont(font_imie);
	nazwisko.SetFont(font_nazwisko);
	//koloru
	imie.SetColor(Color(0,0,0));
	nazwisko.SetColor(Color(0,0,0));
	//obrócenie
	imie.SetRotation(ANGLE_NAME);
	nazwisko.SetRotation(ANGLE_NAME);
	//ustawienie pozycji
	imie.SetPosition(FIRST_NAME_CENTER_POINT);
	nazwisko.SetPosition(SURNAME_CENTER_POINT);

	//ustawienie domyœlnego nazwiska...
	nazwisko_text="Nazwisko";
	nazwisko.SetText(nazwisko_text);
	MakeSmaller(nazwisko,SURNAME_WEIGHT,SURNAME_RIGHT_POINT);
	//...i imienia
	imie_text="Imiê";
	imie.SetText(imie_text);
	MakeSmaller(imie,FIRST_NAME_WEIGHT,FIRST_NAME_RIGHT_POINT);
}


ChangeName::~ChangeName(void)
{
}


	
bool is_text(char c)
{
	if((c>=L'A'&&c<=L'Z')	||	(c>=L'a'&&c<=L'z'))
		return true;
	string pol="êó¹œ³¿ŸæñÊÓ¥Œ£¯ÆÑ";
	if(pol.find_first_of(c)!=string::npos)return true;
	return false;
}
int ChangeName::AddChar(char c)
{
	if(!wid)return -1;
	if(!is_text(c))
	{
		if(c!=0x08	&&	c!=0x0d)
			::MessageBeep(MB_ICONINFORMATION);
		return -2;
	}
	if(wid==firstname)
	{
		imie_text.insert(imie_text.end()-1,c);
		imie.SetText(imie_text);
		MakeSmaller(imie,FIRST_NAME_WEIGHT,FIRST_NAME_RIGHT_POINT);
	}
	else
	{
		nazwisko_text.insert(nazwisko_text.end()-1,c);
		nazwisko.SetText(nazwisko_text);
		MakeSmaller(nazwisko,SURNAME_WEIGHT,SURNAME_RIGHT_POINT);
	}
	return 0;
}

int ChangeName::DeleteChar()
{
	if(!wid)return -1;
	if(wid==firstname)
	{
		if(imie_text.size()<2)
		{
			::MessageBeep(MB_ICONINFORMATION);
			return -1;
		}
		imie_text.erase(imie_text.end()-2);
		imie.SetText(imie_text);
		if(imie_text.size()>1) MakeSmaller(imie,FIRST_NAME_WEIGHT,FIRST_NAME_RIGHT_POINT);
	}
	else
	{
		if(nazwisko_text.size()<2)
		{
			::MessageBeep(MB_ICONINFORMATION);
			return -1;
		}
		nazwisko_text.erase(nazwisko_text.end()-2);
		nazwisko.SetText(nazwisko_text);
		if(nazwisko_text.size()>1)MakeSmaller(nazwisko,SURNAME_WEIGHT,SURNAME_RIGHT_POINT);
	}
	return 0;
}

int ChangeName::Click(float x, float y)
{
	if(!wid)
	{
		x-=pkt.x;
		y-=pkt.y;
		if(x<0.f||y<0.f	||
		x>img_size.x||y>img_size.y	||
		podstawa_img.GetPixel(x,y).a==0)return 0;
		Initialize();
		return 1;
	}
	else
	{
		x-=pkt.x;
		y-=pkt.y;
		if(x<0.f||y<0.f	||
		x>img_size.x||y>img_size.y	||
		podstawa_img.GetPixel(x,y).a==0)
		{
			Deinitialize();
			return 1;
		}
		TabOrEnterPressed();
	}
	return 1;
}

int ChangeName::Draw(RenderWindow& wnd)
{
	ChangeCursor();
	if(dark.Draw(wnd)==-1)return 0;
	wnd.Draw(podstawa);
	wnd.Draw(imie);
	wnd.Draw(nazwisko);
	return 0;
}

int ChangeName::GetName(string& firstname,string& surname)
{
	firstname=imie_text;
	surname=nazwisko_text;
	return 0;
}

bool ChangeName::Initialize()
{
	dark.SetBlank(true);
	wid=firstname;

	if(imie_text=="Imiê")
	{
		imie_text.clear();
		imie.SetText(nazwisko_text);
	}

	timer.Reset();
	cursor=true;
	imie_text+='_';
	imie.SetText(imie_text);
	MakeSmaller(imie,FIRST_NAME_WEIGHT,FIRST_NAME_RIGHT_POINT);
	
	return false;
}

bool ChangeName::Deinitialize()
{
	DeleteCursor();
	dark.SetBlank(false);
	wid=idle;
	return false;
}

int ChangeName::TabOrEnterPressed()
{
	if(!wid)return 0;
	if(wid==firstname)
	{
		DeleteCursor();
		wid=surname;
		if(nazwisko_text=="Nazwisko")
		{
			nazwisko_text.clear();
			nazwisko.SetText(nazwisko_text);
		}
		nazwisko_text+='_';
		nazwisko.SetText(nazwisko_text);
		MakeSmaller(nazwisko,SURNAME_WEIGHT,SURNAME_RIGHT_POINT);
		cursor=true;
	}
	else
	{
		success_entered=true;
		Deinitialize();
	}
	return 0;
}

int ChangeName::ChangeCursor()
{
	if(!wid)return 0;
	if(timer.GetElapsedTime()<0.5f)return -1;
	timer.Reset();
	wchar_t c;
	if(cursor)c=L'_';
	else c=L' ';
	cursor=!cursor;
	if(wid==firstname)
	{
		imie_text.erase(imie_text.end()-1);
		imie_text+=c;
		imie.SetText(imie_text);
		//MakeSmaller(imie,FIRST_NAME_WEIGHT,FIRST_NAME_RIGHT_POINT);
	}
	else
	{
		nazwisko_text.erase(nazwisko_text.end()-1);
		nazwisko_text+=c;
		nazwisko.SetText(nazwisko_text);
		//MakeSmaller(nazwisko,SURNAME_WEIGHT,SURNAME_RIGHT_POINT);
	}
	return 0;
}

int ChangeName::DeleteCursor(void)
{
	if(wid==firstname)
	{
		imie_text.erase(imie_text.end()-1);
		imie.SetText(imie_text);
		MakeSmaller(imie,FIRST_NAME_WEIGHT,FIRST_NAME_RIGHT_POINT);
	}
	else
	{
		nazwisko_text.erase(nazwisko_text.end()-1);
		nazwisko.SetText(nazwisko_text);
		MakeSmaller(nazwisko,SURNAME_WEIGHT,SURNAME_RIGHT_POINT);
	}
	return 0;
}
