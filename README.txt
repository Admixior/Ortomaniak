[Wymagania]
Konieczne do uruchomienia:
- system operacyjny Windows XP service pack 2 lub nowszy
- pakiet: Microsoft Visual Studio Redistributable Package 2010 x86 
  ( https://www.microsoft.com/en-us/download/details.aspx?id=5555 )
- Zainstalowane sterowniki do karty graficznej.


[Kompilacja]

Do budowania zalecane jest środowisko Microsoft Visual Studio 2010.

Zewnętrzne biblioteki:

1. Do Ortomaniaka została użyta biblioteka graficzna SFML http://www.sfml-dev.org/ (licencja zlib/png license, otwarta i  dormowa do jakiego kolwiek użytku), która jest oparta na OpenGL.

2. Do serwera został użyty silnik SFGUI (również na licencji zlib/png license) który jest oparty na wyżej wymienionej bibliotece SFML.

Wszystkie pozostałe biblioteki dostarczane są wraz z systemem windows, lub w pakiecie MSVS Redistributable Package 2010.



[Uruchamianie]
Ortomaniak uruchamiany jest z pliku ".\Ortomaniak\Ortomaniak.exe"

FAQ:
Problem: Program się nie uruchamia, wyświetla się komunikat o braku bibliteki: msvcr100.dll lub innej. 
Rozwiązanie: Należy zainstalować Microsoft Visual Studio Redistributable Package 2010 x86

Problem: Program się uruchamia, widać szczątkową grafikę (czarne tło).
Rozwiązanie: Należy zainstalować sterowniki do karty graficznej.


[Uruchamianie serwera]
1. Serwer uruchamiany jest z pliku ".\Serwer\OrtoSerwer.exe"
2. Przed uruchomieniem można konfigurować sposób przeprowadzania testu online za pomocą pliku "./Serwer/konfiguracja.txt" , lub skorzystać z domyślnych ustawień.

[FAQ admina]
3. Wszelka komunikacja (serwer <-> klient) jest zapisywana w sposób czytelny dla użytkownika w pliku "./Serwer/logs.txt"
4. Zapisana lista użytkowników oraz wyników jest dostępna w pliku "./Serwer/wyniki.txt". Dane zapisane w  formacie TSV w celu ułatwienia przenoszenia do innych programów (excel, word).
5. Aby przeprowadzać testy przez internet należy skonfigurować przekierowanie portów na routerze.

