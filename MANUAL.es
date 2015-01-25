xdx - Cliente TCP/IP del Cluster DX y cliente del chat ON4KST para radioaficionados
===================================================================================
 
Xdx es un cliente para Cluster DX que muestra una lista con los anuncios
DX por un lado, y en una ventana aparte los mensajes WWV, WCY, "To ALL" y otros mensajes
del servidor. Se puede utilizar también para acceder al chat de ON4KST.

Referencia de teclas rápidas
============================
Menú principal:
        Alt-P           Abre el menú Programa
            Ctl-L       Visualiza el registro de conexión
            Ctl-Q       Salir de Xdx con una ventana de cierre
        Alt-H           Abre el menú Servidor
            Ctl-O       Conexión al servidor cluster
            Ctl-C       Desconexión del servidor cluster
        Alt-S           Abre el menú de Configuración
            Ctl-K       Conmuta ventana de teclas de función
            Ctl-R       Conmuta auto reconectar
            Ctl-S       Conmuta ventana de resaltado
            Ctl-P       Abre la ventana de preferencias
        Alt-E           Abre el menú de ayuda
            Ctl-H       Ver este manual
            Ctl-A       Sobre Xdx

Ventana de teclas de función:
        F1 - F8         Comandos F1 a F8 guardados

Ventana de resaltado:
        Alt-1 - Alt-8   Cambia entre las entradas de resaltado
        Ctl-1 - Ctl-8   Activa resaltados (conmuta casillas)

General:
        Alt-0           Devuelve el foco a la ventana de entrada de comandos



¿Qué es un Cluster DX?
======================
Un Cluster DX es un medio de comunicación entre radioficionados que se intercambian
información, en tiempo real, de las estaciones DX (estaciones interesantes o raras
de radioaficionados de todo el mundo) que hay en las bandas.

Los usuarios que están conectados al Cluster DX son capaces de anunciar spots
DX, enviarse mensajes personales, enviar y recibir mensajes de correo,
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

El chat ON4KST utiliza un subconjunto de comandos del DX-cluster. Se recomienda
escribir '/help' en la ventana después de conectarse. Todos los comandos
comienzan por '/'.


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
Cuando haga doble clic sobre un spot DX, su equipo se ajustará a la
frecuencia del spot. Necesitará el binario rigctl del paquete hamlib.
Por favor, modifique el ID de su equipo en la línea de comando de rigctl
en la ventana de preferencias, p.e. 'rigctl -m 210 set_freq %d' usará ID
210 (Kenwood TS-870), ejecutar "rigctl ---list" para ver la lista
completa de modelos.

Navegadores web y lectores de correo
====================================
Una URL que aparezca en la ventana de chat se volverá azul y subrayada
cuando pase el ratón por encima. Haciendo clic sobre el enlace, se
abrirá su navegador o lector de correo preferido (ver ventana de
preferencias):

Ejecutar el navegador web de gnome cuando se hace clic sobre una
URL: 'epiphany %s'.
Ejecutar el mozilla-mail al hacer clic en una dirección de correo:
'mozilla -compose "to=%s"'.

El navegador web y las aplicaciones de correo no están establecidas por
defecto en la ventana de preferencias. Si deja estos campos vacios se ejecutarán
las aplicaciones que utilice en su escritorio que traten estas URIs. En algunos
casos podría ser necesario comprobar la configuración de escritorio de
sus aplicaciones favoritas. Si falla, podría ser necesario que tenga que editar
manualmente el fichero ~/.local/share/applications/mimeapps.list
para forzar sus preferencias. Esto es debido a los cambios en GTK+ 2.24
que convirtió en obsoleta la manera inicial de tratar estas URIs.

Resaltado
=========
La barra lateral de chat permite introducir 8 palabras distintas que se pueden
destacar cuando aparezcan en la ventana de chat. Cuando se selecciona la palabra
con la casilla de selección adyacente, xdx buscará la palabra en el texto que va saliendo
y la destacará con colores cuando la encuentre. Si no se selecciona ninguna palabra,
solamente buscará el texto que aparece después del prompt. Se puede elegir el color
para cada palabra en la pestaña 3 de la ventana de preferencias. Puede habilitar
o deshabilitar las palabras con Ctrl-1 a Ctrl-8 y puede cambiar entre las palabras
con Alt-1 a Alt-8. Alt-0 vuelve al anterior.

Teclas de función
=================
Tecleando 'Ctl-k' se cambia a la ventana de teclas de función. Se pueden asignar
comandos a las teclas F1 a F8 haciendo clic con el botón derecho del ratón
e introduciendo el comando en la ventana emergente. Los comandos se enviarán solo
cuando hay una conexión activa al cluster. Se pueden enviar los comandos incluso
cuando la ventana de teclas de función está oculta.

Soporte de sonido
=================
Se puede reproducir un sonido cuando esta activado el destacado de palabras en la
ventana de chat. Para que funcione el sonido debe utilizar un programa secundario
y configurarlo en la primera pestaña de la ventana de preferencias: 'play %s'
utilizará el comando play, que es parte del paquete sox; 'esdplay %s' utilizará
esdplay que es útil cuando se utiliza gnome y esound. 'aplay %s' usa aplay de las
utilidades ALSA.

Emoticones (smileys)
====================
Existe soporte para un número limitado de emoticones en la ventana de chat:
:)  :-)  :))  :-))  ;)  ;-)  :(  :-(  :((  :-((

Fichero de soporte de paises/entidades
======================================
Con la distribución de Xdx se incluye el fichero cty.dat que proporciona
información del pais/entidad origen del indicativo DX. Este fichero está
mantenido por Jim Reisert, AD1C y puede descargar la última versión en:

	http://www.country-files.com/cty/cty.dat

El fichero de especificación de paises/entidades es:

	http://www.country-files.com/cty/backup/format.htm

Se realizan actualizaciones a este fichero generalmente antes de los principales
concursos DX. Este fichero se actualiza mas a menudo que Xdx por lo que la versión
que incluye el programa quedará caducada en un periodo corto de tiempo. En lugar
de generar versiones nuevas de Xdx solo para actualizar el fichero cty.dat, se
dan a continuación los pasos para actualizarlo desde su directorio raiz. Se puede
hacer de dos formas:

1. Colocar el fichero cty.dat actualizado en el directorio de preferencias de Xdx que es
   $HOME/.xdx en plataformas POSIX.

2. Si mantiene el fichero cty.dat en otro directorio para su uso con programas de
   registro de contactos, establezca la variable de entorno XDX_CTY con la ruta del
   fichero cty.dat, p.e.
   $HOME/log/cty.dat o utilice las opciones '-c' or '--cty_dat' en linea de comandos
   del programa Xdx para establecer la ruta.

La opción de línea de comandos tiene preferencia sobre la variable de entorno XDX_CTY.

Cuando se utiliza la opción de linea de comandos, o la variable de entorno, el fichero
cty.dat que resida en el directorio de preferencias no se cargará a menos que la ruta
especificada no termine en "cty.dat" o que cty.dat no sea un fichero, sino un directorio.
En tal caso se comprobará el directorio de preferencias. Se usará la versión cty.dat
instalada por el programa en caso de que fallen las rutas especificadas por la variable
de entorno o en la linea de comandos, o que el directorio de preferencias no contenga
un fichero cty.dat. Actualmente no se comprueba si el fichero cty.dat especificado se
ajusta a las especificaciones.

Si por cualquier razón no se desea que el programa determine el pais/entidad del indicativo,
puede crear un fichero de 0 bytes con el nombre cty.dat y pasarlo como entrada al programa
Xdx utilizando los métodos descritos arriba. De esta forma Xdx no averiguará el pais/entidad.

Soporte y licencia
==================
Xdx es libre y publicado bajo la licencia GNU GPL. Su autor es
Joop Stakenborg <pg4i@amsat.org>.
Nate Bargmann <n0nb@n0nb.us> (mantenedor actual)

Por favor, envie un informe si encuentra fallos o si desea mejoras.
