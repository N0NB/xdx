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
 * gui.h
 */

typedef struct guitype {
	GtkWidget *window;
	GtkItemFactory *item_factory;
	GList *hostnamehistory;
	GList *porthistory;
	GList *txhistory;
	gchar *preferencesdir;
	guint updown;
	guint txitem;
} guitype;

guitype *gui;

guitype *new_gui(void);
GtkWidget *create_mainwindow (void);
gboolean on_mainwindow_delete_event (GtkWidget * widget, GdkEvent * event,
				     gpointer user_data);
gboolean on_mainwindow_destroy_event (GtkWidget * widget, GdkEvent * event,
				     gpointer user_data);
void on_mainentry_activate (GtkEditable * editable, gpointer user_data);
gboolean on_mainwindow_key_press_event(GtkWidget *widget, GdkEventKey *event,
				     gpointer user_data);
gboolean on_maintext_motion_notify_event (GtkWidget * widget, GdkEvent * event,
				     gpointer user_data);

/* menus */

void on_open_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_close_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_about_activate (GtkMenuItem * menuitem, gpointer user_data);
