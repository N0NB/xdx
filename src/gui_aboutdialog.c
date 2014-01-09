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
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * gui_aboutdialog.c - creation of the about dialog
 */

#include <gtk/gtk.h>
#include <string.h>

#include "config.h"
#include "gui.h"
#include "preferences.h"
#include "utils.h"

extern preferencestype preferences;

/*
static void
handle_url (GtkAboutDialog *about, const char *link, gpointer data)
{
        openurl (link);
}
*/

/*
static void
handle_email (GtkAboutDialog *about, const char *link, gpointer data)
{
        openmail (link);
}
*/

/*
 * called from the menu
 */
void
on_about_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar *authors[] = { "Joop Stakenborg, PG4I <pg4i@amsat.org>",
	                     "Nate Bargmann, N0NB <n0nb@n0nb.us>",
	                     NULL };

	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file
		(PACKAGE_DATA_DIR "/pixmaps/xdx-logo.png", NULL);

//        gtk_about_dialog_set_url_hook (handle_url, NULL, NULL);
//        gtk_about_dialog_set_email_hook (handle_email, NULL, NULL);

	gtk_show_about_dialog (GTK_WINDOW(gui->window), 
		"authors", authors,
		"comments", _("Tcp/ip DX-cluster and ON4KST chat client for amateur radio operators"),
		"license", 
"Copyright (C) 2002 - 2007 Joop Stakenborg <pg4i@amsat.org>\n"
"Copyright (C) 2014 Nate Bargmann <n0nb@n0nb.us>\n"
"\n"
"This program is free software; you can redistribute it and/or modify\n"
"it under the terms of the GNU General Public License as published by\n"
"the Free Software Foundation; either version 2 of the License, or\n"
"(at your option) any later version.\n"
"\n"
"This program is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
"GNU Library General Public License for more details.\n"
"\n"
"You should have received a copy of the GNU General Public License\n"
"along with this program; if not, write to the Free Software\n"
"Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.\n",
		"website", "https://github.com/N0NB/xdx",
		"logo", pixbuf,
		"translator-credits",
"Dutch: Joop Stakenborg PG4I <pg4i@amsat.org>\n"
"French: Jean-Luc Coulon F5IBH <jean-luc.coulon@wanadoo.fr>\n"
"Polish: Boguslaw Ciastek SQ5TB <SQ5TB@tlen.pl>\n"
"Portuguese: David Quental CT1DRB <ct1drb@iol.pt>\n"
"Spanish: Baltasar Perez EC8AYR <ec8ayr@yahoo.com>\n",
		"version", PACKAGE_VERSION,
		NULL);
}
