/*
 * xdx - GTK+ DX-cluster client for amateur radio
 * Copyright (C) 2002 Joop Stakenborg <pa4tu@amsat.org>
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
#include <gdk/gdkkeysyms.h>

#include "gui.h"
#include "utils.h"

#define XDX_HOMEPAGE "http://people.debian.org/~pa3aba/xdx.html"

/*
 * the link is clicked in the about dialog
 */
static gboolean on_weblink_button_press_event (GtkWidget *widget, 
  GdkEventButton *event)
{
  openurl (XDX_HOMEPAGE);
  return FALSE;
}


static void event_destroy(gpointer data)
{
  GtkWidget *aboutdialog = (GtkWidget *)data;
  
  aboutdialog = NULL;
}

/*
 * called from the menu
 */
void
on_about_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  GtkWidget *aboutdialog, *vbox, *stock, *aboutlabel, *separator, *eventbox;
  GString *labeltext = g_string_new ("");
  PangoFontDescription *font;
  GdkCursor *cursor;

  aboutdialog = gtk_dialog_new_with_buttons (_("xdx - about"), 
    GTK_WINDOW (gui->window), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
    GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

  g_signal_connect(G_OBJECT(aboutdialog), "destroy",
                         GTK_SIGNAL_FUNC(event_destroy), aboutdialog);
  g_signal_connect(G_OBJECT(aboutdialog), "response",
                         GTK_SIGNAL_FUNC(gtk_widget_destroy), NULL);


  vbox = gtk_vbox_new (FALSE, 8);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (aboutdialog)->vbox), vbox, FALSE,
		      FALSE, 0);

  g_string_printf (labeltext, _("%s version %s"),
		   PACKAGE, VERSION);
  aboutlabel = gtk_label_new (labeltext->str);
  font = pango_font_description_from_string("helvetica bold 25");
  gtk_widget_modify_font(aboutlabel, font);
  pango_font_description_free(font);
  gtk_box_pack_start (GTK_BOX (vbox), aboutlabel, FALSE, FALSE, 0);

  separator = gtk_hseparator_new();
  gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, FALSE, 0);

  g_string_printf (labeltext, "Copyright (C) 2002, Joop Stakenborg <pg4i@amsat.org>");
  aboutlabel = gtk_label_new (labeltext->str);
  gtk_box_pack_start (GTK_BOX (vbox), aboutlabel, FALSE, FALSE, 0);

  eventbox = gtk_event_box_new();
  g_signal_connect(G_OBJECT(eventbox), "button_press_event",
			G_CALLBACK(on_weblink_button_press_event), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), eventbox, FALSE, FALSE, 0);

  g_string_printf (labeltext, "<u>"XDX_HOMEPAGE"</u>");
  aboutlabel = gtk_label_new (labeltext->str);
  gtk_label_set_use_markup(GTK_LABEL(aboutlabel), TRUE);
  gtk_container_add (GTK_CONTAINER (eventbox), aboutlabel);

  g_string_printf (labeltext, _("Published under the GNU General Public License"));
  aboutlabel = gtk_label_new (labeltext->str);
  g_string_free (labeltext, TRUE);
  gtk_box_pack_start (GTK_BOX (vbox), aboutlabel, FALSE, FALSE, 0);

  gtk_widget_show_all(aboutdialog);

  cursor = gdk_cursor_new(GDK_HAND2);
  gdk_window_set_cursor(eventbox->window, cursor);
  gdk_cursor_unref(cursor);
}
