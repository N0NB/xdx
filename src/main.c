/*  
 *  xdx - GTK+ DX-cluster client for amateur radio
 *  Copyright (C) 2002-2003 Joop Stakenborg <pg4i@amsat.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 *  main.c - start of gtk loop.
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include "gui.h"
#include "utils.h"
#include "preferences.h"
#include "history.h"
#include "utils.h"
#include "locale.h"

extern preferencestype preferences;
GdkColormap *colormap;

int
main (int argc, char *argv[])
{
  GtkWidget *treeview, *maintext, *vpaned, *highmenu,
    *highcheck1, *highcheck2, *highcheck3, *highcheck4, *highcheck5,
    *highcheck6, *highcheck7, *highcheck8;
  GtkTreeViewColumn *column;
  gchar *lang, **wsplit, *colorstr;
  GString *greeting = g_string_new ("");
  PangoFontDescription *font_description;
  gint pango_size;
  GtkTextBuffer *buffer;
	
#ifdef ENABLE_NLS
  bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (PACKAGE, "UTF-8");
  textdomain (PACKAGE);
#endif

  putenv ("TZ=GMT");
  tzset ();

  add_pixmap_directory(PACKAGE_DATA_DIR G_DIR_SEPARATOR_S "pixmaps");
  add_pixmap_directory(PACKAGE_SOURCE_DIR G_DIR_SEPARATOR_S "pixmaps");

  lang = gtk_set_locale ();	/* don't free lang */
  gtk_init (&argc, &argv);
  setlocale(LC_NUMERIC, "C");

  colormap = gdk_colormap_get_system();
  create_mainwindow ();
  dircheck ();
  loadpreferences ();
  loadhistory ();

  maintext = g_object_get_data (G_OBJECT (gui->window), "maintext");
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (maintext));
  colorstr = g_strdup_printf ("#%02X%02X%02X",
	(preferences.highcolor1.red * 255) / 65535,
	preferences.highcolor1.green * 255 / 65535,
	preferences.highcolor1.blue * 255 / 65535);
  gtk_text_buffer_create_tag (buffer, "highcolor1", "background",
	colorstr, NULL);
  colorstr = g_strdup_printf ("#%02X%02X%02X",
	preferences.highcolor2.red * 255 / 65535,
	preferences.highcolor2.green * 255 / 65535,
	preferences.highcolor2.blue * 255 / 65535);
  gtk_text_buffer_create_tag (buffer, "highcolor2", "background",
	colorstr, NULL);
  colorstr = g_strdup_printf ("#%02X%02X%02X",
	preferences.highcolor3.red * 255 / 65535,
	preferences.highcolor3.green * 255 / 65535,
	preferences.highcolor3.blue * 255 / 65535);
  gtk_text_buffer_create_tag (buffer, "highcolor3", "background",
	colorstr, NULL);
  colorstr = g_strdup_printf ("#%02X%02X%02X",
	preferences.highcolor4.red * 255 / 65535,
	preferences.highcolor4.green * 255 / 65535,
	preferences.highcolor4.blue * 255 / 65535);
  gtk_text_buffer_create_tag (buffer, "highcolor4", "background",
	colorstr, NULL);
  colorstr = g_strdup_printf ("#%02X%02X%02X",
	preferences.highcolor5.red * 255 / 65535,
	preferences.highcolor5.green * 255 / 65535,
	preferences.highcolor5.blue * 255 / 65535);
  gtk_text_buffer_create_tag (buffer, "highcolor5", "background",
	colorstr, NULL);
  colorstr = g_strdup_printf ("#%02X%02X%02X",
	preferences.highcolor6.red * 255 / 65535,
	preferences.highcolor6.green * 255 / 65535,
	preferences.highcolor6.blue * 255 / 65535);
  gtk_text_buffer_create_tag (buffer, "highcolor6", "background",
	colorstr, NULL);
  colorstr = g_strdup_printf ("#%02X%02X%02X",
	preferences.highcolor7.red * 255 / 65535,
	preferences.highcolor7.green * 255 / 65535,
	preferences.highcolor7.blue * 255 / 65535);
  gtk_text_buffer_create_tag (buffer, "highcolor7", "background",
	colorstr, NULL);
  colorstr = g_strdup_printf ("#%02X%02X%02X",
	preferences.highcolor8.red * 255 / 65535,
	preferences.highcolor8.green * 255 / 65535,
	preferences.highcolor8.blue * 255 / 65535);
  gtk_text_buffer_create_tag (buffer, "highcolor8", "background",
	colorstr, NULL);
  g_free (colorstr);

  highcheck1 = g_object_get_data (G_OBJECT (gui->window), "highcheck1");
  highcheck2 = g_object_get_data (G_OBJECT (gui->window), "highcheck2");
  highcheck3 = g_object_get_data (G_OBJECT (gui->window), "highcheck3");
  highcheck4 = g_object_get_data (G_OBJECT (gui->window), "highcheck4");
  highcheck5 = g_object_get_data (G_OBJECT (gui->window), "highcheck5");
  highcheck6 = g_object_get_data (G_OBJECT (gui->window), "highcheck6");
  highcheck7 = g_object_get_data (G_OBJECT (gui->window), "highcheck7");
  highcheck8 = g_object_get_data (G_OBJECT (gui->window), "highcheck8");
  if (preferences.highmenu[0] == '1')
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(highcheck1), TRUE);
  if (preferences.highmenu[1] == '1')
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(highcheck2), TRUE);
  if (preferences.highmenu[2] == '1')
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(highcheck3), TRUE);
  if (preferences.highmenu[3] == '1')
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(highcheck4), TRUE);
  if (preferences.highmenu[4] == '1')
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(highcheck5), TRUE);
  if (preferences.highmenu[5] == '1')
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(highcheck6), TRUE);
  if (preferences.highmenu[6] == '1')
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(highcheck7), TRUE);
  if (preferences.highmenu[7] == '1')
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(highcheck8), TRUE);

  treeview = g_object_get_data (G_OBJECT (gui->window), "treeview");
  vpaned = g_object_get_data (G_OBJECT (gui->window), "vpaned");

  font_description = pango_font_description_from_string (preferences.dxfont);
  gtk_widget_modify_font (GTK_WIDGET(treeview), font_description);
  pango_font_description_free (font_description);
  font_description = pango_font_description_from_string (preferences.allfont);
  gtk_widget_modify_font (GTK_WIDGET(maintext), font_description);
  pango_size = pango_font_description_get_size (font_description);
  /* line spacing is half character size */
  g_object_set (G_OBJECT(maintext), "pixels-below-lines",
    PANGO_PIXELS (pango_size) / 2, NULL);
  pango_font_description_free (font_description);

  wsplit = g_strsplit (preferences.columnwidths, ",", 0);
  column = gtk_tree_view_get_column (GTK_TREE_VIEW(treeview), 0);
  if (!preferences.col0visible)
    gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
  else
    gtk_tree_view_column_set_fixed_width (column, atoi(wsplit[0]));
  column = gtk_tree_view_get_column (GTK_TREE_VIEW(treeview), 1);
  if (!preferences.col1visible)
    gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
  else
    gtk_tree_view_column_set_fixed_width (column, atoi(wsplit[1]));
  column = gtk_tree_view_get_column (GTK_TREE_VIEW(treeview), 2);
  if (!preferences.col2visible)
    gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
  else
    gtk_tree_view_column_set_fixed_width (column, atoi(wsplit[2]));
  column = gtk_tree_view_get_column (GTK_TREE_VIEW(treeview), 3);
  if (!preferences.col3visible)
    gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
  else
    gtk_tree_view_column_set_fixed_width (column, atoi(wsplit[3]));
  column = gtk_tree_view_get_column (GTK_TREE_VIEW(treeview), 4);
  if (!preferences.col4visible)
    gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
  else
    gtk_tree_view_column_set_fixed_width (column, atoi(wsplit[4]));
  column = gtk_tree_view_get_column (GTK_TREE_VIEW(treeview), 5);
  if (!preferences.col5visible)
    gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
  else
    gtk_tree_view_column_set_fixed_width (column, atoi(wsplit[5]));
  g_strfreev (wsplit);

  gtk_widget_show_all (gui->window);
  gtk_window_move (GTK_WINDOW(gui->window), 
    preferences.x, preferences.y);
  gtk_window_resize (GTK_WINDOW(gui->window), 
    preferences.width, preferences.height);
  gtk_paned_set_position (GTK_PANED (vpaned), preferences.handlebarpos);

  menu_set_sensitive (gui->ui_manager, "/MainMenu/HostMenu/Close", FALSE); /* do not translate */
  g_string_printf (greeting, _("Welcome to %s"), PACKAGE);
  updatestatusbar(greeting, FALSE);
  g_string_free (greeting, TRUE);

  gtk_main ();
  return 0;
}
