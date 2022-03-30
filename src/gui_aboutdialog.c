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
 * gui_aboutdialog.c - creation of the about dialog
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

#include <string.h>

#include <gtk/gtk.h>

#include "gui.h"
#include "preferences.h"
#include "utils.h"


extern preferencestype preferences;


/* When mailapp or browserapp is not set in preferences, openmail and openurl
 * will return FALSE so we pass that up into the GTK+ signal handlers so
 * the GtkAboutDialog signal handler will call the mail or browser app defined
 * by desktop defaults.
 */
static
gboolean handle_uri_hook(GtkAboutDialog *about,
                         const char     *link,
                         gpointer        data)
{
    if (g_str_has_prefix(link, "mailto:")) {
        return openmail(link + 7);
    } else {
        return openurl(link);
    }
}


/*
 * called from the menu
 */
void
on_about_activate(GtkMenuItem   *menuitem,
                  gpointer       user_data)
{
    const gchar *authors[] = {
        "Joop Stakenborg, PG4I <pg4i@amsat.org>",
        "Nate Bargmann, N0NB <n0nb@n0nb.us>",
        NULL
    };

    gchar *license =
        "Copyright (C) 2002-2007 Joop Stakenborg <pg4i@amsat.org>\n"
        "Copyright (C) 2014-2022 Nate Bargmann <n0nb@n0nb.us>\n"
        "\n"
        "This program is free software; you can redistribute it and/or modify\n"
        "it under the terms of the GNU General Public License as published by\n"
        "the Free Software Foundation; either version 2 of the License, or\n"
        "(at your option) any later version.\n"
        "\n"
        "This program is distributed in the hope that it will be useful,\n"
        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
        "GNU General Public License for more details.\n"
        "\n"
        "You should have received a copy of the GNU General Public License along\n"
        "with this program; if not, write to the Free Software Foundation, Inc.,\n"
        "51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.\n";

    gchar *translators =
        "Dutch: Joop Stakenborg PG4I <pg4i@amsat.org>\n"
        "French: Jean-Luc Coulon F5IBH <jean-luc.coulon@wanadoo.fr>\n"
        "Polish: Boguslaw Ciastek SQ5TB <SQ5TB@tlen.pl>\n"
        "Portuguese: David Quental CT1DRB <ct1drb@radiotelegrafistas.pt>\n"
        "Spanish: Baltasar Perez EC8AYR <ec8ayr@yahoo.com>\n";

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file
                        (PACKAGE_DATA_DIR "/pixmaps/xdx-logo.png", NULL);

    GtkWidget *about = gtk_about_dialog_new();
    GtkAboutDialog *ad = GTK_ABOUT_DIALOG(about);

    gtk_about_dialog_set_program_name(ad, PACKAGE_NAME);
    gtk_about_dialog_set_authors(ad, authors);
    gtk_about_dialog_set_comments(ad,
                                  _("TCP/IP DX-cluster and ON4KST chat client for amateur radio operators"));
    gtk_about_dialog_set_license(ad, license);
    gtk_about_dialog_set_website(ad, "https://github.com/N0NB/xdx");
    gtk_about_dialog_set_logo(ad, pixbuf);
    gtk_about_dialog_set_translator_credits(ad, translators);
    gtk_about_dialog_set_version(ad, PACKAGE_VERSION);

    gtk_window_set_transient_for(GTK_WINDOW(ad), GTK_WINDOW(gui->window));

    g_signal_connect(ad, "activate-link", G_CALLBACK(handle_uri_hook), NULL);

    gtk_dialog_run(GTK_DIALOG(ad));
    gtk_widget_destroy(GTK_WIDGET(ad));
}
