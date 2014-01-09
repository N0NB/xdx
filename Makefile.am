## Process this file with automake to produce Makefile.in

DESKTOP_FILES = Xdx.desktop

SHARE_FILES = MANUAL MANUAL.es MANUAL.fr MANUAL.nl MANUAL.pl cty.dat

GNUPLOT_FILES = gnuplot/wwv.gnuplot

PIXMAP_FILES = pixmaps/bigsmile.png pixmaps/cry.png pixmaps/sad.png \
	pixmaps/smile.png pixmaps/wink.png pixmaps/xdx-logo.png \
	pixmaps/xdx.png pixmaps/xdx.xpm

SOUND_FILES = sounds/attention.wav

EXTRA_DIST = \
	xdx.1.in

SUBDIRS = m4 po src

desktopdir = $(datadir)/applications
dist_desktop_DATA = $(DESKTOP_FILES)

man_MANS = xdx.1

dist_pkgdata_DATA = $(SHARE_FILES)

nobase_dist_pkgdata_DATA = $(PIXMAP_FILES) $(SOUND_FILES) $(GNUPLOT_FILES)

ACLOCAL_AMFLAGS = -I m4
