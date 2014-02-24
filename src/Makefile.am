## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	$(GTK_CFLAGS) -I.. \
	-DPACKAGE_DATA_DIR=\"$(datadir)/xdx\" \
	-DPACKAGE_LOCALE_DIR=\"$(datadir)/locale\" \
	-DPACKAGE_SOURCE_DIR=\"$(srcdir)\" \
	-DG_DISABLE_DEPRECATED \
	-DGDK_DISABLE_DEPRECATED \
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED

bin_PROGRAMS = xdx

xdx_SOURCES = \
	cmd_opts.c \
	cmd_opts.h \
	gtksourceiter.c \
	gtksourceiter.h \
	gui.c \
	gui.h \
	gui_aboutdialog.c \
	gui_aboutdialog.h \
	gui_closedialog.c \
	gui_closedialog.h \
	gui_exitdialog.c \
	gui_exitdialog.h \
	gui_logdialog.c \
	gui_logdialog.h \
	gui_manualdialog.c \
	gui_manualdialog.h \
	gui_opendialog.c \
	gui_opendialog.h \
	gui_settingsdialog.c \
	gui_settingsdialog.h \
	history.c \
	history.h \
	hyperlink.c \
	hyperlink.h \
	main.c \
	net.c \
	net.h \
	preferences.c \
	preferences.h \
	save.c \
	save.h \
	text.c \
	text.h \
	types.h \
	utils.c \
	utils.h

xdx_LDADD = $(GTK_LIBS)
