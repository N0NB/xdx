xdx - Cliente tcp/ip de Clusters DX para radioaficionados
=========================================================
 
Xdx es un cliente para Clusters DX que muestra una lista con los anuncios
DX por un lado, y en una ventana aparte, los WWV, WCY, "To ALL" y otros mensajes
del servidor.

¿Qué es un Cluster DX?
======================
Un Cluster DX es un medio de comunicación entre radioficionados que se intercambian
información, en tiempo real, de las estaciones DX (estaciones interesantes o raras
de radioaficionados de todo el mundo) que hay en las bandas.

Los usuarios que están conectados al Cluster DX son capaces de anunciar spots
DX, enviarse mensajes personales hablados, enviar y recibir mensajes de correo,
buscar y recuperar datos archivados, y acceder a información de bases de datos.

Una lista de Cluster DX en http://ve9wh.weblink.nbtel.net/dxclusters.html.

Comandos
========
Estos son los comandos básicos para iniciarse:

announce/full 'msg': Envía una línea de texto a todas las estaciones conectadas.
bye: Sale del Cluster DX.
dx 'frequency' 'callsign' 'comment': Envia información de un DX.
show/dx: Ver los spots DX anteriores.

Ejemplos
========
1) dx 28002.2 xz7a trabajado con un dipolo 80m!!
2) sh/dx on hf/cw 50

La mayoría de los clusters proporcionan ayuda enviando un '?' o 'help <comando>'.

Autologin
=========
Lo primero que querrá hacer después de arrancar xdx es seleccionar
Configuración->Preferencias y habilitar el autologin. Debería identificarse
en el cluster con su indicativo. Después de estar conectado se pueden
enviar varios comandos al cluster. Podría introducirlos en el apartado
'Comandos', separados por comas, p.e. set/page 0,unset/beep' deshabilitará
la paginación y no se emitirán mas pitidos.

Guardando información DX
========================
Los mensajes individuales se pueden guardar en un fichero cuando se activa
la opción en la ventana de preferencias.

$HOME/.xdx/dxspots  Spots DX con el formato que se muestra en la ventana.
$HOME/.xdx/wwv      Anuncios WCY/WWV con información de propagación.
$HOME/.xdx/toall    Mensajes de chat.
$HOME/.xdx/wx       Información del tiempo.

Soporte Hamlib
==============
Cuando haga doble click sobre un spot DX, su equipo se ajustará a la
frecuencia del spot. Necesitará el binario rigctl del paquete hamlib.
Por favor, modifique el ID de su equipo en la línea de comando de rigctl
en la ventana de preferencias, p.e. 'rigctl -m 210 set_freq %d' usará ID
210 (Kenwood TS-870), ejecutar "rigctl ---list" para ver la lista
completa de modelos.

Navegadores web y lectores de correo
====================================
Una URL que aparezca en la ventana de chat se volverá azul y subrayada
cuando pase el ratón por encima. Haciendo click sobre el enlace, se
abrirá su navegador o lector de correo preferido (ver ventana de
preferencias):

Ejecutar el navegador web de gnome cuando se hace click sobre una
URL: 'epiphany %s'.
Ejecutar el mozilla-mail al hacer click en una dirección de correo:
'mozilla -compose "to=%s"'.

Emoticones (smileys)
====================
Existe soporte para un número limitado de emoticones en la ventana de chat:
:)  :-)  :))  :-))  ;)  ;-)  :(  :-(  :((  :-((

Soporte y licencia
==================
Xdx es libre y publicado bajo la licencia GNU GPL. Su autor es
Joop Stakenborg <pg4i@amsat.org> (antiguos indicativos: pa4tu y pa3aba).

Por favor, envie un informe si encontró fallos o si desea mejoras.
