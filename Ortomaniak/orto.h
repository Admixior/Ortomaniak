#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "resource.h"
#include "SerwerConnect.h"

//pomocnicze makro
#define BYTE unsigned char

// ³adowaine czcionki
bool LoadFont(short id_font, sf::Font &font, unsigned int size, sf::Unicode::Text user_txt="");

//zmniejszanie textu do mieszcz¹cego siê w polu (right point)
bool MakeSmaller(sf::String &txt,float size,float right_point);

//define do ³adowania z zasobów aplikacji
#define zaladuj(x,data_type) (const char*)(LockResource(LoadResource(0,FindResource(0,MAKEINTRESOURCE(x),data_type))))
#define rozmiar(x,data_type) SizeofResource(0,FindResource(0,MAKEINTRESOURCE(x),data_type))

//deklaracje obiektów
extern SerwerConnect komputer;		//g³ówny komputer
extern bool WindowIsOpen;			//czy g³ówne okno jest teraz otwarte (u¿ywane do wypisywania b³êdów)

//funkcja centruj¹ca tekst
inline void SetCenterText(sf::String& txt)
{
	txt.SetCenter(txt.GetRect().GetWidth()/2.f,txt.GetRect().GetHeight()/2.f);
}


//funkcja zmieniaj¹ca polskie znaki w wchar_t  na  char
char ANSItoASCI(wchar_t c);
//=============================  G  U  I  =================================\\
//****************** Wspó³rzêdne wszystkich obiektów **********************\\

///************  <I N T E R F A C E>  ******************
// * * * * * * EDITION * * * * * * *
#define FONT_EDITION IDR_RINGM
#define SPACE_TO_WRITE_VERSION 43.f,59.f
#define VERSION_TEXT_CENTER_POINT 300.f,310.f
#define VERSION_TEXT_ANGLE 7.30f
#define VERSION_TEXT_WEIGHT 220
#define VERSION_TEXT_RIGHT_POINT 460.f-305.f

//* * * * * * *  NAME * * * * * * * *
#define FONT_NAME IDR_MASZYNAAEG
#define ANGLE_NAME 22.66f

#define FIRST_NAME_CENTER_POINT 1680.f,520.f
#define FIRST_NAME_RIGHT_POINT 150.f
#define FIRST_NAME_WEIGHT 70

#define SURNAME_CENTER_POINT 1715.f,594.f
#define SURNAME_RIGHT_POINT 142.f
#define SURNAME_WEIGHT 50

//-- change name--
#define CHG_NAME_POSITION 1478.f,357.f

///******************  <F A C E>  **********************
#define START_FACE 1532.f,55.f
#define START_HEART_LEFT 1640.f,160.f
#define START_HEART_RIGHT 1754.f,155.f


///************  <E N T E R   N A M E>  ******************
#define ENTER_NAME_START_LEFT
#define ENTER_NAME_START_UP
#define ENTER_NAME_START_RIGHT
#define ENTER_NAME_START_DOWN

#define ENTER_NAME_IMAGE_POINT

#define ENTER_NAME_FONT
#define ENTER_NAME_FIRST_NAME_POINT
#define ENTER_NAME_SURNAME_POINT

///************  <S T A T Y S T Y K I>  ******************
#define FONT_STATS IDR_MASZYNAAEG
#define TEXT_STATS_KONKURS "Próba: I  II III"
#define TEXT_STATS_STD "Wyniki:"
#define TEXT_PROBA_CENTER_POINT 1710.f,852.f
#define STATS_ANGLE -22.66f
#define FONT_PROBA_TEXT_WEIGHT 26

//- -- - wyniki punktowe - -- -
#define FONT_STATS_WEIGHT 30
#define PROBA_COLOR_PLUS Color(0x00,0x62,0x00)
#define PROBA_COLOR_MINUS Color(0xcb,0x00,0x00)

//+++++++++ KONKURS ---------
#define PROBA_1_PLUS 1677.f,897.f
#define PROBA_2_PLUS 1736.f,922.f
#define PROBA_3_PLUS 1793.f,946.f

#define PROBA_1_MINUS 1649.f,952.f
#define PROBA_2_MINUS 1708.f,976.f
#define PROBA_3_MINUS 1766.f,1000.f

//++++++++ TRENING ----------

#define TRENING_PLUS 1687.f,900.f
#define TRENING_MINUS 1659.f,955.f


///************  <M E N U>  ******************

#define POS_MENU_X				625.f
#define POS_MENU_Y				(-196.f)
#define POS_MENU_SHOW			(-280.f) //nie ingeruje w rzeczywist¹ pozycjê menu(tylko w napisy)
#define MENU_FONT				IDR_RINGM
#define TIME_SHOW_MENU			2.f
#define TIME_HIDE_MENU			1.2f
#define MENU_COLOR_BTNACTIVE	sf::Color(0.f,0.f,0.f,255.f)
#define MENU_COLOR_BTNMOUSEOVER	sf::Color(255.f,255.f,255.f,255.f)
#define MENU_COLOR_BTNDEACTIVE	sf::Color(50.f,0.f,0.f,70.f)

#define MENU_1_TEXT "Konkurs"
#define MENU_2_TEXT "Trening"
#define MENU_3_TEXT "Autorzy"
#define MENU_4_TEXT "Wyjscie"

#define MENU_1_CEN_POINT 1032.f,228.f
#define MENU_2_CEN_POINT 1033.f,434.f
#define MENU_3_CEN_POINT 1024.f,641.f
#define	MENU_4_CEN_POINT 1039.f,827.f

#define MENU_1_SIZE 124
#define MENU_2_SIZE 0x7f
#define MENU_3_SIZE 131
#define MENU_4_SIZE 0200


///************* <G A M E> *****************
#define GAME_BELKA				554.f,0.f
#define GAME_PODSTAWA			554.f,0.f
#define GAME_SLOWKA_START		120
#define GAME_SLOWKA_END			955
#define GAME_SLOWKA_OS			1025
#define GAME_FONT				IDR_ELEMENTARZ
#define GAME_FONT_SIZE_WORD		65.f
#define GAME_FONT_SIZE_ODP		80.f
#define GAME_SPACE_WORD			GAME_FONT_SIZE_WORD+15
#define GAME_LEFT_POINT_ODP		672.f,935.f
#define GAME_RIGHT_POINT_ODP	1388.f,935.f
#define GAME_SLOWKO_SPEED		15.f		//in sec
#define GAME_RED_FIELD_SPEED	1.f			//in sec


///************* <S C H O O L   A C C E P T> *****************
#define SA_MAIN_IMG_POS					408.f,240.f
#define SA_AGR_IMG_POS					1410.f,642.f
#define SA_DEFAULT_FONT					IDR_MASZYNAAEG
#define SA_FIRSTNAME_POS				825.f,409.f
#define SA_SURNAME_POS					825.f,488.f
#define SA_SCHOOL_CENTER				820.f,583.f
#define SA_ACCEPTTEXT_POS				1469.f, 572.f
#define SA_DENYTEXT_POS					1427.f,664.f
#define SA_TEXT_ROTATE					-26.42f
#define SA_PREVSCHL_BTNPOS				969.f,694.f
#define SA_NEXTSCHL_BTNPOS				1075.f,694.f
#define SA_FONT_FIRSTANDSURNAME_SIZE	35.f
#define SA_FONT_SCHOOLSIZE				25.f
#define SA_FONTBTN_SIZE					40.f

///**************** <O R T O   C O N F>   *******************
#define CONF_SCROLL_RIGHT		1410.f
#define CONF_SCROLL_LEFT		1080.f

#define CONF_WORD_POS_Y			655.f
#define WORD_STEP				5
#define MIN_WORD_STEP			10
#define MAX_WORD_STEP			8
#define DEFAULT_WORD_VALUE		3

#define CONF_TIME_POS_Y			510.f
#define TIME_STEP				1
#define MAX_TIME_STEP			8
#define MIN_TIME_STEP			2
#define DEFAULT_TIME_VALUE		5

struct my_Vector2f
{
	float x,y;
	operator const sf::Vector2f()const{return sf::Vector2f(x,y);}
};
const my_Vector2f CONF_POS_ONOFF []=
{
	{733.f,298.f},
	{733.f,401.f},
	{733.f,503.f},
	{733.f,605.f},
	{733.f,707.f},
	{733.f,809.f},
	{1322.f,196.f},
	{1322.f,298.f},
	{1322.f,401.f}
};

#define CONF_SCROLL_FONT		IDR_MASZYNAAEG
#define CONF_SCROLL_TEXTPOS_1	1241.f,545.f
#define CONF_SCROLL_TEXTPOS_2	1241.f,695.f
#define CONF_SCROLL_TEXTSIZE	30

#define CONF_LAYOUT_POS			311.f,65.f
#define CONF_ACCEPT_POS			1235.f,805.f
#define CONF_EXIT_POS			1239.f,868.f


#define CONF_FONT_TYPE			IDR_MASZYNAAEG
#define CONF_ACCEPT_SIZE_SMALL	45
#define CONF_ACCEPT_SIZE_BIG	50
#define CONF_EXIT_SIZE_SMALL	45
#define CONF_EXIT_SIZE_BIG		50

#define CONF_MSG_SHOW_MENU		0x01

///**************** <L I S T   B A D   O D P>   *******************
#define LISTBADODP_KARTKA_POS	595.f,60.f
#define LISTBADODP_NACHYLENIE	-3.45f

//--
#define LISTBADODP_FONT			IDR_ELEMENTARZ
#define LISTBADODP_FONTSIZE		50.f

#define LISTBADODP_POSSTART_X	668.f
#define LISTBADODP_POSSTART_Y	182.f
#define LISTBADODP_POSMOVE_X	-2.f*2.f
#define LISTBADODP_POSMOVE_Y	27.8f*2.f

#define LISTBADODP_TIMENEXTCHAR	0.01f			//w sekundach

//--
#define LISTBADODP_CONTINUETEXT					"Naciœnij aby kontynuowaæ"
#define LISTBADODP_CONTINUETEXT_POS				640.f,665.f
#define LISTBADODP_CONTINUETEXT_SIZE			70.f
#define LISTBADODP_CONTINUETEXT_COLORMAX		160.f
#define LISTBADODP_CONTINUETEXT_COLORMIN		50.f
#define LISTBADODP_CONTINUETEXT_TIMENEXTCOLOR	0.01f //w sekundach
#define LISTBADODP_CONTINUETEXT_JMP_COLOR		(LISTBADODP_CONTINUETEXT_COLORMAX - LISTBADODP_CONTINUETEXT_COLORMIN) *  \
												LISTBADODP_CONTINUETEXT_TIMENEXTCOLOR / 1.f


///**************** <W a i t F o r S e r w e r>   *******************
#define WFS_TEXT_PROSZE_CZEKAC			"Proszê czekaæ"
#define WFS_TEXT_PROSZE_CZEKAC_SIZE		60.f
#define WFS_TEXT_MSG_SIZE				80.f
#define WFS_TEXT_FONT_TYPE				IDR_MASZYNAAEG
#define WFS_TEXT_PROSZE_CZEKAC_POS		960.f,687.f
#define WFS_TEXT_WIADOMOSC_POS			960.f,781.f
#define WFS_LOADING_IMG_POS				960.f,536.f
#define WFS_LOADING_IMG_ROTATE			-360.f/11.f
#define WFS_LOADING_IMG_RATE			1.f/11.f


///********************** <A U T O R Z Y>   *************************
#define AUTORZY_POS_IMG		319.f, 38.f