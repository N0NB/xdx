xdx - klient DX-cluster poprzez tcp/ip dla radioamator�w
========================================================

Xdx jest klientem DX-cluster, kt�ry wy�wietla list� z og�oszeniami DX oraz
pokazuje w oddzielnym polu wiadomo�ci WWV, WCY, "To all" i inne przesy�ane
z serwera.
 
Co to jest DX Cluster?
======================
DX Cluster stanowi pomoc dla radioamator�w i s�u�y do informowania wszystkich 
o s�yszanych w danej chwili stacjach DX (interesuj�cych b�d� niespotykanych
stacjach kr�tkofalarskich na ca�ym �wiecie)

U�ytkownicy pod��czeni do DX Clustera maj� mo�liwo�� publikowania og�osze�
o spotach DX i odpowiadania na og�oszenia, wys�ania prywatnych komunikat�w
do innych u�ytkownik�w, wysy�ania i odbierania poczty, wyszukiwania i odbierania
danych archiwalnych i dost�pu do informacji zgromadzonych bazach danych.

Lista DX Cluster�w dost�pna jest pod adresem:
http://ve9wh.weblink.nbtel.net/dxclusters.html.

Polecenia
=========
Poni�ej znajduje si� kilka podstawowych polece�, pozwalaj�cych na rozpocz�cie
korzystania z DX Clustera:

announce/full 'wiadomo��' : Wysy�a linijk� tekstu do wszystkich po��czonych stacji.
bye: Wyj�cie z DX Cluster.
dx 'cz�stotliwo��' 'znak' 'komentarz': Wys�anie informacji o DX.
show/dx: Pokazuje poprzednio zalogowane spoty DX.


Przyk�ady polece�
=================
1) dx 28002.2 xz7a worked with 80m dipole!!
2) sh/dx on hf/cw 50

Wi�kszo�� cluster�w oferuje pomoc po wpisaniu '?' lub 'help <polecenie>'.

Automatyczne logowanie
======================
Jedn� z pierwszych czynno�ci jak� powiniene� wykona� po uruchomieniu xdx jest
w��czenie funkcji automatycznego logowania w oknie "Ustawienia->Preferencje".
Logowanie powinno zawsze nast�powa� przy u�yciu w�asnego znaku wywo�awczego.
Ka�dorazowo po zalogowaniu mo�liwe jest wysy�anie do clustera kilku polece�.
Mo�esz je okre�li� w polu tekstowym "Polecenia" oddzielaj�c ka�de za pomoc�
przecinka, np. set/page 0,unset beep' spowoduje wy��czenie przewijania listy
i brz�czyka.

Zapami�tywanie informacji DX
============================
W przypadku, gdy zaznaczona zostanie odpowiednia opcja w oknie preferencji,
poszczeg�lne wiadomo�ci mog� by� zapami�tywane do plik�w.

$HOME/.xdx/dxspots  Spoty DX wy�wietlane na li�cie w g�rnej cz�ci programu
$HOME/.xdx/wwv      Og�oszenia WCY/WWV z informacj� o propagacji.
$HOME/.xdx/toall    Wiadomo�ci chat wy�wietlane w dolnej cz�ci programu.
$HOME/.xdx/wx       Informacje pogodowe.

Wsparcie dla Hamlib
===================
Kiedy podw�jnie klikniesz na spocie DX, spowoduje to ustawienie odpowiedniej
cz�stotliwo�ci w twoim radiu. Aby by�o to mo�liwe musisz mie� zainstalowany
program rigctl dost�pny z bibliotek� hamlib. Zmodyfikuj ID w wierszu polece�
rigctl znajduj�cym si� oknie preferencji, zgodnie z posiadanym przez siebie
urz�dzeniem, np. 'rigctl -m 210 set_freq %d' u�ywa ID 210 (Kenwood TS-870),
zobacz "rigctl ---list" aby zobaczy� list� wszystkich modeli.

Przegl�darki internetowe i programy pocztowe
============================================
Adresy internetowe w oknie chat wyr�niane s� kolorem niebieskim i podkre�lane,
gdy tylko wskazuje na nie kursor myszy. Klikni�cie na nim powoduje otwarcie
��cza w zdefiniowanej przez ciebie przegl�darce internetowej b�d� programie
pocztowym (zobacz okno preferencji):

Uruchom przegl�dark� internetow� Gnome gdy klikni�ty zosta� URL: 'epiphany %s'.
Uruchom mozilla-mail gdy klikni�ty zosta� adres poczty elektronicznej:
'mozilla -compose "to=%s"'.

Emotikony
=========
Program posiada wsparcie dla ograniczonej liczby emotikon w oknie chata:
:)  :-)  :))  :-))  ;)  ;-)  :(  :-(  :((  :-((

Licencja i wsparcie
===================
Xdx jest bezp�atny i zosta� opublikowany na podstawie licencji GNU GPL.
Program ten zosta� napisany przez Joop'a Stakenborg <pg4i@amsat.org>
(poprzednie znaki: PA4TU i PA3ABA).

Prosz� przy�lij informacj� je�eli znajdziesz b��d lub chcesz rozbudowy programu.
