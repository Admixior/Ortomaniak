#include "orto.h"
#include <iostream>
#include <my\errnet.h>


//g³ówny komputer - definicja
SerwerConnect komputer;

extern bool WindowIsOpen;
//bezpieczeñstwo
__int64 LastSerwerTime;

bool CheckSerwerAuth(NetPacket* packet,__int64 hash)
{
	if(packet->id!=NET_MSG_COMPUTER)throw NET_ERROR_BAD_ID;
	if(packet->time<LastSerwerTime)throw NET_ERROR_BAD_KEY;
	if(hash!=GenerateKey(packet->key,packet->time))throw NET_ERROR_BAD_KEY;
	return true;
}

//które okno aktualnie otwarte (konsola/SFML - Window)
bool WindowIsOpen;

//³adowanie czcionki do pamiêci - definicja
bool LoadFont(short id_font, sf::Font &font,unsigned int size, sf::Unicode::Text user_txt)
{
	if(!user_txt.operator std::wstring().size())
		user_txt=L"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM "
			L"`-=[]\\;',./~!@#$%^&*()_+{}|:\"<>?"
			L"1234567890"
			L"êó¹œ³¿ŸæñÊÓ¥Œ£¯ÆÑ";
	if(!font.LoadFromMemory(zaladuj(id_font,L"FNT"),rozmiar(id_font,L"FNT"),size,user_txt))
	{
		font=font.GetDefaultFont();
		MessageBox(0,L"Czcionka nie mo¿e zostaæ za³adowana",L"ERROR",MB_SYSTEMMODAL);
		return false;
	}
	return true;
}

//zmniejszanie textu do mieszcz¹cego siê
bool MakeSmaller(sf::String &txt,float size,float right_point)
{
	do
	{
		txt.SetSize(size);
	}while(txt.GetCharacterPos(-1).x/2.f>right_point&&--size);
	
	txt.SetCenter(txt.GetRect().GetWidth()/2.f,txt.GetRect().GetHeight()/2.f);
	if(!size)MessageBox(0,L"Zbyt d³ugi text",L"Uwaga!",MB_SYSTEMMODAL);
	return size==0;
}


std::string& change_char_for_std(std::string& x)
{
	if(!x.size())return x;
	unsigned int size=x.size(), act=0;
	while(act<size)
	{
		if(x[act]=='Ê')x[act]='\xA8';
		else if(x[act]=='ê')x[act]='\xA9';
		else if(x[act]=='Ó')x[act]='\xE0';
		else if(x[act]=='ó')x[act]='\xA2';
		else if(x[act]=='¥')x[act]='\xA4';
		else if(x[act]=='¹')x[act]='\xA5';
		else if(x[act]=='Œ')x[act]='\x97';
		else if(x[act]=='œ')x[act]='\x98';
		else if(x[act]=='£')x[act]='\x9D';
		else if(x[act]=='³')x[act]='\x88';
		else if(x[act]=='¯')x[act]='\xBD';
		else if(x[act]=='¿')x[act]='\xBE';
		else if(x[act]=='')x[act]='\x8D';
		else if(x[act]=='Ÿ')x[act]='\xAB';
		else if(x[act]=='Æ')x[act]='\x8F';
		else if(x[act]=='æ')x[act]='\x86';
		else if(x[act]=='Ñ')x[act]='\xE3';
		else if(x[act]=='ñ')x[act]='\xE4';
		act++;
	}
	return x;
}

std::string change_char_for_std(char* x)
{
	std::string str=x;
	return change_char_for_std(str);
}


int ShowErr(int numErr,string who,int exnum,int linenum)
{
	stringstream ia;
	ia<<ErrText(numErr);
	if(exnum)
		ia<<":WSAError("<<exnum<<"): "<<WSAError(exnum);
	if(linenum)
		ia<<":At line:"<<linenum;
	//if(WindowIsOpen)
		//MessageBoxA(0,ia.str().c_str(),who.c_str(),MB_TOPMOST|MB_SETFOREGROUND);
	//else
	{
		string msg=ia.str();
		cout<<"\n\nERROR: "<<who<<endl<<change_char_for_std(msg);
	}
	return 0;
}

char ANSItoASCI(wchar_t c)
{
	/*wstring txtansi(L"ÊêÓó¥¹Œœ£³¯¿ŸÆæÑñ");
	string txtascii("ÊêÓó¥¹Œœ£³¯¿ŸÆæÑñ");
	int x=txtansi.find_first_of(c);
	if(x!=std::string::npos)return txtascii[x];
	return c;
*/
	switch(c)
	{
	case L'Ê':return 'Ê';
	case L'ê':return 'ê';
	case L'Ó':return 'Ó';
	case L'ó':return 'ó';
	case L'¥':return '¥';
	case L'¹':return '¹';
	case L'Œ':return 'Œ';
	case L'œ':return 'œ';
	case L'£':return '£';
	case L'³':return '³';
	case L'¯':return '¯';
	case L'¿':return '¿';
	case L'':return '';
	case L'Ÿ':return 'Ÿ';
	case L'Æ':return 'Æ';
	case L'æ':return 'æ';
	case L'Ñ':return 'Ñ';
	case L'ñ':return 'ñ';
	default:return c;
	}
}