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

int
main (int argc, char *argv[])
{
  GtkWidget *treeview, *maintext, *vpaned;
  GtkTreeViewColumn *column;
  gchar *lang, **wsplit;
  GString *greeting = g_string_new ("");
  PangoFontDescription *font_description;
  gint pango_size;

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

  create_mainwindow ();
  dircheck ();
  loadpreferences ();
  loadhistory ();

  treeview = g_object_get_data (G_OBJECT (gui->window), "treeview");
  maintext = g_object_get_data (G_OBJECT (gui->window), "maintext");
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

  menu_set_sensitive (gui->item_factory, "/Host/Close", FALSE); /* do not translate */
  g_string_printf (greeting, _("Welcome to %s"), PACKAGE);
  updatestatusbar(greeting, FALSE);
  g_string_free (greeting, TRUE);

  gtk_main ();
  return 0;
}
