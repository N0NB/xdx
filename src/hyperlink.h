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
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * hyperlink.h
 */


#ifndef XDX_HYPERLINK_H
#define XDX_HYPERLINK_H

gboolean on_maintext_visibility_notify_event (GtkWidget * widget,
          GdkEventVisibility *event, gpointer user_data);
gboolean on_maintext_motion_notify_event (GtkWidget * widget,
          GdkEventMotion *event, gpointer user_data);
gboolean on_maintext_event_after (GtkWidget * widget,
          GdkEventKey *event, gpointer user_data);

#endif /* XDX_HYPERLINK_H */
