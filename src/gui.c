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
 * gui.c - where the main window and all the dialogs are created
 */

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "types.h"
#include "gui.h"
#include "preferences.h"
#include "net.h"
#include "utils.h"
#include "history.h"

#define HOSTNAMEHISTORY 10
#define PORTHISTORY     10
#define XDX_HOMEPAGE "http://people.debian.org/~pa3aba/xdx.html"

/**********************************MAIN WINDOW********************************/

static GtkItemFactoryEntry menu_items[] = {
  {N_("/_Program"), NULL, NULL, 0, "<Branch>"},
  {N_("/_Program/Quit"), "<control>Q", gtk_main_quit, 0, "<StockItem>",
   GTK_STOCK_QUIT},
  {N_("/_Host"), NULL, NULL, 0, "<Branch>"},
  {N_("/_Host/Open"), "<control>O", on_open_activate, 0, "<StockItem>",
   GTK_STOCK_OPEN},
  {N_("/_Host/Close"), "<control>C", on_close_activate, 0, "<StockItem>",
   GTK_STOCK_CLOSE},
  {N_("/H_elp"), NULL, NULL, 0, "<LastBranch>"},
  {N_("/H_elp/About"), "<control>H", on_about_activate, 0, "<StockItem>",
   GTK_STOCK_HELP},
};

guitype *new_gui(void)
{
        guitype *gui = g_new0(guitype, 1);
        gui->window = NULL;
	gui->item_factory = NULL;
	gui->hostnamehistory = NULL;
	gui->porthistory = NULL;
	gui->txhistory = NULL;
	gui->preferencesdir = NULL;
	gui->updown = 0;
	gui->txitem = 0;
        return(gui);
}

static gchar *menu_translate(const gchar *path, gpointer data)
{
  gchar *retval = gettext(path);
  return retval;
}


static void
get_main_menu (GtkWidget * window, GtkWidget ** menubar)
{
  GtkAccelGroup *accel_group;

  gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);

  accel_group = gtk_accel_group_new ();
  gui->item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>",
				       accel_group);
  gtk_item_factory_set_translate_func(gui->item_factory, menu_translate, NULL, NULL);
  gtk_item_factory_create_items (gui->item_factory, nmenu_items, menu_items, NULL);
  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
  if (menubar)
    *menubar = gtk_item_factory_get_widget (gui->item_factory, "<main>");
}

void
create_mainwindow (void)
{
  GtkWidget *mainvbox, *handlebox, *mainmenubar, *vpaned1, *clistscrolledwindow,
	*mainscrolledwindow, *maintext, *mainentry, *mainstatusbar, *treeview;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTextBuffer *buffer;
  GtkTreeStore *model;
  GdkPixbuf *icon = NULL;
  GError *err = NULL;
  GString *msg = g_string_new ("");
  servertype *cluster;

  gui = new_gui();
  gui->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  icon = gdk_pixbuf_new_from_file (PACKAGE_DATA_DIR "/pixmaps/xdx.png", &err);
  if (err)
  {
    g_string_printf (msg, _("Error loading icon: %s"), err->message);
    updatestatusbar (msg);
    g_string_free (msg, TRUE);
    g_error_free (err);
    err = NULL;
  }
  if (icon)
  {
  gtk_window_set_icon (GTK_WINDOW (gui->window), icon);
  gdk_pixbuf_unref (icon);
  }

  mainvbox = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (mainvbox);
  gtk_container_add (GTK_CONTAINER (gui->window), mainvbox);

  handlebox = gtk_handle_box_new ();
  gtk_widget_show (handlebox);
  gtk_box_pack_start (GTK_BOX (mainvbox), handlebox, FALSE, FALSE, 0);

  get_main_menu (gui->window, &mainmenubar);
  gtk_container_add (GTK_CONTAINER (handlebox), mainmenubar);
  gtk_widget_show (mainmenubar);

  vpaned1 = gtk_vpaned_new ();
  gtk_widget_show (vpaned1);
  gtk_box_pack_start (GTK_BOX (mainvbox), vpaned1, TRUE, TRUE, 0);

  clistscrolledwindow = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (clistscrolledwindow);
  gtk_paned_pack1 (GTK_PANED (vpaned1), clistscrolledwindow, TRUE, TRUE);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (clistscrolledwindow),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

  model = gtk_tree_store_new (N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING,
			      G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
			      G_TYPE_STRING);
  treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (model));
  g_object_unref (G_OBJECT (model));

  renderer = gtk_cell_renderer_text_new ();

  column =
    gtk_tree_view_column_new_with_attributes (_("Spotter"), renderer, "text",
					      FROM_COLUMN, NULL);
  gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(column), GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(column), TRUE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

  column =
    gtk_tree_view_column_new_with_attributes ("QRG", renderer, "text",
					      FREQ_COLUMN, NULL);
  gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(column), GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(column), TRUE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

  column =
    gtk_tree_view_column_new_with_attributes ("DX", renderer, "text",
					      DX_COLUMN, NULL);
  gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(column), GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(column), TRUE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

  column =
    gtk_tree_view_column_new_with_attributes (_("Remarks"), renderer, "text",
					      REM_COLUMN, NULL);
  gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(column), GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(column), TRUE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

  column =
    gtk_tree_view_column_new_with_attributes (_("Time"), renderer, "text",
					      TIME_COLUMN, NULL);
  gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(column), GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(column), TRUE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

  column =
    gtk_tree_view_column_new_with_attributes ("Info", renderer, "text",
					      INFO_COLUMN, NULL);
  gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(column), GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(column), TRUE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

  gtk_widget_show (treeview);
  gtk_container_add (GTK_CONTAINER (clistscrolledwindow), treeview);

  mainscrolledwindow = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (mainscrolledwindow);
  gtk_paned_pack2 (GTK_PANED (vpaned1), mainscrolledwindow, TRUE, TRUE);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (mainscrolledwindow),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

  maintext = gtk_text_view_new ();
  gtk_widget_show (maintext);
  gtk_container_add (GTK_CONTAINER (mainscrolledwindow), maintext);
  GTK_WIDGET_UNSET_FLAGS (maintext, GTK_CAN_FOCUS);
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (maintext));
  gtk_text_buffer_create_tag (buffer, "blue_foreground", "foreground", "blue",
			      NULL);
  gtk_text_buffer_create_tag (buffer, "red_foreground", "foreground", "red",
			      NULL);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (maintext), GTK_WRAP_WORD);

  mainentry = gtk_entry_new ();
  gtk_entry_set_max_length (GTK_ENTRY (mainentry), 80);
  gtk_widget_show (mainentry);
  gtk_box_pack_start (GTK_BOX (mainvbox), mainentry, FALSE, FALSE, 0);

  mainstatusbar = gtk_statusbar_new ();
  gtk_widget_show (mainstatusbar);
  gtk_box_pack_start (GTK_BOX (mainvbox), mainstatusbar, FALSE, FALSE, 0);

  g_signal_connect (G_OBJECT (gui->window), "destroy",
		    G_CALLBACK (on_mainwindow_destroy_event), NULL);
  g_signal_connect (G_OBJECT (gui->window), "delete_event",
		    G_CALLBACK (on_mainwindow_delete_event), NULL);
  g_signal_connect (G_OBJECT (mainentry), "activate",
		    G_CALLBACK (on_mainentry_activate), NULL);
  g_signal_connect (G_OBJECT (gui->window), "key_press_event",
		    G_CALLBACK (on_mainwindow_key_press_event), NULL);
  g_signal_connect (G_OBJECT(maintext), "motion_notify_event",
		    G_CALLBACK (on_maintext_motion_notify_event), NULL);

  g_object_set_data (G_OBJECT (gui->window), "maintext", maintext);
  g_object_set_data (G_OBJECT (gui->window), "treeview", treeview);
  g_object_set_data (G_OBJECT (gui->window), "mainstatusbar", mainstatusbar);
  g_object_set_data (G_OBJECT (gui->window), "mainentry", mainentry);
  g_object_set_data (G_OBJECT (gui->window), "model", model);
  g_object_set_data (G_OBJECT (gui->window), "buffer", buffer);
  g_object_set_data (G_OBJECT (gui->window), "vpaned1", vpaned1);

  cluster = new_cluster();
  g_object_set_data(G_OBJECT (gui->window), "cluster", cluster);

  gtk_widget_grab_focus (mainentry);

  return;
}

/*
 * hit <enter> in the entry widget
 */

void
on_mainentry_activate (GtkEditable * editable, gpointer user_data)
{
  gchar *entry;
  GString *str = g_string_new ("");
  GtkWidget *mainentry;

  mainentry = g_object_get_data (G_OBJECT (gui->window), "mainentry");
  entry = gtk_editable_get_chars (GTK_EDITABLE (mainentry), 0, -1);
  str = g_string_new(entry);
  tx (str);
  g_string_free(str, TRUE);
  g_free (entry);
  gtk_editable_delete_text (GTK_EDITABLE (mainentry), 0, -1);
  gtk_widget_grab_focus (GTK_WIDGET (mainentry));
}

/*
 * called at program exit
 */

gboolean
on_mainwindow_delete_event (GtkWidget * widget, GdkEvent * event,
			    gpointer user_data)
{
  GtkWidget *vpaned1, *treeview;
  GList * columns;
  gint i, length;
  servertype *cluster;
  GString *w = g_string_new("");

  cluster = g_object_get_data(G_OBJECT(widget), "cluster");
  if (cluster->sockethandle != -1)
    cldisconnect(FALSE);

  vpaned1 = g_object_get_data (G_OBJECT(gui->window), "vpaned1");
  preferences.panedpos = gtk_paned_get_position(GTK_PANED(vpaned1));

  gtk_window_get_position(GTK_WINDOW(gui->window), &preferences.x, &preferences.y);
  gtk_window_get_size(GTK_WINDOW(gui->window), &preferences.width, &preferences.height);

  treeview = g_object_get_data (G_OBJECT(gui->window), "treeview");
  columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
  length = g_list_length(columns);
  for (i = 0; i < length; i++) 
      g_string_append_printf(w, "%d,", gtk_tree_view_column_get_width
        (gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), i)));
  g_list_free(columns);
  preferences.columnwidths = g_strdup(w->str);
  g_string_free(w, TRUE);

  savehistory ();
  savepreferences ();

  return FALSE;
}

gboolean
on_mainwindow_destroy_event (GtkWidget * widget, GdkEvent * event,
			    gpointer user_data)
{
  gint i, n;
  GList *link;
  servertype *cluster;

  gui->window = NULL;
  gui->item_factory = NULL;

  cluster = g_object_get_data(G_OBJECT(widget), "cluster");
  g_free(cluster);

  g_free(preferences.columnwidths);

  link = gui->hostnamehistory;
  while (link)
	{
		g_free(link->data);
	        link = link->next;
	}
      g_list_free(gui->hostnamehistory);
      gui->hostnamehistory = NULL;
	
  link = gui->porthistory;
  while (link)
	{
		g_free(link->data);
	        link = link->next;
	}
      g_list_free(gui->porthistory);
      gui->porthistory = NULL;

  link = gui->txhistory;
  while (link)
	{
		g_free(link->data);
	        link = link->next;
	}
      g_list_free(gui->txhistory);
      gui->txhistory = NULL;
	
  g_free(gui->preferencesdir);
  gui->preferencesdir = NULL;
  g_free(gui);

  gtk_main_quit ();

  return FALSE;
}


gboolean on_mainwindow_key_press_event(GtkWidget *widget, GdkEventKey *event,
					 gpointer user_data)
{

  GtkWidget *mainentry;

  mainentry = g_object_get_data (G_OBJECT (gui->window), "mainentry");
  if (GTK_WIDGET_HAS_FOCUS(mainentry))
  {
    switch (event->keyval)
    {
      case GDK_Up:
        gtk_signal_emit_stop_by_name(GTK_OBJECT(widget), "key_press_event");
        tx_previous();
      break;
      case GDK_Down:
        gtk_signal_emit_stop_by_name(GTK_OBJECT(widget), "key_press_event");
        tx_next();
      break;
      default:
      break;
    }
  }
  return FALSE;
}

gboolean on_maintext_motion_notify_event (GtkWidget * widget, GdkEvent * event,
                                         gpointer user_data)
{

/* TODO: catch uri's and click on them */	

}
/********************************* MENUS *************************************/

void
on_open_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  GtkWidget *opendialog, *hostnamecombo, *portcombo, *hbox, *stock, *table,
    *hostlabel, *portlabel, *mainentry;
  gint response;
  GList *node;
  gboolean result;
  servertype *cluster;

  gtk_widget_set_sensitive (gui->window, 0);
  opendialog = gtk_dialog_new_with_buttons (_("xdx - open connection"),
					    GTK_WINDOW (gui->window),
					    GTK_DIALOG_MODAL |
					    GTK_DIALOG_DESTROY_WITH_PARENT,
					    GTK_STOCK_OK, GTK_RESPONSE_OK,
					    GTK_STOCK_CANCEL,
					    GTK_RESPONSE_CANCEL, NULL);

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
  hostnamecombo = gtk_combo_new ();
  if (gui->hostnamehistory)
    gtk_combo_set_popdown_strings (GTK_COMBO (hostnamecombo),
				   gui->hostnamehistory);
  gtk_table_attach_defaults (GTK_TABLE (table), hostnamecombo, 1, 2, 0, 1);
  gtk_label_set_mnemonic_widget (GTK_LABEL (hostlabel), GTK_COMBO(hostnamecombo)->entry);
  portlabel = gtk_label_new_with_mnemonic (_("_Port"));
  gtk_table_attach_defaults (GTK_TABLE (table), portlabel, 0, 1, 1, 2);
  portcombo = gtk_combo_new ();
  if (gui->porthistory)
    gtk_combo_set_popdown_strings (GTK_COMBO (portcombo), gui->porthistory);
  gtk_table_attach_defaults (GTK_TABLE (table), portcombo, 1, 2, 1, 2);
  gtk_label_set_mnemonic_widget (GTK_LABEL (portlabel), GTK_COMBO(portcombo)->entry);
  gtk_widget_show_all (hbox);
  gtk_widget_set_sensitive (gui->window, 0);
  response = gtk_dialog_run (GTK_DIALOG (opendialog));

  if (response == GTK_RESPONSE_OK)
    {
      cluster = g_object_get_data(G_OBJECT(gui->window), "cluster");
      cluster->host = gtk_editable_get_chars (GTK_EDITABLE
				      (GTK_COMBO (hostnamecombo)->entry), 0, -1);
      cluster->port = gtk_editable_get_chars (GTK_EDITABLE
				      (GTK_COMBO (portcombo)->entry), 0, -1);
      if (!g_ascii_strcasecmp (cluster->host, ""))
	cluster->host = "localhost";
      if (!g_ascii_strcasecmp (cluster->port, ""))
	cluster->port = "8000";
      

      /* let's see if we need to add host/port to the history */
      node = NULL;
      node = g_list_find_custom (gui->hostnamehistory, cluster->host, 
	      (GCompareFunc)g_ascii_strncasecmp);
      if (!node)
	gui->hostnamehistory = g_list_prepend (gui->hostnamehistory, g_strdup(cluster->host));

      if (g_list_length (gui->hostnamehistory) > HOSTNAMEHISTORY)
	gui->hostnamehistory =
	  g_list_remove (gui->hostnamehistory,
			 g_list_last (gui->hostnamehistory)->data);
      
      node = NULL;
      node = g_list_find_custom (gui->porthistory, cluster->port,
	      (GCompareFunc)g_ascii_strncasecmp);
      if (!node)
	gui->porthistory = g_list_prepend (gui->porthistory, g_strdup(cluster->port));

      if (g_list_length (gui->porthistory) > PORTHISTORY)
	gui->porthistory =
	  g_list_remove (gui->porthistory, g_list_last (gui->porthistory)->data);

      menu_set_sensitive (gui->item_factory, "/Host/Open", FALSE);
      menu_set_sensitive (gui->item_factory, "/Host/Close", FALSE);

      result = clresolve (cluster);

    }

  gtk_widget_destroy (opendialog);
  gtk_widget_set_sensitive (gui->window, 1);
  mainentry = g_object_get_data (G_OBJECT (gui->window), "mainentry");
  gtk_widget_grab_focus (GTK_WIDGET (mainentry));

  if (!result)
  {
    menu_set_sensitive (gui->item_factory, "/Host/Open", TRUE);
    menu_set_sensitive (gui->item_factory, "/Host/Close", FALSE);
  }
    
}


void
on_close_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  GtkWidget *closedialog, *closelabel, *hbox, *stock, *mainentry;
  GString *labeltext = g_string_new ("");
  gint response;
  servertype *cluster;

  gtk_widget_set_sensitive (gui->window, 0);
  closedialog = gtk_dialog_new_with_buttons (_("xdx - close connection"),
					     GTK_WINDOW (gui->window),
					     GTK_DIALOG_MODAL |
					     GTK_DIALOG_DESTROY_WITH_PARENT,
					     GTK_STOCK_OK, GTK_RESPONSE_OK,
					     GTK_STOCK_CANCEL,
					     GTK_RESPONSE_CANCEL, NULL);
  hbox = gtk_hbox_new (FALSE, 8);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 8);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (closedialog)->vbox), hbox, FALSE,
		      FALSE, 0);
  stock =
    gtk_image_new_from_stock (GTK_STOCK_DIALOG_QUESTION,
			      GTK_ICON_SIZE_DIALOG);
  gtk_box_pack_start (GTK_BOX (hbox), stock, FALSE, FALSE, 0);

  cluster = (servertype *)g_object_get_data(G_OBJECT(gui->window), "cluster");
  g_string_printf (labeltext, _("Close connection to %s ?"),
		   cluster->host);
  closelabel = gtk_label_new_with_mnemonic (labeltext->str);
  g_string_free (labeltext, TRUE);

  gtk_box_pack_start (GTK_BOX (hbox), closelabel, TRUE, TRUE, 0);
  gtk_widget_show_all (hbox);
  response = gtk_dialog_run (GTK_DIALOG (closedialog));

  if (response == GTK_RESPONSE_OK)
    cldisconnect (TRUE);

  gtk_widget_destroy (closedialog);

  gtk_widget_set_sensitive (gui->window, 1);
  mainentry = g_object_get_data (G_OBJECT (gui->window), "mainentry");
  gtk_widget_grab_focus (GTK_WIDGET (mainentry));
}

/*
 * the link is clicked in the about dialog
 */

static gboolean on_weblink_button_press_event(GtkWidget *widget, GdkEventButton *event)
{
  static gchar *cmdline = "mozilla -remote openURL\\(%s\\)";
  gchar buf[1024];
  gint result;
  GString *msg = g_string_new ("");

  g_snprintf(buf, sizeof(buf), cmdline, XDX_HOMEPAGE);
  result = system(buf);
  if (result != 0)
  {
    g_string_printf (msg, _("No running mozilla window found"));
    updatestatusbar (msg);
    g_string_free (msg, TRUE);
  }
  return FALSE;
}


static void event_destroy(gpointer data)
{
  GtkWidget *aboutdialog = (GtkWidget *)data;
  
  aboutdialog = NULL;
}


void
on_about_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  GtkWidget *aboutdialog, *vbox, *stock, *aboutlabel, *separator, *eventbox;
  GString *labeltext = g_string_new ("");
  PangoFontDescription *font;
  GdkCursor *cursor;

  aboutdialog = gtk_dialog_new_with_buttons (_("xdx - about"), NULL, 0,
					     GTK_STOCK_OK, GTK_RESPONSE_OK,
					     NULL);

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

  g_string_printf (labeltext, "Copyright (C) 2002, Joop Stakenborg <pa4tu@amsat.org>");
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
