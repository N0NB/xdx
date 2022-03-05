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
 * gui_closedialog.c - dialog for closing a connection
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
#include "gui_closedialog.h"
#include "net.h"
#include "utils.h"


/*
 * called from the menu
 */
void
on_close_activate(GtkMenuItem   *menuitem,
                  gpointer       user_data)
{
    GtkWidget *closedialog, *closelabel, *hbox, *vbox, *stock, *mainentry;
    GString *labeltext = g_string_new("");
    GString *msg = g_string_new("");
    gint response;
    servertype *cluster;

    gtk_widget_set_sensitive(gui->window, 0);
    closedialog = gtk_dialog_new_with_buttons(_("Xdx - Close connection"),
                  GTK_WINDOW(gui->window),
                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                  _("Cancel"),
                  GTK_RESPONSE_CANCEL,
                  _("OK"),
                  GTK_RESPONSE_OK,
                  NULL);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 8);

    vbox = gtk_dialog_get_content_area(GTK_DIALOG(closedialog));
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    stock = gtk_image_new_from_icon_name("dialog-question",
                                         GTK_ICON_SIZE_DIALOG);
    gtk_box_pack_start(GTK_BOX(hbox), stock, FALSE, FALSE, 0);

    cluster = (servertype *)g_object_get_data(G_OBJECT(gui->window), "cluster");
    g_string_printf(labeltext, _("Close connection to %s ?"),
                    cluster->host);
    closelabel = gtk_label_new_with_mnemonic(labeltext->str);
    g_string_free(labeltext, TRUE);

    gtk_box_pack_start(GTK_BOX(hbox), closelabel, TRUE, TRUE, 0);
    gtk_widget_show_all(hbox);
    response = gtk_dialog_run(GTK_DIALOG(closedialog));

    if (response == GTK_RESPONSE_OK) {
        g_string_printf(msg, _("Connection closed"));
        logconnection(msg);
        cldisconnect(msg, FALSE);
        g_string_free(msg, TRUE);
    }

    gtk_widget_destroy(closedialog);

    gtk_widget_set_sensitive(gui->window, 1);
    mainentry = g_object_get_data(G_OBJECT(gui->window), "mainentry");
    gtk_widget_grab_focus(GTK_WIDGET(mainentry));
}
