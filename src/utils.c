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
 * utils.c - private functions that don't belong in other modules
 */

#include <gtk/gtk.h>
#include "utils.h"
#include "gui.h"

static gint statusbartimer = -1;
static GList *pixmaps_directories = NULL;

/* 
 * use this function to set the directory containing installed pixmaps
 */

void
add_pixmap_directory (const gchar * directory)
{
  pixmaps_directories = g_list_prepend (pixmaps_directories,
					g_strdup (directory));
}

/* 
 * print a message to the statusbar 
 */

void
updatestatusbar (GString * statusmessage)
{
  GtkWidget *mainstatusbar;

  mainstatusbar = g_object_get_data (G_OBJECT (gui->window), "mainstatusbar");
  gtk_statusbar_pop (GTK_STATUSBAR (mainstatusbar), 1);
  gtk_statusbar_push (GTK_STATUSBAR (mainstatusbar), 1, statusmessage->str);
}

/* 
 * enable/disable menus
 */

void
menu_set_sensitive (GtkItemFactory * ifa, const gchar * path, gboolean sens)
{
  GtkWidget *w;

  g_return_if_fail (ifa != NULL);

  w = gtk_item_factory_get_item (ifa, path);
  gtk_widget_set_sensitive (w, sens);
}
