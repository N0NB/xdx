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
 * gui.h
 */


#ifndef XDX_GUI_H
#define XDX_GUI_H

typedef struct guitype {
    GtkWidget       *window;
    GList           *hostnamehistory;
    GList           *porthistory;
    GList           *txhistory;
    gchar           *preferencesdir;
    guint            updown;
    guint            txitem;
    gint             statusbartimer;
    gchar           *statusbarmessage;
    gchar           *url;
    gchar           *prompttagname;
    gchar           *calltagname;
    gchar           *senttagname;
    gchar           *wwvtagname;
    gchar           *wxtagname;
    gchar           *high1tagname;
    gchar           *high2tagname;
    gchar           *high3tagname;
    gchar           *high4tagname;
    gchar           *high5tagname;
    gchar           *high6tagname;
    gchar           *high7tagname;
    gchar           *high8tagname;
} guitype;

extern guitype *gui;

guitype *new_gui(void);
void create_mainwindow(void);

gboolean on_mainwindow_delete_event(GtkWidget   *widget,
                                    GdkEvent    *event,
                                    gpointer     user_data);

gboolean on_mainwindow_destroy_event(GtkWidget  *widget,
                                     GdkEvent   *event,
                                     gpointer    user_data);

void on_mainentry_activate(GtkTextBuffer    *buffer,
                           gpointer          user_data);

gboolean on_mainwindow_key_press_event(GtkWidget    *widget,
                                       GdkEventKey  *event,
                                       gpointer      user_data);

gboolean double_click(GtkWidget         *widget,
                      GdkEventButton    *event,
                      gpointer           user_data);

void on_fbutton_clicked(GtkButton   *button,
                        gpointer     user_data);

gboolean on_fbutton_press(GtkButton         *button,
                          GdkEventButton    *event,
                          gpointer           user_data);

void on_quit_activate(GtkMenuItem   *menuitem,
                      gpointer       user_data);

void on_sidebar_activate(GtkAction  *action,
                         gpointer    user_data);

void on_fkeys_activate(GtkAction    *action,
                       gpointer      user_data);

void on_reconnect_activate(GtkAction    *action,
                           gpointer      user_data);

void on_highentry_changed(GtkEditable   *editable,
                          gpointer       user_data);

gboolean on_highentry_clicked(GtkEditable       *entry,
                              GdkEventButton    *event,
                              gpointer           user_data);

extern GValue grid_spacing;

#endif /* XDX_GUI_H */
