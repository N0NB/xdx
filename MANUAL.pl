xdx - klient DX-cluster poprzez tcp/ip dla radioamatorów
========================================================

Xdx jest klientem DX-cluster, który wy¶wietla listê z og³oszeniami DX oraz
pokazuje w oddzielnym polu wiadomo¶ci WWV, WCY, "To all" i inne przesy³ane
z serwera.
 
Co to jest DX Cluster?
======================
DX Cluster stanowi pomoc dla radioamatorów i s³u¿y do informowania wszystkich 
o s³yszanych w danej chwili stacjach DX (interesuj±cych b±d¼ niespotykanych
stacjach krótkofalarskich na ca³ym ¶wiecie)

U¿ytkownicy pod³±czeni do DX Clustera maj± mo¿liwo¶æ publikowania og³oszeñ
o spotach DX i odpowiadania na og³oszenia, wys³ania prywatnych komunikatów
do innych u¿ytkowników, wysy³ania i odbierania poczty, wyszukiwania i odbierania
danych archiwalnych i dostêpu do informacji zgromadzonych bazach danych.

Lista DX Clusterów dostêpna jest pod adresem:
http://ve9wh.weblink.nbtel.net/dxclusters.html.

Polecenia
=========
Poni¿ej znajduje siê kilka podstawowych poleceñ, pozwalaj±cych na rozpoczêcie
korzystania z DX Clustera:

announce/full 'wiadomo¶æ' : Wysy³a linijkê tekstu do wszystkich po³±czonych stacji.
bye: Wyj¶cie z DX Cluster.
dx 'czêstotliwo¶æ' 'znak' 'komentarz': Wys³anie informacji o DX.
show/dx: Pokazuje poprzednio zalogowane spoty DX.


Przyk³ady poleceñ
=================
1) dx 28002.2 xz7a worked with 80m dipole!!
2) sh/dx on hf/cw 50

Wiêkszo¶æ clusterów oferuje pomoc po wpisaniu '?' lub 'help <polecenie>'.

Automatyczne logowanie
======================
Jedn± z pierwszych czynno¶ci jak± powiniene¶ wykonaæ po uruchomieniu xdx jest
w³±czenie funkcji automatycznego logowania w oknie "Ustawienia->Preferencje".
Logowanie powinno zawsze nastêpowaæ przy u¿yciu w³asnego znaku wywo³awczego.
Ka¿dorazowo po zalogowaniu mo¿liwe jest wysy³anie do clustera kilku poleceñ.
Mo¿esz je okre¶liæ w polu tekstowym "Polecenia" oddzielaj±c ka¿de za pomoc±
przecinka, np. set/page 0,unset beep' spowoduje wy³±czenie przewijania listy
i brzêczyka.

Zapamiêtywanie informacji DX
============================
W przypadku, gdy zaznaczona zostanie odpowiednia opcja w oknie preferencji,
poszczególne wiadomo¶ci mog± byæ zapamiêtywane do plików.

$HOME/.xdx/dxspots  Spoty DX wy¶wietlane na li¶cie w górnej czê¶ci programu
$HOME/.xdx/wwv      Og³oszenia WCY/WWV z informacj± o propagacji.
$HOME/.xdx/toall    Wiadomo¶ci chat wy¶wietlane w dolnej czê¶ci programu.
$HOME/.xdx/wx       Informacje pogodowe.

Wsparcie dla Hamlib
===================
Kiedy podwójnie klikniesz na spocie DX, spowoduje to ustawienie odpowiedniej
czêstotliwo¶ci w twoim radiu. Aby by³o to mo¿liwe musisz mieæ zainstalowany
program rigctl dostêpny z bibliotek± hamlib. Zmodyfikuj ID w wierszu poleceñ
rigctl znajduj±cym siê oknie preferencji, zgodnie z posiadanym przez siebie
urz±dzeniem, np. 'rigctl -m 210 set_freq %d' u¿ywa ID 210 (Kenwood TS-870),
zobacz "rigctl ---list" aby zobaczyæ listê wszystkich modeli.

Przegl±darki internetowe i programy pocztowe
============================================
Adresy internetowe w oknie chat wyró¿niane s± kolorem niebieskim i podkre¶lane,
gdy tylko wskazuje na nie kursor myszy. Klikniêcie na nim powoduje otwarcie
³±cza w zdefiniowanej przez ciebie przegl±darce internetowej b±d¼ programie
pocztowym (zobacz okno preferencji):

Uruchom przegl±darkê internetow± Gnome gdy klikniêty zosta³ URL: 'epiphany %s'.
Uruchom mozilla-mail gdy klikniêty zosta³ adres poczty elektronicznej:
'mozilla -compose "to=%s"'.

Emotikony
=========
Program posiada wsparcie dla ograniczonej liczby emotikon w oknie chata:
:)  :-)  :))  :-))  ;)  ;-)  :(  :-(  :((  :-((

Licencja i wsparcie
===================
Xdx jest bezp³atny i zosta³ opublikowany na podstawie licencji GNU GPL.
Program ten zosta³ napisany przez Joop'a Stakenborg <pg4i@amsat.org>
(poprzednie znaki: PA4TU i PA3ABA).

Proszê przy¶lij informacjê je¿eli znajdziesz b³±d lub chcesz rozbudowy programu.
