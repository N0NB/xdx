#!/bin/sh
# Run this to generate all the initial makefiles, etc.

AUTORECONF=autoreconf
AUTOMAKE=automake

SRCDIR=`dirname $0`
test -z "$SRCDIR" && SRCDIR=.

ORIGDIR=`pwd`
PROJECT=Xdx
FILE=src/main.c
DIE=0

($AUTORECONF --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have autoconf installed to compile $PROJECT."
        echo "Download the appropriate package for your distribution,"
        echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/autoconf."
        DIE=1
}

($AUTOMAKE --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have automake installed to compile $PROJECT."
        echo "Download the appropriate package for your distribution,"
        echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/automake."
        DIE=1
}

(gettext --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "You must have gettext installed to compile $PROJECT."
    echo "Download the appropriate package for your distribution,"
    echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/gettext."
    DIE=1
}


if test "$DIE" -eq 1; then
        exit 1
fi

cd $SRCDIR

test -f $FILE || {
        echo "You must run this script in the top-level $PROJECT directory"
        exit 1
}

# gettextize initializes the GNU Gettext system and adds various files
# and modifies Makefile.am, configure.ac, m4/Makefile.am.  See
# README.bootstrap for more information.

echo "Running gettextize...  Ignore non-fatal messages."
gettextize --force --copy --no-changelog

###################################################################
### autoreconf is now the preferred way to process configure.ac ###
### which should handle compiler variations and ensures that    ###
### subtools are processed in the correct order.                ###
###################################################################

echo "Running '$AUTORECONF -i' to process configure.ac"
echo "and generate the configure script."

# Tell autoreconf to install needed build system files

$AUTORECONF -i

cd $ORIGDIR
