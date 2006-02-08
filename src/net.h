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
 *  net.h
 */

#include <gtk/gtk.h>

#define MESSAGE_RX 1
#define MESSAGE_TX 2

typedef struct servertype {
	gchar *host;
	gchar *port;
	GIOChannel *rxchannel;
	guint source_id;
	gint sockethandle;
  gboolean connected;
} servertype;

servertype *new_cluster(void);
gboolean clresolve (servertype *cluster);
void cldisconnect (GString *msg, gboolean timeout);
gboolean close_connection (GIOChannel * channel, GIOCondition cond, gpointer data);
gboolean rx (GIOChannel * channel, GIOCondition cond, gpointer data);
void tx (GString * message);
