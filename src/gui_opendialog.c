/*
 * xdx - GTK+ DX-cluster client for amateur radio
 * Copyright (C) 2002-2003 Joop Stakenborg <pg4i@amsat.org>
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
 * gui_opendialog.c - dialog for opening a connection
 */

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
on_open_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  GtkWidget *opendialog, *hostnamecombo, *portcombo, *hbox, *stock, *table,
    *hostlabel, *portlabel, *mainentry;
  gint i, num, response;
  GList *node;
  gboolean result = FALSE;
  servertype *cluster;
  gchar *s;

  gtk_widget_set_sensitive (gui->window, 0);
  opendialog = gtk_dialog_new_with_buttons (_("xdx - open connection"),
					    GTK_WINDOW (gui->window),
					    GTK_DIALOG_MODAL |
					    GTK_DIALOG_DESTROY_WITH_PARENT,
					    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					    GTK_STOCK_OK,
					    GTK_RESPONSE_OK, NULL);

  hbox = gtk_hbox_new (FALSE, 8);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 8);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (opendialog)->vbox), hbox, FALSE,
		      FALSE, 0);
  stock =
    gtk_image_new_from_stock (GTK_STOCK_DIALOG_QUESTION,
			      GTK_ICON_SIZE_DIALOG);
  gtk_box_pack_start (GTK_BOX (hbox), stock, FALSE, FALSE, 0);
  table = gtk_table_new (2, 2, FALSE);
  gtk_table_set_row_spacings (GTK_TABLE (table), 4);
  gtk_table_set_col_spacings (GTK_TABLE (table), 4);
  gtk_box_pack_start (GTK_BOX (hbox), table, TRUE, TRUE, 0);
  hostlabel = gtk_label_new_with_mnemonic (_("_Hostname"));
  gtk_table_attach_defaults (GTK_TABLE (table), hostlabel, 0, 1, 0, 1);
  hostnamecombo = gtk_combo_box_entry_new_text ();
  if (gui->hostnamehistory)
  {
        num = g_list_length (gui->hostnamehistory);
        for (i = 0; i < num; i++)
        {
          s = g_list_nth_data (gui->hostnamehistory, i);
          gtk_combo_box_prepend_text (GTK_COMBO_BOX (hostnamecombo), s);
	}
  }
  gtk_table_attach_defaults (GTK_TABLE (table), hostnamecombo, 1, 2, 0, 1);
  portlabel = gtk_label_new_with_mnemonic (_("_Port"));
  gtk_table_attach_defaults (GTK_TABLE (table), portlabel, 0, 1, 1, 2);
  portcombo = gtk_combo_box_entry_new_text ();
  if (gui->porthistory)
  {
        num = g_list_length (gui->porthistory);
        for (i = 0; i < num; i++)
        {
          s = g_list_nth_data (gui->porthistory, i);
          gtk_combo_box_prepend_text (GTK_COMBO_BOX (portcombo), s);
	}
  }
  gtk_table_attach_defaults (GTK_TABLE (table), portcombo, 1, 2, 1, 2);
  gtk_widget_show_all (hbox);
  gtk_widget_set_sensitive (gui->window, 0);
  gtk_combo_box_set_active
    (GTK_COMBO_BOX (portcombo), g_list_length(gui->porthistory) - 1);
  gtk_combo_box_set_active
    (GTK_COMBO_BOX (hostnamecombo), g_list_length(gui->hostnamehistory) - 1);
  response = gtk_dialog_run (GTK_DIALOG (opendialog));

  if (response == GTK_RESPONSE_OK)
    {
      cluster = g_object_get_data(G_OBJECT(gui->window), "cluster");
      cluster->host = gtk_editable_get_chars
                (GTK_EDITABLE (GTK_BIN(hostnamecombo)->child), 0, -1);
      cluster->port = gtk_editable_get_chars
                (GTK_EDITABLE (GTK_BIN(portcombo)->child), 0, -1);
      if (!g_ascii_strcasecmp (cluster->host, ""))
        cluster->host = "localhost";
      if (!g_ascii_strcasecmp (cluster->port, ""))
        cluster->port = "8000";
      

      /* let's see if we need to add host/port to the history */
      node = NULL;
      node = g_list_find_custom (gui->hostnamehistory, cluster->host, 
	      (GCompareFunc)g_ascii_strncasecmp);
      if (!node)
        gui->hostnamehistory = g_list_prepend (gui->hostnamehistory, 
          g_strdup(cluster->host));
      else
        { /* add last connection to the top of the list */
          if (g_list_position(gui->hostnamehistory, node) != 0)
          {
            g_free(node->data);
            gui->hostnamehistory = 
              g_list_remove_link(gui->hostnamehistory, node);
            gui->hostnamehistory = 
              g_list_prepend(gui->hostnamehistory, g_strdup(cluster->host));
          }
        }

      if (g_list_length (gui->hostnamehistory) > HOSTNAMEHISTORY)
        gui->hostnamehistory = g_list_remove (gui->hostnamehistory, 
          g_list_last (gui->hostnamehistory)->data);
      node = NULL;
      node = g_list_find_custom (gui->porthistory, cluster->port,
	      (GCompareFunc)g_ascii_strncasecmp);
      if (!node)
        gui->porthistory = g_list_prepend (gui->porthistory, g_strdup(cluster->port));
      else
        {
          if (g_list_position(gui->porthistory, node) != 0)
          {
            g_free(node->data);
            gui->porthistory = 
              g_list_remove_link(gui->porthistory, node);
            gui->porthistory = 
              g_list_prepend(gui->porthistory, g_strdup(cluster->port));
          }
        }

      if (g_list_length (gui->porthistory) > PORTHISTORY)
        gui->porthistory = g_list_remove (gui->porthistory, 
          g_list_last (gui->porthistory)->data);

      menu_set_sensitive (gui->ui_manager, "/MainMenu/HostMenu/Open", FALSE);
      menu_set_sensitive (gui->ui_manager, "/MainMenu/HostMenu/Close", FALSE);

      result = clresolve (cluster);
    }

  gtk_widget_destroy (opendialog);
  gtk_widget_set_sensitive (gui->window, 1);
  mainentry = g_object_get_data (G_OBJECT (gui->window), "mainentry");
  gtk_widget_grab_focus (GTK_WIDGET (mainentry));

  if (!result)
  {
    menu_set_sensitive (gui->ui_manager, "/MainMenu/HostMenu/Open", TRUE);
    menu_set_sensitive (gui->ui_manager, "/MainMenu/HostMenu/Close", FALSE);
  }
    
}
