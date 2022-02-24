/*
 * xdx - GTK+ DX-cluster client for amateur radio
 * Copyright (C) 2002-2006 Joop Stakenborg <pg4i@amsat.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
 * preferences.c - private functions for saving and recalling xdx preferences.
 */


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <gtk/gtk.h>

#include "gui.h"
#include "preferences.h"
#include "utils.h"


preferencestype preferences;


/*
 * check if ~/.xdx directory exists
 */
void
dircheck()
{
    struct stat statdir;

    gui->preferencesdir = g_strdup_printf("%s/.%s", g_get_home_dir(), PACKAGE);

    if (stat(gui->preferencesdir, &statdir) == -1) {
        if (mkdir(gui->preferencesdir, S_IRUSR | S_IWUSR | S_IXUSR) == -1)
            g_error(_("Creating ~/.%s directory."), PACKAGE);
    } else if (!S_ISDIR(statdir.st_mode))
        g_error(_("~/.%s is not a directory."), PACKAGE);
}


/*
 * look up settings in ~/.xdx/preferences
 */

void
loadpreferences(void)
{
    gchar *preferencesfile, label[100], value[100];
    FILE *fp;

    /* defaults */
    preferences.x = 10;
    preferences.y = 30;
    preferences.width = 750;
    preferences.height = 550;
    preferences.columnwidths =
        g_strdup_printf("%d,%d,%d,%d,%d,%d,%d",
                        COL0WIDTH, COL1WIDTH, COL2WIDTH, COL3WIDTH, COL4WIDTH, COL5WIDTH, COL6WIDTH);
    preferences.autologin = 0;
    preferences.callsign = g_strdup("N0CALL");
    preferences.commands = g_strdup("set/page 0");
    preferences.savedx = 0;
    preferences.savewwv = 0;
    preferences.savetoall = 0;
    preferences.savewx = 0;
    preferences.hamlib = 0;
    preferences.rigctl = g_strdup("rigctl -m 1 -r /dev/rig set_freq %d");;
    preferences.browserapp = g_strdup("?");
    preferences.mailapp = g_strdup("?");
    preferences.soundapp = g_strdup("play %s");
    preferences.col0visible = 1;
    preferences.col1visible = 1;
    preferences.col2visible = 1;
    preferences.col3visible = 1;
    preferences.col4visible = 1;
    preferences.col5visible = 1;
    preferences.col6visible = 1;
    preferences.dxfont = g_strdup("Sans 10");
    preferences.allfont = g_strdup("Sans 10");
    preferences.localecho = 1;
    preferences.handlebarpos = 350;
    preferences.highword1 = g_strdup("?");
    preferences.highword2 = g_strdup("?");
    preferences.highword3 = g_strdup("?");
    preferences.highword4 = g_strdup("?");
    preferences.highword5 = g_strdup("?");
    preferences.highword6 = g_strdup("?");
    preferences.highword7 = g_strdup("?");
    preferences.highword8 = g_strdup("?");
    gdk_rgba_parse(&preferences.highcolor1, "red");
    gdk_rgba_parse(&preferences.highcolor2, "red");
    gdk_rgba_parse(&preferences.highcolor3, "red");
    gdk_rgba_parse(&preferences.highcolor4, "red");
    gdk_rgba_parse(&preferences.highcolor5, "red");
    gdk_rgba_parse(&preferences.highcolor6, "red");
    gdk_rgba_parse(&preferences.highcolor7, "red");
    gdk_rgba_parse(&preferences.highcolor8, "red");
    preferences.highmenu = g_strdup("00000000");
    preferences.sidebar = 1;
    preferences.fbox = 1;
    preferences.reconnect = 0;
    preferences.playsound = 0;
    preferences.keepalive = 0;
    gdk_rgba_parse(&preferences.promptcolor, "green");
    gdk_rgba_parse(&preferences.sentcolor, "red");
    gdk_rgba_parse(&preferences.wwvcolor, "darkgreen");
    gdk_rgba_parse(&preferences.wxcolor, "magenta");
    preferences.f1command = g_strdup("^");
    preferences.f2command = g_strdup("^");
    preferences.f3command = g_strdup("^");
    preferences.f4command = g_strdup("^");
    preferences.f5command = g_strdup("^");
    preferences.f6command = g_strdup("^");
    preferences.f7command = g_strdup("^");
    preferences.f8command = g_strdup("^");

    /* open preferences file */
    preferencesfile = g_strdup_printf("%s/preferences", gui->preferencesdir);
    fp = fopen(preferencesfile, "r");

    if (fp) {
        while (!feof(fp)) {
            if (fscanf(fp, "%s %s", label, value) == EOF) break;

            if (!g_ascii_strcasecmp(label, "x"))
                preferences.x = atoi(value);
            else if (!g_ascii_strcasecmp(label, "y"))
                preferences.y = atoi(value);
            else if (!g_ascii_strcasecmp(label, "width"))
                preferences.width = atoi(value);
            else if (!g_ascii_strcasecmp(label, "height"))
                preferences.height = atoi(value);
            else if (!g_ascii_strcasecmp(label, "columnwidths2"))
                preferences.columnwidths = g_strdup(value);
            else if (!g_ascii_strcasecmp(label, "autologin"))
                preferences.autologin = atoi(value);
            else if (!g_ascii_strcasecmp(label, "callsign"))
                preferences.callsign = g_strdup(value);
            else if (!g_ascii_strcasecmp(label, "commands")) {
                g_strdelimit(value, "_", ' ');
                preferences.commands = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "savedx"))
                preferences.savedx = atoi(value);
            else if (!g_ascii_strcasecmp(label, "savewwv"))
                preferences.savewwv = atoi(value);
            else if (!g_ascii_strcasecmp(label, "savetoall"))
                preferences.savetoall = atoi(value);
            else if (!g_ascii_strcasecmp(label, "savewx"))
                preferences.savewx = atoi(value);
            else if (!g_ascii_strcasecmp(label, "hamlib"))
                preferences.hamlib = atoi(value);
            else if (!g_ascii_strcasecmp(label, "rigctl")) {
                g_strdelimit(value, "~", ' ');
                preferences.rigctl = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "browserapp")) {
                g_strdelimit(value, "~", ' ');
                preferences.browserapp = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "mailapp")) {
                g_strdelimit(value, "~", ' ');
                preferences.mailapp = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "soundapp")) {
                g_strdelimit(value, "~", ' ');
                preferences.soundapp = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "col0visible"))
                preferences.col0visible = atoi(value);
            else if (!g_ascii_strcasecmp(label, "col1visible"))
                preferences.col1visible = atoi(value);
            else if (!g_ascii_strcasecmp(label, "col2visible"))
                preferences.col2visible = atoi(value);
            else if (!g_ascii_strcasecmp(label, "col3visible"))
                preferences.col3visible = atoi(value);
            else if (!g_ascii_strcasecmp(label, "col4visible"))
                preferences.col4visible = atoi(value);
            else if (!g_ascii_strcasecmp(label, "col5visible"))
                preferences.col5visible = atoi(value);
            else if (!g_ascii_strcasecmp(label, "col6visible"))
                preferences.col6visible = atoi(value);
            else if (!g_ascii_strcasecmp(label, "dxfont")) {
                g_strdelimit(value, "~", ' ');
                preferences.dxfont = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "allfont")) {
                g_strdelimit(value, "~", ' ');
                preferences.allfont = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "localecho"))
                preferences.localecho = atoi(value);
            else if (!g_ascii_strcasecmp(label, "handlebarpos"))
                preferences.handlebarpos = atoi(value);
            else if (!g_ascii_strcasecmp(label, "highword1")) {
                g_strdelimit(value, "~", ' ');
                preferences.highword1 = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "highword2")) {
                g_strdelimit(value, "~", ' ');
                preferences.highword2 = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "highword3")) {
                g_strdelimit(value, "~", ' ');
                preferences.highword3 = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "highword4")) {
                g_strdelimit(value, "~", ' ');
                preferences.highword4 = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "highword5")) {
                g_strdelimit(value, "~", ' ');
                preferences.highword5 = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "highword6")) {
                g_strdelimit(value, "~", ' ');
                preferences.highword6 = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "highword7")) {
                g_strdelimit(value, "~", ' ');
                preferences.highword7 = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "highword8")) {
                g_strdelimit(value, "~", ' ');
                preferences.highword8 = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "highcolor1"))
                gdk_rgba_parse(&preferences.highcolor1, value);
            else if (!g_ascii_strcasecmp(label, "highcolor2"))
                gdk_rgba_parse(&preferences.highcolor2, value);
            else if (!g_ascii_strcasecmp(label, "highcolor3"))
                gdk_rgba_parse(&preferences.highcolor3, value);
            else if (!g_ascii_strcasecmp(label, "highcolor4"))
                gdk_rgba_parse(&preferences.highcolor4, value);
            else if (!g_ascii_strcasecmp(label, "highcolor5"))
                gdk_rgba_parse(&preferences.highcolor5, value);
            else if (!g_ascii_strcasecmp(label, "highcolor6"))
                gdk_rgba_parse(&preferences.highcolor6, value);
            else if (!g_ascii_strcasecmp(label, "highcolor7"))
                gdk_rgba_parse(&preferences.highcolor7, value);
            else if (!g_ascii_strcasecmp(label, "highcolor8"))
                gdk_rgba_parse(&preferences.highcolor8, value);
            else if (!g_ascii_strcasecmp(label, "highmenu"))
                preferences.highmenu = g_strdup(value);
            else if (!g_ascii_strcasecmp(label, "sidebar"))
                preferences.sidebar = atoi(value);
            else if (!g_ascii_strcasecmp(label, "fbox"))
                preferences.fbox = atoi(value);
            else if (!g_ascii_strcasecmp(label, "reconnect"))
                preferences.reconnect = atoi(value);
            else if (!g_ascii_strcasecmp(label, "playsound"))
                preferences.playsound = atoi(value);
            else if (!g_ascii_strcasecmp(label, "keepalive"))
                preferences.keepalive = atoi(value);
            else if (!g_ascii_strcasecmp(label, "promptcolor"))
                gdk_rgba_parse(&preferences.promptcolor, value);
            else if (!g_ascii_strcasecmp(label, "sentcolor"))
                gdk_rgba_parse(&preferences.sentcolor, value);
            else if (!g_ascii_strcasecmp(label, "wwvcolor"))
                gdk_rgba_parse(&preferences.wwvcolor, value);
            else if (!g_ascii_strcasecmp(label, "wxcolor"))
                gdk_rgba_parse(&preferences.wxcolor, value);
            else if (!g_ascii_strcasecmp(label, "f1command")) {
                g_strdelimit(value, "~", ' ');
                preferences.f1command = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "f2command")) {
                g_strdelimit(value, "~", ' ');
                preferences.f2command = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "f3command")) {
                g_strdelimit(value, "~", ' ');
                preferences.f3command = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "f4command")) {
                g_strdelimit(value, "~", ' ');
                preferences.f4command = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "f5command")) {
                g_strdelimit(value, "~", ' ');
                preferences.f5command = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "f6command")) {
                g_strdelimit(value, "~", ' ');
                preferences.f6command = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "f7command")) {
                g_strdelimit(value, "~", ' ');
                preferences.f7command = g_strdup(value);
            } else if (!g_ascii_strcasecmp(label, "f8command")) {
                g_strdelimit(value, "~", ' ');
                preferences.f8command = g_strdup(value);
            }
        }

        fclose(fp);
    }

    g_free(preferencesfile);
}


/*
 * save preferences in ~/.xdx/preferences
 */
void
savepreferences(void)
{
    gchar *preferencesfile, *str;
    FILE *fp;

    /* open preferences file */
    preferencesfile = g_strdup_printf("%s/preferences", gui->preferencesdir);
    fp = fopen(preferencesfile, "w");

    if (fp) {
        fprintf(fp, "version %s\n", VERSION);
        str = g_strdup_printf("%d", preferences.x);
        fprintf(fp, "x %s\n", str);
        str = g_strdup_printf("%d", preferences.y);
        fprintf(fp, "y %s\n", str);
        str = g_strdup_printf("%d", preferences.width);
        fprintf(fp, "width %s\n", str);
        str = g_strdup_printf("%d", preferences.height);
        fprintf(fp, "height %s\n", str);
        str = g_strdup_printf("%s", preferences.columnwidths);
        fprintf(fp, "columnwidths2 %s\n", str);
        str = g_strdup_printf("%d", preferences.autologin);
        fprintf(fp, "autologin %s\n", str);
        str = g_strdup_printf("%s", preferences.callsign);
        fprintf(fp, "callsign %s\n", str);
        str = g_strdup_printf("%s", preferences.commands);
        g_strdelimit(str, " ", '_');
        fprintf(fp, "commands %s\n", str);
        str = g_strdup_printf("%d", preferences.savedx);
        fprintf(fp, "savedx %s\n", str);
        str = g_strdup_printf("%d", preferences.savewwv);
        fprintf(fp, "savewwv %s\n", str);
        str = g_strdup_printf("%d", preferences.savetoall);
        fprintf(fp, "savetoall %s\n", str);
        str = g_strdup_printf("%d", preferences.savewx);
        fprintf(fp, "savewx %s\n", str);
        str = g_strdup_printf("%d", preferences.hamlib);
        fprintf(fp, "hamlib %s\n", str);
        str = g_strdup_printf("%s", preferences.rigctl);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "rigctl %s\n", str);
        str = g_strdup_printf("%s", preferences.browserapp);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "browserapp %s\n", str);
        str = g_strdup_printf("%s", preferences.mailapp);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "mailapp %s\n", str);
        str = g_strdup_printf("%s", preferences.soundapp);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "soundapp %s\n", str);
        str = g_strdup_printf("%d", preferences.col0visible);
        fprintf(fp, "col0visible %s\n", str);
        str = g_strdup_printf("%d", preferences.col1visible);
        fprintf(fp, "col1visible %s\n", str);
        str = g_strdup_printf("%d", preferences.col2visible);
        fprintf(fp, "col2visible %s\n", str);
        str = g_strdup_printf("%d", preferences.col3visible);
        fprintf(fp, "col3visible %s\n", str);
        str = g_strdup_printf("%d", preferences.col4visible);
        fprintf(fp, "col4visible %s\n", str);
        str = g_strdup_printf("%d", preferences.col5visible);
        fprintf(fp, "col5visible %s\n", str);
        str = g_strdup_printf("%d", preferences.col6visible);
        fprintf(fp, "col6visible %s\n", str);
        str = g_strdup_printf("%s", preferences.dxfont);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "dxfont %s\n", str);
        str = g_strdup_printf("%s", preferences.allfont);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "allfont %s\n", str);
        str = g_strdup_printf("%d", preferences.localecho);
        fprintf(fp, "localecho %s\n", str);
        str = g_strdup_printf("%d", preferences.handlebarpos);
        fprintf(fp, "handlebarpos %s\n", str);
        str = g_strdup_printf("%s", preferences.highword1);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "highword1 %s\n", str);
        str = g_strdup_printf("%s", preferences.highword2);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "highword2 %s\n", str);
        str = g_strdup_printf("%s", preferences.highword3);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "highword3 %s\n", str);
        str = g_strdup_printf("%s", preferences.highword4);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "highword4 %s\n", str);
        str = g_strdup_printf("%s", preferences.highword5);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "highword5 %s\n", str);
        str = g_strdup_printf("%s", preferences.highword6);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "highword6 %s\n", str);
        str = g_strdup_printf("%s", preferences.highword7);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "highword7 %s\n", str);
        str = g_strdup_printf("%s", preferences.highword8);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "highword8 %s\n", str);
        fprintf(fp, "highcolor1 %s\n",
                gdk_rgba_to_string(&preferences.highcolor1));
        fprintf(fp, "highcolor2 %s\n",
                gdk_rgba_to_string(&preferences.highcolor2));
        fprintf(fp, "highcolor3 %s\n",
                gdk_rgba_to_string(&preferences.highcolor3));
        fprintf(fp, "highcolor4 %s\n",
                gdk_rgba_to_string(&preferences.highcolor4));
        fprintf(fp, "highcolor5 %s\n",
                gdk_rgba_to_string(&preferences.highcolor5));
        fprintf(fp, "highcolor6 %s\n",
                gdk_rgba_to_string(&preferences.highcolor6));
        fprintf(fp, "highcolor7 %s\n",
                gdk_rgba_to_string(&preferences.highcolor7));
        fprintf(fp, "highcolor8 %s\n",
                gdk_rgba_to_string(&preferences.highcolor8));
        str = g_strdup_printf("%s", preferences.highmenu);
        fprintf(fp, "highmenu %s\n", str);
        str = g_strdup_printf("%d", preferences.sidebar);
        fprintf(fp, "sidebar %s\n", str);
        str = g_strdup_printf("%d", preferences.fbox);
        fprintf(fp, "fbox %s\n", str);
        str = g_strdup_printf("%d", preferences.reconnect);
        fprintf(fp, "reconnect %s\n", str);
        str = g_strdup_printf("%d", preferences.playsound);
        fprintf(fp, "playsound %s\n", str);
        str = g_strdup_printf("%d", preferences.keepalive);
        fprintf(fp, "keepalive %s\n", str);
        fprintf(fp, "promptcolor %s\n",
                gdk_rgba_to_string(&preferences.promptcolor));
        fprintf(fp, "sentcolor %s\n",
                gdk_rgba_to_string(&preferences.sentcolor));
        fprintf(fp, "wwvcolor %s\n",
                gdk_rgba_to_string(&preferences.wwvcolor));
        fprintf(fp, "wxcolor %s\n",
                gdk_rgba_to_string(&preferences.wxcolor));
        str = g_strdup_printf("%s", preferences.f1command);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "f1command %s\n", str);
        str = g_strdup_printf("%s", preferences.f2command);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "f2command %s\n", str);
        str = g_strdup_printf("%s", preferences.f3command);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "f3command %s\n", str);
        str = g_strdup_printf("%s", preferences.f4command);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "f4command %s\n", str);
        str = g_strdup_printf("%s", preferences.f5command);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "f5command %s\n", str);
        str = g_strdup_printf("%s", preferences.f6command);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "f6command %s\n", str);
        str = g_strdup_printf("%s", preferences.f7command);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "f7command %s\n", str);
        str = g_strdup_printf("%s", preferences.f8command);
        g_strdelimit(str, " ", '~');
        fprintf(fp, "f8command %s\n", str);
        g_free(str);
        fclose(fp);
    }

    g_free(preferencesfile);
}
