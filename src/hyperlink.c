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
 * hyperlink.c - clicking on links
 */
 
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "hyperlink.h"
#include "gui.h"

gboolean findw (gunichar ch, gpointer user_data)
{
  switch (ch)
  {
    case ' ':
    case '\n':
    case '\t':
    case '\r':
      return TRUE;
    default:
      return FALSE;
  }
}

/* 
 * change cursor depending on whether we have a link or not
 */
static void
set_cursor (GtkTextView *text_view, gint x, gint y)
{
  GtkTextBuffer *buffer;
  GtkTextIter iter, startword, endword, start, end;
  GdkCursor *hand_cursor, *normal_cursor;
  gchar *word;

  buffer = gtk_text_view_get_buffer (text_view);
  gtk_text_view_get_iter_at_location (text_view, &iter, x, y);
  
  hand_cursor = gdk_cursor_new (GDK_HAND2);
  normal_cursor = gdk_cursor_new (GDK_XTERM);

  gtk_text_buffer_get_bounds (buffer, &start, &end);
  startword = iter;
  endword = iter;
  
  if (gtk_text_iter_forward_find_char (&endword, findw, NULL, &end) &&
    gtk_text_iter_backward_find_char (&startword, findw, NULL, &start))
  {
    gtk_text_iter_forward_char (&startword); /* advance one char */
    word = gtk_text_iter_get_text (&startword, &endword);
    if (!g_strrstr (word, " ") && g_strrstr (word, "http://") 
      || g_strrstr (word, "www."))
    {
      gdk_window_set_cursor (gtk_text_view_get_window 
        (text_view, GTK_TEXT_WINDOW_TEXT), hand_cursor);
      gtk_text_buffer_apply_tag_by_name (buffer, "url", &startword, &endword);
      gui->url = g_strdup(word);
    }
    else
    {
      gdk_window_set_cursor (gtk_text_view_get_window (text_view, 
        GTK_TEXT_WINDOW_TEXT), normal_cursor);
      gtk_text_buffer_remove_tag_by_name (buffer, "url", &start, &end);
      gui->url = g_strdup("");
    }
  }
}

/*
 * click on a link
 */
gboolean on_maintext_event_after (GtkWidget * widget, 
          GdkEventKey *event, gpointer user_data)
{
  GtkTextIter start, end, iter;
  GtkTextBuffer *buffer;
  GdkEventButton *ev;
  gint x, y;

  if (event->type != GDK_BUTTON_RELEASE) return FALSE;
  ev = (GdkEventButton *)event;
  if (ev->button != 1) return FALSE;

  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (widget));

  /* we shouldn't follow a link if the user has selected something */
  gtk_text_buffer_get_selection_bounds (buffer, &start, &end);
  if (gtk_text_iter_get_offset (&start) != gtk_text_iter_get_offset (&end))
    return FALSE;

  gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (widget),
    GTK_TEXT_WINDOW_WIDGET, ev->x, ev->y, &x, &y);
  gtk_text_view_get_iter_at_location (GTK_TEXT_VIEW (widget), &iter, x, y);

  if (gtk_text_iter_get_tags (&iter))
    openurl (gui->url);

  return FALSE;
}


gboolean on_maintext_visibility_notify_event (GtkWidget * widget, 
          GdkEventVisibility *event, gpointer user_data)
{
  gint wx, wy, bx, by;
  
  gdk_window_get_pointer (widget->window, &wx, &wy, NULL);
  gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (widget), 
                                         GTK_TEXT_WINDOW_WIDGET,
                                         wx, wy, &bx, &by);
  set_cursor (GTK_TEXT_VIEW (widget), bx, by);

  return FALSE;
}


gboolean on_maintext_motion_notify_event (GtkWidget * widget, 
          GdkEventMotion *event, gpointer user_data)
{
  gint x, y;

  gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (widget), 
                                         GTK_TEXT_WINDOW_WIDGET,
                                         event->x, event->y, &x, &y);
  set_cursor (GTK_TEXT_VIEW (widget), x, y);
  gdk_window_get_pointer (widget->window, NULL, NULL, NULL);

  return FALSE;
}
