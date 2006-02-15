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
 * gui.c - where the main window is created
 */

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdlib.h>

#include "types.h"
#include "gui.h"
#include "preferences.h"
#include "net.h"
#include "utils.h"
#include "history.h"
#include "hyperlink.h"
#include "gui_opendialog.h"
#include "gui_logdialog.h"
#include "gui_closedialog.h"
#include "gui_aboutdialog.h"
#include "gui_settingsdialog.h"
#include "gui_manualdialog.h"

extern preferencestype preferences;

/**********************************MAIN WINDOW********************************/
guitype *new_gui(void)
{
  guitype *gui = g_new0(guitype, 1);
  gui->window = NULL;
  gui->action_group = NULL;
  gui->ui_manager = NULL;
  gui->hostnamehistory = NULL;
  gui->porthistory = NULL;
  gui->txhistory = NULL;
  gui->preferencesdir = NULL;
  gui->updown = 0;
  gui->txitem = 0;
  gui->statusbartimer = -1;
  gui->statusbarmessage = NULL;
  return(gui);
}

static void
get_main_menu (GtkWidget *window, GtkWidget **menubar)
{
  GtkAccelGroup *accel_group;


static GtkActionEntry entries[] = {
	{ "ProgramMenu", NULL, N_("_Program") },
	{ "HostMenu", NULL, N_("_Host") },
	{ "SettingsMenu", NULL, N_("_Settings") },
	{ "HelpMenu", NULL, N_("H_elp") },
	{ "HighMenu", NULL, N_("Highlights") },
	
	{ "Quit", GTK_STOCK_QUIT, N_("Quit"),
		"<control>Q", "Quit Program", G_CALLBACK(on_quit_activate) },
	{ "Open", GTK_STOCK_CONNECT, N_("Connect..."),
		"<control>O", "Open Connection", G_CALLBACK(on_open_activate) },
	{ "Close", GTK_STOCK_DISCONNECT, N_("Disconnect"),
		"<control>C", "Close Connection", G_CALLBACK(on_close_activate) },
	{ "ShowLog", GTK_STOCK_OPEN, N_("Connection Log"),
		"<control>L", "Show connection log", G_CALLBACK(on_log_activate) },
	{ "Preferences", GTK_STOCK_PREFERENCES, N_("Preferences..."),
		"<control>P", "Settings for xdx", G_CALLBACK(on_settings_activate) },
	{ "Manual", GTK_STOCK_HELP, N_("Manual"),
		"F1", "Read the manual", G_CALLBACK(on_manual_activate) },
	{ "About", GTK_STOCK_HELP, N_("About"),
		"<control>A", "About xdx", G_CALLBACK(on_about_activate) },
};


static GtkToggleActionEntry toggle_entries[] =
{
  { "Reconnect", NULL, N_("Auto Reconnect"), "<control>R", "Auto Reconnect on/off",
   G_CALLBACK(on_reconnect_activate) },
  { "Sidebar", NULL, N_("Chat sidebar"), "F4", "Chat sidebar on/off",
   G_CALLBACK(on_sidebar_activate) },
};

static const char *ui_description =
"<ui>"
"  <menubar name='MainMenu'>"
"    <menu action='ProgramMenu'>"
"      <menuitem action='ShowLog'/>"
"      <menuitem action='Quit'/>"
"    </menu>"
"    <menu action='HostMenu'>"
"      <menuitem action='Open'/>"
"      <menuitem action='Close'/>"
"    </menu>"
"    <menu action='SettingsMenu'>"
"      <menuitem action='Reconnect'/>"
"      <menuitem action='Sidebar'/>"
"      <separator name='sep2'/>"
"      <menuitem action='Preferences'/>"
"    </menu>"
"    <menu action='HelpMenu'>"
"      <menuitem action='Manual'/>"
"      <menuitem action='About'/>"
"    </menu>"
"  </menubar>"
"</ui>";

  accel_group = gtk_accel_group_new ();
  gui->action_group = gtk_action_group_new ("MenuActions");
  gtk_action_group_set_translation_domain (gui->action_group, PACKAGE);
  gtk_action_group_add_actions
    (gui->action_group, entries, G_N_ELEMENTS (entries), window);
  gtk_action_group_add_toggle_actions
    (gui->action_group, toggle_entries, G_N_ELEMENTS (toggle_entries), window);
  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
  gui->ui_manager = gtk_ui_manager_new ();
  gtk_ui_manager_insert_action_group (gui->ui_manager, gui->action_group, 0);
  accel_group = gtk_ui_manager_get_accel_group (gui->ui_manager);
  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
  gtk_ui_manager_add_ui_from_string (gui->ui_manager, ui_description, -1, NULL);
  *menubar = gtk_ui_manager_get_widget (gui->ui_manager, "/MainMenu");
}

void
create_mainwindow (void)
{
  GtkWidget *mainvbox, *handlebox, *mainmenubar, *vpaned, *clistscrolledwindow,
    *mainscrolledwindow, *maintext, *mainentry, *mainstatusbar, *treeview,
    *frame, *chathbox, *highvbox, *hbox, *highframe,
    *highentry1, *highentry2, *highentry3, *highentry4, *highentry5,
    *highentry6, *highentry7, *highentry8, *highcheck1, *highcheck2,
    *highcheck3, *highcheck4, *highcheck5, *highcheck6, *highcheck7,
    *highcheck8;
  GtkCellRenderer *renderer, *boldrenderer;
  GtkTreeViewColumn *column;
  GtkTextBuffer *buffer, *entrybuffer;
  GtkTreeStore *model;
  GdkPixbuf *icon = NULL;
  GError *err = NULL;
  servertype *cluster;
  PangoFontDescription *font_description;
  gint pango_size;
  GtkTooltips *tooltips;
  GtkAccelGroup *key_toggle;
  gchar *str;

  gui = new_gui();
  gui->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  icon = gdk_pixbuf_new_from_file (PACKAGE_DATA_DIR "/pixmaps/xdx.png", &err);
  if (err)
  {
    g_warning (_("Error loading icon: %s"), err->message);
    g_error_free (err);
    err = NULL;
  }
  if (icon)
  {
  gtk_window_set_icon (GTK_WINDOW (gui->window), icon);
  g_object_unref (icon);
  }

  mainvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (gui->window), mainvbox);

  handlebox = gtk_handle_box_new ();
  gtk_box_pack_start (GTK_BOX (mainvbox), handlebox, FALSE, TRUE, 0);
  get_main_menu (gui->window, &mainmenubar);
  gtk_container_add (GTK_CONTAINER (handlebox), mainmenubar);

  clistscrolledwindow = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (clistscrolledwindow),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

  model = gtk_tree_store_new (N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING,
			      G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
			      G_TYPE_STRING);
  treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (model));
  g_object_unref (G_OBJECT (model));

  renderer = gtk_cell_renderer_text_new ();
  boldrenderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (boldrenderer), "weight", "bold", NULL);

  column =
    gtk_tree_view_column_new_with_attributes (_("Spotter"), renderer, "text",
					      FROM_COLUMN, NULL);
  gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN(column), 
    GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_resizable (GTK_TREE_VIEW_COLUMN(column), TRUE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

  column =
    gtk_tree_view_column_new_with_attributes ("QRG", renderer, "text",
					      FREQ_COLUMN, NULL);
  gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN(column), 
    GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_resizable (GTK_TREE_VIEW_COLUMN(column), TRUE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

  column =
    gtk_tree_view_column_new_with_attributes ("DX", boldrenderer, "text",
					      DX_COLUMN, NULL);
  gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(column), 
    GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(column), TRUE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

  column =
    gtk_tree_view_column_new_with_attributes (_("Remarks"), renderer, "text",
					      REM_COLUMN, NULL);
  gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(column), 
    GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(column), TRUE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

  column =
    gtk_tree_view_column_new_with_attributes (_("Time"), renderer, "text",
					      TIME_COLUMN, NULL);
  gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(column), 
    GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(column), TRUE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

  column =
    gtk_tree_view_column_new_with_attributes (_("Info"), renderer, "text",
					      INFO_COLUMN, NULL);
  gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(column), 
    GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(column), TRUE);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

  gtk_container_add (GTK_CONTAINER (clistscrolledwindow), treeview);

  chathbox = gtk_hbox_new (FALSE, 0);
  mainscrolledwindow = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (mainscrolledwindow),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

  maintext = gtk_text_view_new ();
  gtk_container_add (GTK_CONTAINER (mainscrolledwindow), maintext);
  gtk_text_view_set_editable (GTK_TEXT_VIEW (maintext), FALSE);
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (maintext));
  gtk_text_buffer_create_tag (buffer, "wwv", "foreground", "darkgreen",
			      NULL);
  gtk_text_buffer_create_tag (buffer, "wx", "foreground", "magenta",
			      NULL);
  gtk_text_buffer_create_tag (buffer, "sent", "foreground", "red",
			      NULL);
  gtk_text_buffer_create_tag (buffer, "prompt", "foreground", "darkblue",
			      NULL);
  gtk_text_buffer_create_tag (buffer, "call", "foreground", "darkblue",
			      "weight", PANGO_WEIGHT_BOLD, NULL);
  gtk_text_buffer_create_tag (buffer, "url", "foreground", "blue", 
				    "underline", PANGO_UNDERLINE_SINGLE, NULL);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (maintext), GTK_WRAP_WORD);
  gtk_box_pack_start (GTK_BOX (chathbox), mainscrolledwindow, TRUE, TRUE, 0);

  highframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (chathbox), highframe, FALSE, FALSE, 0);
  highvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (highframe), highvbox);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (highvbox), hbox);
  highentry1 = gtk_entry_new ();
  gtk_widget_set_size_request (highentry1, 80, -1);
  gtk_box_pack_start (GTK_BOX (hbox), highentry1, FALSE, FALSE, 0);
  highcheck1 = gtk_check_button_new ();
  gtk_box_pack_start (GTK_BOX (hbox), highcheck1, FALSE, FALSE, 0);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (highvbox), hbox);
  highentry2 = gtk_entry_new ();
  gtk_widget_set_size_request (highentry2, 80, -1);
  gtk_box_pack_start (GTK_BOX (hbox), highentry2, FALSE, FALSE, 0);
  highcheck2 = gtk_check_button_new ();
  gtk_box_pack_start (GTK_BOX (hbox), highcheck2, FALSE, FALSE, 0);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (highvbox), hbox);
  highentry3 = gtk_entry_new ();
  gtk_widget_set_size_request (highentry3, 80, -1);
  gtk_box_pack_start (GTK_BOX (hbox), highentry3, FALSE, FALSE, 0);
  highcheck3 = gtk_check_button_new ();
  gtk_box_pack_start (GTK_BOX (hbox), highcheck3, FALSE, FALSE, 0);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (highvbox), hbox);
  highentry4 = gtk_entry_new ();
  gtk_widget_set_size_request (highentry4, 80, -1);
  gtk_box_pack_start (GTK_BOX (hbox), highentry4, FALSE, FALSE, 0);
  highcheck4 = gtk_check_button_new ();
  gtk_box_pack_start (GTK_BOX (hbox), highcheck4, FALSE, FALSE, 0);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (highvbox), hbox);
  highentry5 = gtk_entry_new ();
  gtk_widget_set_size_request (highentry5, 80, -1);
  gtk_box_pack_start (GTK_BOX (hbox), highentry5, FALSE, FALSE, 0);
  highcheck5 = gtk_check_button_new ();
  gtk_box_pack_start (GTK_BOX (hbox), highcheck5, FALSE, FALSE, 0);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (highvbox), hbox);
  highentry6 = gtk_entry_new ();
  gtk_widget_set_size_request (highentry6, 80, -1);
  gtk_box_pack_start (GTK_BOX (hbox), highentry6, FALSE, FALSE, 0);
  highcheck6 = gtk_check_button_new ();
  gtk_box_pack_start (GTK_BOX (hbox), highcheck6, FALSE, FALSE, 0);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (highvbox), hbox);
  highentry7 = gtk_entry_new ();
  gtk_widget_set_size_request (highentry7, 80, -1);
  gtk_box_pack_start (GTK_BOX (hbox), highentry7, FALSE, FALSE, 0);
  highcheck7 = gtk_check_button_new ();
  gtk_box_pack_start (GTK_BOX (hbox), highcheck7, FALSE, FALSE, 0);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (highvbox), hbox);
  highentry8 = gtk_entry_new ();
  gtk_widget_set_size_request (highentry8, 80, -1);
  gtk_box_pack_start (GTK_BOX (hbox), highentry8, FALSE, FALSE, 0);
  highcheck8 = gtk_check_button_new ();
  gtk_box_pack_start (GTK_BOX (hbox), highcheck8, FALSE, FALSE, 0);

  key_toggle = gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(gui->window), key_toggle);
  gtk_widget_add_accelerator(highcheck1, "clicked", key_toggle, GDK_1, GDK_CONTROL_MASK, 0);
  gtk_widget_add_accelerator(highcheck2, "clicked", key_toggle, GDK_2, GDK_CONTROL_MASK, 0);
  gtk_widget_add_accelerator(highcheck3, "clicked", key_toggle, GDK_3, GDK_CONTROL_MASK, 0);
  gtk_widget_add_accelerator(highcheck4, "clicked", key_toggle, GDK_4, GDK_CONTROL_MASK, 0);
  gtk_widget_add_accelerator(highcheck5, "clicked", key_toggle, GDK_5, GDK_CONTROL_MASK, 0);
  gtk_widget_add_accelerator(highcheck6, "clicked", key_toggle, GDK_6, GDK_CONTROL_MASK, 0);
  gtk_widget_add_accelerator(highcheck7, "clicked", key_toggle, GDK_7, GDK_CONTROL_MASK, 0);
  gtk_widget_add_accelerator(highcheck8, "clicked", key_toggle, GDK_8, GDK_CONTROL_MASK, 0);

  vpaned = gtk_vpaned_new ();
  gtk_paned_add1 (GTK_PANED (vpaned), clistscrolledwindow);
  gtk_paned_add2 (GTK_PANED (vpaned), chathbox);
  gtk_box_pack_start (GTK_BOX (mainvbox), vpaned, TRUE, TRUE, 0);
  tooltips = gtk_tooltips_new ();

  gtk_tooltips_set_tip(tooltips, highentry1, _("Enter a word to highlight"), NULL);
  gtk_tooltips_set_tip(tooltips, highentry2, _("Enter a word to highlight"), NULL);
  gtk_tooltips_set_tip(tooltips, highentry3, _("Enter a word to highlight"), NULL);
  gtk_tooltips_set_tip(tooltips, highentry4, _("Enter a word to highlight"), NULL);
  gtk_tooltips_set_tip(tooltips, highentry5, _("Enter a word to highlight"), NULL);
  gtk_tooltips_set_tip(tooltips, highentry6, _("Enter a word to highlight"), NULL);
  gtk_tooltips_set_tip(tooltips, highentry7, _("Enter a word to highlight"), NULL);
  gtk_tooltips_set_tip(tooltips, highentry8, _("Enter a word to highlight"), NULL);
  str = g_strdup_printf (_("Enable/disable highlight [Ctrl+%d]"), 1);
  gtk_tooltips_set_tip(tooltips, highcheck1, str, NULL);
  str = g_strdup_printf (_("Enable/disable highlight [Ctrl+%d]"), 2);
  gtk_tooltips_set_tip(tooltips, highcheck2, str, NULL);
  str = g_strdup_printf (_("Enable/disable highlight [Ctrl+%d]"), 3);
  gtk_tooltips_set_tip(tooltips, highcheck3, str, NULL);
  str = g_strdup_printf (_("Enable/disable highlight [Ctrl+%d]"), 4);
  gtk_tooltips_set_tip(tooltips, highcheck4, str, NULL);
  str = g_strdup_printf (_("Enable/disable highlight [Ctrl+%d]"), 5);
  gtk_tooltips_set_tip(tooltips, highcheck5, str, NULL);
  str = g_strdup_printf (_("Enable/disable highlight [Ctrl+%d]"), 6);
  gtk_tooltips_set_tip(tooltips, highcheck6, str, NULL);
  str = g_strdup_printf (_("Enable/disable highlight [Ctrl+%d]"), 7);
  gtk_tooltips_set_tip(tooltips, highcheck7, str, NULL);
  str = g_strdup_printf (_("Enable/disable highlight [Ctrl+%d]"), 8);
  gtk_tooltips_set_tip(tooltips, highcheck8, str, NULL);

  mainentry = gtk_text_view_new ();
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(mainentry), GTK_WRAP_WORD);
  frame = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (mainvbox), frame, FALSE, TRUE, 0);
  gtk_container_add (GTK_CONTAINER (frame), mainentry);

  /* height of the frame is 2 times font size */
  font_description = pango_font_description_copy
    (gtk_widget_get_style (GTK_WIDGET(mainentry))->font_desc);
  pango_size = pango_font_description_get_size (font_description);
  gtk_widget_set_size_request (frame, -1, 4 * PANGO_PIXELS(pango_size));

  mainstatusbar = gtk_statusbar_new ();
  gtk_box_pack_start (GTK_BOX (mainvbox), mainstatusbar, FALSE, TRUE, 0);

  g_signal_connect (G_OBJECT (gui->window), "destroy",
		    G_CALLBACK (on_mainwindow_destroy_event), NULL);
  g_signal_connect (G_OBJECT (gui->window), "delete_event",
		    G_CALLBACK (on_mainwindow_delete_event), NULL);
  entrybuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (mainentry));
  g_signal_connect (G_OBJECT (entrybuffer), "changed",
		    G_CALLBACK (on_mainentry_activate), NULL);
  g_signal_connect (G_OBJECT (gui->window), "key_press_event",
		    G_CALLBACK (on_mainwindow_key_press_event), NULL);
  g_signal_connect (G_OBJECT(maintext), "motion_notify_event",
		    G_CALLBACK (on_maintext_motion_notify_event), NULL);
  g_signal_connect (G_OBJECT (maintext), "event-after",
        G_CALLBACK (on_maintext_event_after), NULL);
  g_signal_connect (G_OBJECT (treeview), "button-press-event",
        G_CALLBACK (double_click), NULL);

  g_signal_connect ((gpointer) highcheck1, "toggled",
        G_CALLBACK (on_highcheck_toggled), GINT_TO_POINTER(1));
  g_signal_connect ((gpointer) highcheck2, "toggled",
        G_CALLBACK (on_highcheck_toggled), GINT_TO_POINTER(2));
  g_signal_connect ((gpointer) highcheck3, "toggled",
        G_CALLBACK (on_highcheck_toggled), GINT_TO_POINTER(3));
  g_signal_connect ((gpointer) highcheck4, "toggled",
        G_CALLBACK (on_highcheck_toggled), GINT_TO_POINTER(4));
  g_signal_connect ((gpointer) highcheck5, "toggled",
        G_CALLBACK (on_highcheck_toggled), GINT_TO_POINTER(5));
  g_signal_connect ((gpointer) highcheck6, "toggled",
        G_CALLBACK (on_highcheck_toggled), GINT_TO_POINTER(6));
  g_signal_connect ((gpointer) highcheck7, "toggled",
        G_CALLBACK (on_highcheck_toggled), GINT_TO_POINTER(7));
  g_signal_connect ((gpointer) highcheck8, "toggled",
        G_CALLBACK (on_highcheck_toggled), GINT_TO_POINTER(8));

  g_signal_connect (G_OBJECT (highentry1), "changed",
        G_CALLBACK (on_highentry_changed), GINT_TO_POINTER(1));
  g_signal_connect (G_OBJECT (highentry2), "changed",
        G_CALLBACK (on_highentry_changed), GINT_TO_POINTER(2));
  g_signal_connect (G_OBJECT (highentry3), "changed",
        G_CALLBACK (on_highentry_changed), GINT_TO_POINTER(3));
  g_signal_connect (G_OBJECT (highentry4), "changed",
        G_CALLBACK (on_highentry_changed), GINT_TO_POINTER(4));
  g_signal_connect (G_OBJECT (highentry5), "changed",
        G_CALLBACK (on_highentry_changed), GINT_TO_POINTER(5));
  g_signal_connect (G_OBJECT (highentry6), "changed",
        G_CALLBACK (on_highentry_changed), GINT_TO_POINTER(6));
  g_signal_connect (G_OBJECT (highentry7), "changed",
        G_CALLBACK (on_highentry_changed), GINT_TO_POINTER(7));
  g_signal_connect (G_OBJECT (highentry8), "changed",
        G_CALLBACK (on_highentry_changed), GINT_TO_POINTER(8));

  g_signal_connect (G_OBJECT (highentry1), "button_press_event",
        G_CALLBACK (on_highentry_clicked), NULL);
  g_signal_connect (G_OBJECT (highentry2), "button_press_event",
        G_CALLBACK (on_highentry_clicked), NULL);
  g_signal_connect (G_OBJECT (highentry3), "button_press_event",
        G_CALLBACK (on_highentry_clicked), NULL);
  g_signal_connect (G_OBJECT (highentry4), "button_press_event",
        G_CALLBACK (on_highentry_clicked), NULL);
  g_signal_connect (G_OBJECT (highentry5), "button_press_event",
        G_CALLBACK (on_highentry_clicked), NULL);
  g_signal_connect (G_OBJECT (highentry6), "button_press_event",
        G_CALLBACK (on_highentry_clicked), NULL);
  g_signal_connect (G_OBJECT (highentry7), "button_press_event",
        G_CALLBACK (on_highentry_clicked), NULL);
  g_signal_connect (G_OBJECT (highentry8), "button_press_event",
        G_CALLBACK (on_highentry_clicked), NULL);

  g_object_set_data (G_OBJECT (gui->window), "maintext", maintext);
  g_object_set_data (G_OBJECT (gui->window), "treeview", treeview);
  g_object_set_data (G_OBJECT (gui->window), "mainstatusbar", mainstatusbar);
  g_object_set_data (G_OBJECT (gui->window), "mainentry", mainentry);
  g_object_set_data (G_OBJECT (gui->window), "model", model);
  g_object_set_data (G_OBJECT (gui->window), "buffer", buffer);
  g_object_set_data (G_OBJECT (gui->window), "vpaned", vpaned);
  g_object_set_data (G_OBJECT (gui->window), "highcheck1", highcheck1);
  g_object_set_data (G_OBJECT (gui->window), "highcheck2", highcheck2);
  g_object_set_data (G_OBJECT (gui->window), "highcheck3", highcheck3);
  g_object_set_data (G_OBJECT (gui->window), "highcheck4", highcheck4);
  g_object_set_data (G_OBJECT (gui->window), "highcheck5", highcheck5);
  g_object_set_data (G_OBJECT (gui->window), "highcheck6", highcheck6);
  g_object_set_data (G_OBJECT (gui->window), "highcheck7", highcheck7);
  g_object_set_data (G_OBJECT (gui->window), "highcheck8", highcheck8);
  g_object_set_data (G_OBJECT (gui->window), "highentry1", highentry1);
  g_object_set_data (G_OBJECT (gui->window), "highentry2", highentry2);
  g_object_set_data (G_OBJECT (gui->window), "highentry3", highentry3);
  g_object_set_data (G_OBJECT (gui->window), "highentry4", highentry4);
  g_object_set_data (G_OBJECT (gui->window), "highentry5", highentry5);
  g_object_set_data (G_OBJECT (gui->window), "highentry6", highentry6);
  g_object_set_data (G_OBJECT (gui->window), "highentry7", highentry7);
  g_object_set_data (G_OBJECT (gui->window), "highentry8", highentry8);
  g_object_set_data (G_OBJECT (gui->window), "highframe", highframe);

  cluster = new_cluster();
  g_object_set_data(G_OBJECT (gui->window), "cluster", cluster);

  gtk_widget_grab_focus (mainentry);

  return;
}

static gchar *gtk_textbuffer_get_chars (GtkTextBuffer *b)
{
  GtkTextIter start, end;

  gtk_text_buffer_get_start_iter (b, &start);
  gtk_text_buffer_get_end_iter (b, &end);
  return gtk_text_buffer_get_text (b, &start, &end, FALSE);
}

/*
 * hit <enter> in the entry widget
 */
void
on_mainentry_activate (GtkTextBuffer *buffer, gpointer user_data)
{
  gchar *entry, *p;
  GString *str = g_string_new ("");
  GtkWidget *mainentry;

  entry = gtk_textbuffer_get_chars (buffer);
  if ((p = g_strrstr (entry, "\n")))
  {
    *p = '\0';
    str = g_string_new (entry);
    tx (str);
    g_string_free (str, TRUE);
    gtk_text_buffer_set_text (buffer, "", 0);
    mainentry = g_object_get_data (G_OBJECT (gui->window), "mainentry");
    gtk_widget_grab_focus (GTK_WIDGET (mainentry));
  }
  g_free (entry);
}

static void syncprefs (void)
{
  GtkWidget *treeview, *vpaned;
  GList * columns;
  gint i, length, width;
  servertype *cluster;
  GString *w = g_string_new("");

  cluster = g_object_get_data(G_OBJECT(gui->window), "cluster");
  if (cluster->sockethandle != -1)
    cldisconnect(NULL, FALSE);

  gtk_window_get_position(GTK_WINDOW(gui->window), &preferences.x, &preferences.y);
  gtk_window_get_size(GTK_WINDOW(gui->window), &preferences.width, &preferences.height);
  vpaned = g_object_get_data (G_OBJECT(gui->window), "vpaned");
  preferences.handlebarpos = gtk_paned_get_position (GTK_PANED(vpaned));

  treeview = g_object_get_data (G_OBJECT(gui->window), "treeview");
  columns = gtk_tree_view_get_columns (GTK_TREE_VIEW(treeview));
  length = g_list_length (columns);
  for (i = 0; i < length; i++)
  {
      width = gtk_tree_view_column_get_width
        (gtk_tree_view_get_column (GTK_TREE_VIEW(treeview), i));
      if (width == 0)
      {
        if (i == 0) width = COL0WIDTH;
        else if (i == 1) width = COL1WIDTH;
        else if (i == 2) width = COL2WIDTH;
        else if (i == 3) width = COL3WIDTH;
        else if (i == 4) width = COL4WIDTH;
        else if (i == 5) width = COL5WIDTH;
      }
      g_string_append_printf (w, "%d,", width);
  }
  g_list_free (columns);
  preferences.columnwidths = g_strdup (w->str);
  g_string_free (w, TRUE);

  savehistory ();
  savepreferences ();
}

static void cleanup (void)
{
  GList *link;
  servertype *cluster;

  gui->action_group = NULL;
  gui->ui_manager = NULL;

  cluster = g_object_get_data(G_OBJECT(gui->window), "cluster");
  if (cluster->host) g_free (cluster->host);
  if (cluster->port) g_free (cluster->port);
  if (cluster->lastcommand) g_free (cluster->lastcommand);
  g_free (cluster);
  gui->window = NULL;

  g_free(preferences.columnwidths);
  g_free(preferences.callsign);
  g_free(preferences.commands);
  g_free(preferences.rigctl);
  g_free(preferences.dxfont);
  g_free(preferences.allfont);

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
  g_free(gui->statusbarmessage);
  gui->statusbarmessage = NULL;
  g_free(gui->url);
  gui->url = NULL;
  g_free(gui);
}

void
on_quit_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  syncprefs ();
  cleanup ();
  gtk_main_quit ();
}

void on_reconnect_activate (GtkAction * action, gpointer user_data)
{
  GtkWidget *reconnectmenu;
  gboolean state;
  servertype *cluster;

  reconnectmenu = gtk_ui_manager_get_widget
    (gui->ui_manager, "/MainMenu/SettingsMenu/Reconnect");
  state = gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(reconnectmenu));
  cluster = g_object_get_data(G_OBJECT(gui->window), "cluster");

  if (state)
    preferences.reconnect = 1;
  else
  {
    preferences.reconnect = 0;
    cluster->reconnect = FALSE;
  }
}

void
on_sidebar_activate (GtkAction * action, gpointer user_data)
{
  GtkWidget *sidemenu, *highframe;
  gboolean state;

  sidemenu = gtk_ui_manager_get_widget
    (gui->ui_manager, "/MainMenu/SettingsMenu/Sidebar");
  highframe = g_object_get_data (G_OBJECT (gui->window), "highframe");
  state = gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(sidemenu));

  if (state)
  {
    preferences.sidebar = 1;
    gtk_widget_show (highframe);
  }
  else
  {
    preferences.sidebar = 0;
    gtk_widget_hide (highframe);
  }
}

void 
on_highcheck_toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
  gboolean state = gtk_toggle_button_get_active (togglebutton);
  if (state)
    preferences.highmenu[GPOINTER_TO_INT(user_data) - 1] = '1';
  else
    preferences.highmenu[GPOINTER_TO_INT(user_data) - 1] = '0';
}

void on_highentry_changed (GtkEditable * editable, gpointer user_data)
{
  gchar *high = gtk_editable_get_chars (GTK_EDITABLE (editable), 0, -1);
  if (g_utf8_strlen(high, -1) < 2)
    high = g_strdup ("?");

  if (GPOINTER_TO_INT(user_data) == 1)
    preferences.highword1 = g_utf8_strdown (high, -1);
  else if (GPOINTER_TO_INT(user_data) == 2)
    preferences.highword2 = g_utf8_strdown (high, -1);
  else if (GPOINTER_TO_INT(user_data) == 3)
    preferences.highword3 = g_utf8_strdown (high, -1);
  else if (GPOINTER_TO_INT(user_data) == 4)
    preferences.highword4 = g_utf8_strdown (high, -1);
  else if (GPOINTER_TO_INT(user_data) == 5)
    preferences.highword5 = g_utf8_strdown (high, -1);
  else if (GPOINTER_TO_INT(user_data) == 6)
    preferences.highword6 = g_utf8_strdown (high, -1);
  else if (GPOINTER_TO_INT(user_data) == 7)
    preferences.highword7 = g_utf8_strdown (high, -1);
  else if (GPOINTER_TO_INT(user_data) == 8)
    preferences.highword8 = g_utf8_strdown (high, -1);
  g_free (high);
}

gboolean on_highentry_clicked
(GtkEditable * entry, GdkEventButton *event, gpointer user_data)
{
  if (event->type==GDK_2BUTTON_PRESS)
  {
    gtk_editable_select_region (entry, 0, -1);
    return TRUE;
  }
  return FALSE;
}

/*
 * called at program exit
 */
gboolean
on_mainwindow_delete_event (GtkWidget * widget, GdkEvent * event,
			    gpointer user_data)
{
  syncprefs ();
  return FALSE;
}

gboolean
on_mainwindow_destroy_event (GtkWidget * widget, GdkEvent * event,
			    gpointer user_data)
{
  cleanup ();
  gtk_main_quit ();
  return FALSE;
}


/* 
 * history of the entry widget 
 */
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
        g_signal_stop_emission_by_name (GTK_OBJECT(widget), "key_press_event");
        tx_previous();
      break;
      case GDK_Down:
        g_signal_stop_emission_by_name (GTK_OBJECT(widget), "key_press_event");
        tx_next();
      break;
      default:
      break;
    }
  }
  return FALSE;
}

gboolean 
double_click (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{  
  GtkTreeIter selected;
  GtkTreeModel *model;
  gchar *getf, **fsplit, *hamlibstr;
  gint setf;

  if ((preferences.hamlib == 1) && (event->type == GDK_2BUTTON_PRESS))
  {
    if (gtk_tree_selection_get_selected
      (gtk_tree_view_get_selection (GTK_TREE_VIEW(widget)), &model, &selected))
    {
      gtk_tree_model_get (model, &selected, 1, &getf, -1);
      fsplit = g_strsplit (getf, ".", -1);
      setf = atoi(fsplit[0]) * 1000 + atoi(fsplit[1]) * 100;
      if (g_strrstr(preferences.rigctl, "%d"))
      {
        hamlibstr = g_strdup_printf (preferences.rigctl, setf);
        system (hamlibstr);
        g_free (hamlibstr);
      }
      g_strfreev (fsplit);
    }
  }
  return FALSE;
}
