## Process this file with automake to produce Makefile.in

EXTRA_DIST = \
	Xdx.desktop \
	xdx.spec \
	xdx.spec.in \
	xdx.1 \
	xdx.1.in \
	MANUAL

SUBDIRS = \
	m4 \
	po \
	src


man_MANS = xdx.1

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
		$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/pixmaps; \
		for pixmap in $(srcdir)/pixmaps/*; do \
			if test -f $$pixmap; then \
				$(INSTALL_DATA) $$pixmap $(DESTDIR)$(pkgdatadir)/pixmaps; \
			fi \
		done \
	fi
	if test -f MANUAL; then \
		$(INSTALL_DATA) MANUAL $(DESTDIR)$(pkgdatadir); \
	fi


dist-hook:
	if test -d pixmaps; then \
		mkdir $(distdir)/pixmaps; \
		for pixmap in pixmaps/*; do \
			if test -f $$pixmap; then \
				cp -p $$pixmap $(distdir)/pixmaps; \
			fi \
		done \
	fi
	if test -d debian; then \
		mkdir $(distdir)/debian; \
		for deb in debian/*; do \
			if test -f $$deb; then \
				cp -p $$deb $(distdir)/debian; \
			fi \
		done \
	fi

rpm: Makefile
	make dist
	$(RPMBUILD) -ta $(PACKAGE)-$(VERSION).tar.gz
	rm $(PACKAGE)-$(VERSION).tar.gz


ACLOCAL_AMFLAGS = -I m4

