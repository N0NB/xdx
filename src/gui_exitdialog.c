/*
 * xdx - GTK+ DX-cluster client for amateur radio
 * Copyright (C) 2002-2006 Joop Stakenborg <pg4i@amsat.org>
 * Copyright (C) 2014 Nate Bargmann <n0nb@n0nb.us>
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
 * gui_exitdialog.c - dialog for exiting the program
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
#include "gui_exitdialog.h"


/*
 * called from the menu or from the window manager close button
 * on the main window.
 */
gboolean
on_exit_dialog(GtkMenuItem   *menuitem,
               gpointer       user_data)
{
    GtkWidget *exitdialog, *closelabel, *hbox, *stock, *mainentry;
    GString *labeltext = g_string_new("");
    gint response;

    gtk_widget_set_sensitive(gui->window, 0);
    exitdialog = gtk_dialog_new_with_buttons(_("Exit"),
                    GTK_WINDOW(gui->window),
                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_STOCK_NO,
                    GTK_RESPONSE_NO,
                    GTK_STOCK_YES,
                    GTK_RESPONSE_YES,
                    NULL);

    hbox = gtk_hbox_new(FALSE, 8);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 8);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(exitdialog)->vbox), hbox, FALSE,
                       FALSE, 0);
    stock = gtk_image_new_from_stock(GTK_STOCK_DIALOG_QUESTION,
                                     GTK_ICON_SIZE_DIALOG);
    gtk_box_pack_start(GTK_BOX(hbox), stock, FALSE, FALSE, 0);

    g_string_printf(labeltext, _("Exit "PACKAGE_NAME"?"));
    closelabel = gtk_label_new_with_mnemonic(labeltext->str);
    g_string_free(labeltext, TRUE);

    gtk_box_pack_start(GTK_BOX(hbox), closelabel, TRUE, TRUE, 0);
    gtk_widget_show_all(hbox);
    response = gtk_dialog_run(GTK_DIALOG(exitdialog));

    if (response == GTK_RESPONSE_YES) {
        on_quit_activate(menuitem, user_data);

        return FALSE;
    }

    gtk_widget_destroy(exitdialog);

    gtk_widget_set_sensitive(gui->window, TRUE);
    mainentry = g_object_get_data(G_OBJECT(gui->window), "mainentry");
    gtk_widget_grab_focus(GTK_WIDGET(mainentry));

    return TRUE;
}
