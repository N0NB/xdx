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
 * gui_logdialog.c - dialog for opening the connection log
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

#include <unistd.h>

#include <gtk/gtk.h>

#include "gui.h"
#include "gui_logdialog.h"
#include "utils.h"


/*
 * called from the menu
 */
void
on_log_activate(GtkMenuItem     *menuitem,
                gpointer         user_data)
{
    GtkWidget *logdialog, *box, *logdialog_scrolledwindow,
        *logdialog_textview;

    gint response;
    FILE *fd;
    gchar *filename;
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    GtkTextMark *mark;
    gint numread = 0;
    gchar buf[1025];

    logdialog = gtk_dialog_new_with_buttons(_("Xdx - Connection log"),
                                            GTK_WINDOW(gui->window),
                                            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                            _("Clear"),
                                            GTK_RESPONSE_CANCEL,
                                            _("Close"),
                                            GTK_RESPONSE_CLOSE,
                                            NULL);
    gtk_widget_set_size_request(logdialog, 600, 300);
    box = gtk_dialog_get_content_area(GTK_DIALOG(logdialog));

    logdialog_scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(box), logdialog_scrolledwindow, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(logdialog_scrolledwindow),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);

    logdialog_textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(logdialog_textview), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(logdialog_textview), FALSE);
    gtk_container_add(GTK_CONTAINER(logdialog_scrolledwindow),
                                    logdialog_textview);

    filename = g_strdup_printf("%s/log.txt", gui->preferencesdir);

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(logdialog_textview));
    gtk_text_buffer_get_bounds(buffer, &start, &end);

    fd = fopen(filename, "r");

    if (fd != NULL) {
        while (!feof(fd)) {
            numread = fread(buf, 1, 1024, fd);
            gtk_text_buffer_insert(buffer, &end, buf, numread);
            mark = gtk_text_buffer_get_mark(buffer, "insert");
            gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(logdialog_textview), mark,
                                         0.0, FALSE, 0.0, 1.0);
        }

        fclose(fd);
    }

    gtk_widget_show_all(logdialog);

    response = gtk_dialog_run(GTK_DIALOG(logdialog));

    if (response == GTK_RESPONSE_CANCEL) {
        gtk_text_buffer_set_text(buffer, "", -1);
        unlink(filename);
        response = gtk_dialog_run(GTK_DIALOG(logdialog));
    }

    gtk_widget_destroy(logdialog);
}
