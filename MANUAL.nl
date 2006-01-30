xdx - tcp/ip DX-cluster client voor zendamateurs
================================================

Xdx is een DX-cluster programma die een lijst toont met DX berichten en 
een apart venster gebruikt voor WWV, WCY, "To ALL" en andere berichten.

Wat is een DX cluster?
======================
Een DX Cluster is een manier voor zendamateurs om elkaar op de hoogte te
houden van actieve DX stations.

Gebruikers die verbonden zijn met een DX-Cluster mogen DX berichten
verzenden, persoonlijke berichten verzenden, mail verzenden en
ontvangen, opgeslagen data doorzoeken en opvragen en informatie
databases gebruiken.

Voor een lijst van DX-Clusters zie:
http://www.cpcug.org/user/wfeidt/Misc/cluster.html.

Commando's
==========
Hieronder enkele commando's om mee te beginnen:

announce/full 'msg': Stuur een bericht aan alle verbonden stations.
bye: Verlaat het DX Cluster.
dx 'frequency' 'callsign' 'comment': Verstuur informatie over een DX station.
show/dx: Toon opgeslagen DX berichten.

Voorbeelden van commando's
==========================
1) dx 28002.2 xz7a worked with 80m dipole!!
2) sh/dx on hf/cw 50

De meeste clusters tonen hulp door een '?' of 'help commando' te
versturen.

Autologin
=========
Het eerste wat je kunt doen na het starten van xdx is 'Instellingen->
Voorkeuren' uit het menu selecteren en autologin aanzetten. Je moet
inloggen met je roepletters. Nadat je verbonden bent kun je een aantal
commando's naar het cluster sturen. Je kunt deze invoeren in het
"Commando's" veld, gescheiden door een komma, 'set/page 0,unset beep'
zet bijvoorbeeld het pagineren en piepen uit.

DX info opslaan
===============
In het voorkeuren venster kun je tevens het opslaan van berichten
aanzetten.

$HOME/.xdx/dxspots  DX spots zoals getoond in de bovenste lijst.
$HOME/.xdx/wwv      WCY/WWV berichten met propagatie info.
$HOME/.xdx/toall    Berichten zoals getoond in het text venster.
$HOME/.xdx/wx       Weer informatie.

Hamlib support
==============
Wanneer je dubbel klikt op een DX bericht zal de frequentie van je
ontvanger worden ingesteld. Hiervoor heb je het rigctl hamlib programma
nodig. Tevens dien je de aanroep van rigctl in het voorkeuren venster
te wijzigen. 'rigctl -m 210 set_freq %d' zal bijvoorbeeld ID 210 gebruiken
(Kenwood TS-870), zie 'rigctl --list' voor een lijst van modellen.

Web browser and mail programma
==============================
Een URL in het text venster zal blauw worden als je je muis er naartoe
beweegt. Als je vervolgens op de link klikt zal deze worden geopend in je
web browser of mail programma (zie het voorkeuren venster):

Starten van de gnome web-browser indien op een URL wordt geklikt:
'epiphany %s'.
Starten van mozilla-mail indien op een URL wordt geklikt:
'mozilla -compose "to=%s"'.

Oplichten
=========
Op bladzijde 3 van het voorkeuren venster kun je 8 verschillende woorden
invoeren om te laten oplichten in het onderste venster. Je moet ook het
oplichten aanzetten voor het betreffende woord in het menu (voorkeuren
-> oplichten).

Smileys
=======
Er is ondersteuning aanwezig voor een beperkt aantal smileys in het text
venster:
:)  :-)  :))  :-))  ;)  ;-)  :(  :-(  :((  :-((

Licentie and ondersteuning
==========================
Xdx is gratis and vrijgegeven onder de GNU GPL licentie. Het programma is
geschreven door Joop Stakenborg <pg4i@amsat.org>.

Stuur SVP een berichtje als je fouten vind in het programma of als je
verbeteringen wilt.
