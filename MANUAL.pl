xdx - klient DX-cluster poprzez tcp/ip dla radioamatorów
========================================================

Xdx jest klientem DX-cluster, który wyświetla listę z ogłoszeniami DX oraz
pokazuje w oddzielnym polu wiadomości WWV, WCY, "To all" i inne przesyłane
z serwera.
 
Co to jest DX Cluster?
======================
DX Cluster stanowi pomoc dla radioamatorów i służy do informowania wszystkich 
o słyszanych w danej chwili stacjach DX (interesujących bądź niespotykanych
stacjach krótkofalarskich na całym świecie)

Użytkownicy podłączeni do DX Clustera mają możliwość publikowania ogłoszeń
o spotach DX i odpowiadania na ogłoszenia, wysłania prywatnych komunikatów
do innych użytkowników, wysyłania i odbierania poczty, wyszukiwania i odbierania
danych archiwalnych i dostępu do informacji zgromadzonych bazach danych.

Lista DX Clusterów dostępna jest pod adresem:
http://www.cpcug.org/user/wfeidt/Misc/cluster.html.

Polecenia
=========
Poniżej znajduje się kilka podstawowych poleceń, pozwalających na rozpoczęcie
korzystania z DX Clustera:

announce/full 'wiadomość' : Wysyła linijkę tekstu do wszystkich połączonych stacji.
bye: Wyjście z DX Cluster.
dx 'częstotliwość' 'znak' 'komentarz': Wysłanie informacji o DX.
show/dx: Pokazuje poprzednio zalogowane spoty DX.


Przykłady poleceń
=================
1) dx 28002.2 xz7a worked with 80m dipole!!
2) sh/dx on hf/cw 50

Większość clusterów oferuje pomoc po wpisaniu '?' lub 'help <polecenie>'.

Automatyczne logowanie
======================
Jedną z pierwszych czynności jaką powinieneś wykonać po uruchomieniu xdx jest
włączenie funkcji automatycznego logowania w oknie "Ustawienia->Preferencje".
Logowanie powinno zawsze następować przy użyciu własnego znaku wywoławczego.
Każdorazowo po zalogowaniu możliwe jest wysyłanie do clustera kilku poleceń.
Możesz je określić w polu tekstowym "Polecenia" oddzielając każde za pomocą
przecinka, np. set/page 0,unset beep' spowoduje wyłączenie przewijania listy
i brzęczyka.

Zapamiętywanie informacji DX
============================
W przypadku, gdy zaznaczona zostanie odpowiednia opcja w oknie preferencji,
poszczególne wiadomości mogą być zapamiętywane do plików.

$HOME/.xdx/dxspots  Spoty DX wyświetlane na liście w górnej części programu
$HOME/.xdx/wwv      Ogłoszenia WCY/WWV z informacją o propagacji.
$HOME/.xdx/toall    Wiadomości chat wyświetlane w dolnej części programu.
$HOME/.xdx/wx       Informacje pogodowe.

Wsparcie dla Hamlib
===================
Kiedy podwójnie klikniesz na spocie DX, spowoduje to ustawienie odpowiedniej
częstotliwości w twoim radiu. Aby było to możliwe musisz mieć zainstalowany
program rigctl dostępny z biblioteką hamlib. Zmodyfikuj ID w wierszu poleceń
rigctl znajdującym się oknie preferencji, zgodnie z posiadanym przez siebie
urządzeniem, np. 'rigctl -m 210 set_freq %d' używa ID 210 (Kenwood TS-870),
zobacz "rigctl ---list" aby zobaczyć listę wszystkich modeli.

Przeglądarki internetowe i programy pocztowe
============================================
Adresy internetowe w oknie chat wyróżniane są kolorem niebieskim i podkreślane,
gdy tylko wskazuje na nie kursor myszy. Kliknięcie na nim powoduje otwarcie
łącza w zdefiniowanej przez ciebie przeglądarce internetowej bądź programie
pocztowym (zobacz okno preferencji):

Uruchom przeglądarkę internetową Gnome gdy kliknięty został URL: 'epiphany %s'.
Uruchom mozilla-mail gdy kliknięty został adres poczty elektronicznej:
'mozilla -compose "to=%s"'.

Emotikony
=========
Program posiada wsparcie dla ograniczonej liczby emotikon w oknie chata:
:)  :-)  :))  :-))  ;)  ;-)  :(  :-(  :((  :-((

Licencja i wsparcie
===================
Xdx jest bezpłatny i został opublikowany na podstawie licencji GNU GPL.
Program ten został napisany przez Joop'a Stakenborg <pg4i@amsat.org>.

Proszę przyślij informację jeżeli znajdziesz błąd lub chcesz rozbudowy programu.
