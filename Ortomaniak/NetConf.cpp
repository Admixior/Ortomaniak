#include <WinSock2.h>
#include "NetConf.h"
//#include "SerwerConnect.h"

//pomocnicze makro
#define BYTE unsigned char

//funkcja kt�ra si� przyda
std::string& change_char_for_std(std::string& x);


const char* ErrText(BYTE NumErr)
{
	static const char* NzwErr[]=
	{
		//--b��dy spowodowane dzia�alno�ci� administratora/programu---///
		"Brak b��du"
		,"Komputer nie zalogowany"
		,"uzytkownik nie zalogowany"
		,"Konieczna jest tre�� wiadomo�ci"
		,"Tre�� wiadomo�ci nie by�a potrzebna"
		,"Niepoprawny nadawca/ID"
		,"Niepoprawny klucz bezpiecze�stwa"
		,"Niepoprawna nazwa komputer (1-30 znak�w)"
		,"Komputer jest ju� zalogowy"
		,"Komputer zalogowa� ju� jednego u�ytkownika, nie mo�na wi�cej"
		,"B��dny odbiorca pakietu"
		,"Funkcja otrzyma�a typ wiadomo�ci kt�ry nie obs�uguje"
		,"Paczka kt�ra zosta�a otrzymana jest za ma�a do okre�lonej czynno�ci"
		,"Paczka kt�ra zosta�a otrzymana zawiera dodatkowe niepotrzebne bajty"
		,"Jest b��d kt�rego nie powinno by�!"
		,"Nie ustawiono adresu ��czenia"
		,"Nie mo�na po��czy�si� z serwerem, z�y adres lub odrzucenie przez zapor�"
		,"Nie mo�na zmieni� adresu��czenia gdzy� komputer jest po��czony"

		//--b��dy spowodowane dzia�alno�ci� u�ytkownika programu
		,"U�ytkownik ju� zalogowany"
		,"Brak osoby o takich danych w bazie"


		//--b��dy spowodowane przez biblioteki/komputer/brak zasob�w/itp...--//
		,"Nie mo�na utworzy� w�tku"
		,"Nie mo�na utworzy� SOCKETA"
		,"Nie mo�na za�adowa� biblioteki"
		,"Funckja Select zawiod�a"
		,"Funkcja select zwr�ci�a nieprzwidzian� warto��"
		,"Funkcja Recv(...); lub Send(...); zawiod�a"
		,"Uruchom program ponownie"
		,"Nie mo�na za�adowa� listy s��wek: data\\slowka.txt [nie istnieje lub nie ma praw dost�pu]"
	};
	if(sizeof(NzwErr)<=NumErr) return "Takiego b��du nie ma";
	return NzwErr[NumErr];
}

//numer kt�ry b�dzie wysy�any od razu po po��czeniu si� aby sprawdzi� czy to napewno prawid�owy program si� konsultuje
const __int32 VerificationNumber = 0x4D7272B5; //sta�a losowa liczba :)

//-- -- Funkcja odpowiadaj�ca za wysy�anie okre�lonej ilo�ci bajt�w
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
// odbieranie okre�lonej ilo�ci bajt�w
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

