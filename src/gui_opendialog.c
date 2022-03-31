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
 * gui_opendialog.c - dialog for opening a connection
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

#include <gtk/gtk.h>

#include "gui.h"
#include "gui_opendialog.h"
#include "net.h"
#include "utils.h"

#define HOSTNAMEHISTORY 10
#define PORTHISTORY     10


/*
 * called from the menu
 */
void
on_open_activate(GtkMenuItem    *menuitem,
                 gpointer        user_data)
{
    GtkWidget *opendialog, *hostnamecombo, *portcombo, *hbox, *vbox, *stock,
              *grid, *hostlabel, *portlabel, *mainentry, *hnc_child, *pc_child,
              *open_menu, *close_menu;
    gint i, num, response;
    GList *node;
    extern GValue grid_spacing;
    gboolean result = FALSE;
    servertype *cluster;
    gchar *s;

    gtk_widget_set_sensitive(gui->window, 0);

    opendialog = gtk_dialog_new_with_buttons(_("Xdx - Open connection"),
                 GTK_WINDOW(gui->window),
                 GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                 _("_Cancel"),
                 GTK_RESPONSE_CANCEL,
                 _("_OK"),
                 GTK_RESPONSE_OK,
                 NULL);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 6);

    vbox = gtk_dialog_get_content_area(GTK_DIALOG(opendialog));
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    stock = gtk_image_new_from_icon_name("dialog-question",
                                 GTK_ICON_SIZE_DIALOG);
    gtk_box_pack_start(GTK_BOX(hbox), stock, FALSE, FALSE, 0);

    grid = gtk_grid_new();
    g_object_set_property(G_OBJECT(grid), "column-spacing", &grid_spacing);
    g_object_set_property(G_OBJECT(grid), "row-spacing", &grid_spacing);
    gtk_box_pack_start(GTK_BOX(hbox), grid, TRUE, TRUE, 0);

    hostlabel = gtk_label_new(_("Hostname"));
    gtk_grid_attach(GTK_GRID(grid), hostlabel, 0, 0, 1, 1);

    hostnamecombo = gtk_combo_box_text_new_with_entry();

    if (gui->hostnamehistory) {
        num = g_list_length(gui->hostnamehistory);

        for (i = 0; i < num; i++) {
            s = g_list_nth_data(gui->hostnamehistory, i);
            gtk_combo_box_text_prepend_text(GTK_COMBO_BOX_TEXT(hostnamecombo), s);
        }
    }

    gtk_grid_attach(GTK_GRID(grid), hostnamecombo, 1, 0, 1, 1);

    portlabel = gtk_label_new(_("Port"));
    gtk_widget_set_halign(GTK_WIDGET(portlabel), GTK_ALIGN_END);
    gtk_grid_attach(GTK_GRID(grid), portlabel, 0, 1, 1, 1);

    portcombo = gtk_combo_box_text_new_with_entry();

    if (gui->porthistory) {
        num = g_list_length(gui->porthistory);

        for (i = 0; i < num; i++) {
            s = g_list_nth_data(gui->porthistory, i);
            gtk_combo_box_text_prepend_text(GTK_COMBO_BOX_TEXT(portcombo), s);
        }
    }

    gtk_grid_attach(GTK_GRID(grid), portcombo, 1, 1, 1, 1);

    gtk_widget_show_all(hbox);

    gtk_widget_set_sensitive(gui->window, 0);

    gtk_combo_box_set_active(GTK_COMBO_BOX(portcombo),
                             g_list_length(gui->porthistory) - 1);

    gtk_combo_box_set_active(GTK_COMBO_BOX(hostnamecombo),
                             g_list_length(gui->hostnamehistory) - 1);

    response = gtk_dialog_run(GTK_DIALOG(opendialog));

    open_menu = g_object_get_data(G_OBJECT(gui->window), "open_item");
    close_menu= g_object_get_data(G_OBJECT(gui->window), "close_item");

    if (response == GTK_RESPONSE_OK) {
        cluster = g_object_get_data(G_OBJECT(gui->window), "cluster");

        hnc_child = gtk_bin_get_child(GTK_BIN(hostnamecombo));
        cluster->host = gtk_editable_get_chars(GTK_EDITABLE(hnc_child), 0, -1);

        pc_child = gtk_bin_get_child(GTK_BIN(portcombo));
        cluster->port = gtk_editable_get_chars(GTK_EDITABLE(pc_child), 0, -1);

        if (!g_ascii_strcasecmp(cluster->host, ""))
            cluster->host = "localhost";

        if (!g_ascii_strcasecmp(cluster->port, ""))
            cluster->port = "8000";


        /* let's see if we need to add host/port to the history */
        node = NULL;
        node = g_list_find_custom(gui->hostnamehistory, cluster->host,
                                  (GCompareFunc)g_ascii_strncasecmp);

        if (!node)
            gui->hostnamehistory = g_list_prepend(gui->hostnamehistory,
                                                  g_strdup(cluster->host));
        else {
            /* add last connection to the top of the list */
            if (g_list_position(gui->hostnamehistory, node) != 0) {
                g_free(node->data);
                gui->hostnamehistory =
                    g_list_remove_link(gui->hostnamehistory, node);
                gui->hostnamehistory =
                    g_list_prepend(gui->hostnamehistory, g_strdup(cluster->host));
            }
        }

        if (g_list_length(gui->hostnamehistory) > HOSTNAMEHISTORY)
            gui->hostnamehistory = g_list_remove(gui->hostnamehistory,
                                                 g_list_last(gui->hostnamehistory)->data);

        node = NULL;
        node = g_list_find_custom(gui->porthistory, cluster->port,
                                  (GCompareFunc)g_ascii_strncasecmp);

        if (!node)
            gui->porthistory = g_list_prepend(gui->porthistory, g_strdup(cluster->port));
        else {
            if (g_list_position(gui->porthistory, node) != 0) {
                g_free(node->data);
                gui->porthistory =
                    g_list_remove_link(gui->porthistory, node);
                gui->porthistory =
                    g_list_prepend(gui->porthistory, g_strdup(cluster->port));
            }
        }

        if (g_list_length(gui->porthistory) > PORTHISTORY)
            gui->porthistory = g_list_remove(gui->porthistory,
                                             g_list_last(gui->porthistory)->data);

        gtk_widget_set_sensitive(GTK_WIDGET(open_menu), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(close_menu), FALSE);

        result = clresolve(cluster);
    }

    gtk_widget_destroy(opendialog);
    gtk_widget_set_sensitive(gui->window, 1);
    mainentry = g_object_get_data(G_OBJECT(gui->window), "mainentry");
    gtk_widget_grab_focus(GTK_WIDGET(mainentry));

    if (!result) {
        gtk_widget_set_sensitive(GTK_WIDGET(open_menu), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(close_menu), FALSE);
    }

}
