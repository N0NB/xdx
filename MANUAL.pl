xdx - klient DX-cluster poprzez tcp/ip dla radioamatorów
========================================================

Xdx jest klientem DX-cluster, który wyświetla listę z ogłoszeniami DX oraz
pokazuje w oddzielnym polu wiadomości WWV, WCY, "To all" i inne przesyłane
z serwera.
Program jest także przygotowany to łączenia z czatem ON4KST
 
Co to jest DX Cluster?
======================
DX Cluster stanowi pomoc dla radioamatorów i służy do informowania wszystkich 
o słyszanych w danej chwili stacjach DX (interesujących bądź niespotykanych
stacjach krótkofalarskich na całym świecie)

Użytkownicy podłączeni do DX Clustera mają możliwość publikowania ogłoszeń
o spotach DX i odpowiadania na ogłoszenia, wysłania prywatnych komunikatów
do innych użytkowników, wysyłania i odbierania poczty, wyszukiwania
i odbierania danych archiwalnych i dostępu do informacji zgromadzonych
bazach danych.

Lista DX Clusterów dostępna jest pod adresem:
http://www.ng3k.com/Misc/cluster.html

Czat ON4KST jest bardziej odpowiedni dla operatorów VHF i UHF. Jest miejscem,
gdzie krótkofalowcy spotykają się aby planować dalekie łączności radiowe, 
moonbounce i meteor scatter. Aby uzyskać więcej informacji na temat czatu
ON4KST odwiedź stronę http://www.on4kst.com.

Polecenia
=========
Poniżej znajduje się kilka podstawowych poleceń, pozwalających na rozpoczęcie
korzystania z DX Clustera:

announce/full 'wiadomość' : Wysyła linijkę tekstu do wszystkich połączonych
                            stacji.
bye: Wyjście z DX Cluster.
dx 'częstotliwość' 'znak' 'komentarz': Wysłanie informacji o DX.
show/dx: Pokazuje poprzednio zalogowane spoty DX.

ON4KST używa zestawu poleceń DX-clustera. Najlepiej jeżeli po nawiązaniu
połączenia wpiszesz '/help'. Wszystkie polecenia muszą się zaczynać od '/'.

Przykłady poleceń
=================
1) dx 28002.2 xz7a worked with 80m dipole!!
2) sh/dx on hf/cw 50

Większość clusterów oferuje pomoc po wpisaniu '?' lub 'help <polecenie>'.

Znak wywoławczy i automatyczne logowanie
========================================
Znak wywoławczy podany na pierwszej stronie okna preferencji jest używany
do rozpoznawania w wierszu poleceń (więc xdx potrafi go kolorować) oraz
przy automatycznym logowaniu. W przypadku, gdy wybrano automatyczne logowanie,
każdorazowo po zalogowaniu możliwe jest wysyłanie do clustera kilku poleceń.
Możesz je określić w polu tekstowym "Polecenia" oddzielając każde za pomocą
przecinka, np. set/page 0,unset beep' spowoduje wyłączenie przewijania listy
i brzęczyka. Możesz także posłużyć się poleceniami gdy wymagane jest hasło.
Kolejne polecenia oddzielane są 0.5 sekundową przerwą.

Pakiety podtrzymujące
=====================
Jeżeli posiadasz łącze słabej jakości bądź zdarzają Ci się przypadkowe
rozłączenia, spróbuj włączyć wysyłanie pakietów podtrzymujących w oknie
preferencji. Spowoduje to regularne wysyłanie co 5 minut znaku backspace.

Zapamiętywanie informacji DX
============================
W przypadku, gdy zaznaczona zostanie odpowiednia opcja w oknie preferencji,
poszczególne wiadomości mogą być zapamiętywane do plików.

$HOME/.xdx/dxspots  Spoty DX wyświetlane na liście w górnej części programu
$HOME/.xdx/wwv      Ogłoszenia WCY/WWV z informacją o propagacji.
$HOME/.xdx/toall    Wiadomości chat wyświetlane w dolnej części programu.
$HOME/.xdx/wx       Informacje pogodowe.

Kiedy zapisywane są dane wwv, dla każdego hosta tworzony jest także plik
z oddzielonymi za pomocą tabulatora wartościami. Jest to przydatne do
sporządzania wykresów. Format tego pliku wygląda następująco:

YYYMMDDHH	SFI	A	K	R

gdzie: SFI oznacza solar flux index na 10,7 cm, indeksy A i K wskazują
aktywność ziemskiego pola magnetycznego, a R odnosi się do numeru komunikatu.
Przykładowy skrypt umieszczony został w katalogu z danymi programu xdx.
Wykorzystywany jest przez gnuplot w celu prezentacji danych od DK0WCY. Plik
wywoływany jest poprzez 'gnuplot wwv.gnuplot'. Efektem działania jest wykres
zapisany do $HOME/.xdx/DK0WCY.png.

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
Adresy internetowe w oknie czat wyróżniane są kolorem niebieskim
i podkreślane, gdy tylko wskazuje na nie kursor myszy. Kliknięcie na nim
powoduje otwarcie łącza w zdefiniowanej przez ciebie przeglądarce internetowej
bądź programie pocztowym (zobacz okno preferencji):

Uruchom przeglądarkę internetową Gnome gdy kliknięty został URL:
'epiphany %s'.
Uruchom mozilla-mail gdy kliknięty został adres poczty elektronicznej:
'mozilla -compose "to=%s"'.

Wyróżnianie tekstu
==================
'Panel czat' pozwala na wprowadzenie do 8 różnych wyrazów, które będą
kolorowane w oknie rozmów. Gdy zaznaczone jest pole wyboru xdx przeszukuje
celem wyróżnienia wszystkie otrzymywane teksty. W przypadku, gdy pole nie
zostało zaznaczone, przeszukiwany jest jedynie tekst za wierszem poleceń.
Wyboru kolorów, jakimi wyróżniane będą poszczególne słowa, można dokonać w 3
zakładce okna preferencji. Możesz szybko przełączyć stan poszczególnych pól
wyboru za pomocą skrótów Ctrl-1 do Ctrl-8, możesz także swobodnie przełączać
sie pomiędzy poszczególnymi polami tekstowymi poprzez kombinację klawiszy
Alt-1 do Alt-8. Naciśnięcie Alt-0 powoduje powrót do pola w którym podajemy
tekst do wysłania.

Dźwięk
======
Kiedy w oknie rozmów aktywne jest wyróżnianie tekstu, istnieje możliwość
odgrywania dźwięku. Aby było to jednak możliwe, musisz posłużyć się
zewnętrznym programem. Ustawień dokonujemy w pierwszym panelu okna
preferencji: 'play %s' spowoduje użycie programu play stanowiącego element
pakietu sox, 'esdplay %s' - programu esdplay, który może się okazać przydatny
kiedy używamy Gnome i esound.

Emotikony
=========
Program posiada wsparcie dla ograniczonej liczby emotikon w oknie chata:
:)  :-)  :))  :-))  ;)  ;-)  :(  :-(  :((  :-((

Licencja i wsparcie
===================
Xdx jest bezpłatny i został opublikowany na podstawie licencji GNU GPL.
Program ten został napisany przez Joop'a Stakenborg <pg4i@amsat.org>.

Proszę przyślij informację jeżeli znajdziesz błąd lub chcesz rozbudowy
programu.

