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
	gui_opendialog.h \
	gui_closedialog.h \
	gui_aboutdialog.h \
	gui_settingsdialog.h \
	gui_manualdialog.h \
	history.h \
	net.h \
	preferences.h \
	text.h \
	types.h \
	utils.h \
  hyperlink.h \
  save.h \
	gui.c \
	gui_opendialog.c \
	gui_closedialog.c \
	gui_aboutdialog.c \
	gui_settingsdialog.c \
	gui_manualdialog.c \
	history.c \
	main.c \
	net.c \
	preferences.c \
	text.c \
	utils.c \
  hyperlink.c \
  save.c
