xdx - klient DX-cluster poprzez tcp/ip oraz czatu  ON4KST dla radioamatorów
===========================================================================

Xdx jest klientem DX-cluster, który wyświetla listę z komunikatami DX oraz
pokazuje w oddzielnym polu wiadomości WWV, WCY, "To all" i inne przesyłane
z serwera.
Program jest także przygotowany to łączenia z czatem ON4KST

Krótki wykaz skrótów klawiaturowych
===================================
Menu główne:
        Alt-P           Otwórz menu Program
            Ctl-L       Pokaż historię połączeń
            Ctl-Q       Wyjście z XDX i okno dialogowe zamknięcia
        Alt-H           Otwórz menu Host
            Ctl-O       Połącz z serwerem clustera
            Ctl-C       Rozłącz z serwerem clustera
        Alt-U           Otwórz menu Ustawienia
            Ctl-K       Przełącz okno z klawiszami funkcyjnymi
            Ctl-R       Przełącz wznawianie połączenia
            Ctl-S       Przełącz wyróżnianie
            Ctl-P       Otwórz okno preferencji
        Alt-O           Otwórz menu Pomoc
            Ctl-H       Pokaż ten podręcznik
            Ctl-A       O programie Xdx

Okno klawiszy funkcyjnych:
        F1 - F8         Zapisane polecenia F1 do F8

Okno wyróżnień:
        Alt-1 - Alt-8   Przełącza pomiędzy polami wyróżnień
        Ctl-1 - Ctl-8   Aktywuje wyróżnienia (przełącza przyciski wyboru)

Ogólne:
        Alt-0           Przywraca fokus do okna wiersza poleceń

Co to jest DX Cluster?
======================
DX Cluster stanowi pomoc dla radioamatorów i służy do informowania wszystkich, 
na bieżąco, o słyszanych w danej chwili stacjach DX (interesujących bądź
niespotykanych stacjach krótkofalarskich na całym świecie)

Użytkownicy podłączeni do DX Clustera mają możliwość publikowania ogłoszeń
o spotach DX i odpowiadania na ogłoszenia, wysłania prywatnych komunikatów
do innych użytkowników, wysyłania i odbierania poczty, wyszukiwania
i odbierania danych archiwalnych i dostępu do informacji zgromadzonych
bazach danych.

Lista DX Clusterów dostępna jest pod adresem:
http://www.ng3k.com/Misc/cluster.html

Czat ON4KST jest bardziej odpowiedni dla operatorów VHF i UHF. Jest miejscem,
gdzie krótkofalowcy spotykają się aby planować dalekie łączności radiowe, 
łączności wykorzystujące odbicie od Księżyca lub meteorów. Aby uzyskać
więcej informacji na temat czatu ON4KST odwiedź stronę http://www.on4kst.com.

Polecenia
=========
Poniżej znajduje się kilka podstawowych poleceń, pozwalających na rozpoczęcie
pracy z DX Clustrem:

announce/full 'wiadomość' : Wysyła linijkę tekstu do wszystkich połączonych
                            stacji.
bye: Wyjście z DX Cluster.
dx 'częstotliwość' 'znak' 'komentarz' : Wysłanie informacji o DX.
show/dx : Pokazuje poprzednio zalogowane spoty DX.

ON4KST używa zestawu poleceń DX-clustera. Najlepiej jeżeli po nawiązaniu
połączenia wpiszesz '/help'. Wszystkie polecenia muszą zaczynać się od '/'.

Przykłady poleceń DX-Clustera
=============================
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

gdzie: SFI oznacza solar flux index, indeksy A i K wskazują aktywność
geomagnetyczną, a R odnosi się do ilości plam widocznych na powierzchni Słońca.
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
zobacz "rigctl --list" aby zobaczyć listę wszystkich modeli.

Przeglądarki internetowe i programy pocztowe
============================================
Adresy internetowe w oknie czat wyróżniane są kolorem niebieskim i podkreślane,
gdy tylko wskazuje na nie kursor myszy. Kliknięcie na adresie powoduje
otwarcie łącza w zdefiniowanej przez użytkownika przeglądarce internetowej
bądź programie pocztowym (zobacz okno preferencji):

Uruchom przeglądarkę internetową Gnome gdy kliknięty został URL:
   'epiphany %s'.
Uruchom mozilla-mail gdy kliknięty został adres poczty elektronicznej:
   'mozilla -compose "to=%s"'.

Domyślnie przeglądarka i program pocztowy nie są ustawione w oknie preferencji. 
Pozostawienie tych ustawień pustych spowoduje użycie do obsługi adresów
domyślnej aplikacji pulpitu. W niektórych przypadkach może być konieczne
sprawdzenie konfiguracji domyślnych aplikacji dla pulpitu. Jeżeli to nie
pomoże, aby wymusić honorowanie twoich ustawień, konieczna może być ręczna
edycja pliku ~/.local/share/applications/mimeapps.list. Ta zmiana spowodowana
jest potraktowaniem w wersji GTK+ 2.24 poprzedniego sposobu obsługi adresów
URI jako przestarzałego. 

Wyróżnianie tekstu
==================
'Panel czat' pozwala na wprowadzenie do 8 różnych wyrazów, które będą
kolorowane w oknie rozmów. Gdy zaznaczone jest pole wyboru Xdx przeszukuje
wszystkie otrzymywane teksty celem wyróżnienia. W przypadku, gdy pole nie
zostało zaznaczone, przeszukiwany jest jedynie tekst za wierszem poleceń.
Wyboru kolorów, jakimi wyróżniane będą poszczególne słowa, można dokonać w 3
zakładce okna preferencji. Możesz szybko przełączyć stan poszczególnych pól
wyboru za pomocą skrótów Ctrl-1 do Ctrl-8, możesz także swobodnie przełączać
się pomiędzy poszczególnymi polami tekstowymi poprzez kombinację klawiszy
Alt-1 do Alt-8. Naciśnięcie Alt-0 powoduje powrót do pola w którym podajemy
tekst do wysłania.

Klawisze funkcyjne
==================

Naciskając 'Ctl-k' przełączamy okno klawiszy funkcyjnych. Poprzez kliknięcie
prawym klawiszem myszy na klawiszach F1 do F8 możliwe jest przypisanie im
poleceń oraz ich edycja w ukazującym się oknie dialogowym. Polecenia są
wysyłane jedynie podczas aktywnego połączenia z clusterem. Polecenia mogą
być wysyłane nawet gdy okno klawiszy funkcyjnych jest ukryte.

Dźwięk
======
Kiedy w oknie rozmów aktywne jest wyróżnianie tekstu, istnieje możliwość
odgrywania dźwięku. Aby było to jednak możliwe, musisz posłużyć się
zewnętrznym programem. Ustawień dokonujemy w pierwszym panelu okna
preferencji: 'play %s' spowoduje użycie programu play stanowiącego część
pakietu sox, 'esdplay %s' - programu esdplay, który może się okazać przydatny
kiedy używamy Gnome i esound. 'aplay %s' użyje aplay pochodzącego z narzędzi
ALSA.

Emotikony
=========
Program posiada wsparcie dla ograniczonej liczby emotikon w oknie chatu:
:)  :-)  :))  :-))  ;)  ;-)  :(  :-(  :((  :-((

Wsparcie dla pliku z krajami
============================
Do programu Xdx dołączony został plik cty.dat zawierający informacje pozwalające
na określenie kraju na podstawie znaku wywoławczego. Plikiem tym zarządza Jim
Reisert AD1C a jego najnowszą wersję można pobrać ze strony: 

	http://www.country-files.com/cty/cty.dat

Specyfikacja pliku z krajami znajduje się pod adresem:

	http://www.country-files.com/cty/backup/format.htm

Aktualizacji tego pliku dokonuje się przed wieloma głównymi zawodami DX.
Plik ten jest aktualizowany znacznie częściej niż Xdx, dlatego też wersja
zamieszczona wraz z kodem źródłowym staje się szybko nieaktualna. Zamiast
dokonywać każdorazowej aktualizacji programu po pojawieniu się nowego pliku
cty.dat, dodano obsługę wczytywania go z katalogu domowego użytkownika. Można
tego dokonać na twa sposoby:

1. Umieszczając zaktualizowany plik cty.dat w twoim katalogu z preferencjami
   Xdx, którym na platformach POSIX jest $HOME/.xdx

2. Jeżeli plik cty.dat trzymany jest w innym katalogu - ustawiając zmienną
   systemową XDX_CTY na ścieżkę do cty.dat, np. $HOME/log/cty.dat lub przy
   poleceniu uruchomienia Xdx dodając opcję '-c' lub '--cty_dat' celem
   ustawienia ścieżki.

Opcja wiersza poleceń nadpisuje ustawienia zawarte w zmiennej środowiskowej.

Gdy ustawiona została zmienna środowiskowa lub opcja wiersza poleceń to nie
zostanie wykorzystany plik cty.dat z katalogu z preferencjami, chyba że
ścieżka  nie zawiera cty.dat lub cty.dat nie jest regularnym plikiem, np.
jest katalogiem. W tych przypadkach sprawdzany będzie także katalog
z preferencjami. 

Zainstalowana wraz xdx wersja pliku cty.dat jest wczytywana w przypadku gdy
zawiodą podstawowe próby i katalog z preferencjami nie zawiera cty.dat.
Na chwilę obecną plik cty.dat nie jest sprawdzany pod kątem zgodności
ze specyfikacją.

Jeżeli, z jakichkolwiek powodów, nie chcesz aby określano kraje, możesz
utworzyć pusty plik o nazwie cty.dat. Używając jednego z powyższych sposobów
do wczytania tego pliku podczas uruchamiania przez Xdx wyłączysz wyszukiwanie
krajów.

Licencja i wsparcie
===================
Xdx jest bezpłatny i został opublikowany na licencji GNU GPL.
Program ten został napisany przez następujące osoby:
Joop Stakenborg PG4I <pg4i@amsat.org>.
Nate Bargmann N0NB <n0nb@n0nb.us> (obecny opiekun)

Tłumaczenie niniejszego podręcznika na język polski wykonał
Bogusław Ciastek SQ5TB <sq5tb@tlen.pl>

Jeżeli znajdziesz błąd lub chcesz rozbudowy programu, proszę przyślij
o tym informację.
