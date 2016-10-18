@echo Podaj adres ip z panelu serwera: 
@set /p ip=
@echo Podaj nazwe tego komputera (bez spacji): 
@set /p nazwa=
@Ortomaniak.exe 6000 %ip% %nazwa%
