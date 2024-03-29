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
 * utils.c - private functions that don't belong in other modules
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

#include <time.h>

#include <gtk/gtk.h>

#include "gui.h"
#include "preferences.h"
#include "utils.h"


extern preferencestype preferences;
static GList *pixmaps_directories = NULL;


/*
 * use this function to set the directory containing installed pixmaps
 */
void
add_pixmap_directory(const gchar *directory)
{
    pixmaps_directories = g_list_prepend(pixmaps_directories,
                                         g_strdup(directory));
}


/*
 * set statusbar  message to the previous message after a timeout occurs
 */
static gint
statusbar_timeout(gpointer data)
{
    GtkWidget *mainstatusbar;

    mainstatusbar = g_object_get_data(G_OBJECT(gui->window), "mainstatusbar");
    gtk_statusbar_pop(GTK_STATUSBAR(mainstatusbar), 1);
    gtk_statusbar_push(GTK_STATUSBAR(mainstatusbar), 1, gui->statusbarmessage);
    g_source_remove(gui->statusbartimer);
    gui->statusbartimer = -1;
    return FALSE;
}


/*
 * print a message to the statusbar. If timeout is set, the statusbar will
 * be set to the previous message after 5 seconds
 */
void
updatestatusbar(GString     *statusmessage,
                gboolean     timeout)
{
    GtkWidget *mainstatusbar;

    mainstatusbar = g_object_get_data(G_OBJECT(gui->window), "mainstatusbar");
    gtk_statusbar_pop(GTK_STATUSBAR(mainstatusbar), 1);
    gtk_statusbar_push(GTK_STATUSBAR(mainstatusbar), 1, statusmessage->str);

    if (timeout) {
        if (gui->statusbartimer != -1)
            g_source_remove(gui->statusbartimer);

        gui->statusbartimer = g_timeout_add(5000, statusbar_timeout, NULL);
    } else gui->statusbarmessage = g_strdup(statusmessage->str);
}


static void
shellcommand(gchar *command)
{
    gchar **args;

    args = g_strsplit(command, " ", 0);
    g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, NULL);
}


/* Returns FALSE if preferences.browserapp setting does not end in "%s" so
 * desktop defined default browser app is called.
 */
gboolean
openurl(const char *url)
{
    if (g_strrstr(preferences.browserapp, "%s")) {
        gchar buf[1024];
        GString *msg = g_string_new("");

        g_snprintf(buf, sizeof(buf), preferences.browserapp, url);
        g_string_printf(msg, _("Starting: %s"), buf);
        updatestatusbar(msg, TRUE);
        shellcommand(buf);
        g_string_free(msg, TRUE);

        return TRUE;
    } else
        return FALSE;
}


/* Returns FALSE if preferences.mailapp setting does not end in "%s" so
 * desktop defined default mail app is called.
 */
gboolean
openmail(const char *url)
{
    if (g_strrstr(preferences.mailapp, "%s")) {
        gchar buf[1024];
        GString *msg = g_string_new("");

        g_snprintf(buf, sizeof(buf), preferences.mailapp, url);
        g_string_printf(msg, _("Starting: %s"), buf);
        updatestatusbar(msg, TRUE);
        shellcommand(buf);
        g_string_free(msg, TRUE);

        return TRUE;
    } else
        return FALSE;
}


void
opensound(const char *file)
{
    gchar buf[1024];
    GString *msg = g_string_new("");

    if (g_strrstr(preferences.soundapp, "%s")) {
        g_snprintf(buf, sizeof(buf), preferences.soundapp, file);
        shellcommand(buf);
    }

    g_string_free(msg, TRUE);
}


/*
 * try to convert an incoming message to utf8
 */
gchar *
try_utf8(const gchar *str)
{
    gsize converted;
    gchar *utf8;

    if (str == NULL) return NULL;

    if (g_utf8_validate(str, -1, NULL)) return g_strdup(str);

    utf8 = g_locale_to_utf8(str, -1, &converted, NULL, NULL);

    if (utf8) return (utf8);

    utf8 = g_convert_with_fallback
           (str, -1, "UTF-8", "ISO-8859-1", ".", &converted, NULL, NULL);

    if (utf8) return (utf8);

    utf8 = g_convert_with_fallback
           (str, -1, "UTF-8", "ISO-8859-15", ".", &converted, NULL, NULL);

    if (utf8) return (utf8);

    utf8 = g_convert_with_fallback
           (str, -1, "UTF-8", "ISO-8859-2", ".", &converted, NULL, NULL);

    if (utf8) return (utf8);

    return (NULL);
}


/* get the current time, returned value has to be freed */
gchar *
xdxgettime(gboolean formatted)
{
    time_t current;
    struct tm *timestruct = NULL;
    gchar stimenow[20];

    time(&current);
    timestruct = localtime(&current);

    if (formatted)
        strftime(stimenow, 20, "%T", timestruct);
    else
        strftime(stimenow, 20, "%H", timestruct);

    return (g_strdup(stimenow));
}


/* get the current date, returned value has to be freed */
gchar *
xdxgetdate(gboolean formatted)
{
    time_t current;
    struct tm *timestruct = NULL;
    gchar datenow[20];

    time(&current);
    timestruct = localtime(&current);

    if (formatted)
        strftime(datenow, 20, "%Y-%m-%d", timestruct);
    else
        strftime(datenow, 20, "%Y%m%d", timestruct);

    return (g_strdup(datenow));
}


/*
 * log a connection
 */
void
logconnection(GString *logstr)
{
    gchar *str, *f;
    FILE *fp;

    str = g_strdup_printf
          ("[%s, %s UTC] %s", xdxgetdate(TRUE), xdxgettime(TRUE), logstr->str);
    f = g_strdup_printf("%s/log.txt", gui->preferencesdir);
    fp = fopen(f, "a");
    fprintf(fp, "%s\n", str);
    fclose(fp);
    g_free(str);
    g_free(f);
}


gchar *
my_strreplace(const char    *str,
              const char    *delimiter,
              const char    *replacement)
{
    gchar **split;
    gchar *ret;

    split = g_strsplit(str, delimiter, 0);
    ret = g_strjoinv(replacement, split);
    g_strfreev(split);

    return ret;
}
