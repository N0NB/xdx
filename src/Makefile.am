## Process this file with automake to produce Makefile.in

INCLUDES=-I/usr/include @PKG_INC@ \
	-DPACKAGE_DATA_DIR=\"$(datadir)/xdx\" \
	-DPACKAGE_LOCALE_DIR=\"$(datadir)/locale\" \
	-DPACKAGE_SOURCE_DIR=\"$(srcdir)\" \
	-DG_DISABLE_DEPRECATED \
	-DGDK_DISABLE_DEPRECATED \
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED

bin_PROGRAMS = xdx

xdx_SOURCES = \
	gui.h \
	history.h \
	net.h \
	preferences.h \
	text.h \
	types.h \
	utils.h \
	gui.c \
	history.c \
	main.c \
	net.c \
	preferences.c \
	text.c \
	utils.c

