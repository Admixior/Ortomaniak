#include <WinSock2.h>
#include "NetConf.h"
//#include "SerwerConnect.h"

//pomocnicze makro
#define BYTE unsigned char

//funkcja która siê przyda
std::string& change_char_for_std(std::string& x);


const char* ErrText(BYTE NumErr)
{
	static const char* NzwErr[]=
	{
		//--b³êdy spowodowane dzia³alnoœci¹ administratora/programu---///
		"Brak b³êdu"
		,"Komputer nie zalogowany"
		,"uzytkownik nie zalogowany"
		,"Konieczna jest treœæ wiadomoœci"
		,"Treœæ wiadomoœci nie by³a potrzebna"
		,"Niepoprawny nadawca/ID"
		,"Niepoprawny klucz bezpieczeñstwa"
		,"Niepoprawna nazwa komputer (1-30 znaków)"
		,"Komputer jest ju¿ zalogowy"
		,"Komputer zalogowa³ ju¿ jednego u¿ytkownika, nie mo¿na wiêcej"
		,"B³êdny odbiorca pakietu"
		,"Funkcja otrzyma³a typ wiadomoœci który nie obs³uguje"
		,"Paczka która zosta³a otrzymana jest za ma³a do okreœlonej czynnoœci"
		,"Paczka która zosta³a otrzymana zawiera dodatkowe niepotrzebne bajty"
		,"Jest b³¹d którego nie powinno byæ!"
		,"Nie ustawiono adresu ³¹czenia"
		,"Nie mo¿na po³¹czyæsiê z serwerem, z³y adres lub odrzucenie przez zaporê"
		,"Nie mo¿na zmieniæ adresu³¹czenia gdzy¿ komputer jest po³¹czony"

		//--b³êdy spowodowane dzia³alnoœci¹ u¿ytkownika programu
		,"U¿ytkownik ju¿ zalogowany"
		,"Brak osoby o takich danych w bazie"


		//--b³êdy spowodowane przez biblioteki/komputer/brak zasobów/itp...--//
		,"Nie mo¿na utworzyæ w¹tku"
		,"Nie mo¿na utworzyæ SOCKETA"
		,"Nie mo¿na za³adowaæ biblioteki"
		,"Funckja Select zawiod³a"
		,"Funkcja select zwróci³a nieprzwidzian¹ wartoœæ"
		,"Funkcja Recv(...); lub Send(...); zawiod³a"
		,"Uruchom program ponownie"
		,"Nie mo¿na za³adowaæ listy s³ówek: data\\slowka.txt [nie istnieje lub nie ma praw dostêpu]"
	};
	if(sizeof(NzwErr)<=NumErr) return "Takiego b³êdu nie ma";
	return NzwErr[NumErr];
}

//numer który bêdzie wysy³any od razu po po³¹czeniu siê aby sprawdziæ czy to napewno prawid³owy program siê konsultuje
const __int32 VerificationNumber = 0x4D7272B5; //sta³a losowa liczba :)

//-- -- Funkcja odpowiadaj¹ca za wysy³anie okreœlonej iloœci bajtów
int SendValue(SOCKET sd,char* value, unsigned __int32 size)
{
	if(!size)return 0;
	for(unsigned int sent=0,x;sent<size;sent+=x)
	{
		x=send(sd,value+sent,size-sent,0);
		if(x==SOCKET_ERROR)throw NET_ERROR_RECV_OR_SEND_FUNCTION_FAIL;
	}
	return size;
}
// odbieranie okreœlonej iloœci bajtów
int ReceiveValue(SOCKET sd,char*  value,unsigned __int32 size)
{
	if(!size)return 0;
	for(unsigned int success=0,x;success<size;success+=x)
	{
		x=recv(sd,value+success,size-success,0);
		if(x==SOCKET_ERROR || x==0)throw NET_ERROR_RECV_OR_SEND_FUNCTION_FAIL;
	}
	return size;
}

int SendPacket(SOCKET sd, NetPacket& to_send)
{
	SendValue(sd,reinterpret_cast<char*>(&to_send),(sizeof(to_send)-sizeof(to_send.msg)));
	SendValue(sd,reinterpret_cast<char*>(to_send.msg),to_send.size);
	return 0;
}

int RecvPacket(SOCKET sd, NetPacket& dest)
{
	ReceiveValue(sd,reinterpret_cast<char*>(&dest),(sizeof(dest)-sizeof(dest.msg)));
	if(!dest.size)return 0;
	dest.msg=new BYTE[dest.size];
	ReceiveValue(sd,reinterpret_cast<char*>(dest.msg),dest.size);
	return 0;
}

__int64 GenerateKey(__int64 KEY, __int64 TIME)
{
	return KEY^TIME;
}

NetPacket* CreatePacket(
	__int32 id,
	__int64 key,
	BYTE nadawca,
	BYTE typ,
	__int32 err,
	__int32 size,
	BYTE* msg
	)
{
	NetPacket* pack=new NetPacket;
	pack->id=id;
	pack->key=GenerateKey(key,pack->time=(((__int64)GetTickCount())|(  ((__int64)(~GetTickCount()))<< (sizeof(__int32)*8) )  ));
	pack->nadawca=nadawca;
	pack->typ=typ;
	pack->err=err;
	pack->size=size;
	pack->msg=msg;
	return pack;
}

