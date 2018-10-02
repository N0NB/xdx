## Process this file with autoconf to create configure. -*- autoconf -*-

dnl Autoconf 2.67 is in Debian Squeeze.
AC_PREREQ([2.67])

AC_INIT([Xdx],
	[2.90~git],
	[xdxclusterclient-discuss@lists.sourceforge.net],
	[xdx],
	[https://github.com/N0NB/xdx])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_AUX_DIR([build-aux])

AM_CPPFLAGS="${AM_CPPFLAGS} -I\$(top_srcdir)/include"

AM_INIT_AUTOMAKE([-Wall subdir-objects])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_USE_SYSTEM_EXTENSIONS
AC_PROG_INSTALL
AC_PROG_CC
AC_LANG_C

dnl Quell AC_COMPILE_IFELSE warnings
AC_GNU_SOURCE

AM_GNU_GETTEXT([external])

dnl Should work with at least the version of gettext in Debian Wheezy
AM_GNU_GETTEXT_VERSION([0.18.1])

# autoheader templates for AM_GNU_GETTEXT checks.
AH_TEMPLATE([ENABLE_NLS], [])
AH_TEMPLATE([HAVE_CATGETS], [])
AH_TEMPLATE([HAVE_GETTEXT], [])
AH_TEMPLATE([HAVE_LC_MESSAGES], [])
AH_TEMPLATE([HAVE_STPCPY], [])

# Checks for header files.
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([arpa/inet.h fcntl.h libintl.h netdb.h netinet/in.h string.h sys/socket.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_FORK
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_CHECK_FUNCS([tzset setlocale putenv alarm bzero gethostbyname inet_ntoa memset mkdir socket strerror])
AC_FUNC_STRFTIME
AC_STRUCT_TM

AX_CFLAGS_WARN_ALL([AM_CFLAGS])

dnl Installed GTK must be at least version 2.24
PKG_CHECK_MODULES([GTK], [gtk+-2.0 >= 2.24.0])
AC_SUBST([GTK_CFLAGS])
AC_SUBST([GTK_LIBS])

dnl Output accumulated flags to the Makefile files.
AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CFLAGS])

AC_CONFIG_FILES([m4/Makefile
src/Makefile
Makefile
xdx.1
Xdx.desktop])

AC_OUTPUT
