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
  GtkWidget *mainstatusbar, *treeview;
  gchar *lang, **wsplit;
  GString *greeting = g_string_new ("");
  gint i;


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
  wsplit = g_strsplit(preferences.columnwidths, ",", 0);
  for (i = 0; i < 5; i++)
    gtk_tree_view_column_set_fixed_width
      (gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), i), atoi(wsplit[i]));
  g_strfreev(wsplit);

  gtk_widget_show_all (gui->window);
  gtk_window_move (GTK_WINDOW(gui->window), 
    preferences.x, preferences.y);
  gtk_window_resize (GTK_WINDOW(gui->window), 
    preferences.width, preferences.height);

  menu_set_sensitive (gui->item_factory, "/Host/Close", FALSE); /* do not translate */
  g_string_printf (greeting, _("Welcome to %s"), PACKAGE);
  updatestatusbar(greeting, FALSE);
  g_string_free (greeting, TRUE);

  gtk_main ();
  return 0;
}
