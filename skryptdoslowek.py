#!/usr/local/bin/
# coding: utf-8

# drobny skrypt ddo sprawdzania poprawności słówek

import urllib2

def check(arg):
	page=urllib2.urlopen("http://sjp.pl/"+arg).read()
	if page.find("dopuszczalne w grach")==-1:
		return 1
	return 0
	


plik = open('slowka.txt')
try:
	tekst = plik.read()
finally:
	plik.close()

tekst=tekst.replace("\xea","ę");
tekst=tekst.replace("\xf3","%c3%b3");
tekst=tekst.replace("\xb9","ą");
tekst=tekst.replace("\x9c","ś");
tekst=tekst.replace("\xb3","ł");
tekst=tekst.replace("\xbf","ż");
tekst=tekst.replace("\x9f","ź");
tekst=tekst.replace("\xe6","ć");
tekst=tekst.replace("\xf1","ń");


tekst=tekst.replace("\xda","Ę");
tekst=tekst.replace("\xe3","%c3%93");
tekst=tekst.replace("\xa9","Ą");
tekst=tekst.replace("\x8c","Ś");
tekst=tekst.replace("\xa3","Ł");
tekst=tekst.replace("\xaf","Ż");
tekst=tekst.replace("\x8f","Ź");
tekst=tekst.replace("\xd6","Ć");
tekst=tekst.replace("\xe1","Ń");
linie=tekst.split("\x0d\x0a");

plik = open('result',"w")

for i in range(len(linie)):
	if linie[i][0]=="@" or linie[i][0]==";" :
		continue
	wyraz=linie[i].split("	");
	if len(wyraz)!=3:
		plik.write( "\nerr "+str(i) )
		continue
	if check(wyraz[0].replace("?",wyraz[1]))==1:
		plik.write(  "\nbad: "+wyraz[0].replace("?",wyraz[1]))
	if check(wyraz[0].replace("?",wyraz[2]))==0:
		plik.write(  "\ngod: "+wyraz[0].replace("?",wyraz[2]))
	plik.flush()
	print str(i)+"/"+str(len(linie))
	
print "end"





