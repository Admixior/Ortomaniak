#include "WaitForSerwer.h"


WaitForSerwer::WaitForSerwer(void)
{
	LoadFont(WFS_TEXT_FONT_TYPE,font,WFS_TEXT_MSG_SIZE);

	// Ustawienie textu: "Proszê czekaæ"
	text_prosze_czekac.SetFont(font);
	text_prosze_czekac.SetText(WFS_TEXT_PROSZE_CZEKAC);
	text_prosze_czekac.SetPosition(WFS_TEXT_PROSZE_CZEKAC_POS);
	text_prosze_czekac.SetSize(WFS_TEXT_PROSZE_CZEKAC_SIZE);
	SetCenterText(text_prosze_czekac);
	
	// Ustawienie sta³ych w³aœciwoœæi wiadomoœci
	text_wiadomosc.SetFont(font);
	text_wiadomosc.SetPosition(WFS_TEXT_WIADOMOSC_POS);
	text_wiadomosc.SetSize(WFS_TEXT_MSG_SIZE);

	//ustawienie obrazka ³adowania
	loading_img.LoadFromMemory(zaladuj(IDB_LOADING,L"PNG"),rozmiar(IDB_LOADING,L"PNG"));
	loading.SetImage(loading_img);
	loading.SetPosition(WFS_LOADING_IMG_POS);
	loading.SetCenter(loading_img.GetWidth()/2,loading_img.GetHeight()/2+1);

	timer.Reset();
	show=false;
}


WaitForSerwer::~WaitForSerwer(void)
{
}

int WaitForSerwer::Draw(sf::RenderWindow& wnd)
{
	if(!show) return dark.Draw(wnd);
	else
	{
		dark.Draw(wnd);
		if(timer.GetElapsedTime()>WFS_LOADING_IMG_RATE)
		{
			loading.Rotate(WFS_LOADING_IMG_ROTATE);
			timer.Reset();
		}
		wnd.Draw(loading);
		wnd.Draw(text_prosze_czekac);
		wnd.Draw(text_wiadomosc);
	}
	return 0;
}

void WaitForSerwer::SetText(std::string txt)
{
	text_wiadomosc.SetText(txt);
	SetCenterText(text_wiadomosc);
}