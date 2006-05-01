xdx - Cliente TCP/IP del Cluster DX y cliente del chat ON4KST para radioaficionados
===================================================================================
 
Xdx es un cliente para Cluster DX que muestra una lista con los anuncios
DX por un lado, y en una ventana aparte los WWV, WCY, "To ALL" y otros mensajes
del servidor.

¿Qué es un Cluster DX?
======================
Un Cluster DX es un medio de comunicación entre radioficionados que se intercambian
información, en tiempo real, de las estaciones DX (estaciones interesantes o raras
de radioaficionados de todo el mundo) que hay en las bandas.

Los usuarios que están conectados al Cluster DX son capaces de anunciar spots
DX, enviarse mensajes personales , enviar y recibir mensajes de correo,
buscar y recuperar datos archivados, y acceder a información de bases de datos.

Una lista de Cluster DX en:
http://www.ng3k.com/Misc/cluster.html

El chat de ON4KST es mas apropiado para operadores de VHF y UHF. Es el lugar
de encuentro de radioaficionados para planear contactos de radio de larga distancia,
rebote lunar y reflexión meteórica. Para mas información del chat de ON4KST
visitar la web http://www.on4kst.com.


Comandos
========
Estos son los comandos básicos para iniciarse:

announce/full 'msg': Envía la línea de texto 'msg' a todas las estaciones conectadas.
bye: Sale del Cluster DX.
dx 'frecuencia' 'indicativo' 'comentario': Envia información de un DX.
show/dx: Ver los spots DX anteriores.

Ejemplos
========
1) dx 28002.2 xz7a trabajado con un dipolo 80m!!
2) sh/dx on hf/cw 50

La mayoría de los clusters proporcionan ayuda enviando un '?' o 'help <comando>'.

Indicativo y Autologin
======================
El indicativo que aparece en la primera página de la ventana de preferencias
se utiliza para entrar al cluster DX (para que xdx pueda colorearlo) y para
autologin. Cuando se habilita el autologin se pueden enviar varios comandos
al cluster de forma automática. Podría introducirlos en el apartado
'Comandos' separados por comas, p.e. 'set/page 0,unset/beep' deshabilitará
la paginación y no se emitirán mas pitidos. Puede utilizar también esta opción
para enviar contraseñas cuando se requiera. Existe un retardo de 0.5 segundos
entre comandos.

Paquetes "Keepalive"
====================
Si tiene una mala conexión a red y experimenta desconexiones aleatorias, puede
probar a habilitar 'Enviar paquetes "keepalive"' en la ventana de preferencias. Con
esta opción activada se envia un caracter de retroceso (backspace)
al servidor cada 5 minutos.

Guardando información DX
========================
Los mensajes individuales se pueden guardar en un fichero cuando se activa
la opción en la ventana de preferencias.

$HOME/.xdx/dxspots  Spots DX con el formato que se muestra en la ventana.
$HOME/.xdx/wwv      Anuncios WCY/WWV con información de propagación.
$HOME/.xdx/toall    Mensajes de chat que se muestran en la ventana inferior.
$HOME/.xdx/wx       Información del tiempo.

Cuando se guardan datos WWV, se guardan para cada host WWV en ficheros independientes
con los valores separados por tabuladores. Esto es útil para crear gráficas. El formato
de este fichero es:

YYYMMDDHH	SFI	A	K	R

Donde SFI es el flujo solar a 10.7cm, A y K son los índices A y K que indican las condiciones
geomagnéticas y R es el número de manchas solares. Se incluye un script de ejemplo
en el directorio de datos de xdx que utiliza gnuplot para mostrar los datos de DK0WCY.
Se puede ejecutar con 'gnuplot wwv.gnuplot'. La gráfica se guarda en $HOME/.xdx/DK0WCY.png.

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

Para destacar
=============
La barra lateral de chat permite introducir 8 palabras distintas que se pueden
destacar cuando aparezcan en la ventana de chat. Cuando se selecciona la palabra
con la casilla de selección adyacente, xdx buscará la palabra en el texto que va saliendo
y la destacará con colores cuando la encuentre. Si no se selecciona ninguna palabra,
solamente buscará el texto que aparece después del prompt. Se puede elegir el color
para cada palabra en la pestaña 3 de la ventana de preferencias. Puede habilitar
o deshabilitar las palabras con Ctrl-1 a Ctrl-8 y puede cambiar entre las palabras
con Alt-1 a Alt-8. Alt-0 vuelve al anterior.

Soporte de sonido
=================
Se puede reproducir un sonido cuando esta activado el destacado de palabras en la
ventana de chat. Para que funcione el sonido debe utilizar un programa secundario
y configurarlo en la primera pestaña de la ventana de preferencias: 'play %s'
utilizará el comando play, que es parte del paquete sox; 'esdplay %s' utilizará
esdplay que es útil cuando se utiliza gnome y esound.

Emoticones (smileys)
====================
Existe soporte para un número limitado de emoticones en la ventana de chat:
:)  :-)  :))  :-))  ;)  ;-)  :(  :-(  :((  :-((

Soporte y licencia
==================
Xdx es libre y publicado bajo la licencia GNU GPL. Su autor es
Joop Stakenborg <pg4i@amsat.org>.

Por favor, envie un informe si encuentra fallos o si desea mejoras.
