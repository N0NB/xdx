xdx - Cliente tcp/ip de Clusters DX para radioaficionados
=========================================================
 
Xdx es un cliente para Clusters DX que muestra una lista con los anuncios
DX por un lado, y en una ventana aparte, los WWV, WCY, "To ALL" y otros mensajes
del servidor.

�Qu� es un Cluster DX?
======================
Un Cluster DX es un medio de comunicaci�n entre radioficionados que se intercambian
informaci�n, en tiempo real, de las estaciones DX (estaciones interesantes o raras
de radioaficionados de todo el mundo) que hay en las bandas.

Los usuarios que est�n conectados al Cluster DX son capaces de anunciar spots
DX, enviarse mensajes personales hablados, enviar y recibir mensajes de correo,
buscar y recuperar datos archivados, y acceder a informaci�n de bases de datos.

Una lista de Cluster DX en http://ve9wh.weblink.nbtel.net/dxclusters.html.

Comandos
========
Estos son los comandos b�sicos para iniciarse:

announce/full 'msg': Env�a una l�nea de texto a todas las estaciones conectadas.
bye: Sale del Cluster DX.
dx 'frequency' 'callsign' 'comment': Envia informaci�n de un DX.
show/dx: Ver los spots DX anteriores.

Ejemplos
========
1) dx 28002.2 xz7a trabajado con un dipolo 80m!!
2) sh/dx on hf/cw 50

La mayor�a de los clusters proporcionan ayuda enviando un '?' o 'help <comando>'.

Autologin
=========
Lo primero que querr� hacer despu�s de arrancar xdx es seleccionar
Configuraci�n->Preferencias y habilitar el autologin. Deber�a identificarse
en el cluster con su indicativo. Despu�s de estar conectado se pueden
enviar varios comandos al cluster. Podr�a introducirlos en el apartado
'Comandos', separados por comas, p.e. set/page 0,unset/beep' deshabilitar�
la paginaci�n y no se emitir�n mas pitidos.

Guardando informaci�n DX
========================
Los mensajes individuales se pueden guardar en un fichero cuando se activa
la opci�n en la ventana de preferencias.

$HOME/.xdx/dxspots  Spots DX con el formato que se muestra en la ventana.
$HOME/.xdx/wwv      Anuncios WCY/WWV con informaci�n de propagaci�n.
$HOME/.xdx/toall    Mensajes de chat.
$HOME/.xdx/wx       Informaci�n del tiempo.

Soporte Hamlib
==============
Cuando haga doble click sobre un spot DX, su equipo se ajustar� a la
frecuencia del spot. Necesitar� el binario rigctl del paquete hamlib.
Por favor, modifique el ID de su equipo en la l�nea de comando de rigctl
en la ventana de preferencias, p.e. 'rigctl -m 210 set_freq %d' usar� ID
210 (Kenwood TS-870), ejecutar "rigctl ---list" para ver la lista
completa de modelos.

Navegadores web y lectores de correo
====================================
Una URL que aparezca en la ventana de chat se volver� azul y subrayada
cuando pase el rat�n por encima. Haciendo click sobre el enlace, se
abrir� su navegador o lector de correo preferido (ver ventana de
preferencias):

Ejecutar el navegador web de gnome cuando se hace click sobre una
URL: 'epiphany %s'.
Ejecutar el mozilla-mail al hacer click en una direcci�n de correo:
'mozilla -compose "to=%s"'.

Emoticones (smileys)
====================
Existe soporte para un n�mero limitado de emoticones en la ventana de chat:
:)  :-)  :))  :-))  ;)  ;-)  :(  :-(  :((  :-((

Soporte y licencia
==================
Xdx es libre y publicado bajo la licencia GNU GPL. Su autor es
Joop Stakenborg <pg4i@amsat.org> (antiguos indicativos: pa4tu y pa3aba).

Por favor, envie un informe si encontr� fallos o si desea mejoras.
