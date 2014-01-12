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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
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

#include <stdio.h>

#include <gtk/gtk.h>

#include "gui.h"
#include "gui_manualdialog.h"
#include "utils.h"


void on_manual_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	GtkWidget *manualdialog, *swindow, *helptextview;
	GtkTextBuffer *buffer;
	GtkTextIter iter;
	PangoFontDescription *font_desc;
	gchar buf[80], *helpfile, *b;
	FILE *in;


	manualdialog = gtk_dialog_new_with_buttons (_("xdx - manual"),
		GTK_WINDOW (gui->window), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
	gtk_widget_set_size_request (manualdialog, 650, 300);

	swindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (manualdialog)->vbox),
		swindow, TRUE, TRUE, 0);
	helptextview = gtk_text_view_new ();
	gtk_text_view_set_editable (GTK_TEXT_VIEW(helptextview), FALSE);
	gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW(helptextview), FALSE);
	gtk_container_add (GTK_CONTAINER (swindow), helptextview);
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(helptextview));
	gtk_text_buffer_get_start_iter (buffer, &iter);

/* TRANSLATORS:
 * Do not translate MANUAL unless you provide a faq in your language,
 * e.g. the polish faq is called MANUAL.pl.
 */
	helpfile = g_strdup_printf ("%s%s%s", PACKAGE_DATA_DIR, G_DIR_SEPARATOR_S, _("MANUAL"));

	g_signal_connect(G_OBJECT(manualdialog), "response",
		G_CALLBACK(gtk_widget_destroy), NULL);


	in = fopen (helpfile, "r");
	if (in)
	{
		do
		{
			if (fgets (buf, 80, in) == NULL)
				break;
			else
			{
				b = g_locale_to_utf8 (buf, -1, NULL, NULL, NULL);
				gtk_text_buffer_insert (buffer, &iter, b, -1);
				g_free (b);
			}
		}
		while (!feof (in));
		fclose (in);
	}
	font_desc = pango_font_description_from_string ("mono");
	gtk_widget_modify_font (helptextview, font_desc);
	pango_font_description_free (font_desc);
	g_free (helpfile);
	gtk_widget_show_all(manualdialog);
}
