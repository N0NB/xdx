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
 * hyperlink.c - clicking on links
 */
 
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#include "hyperlink.h"
#include "gui.h"
#include "utils.h"

/*
 * count number of dots in a link
 */
static gboolean 
linkcontains2dots (gchar *link)
{
  gint dots = 0;
  gchar *linktocheck, *end, *j;

  linktocheck = g_strdup (link);
  end = linktocheck + strlen (linktocheck);
  for (j = linktocheck; j < end; ++j)
    {
      switch (*j)
      {
        case '.':
        case '@':
        case '/':
          dots++;
        break;
      }
    }

  g_free (linktocheck);
  if (dots >= 1) return TRUE;
  return FALSE;
}

/*
 * check if link
 */
static gboolean 
islink (gchar *link)
{
  if (g_strrstr (link, " "))
    return FALSE;
  else if (!g_strrstr (link, "."))
    return FALSE;
  else if (g_strrstr (link, ".."))
    return FALSE;
  else if (!g_ascii_strncasecmp (link, "http://", 7))
    return TRUE;
  else if (!g_ascii_strncasecmp (link, "www.", 4))
    return TRUE;
  else if (!g_ascii_strncasecmp (link, "ftp://", 7))
    return TRUE;
  else if (!g_ascii_strncasecmp (link, "ftp.", 7))
    return TRUE;
  else if ( linkcontains2dots (link))
    return TRUE;
  return FALSE;
}

/*
 * used by set_cursor to find begin/end of a word
 */
static gboolean 
findw (gunichar ch, gpointer user_data)
{
  switch (ch)
  {
    case ' ':
    case ',':
    case ';':
    case '\n':
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

  startword = iter;
  endword = iter;
  
  if (gtk_text_iter_forward_find_char (&endword, findw, NULL, NULL) &&
    gtk_text_iter_backward_find_char (&startword, findw, NULL, NULL))
  {
    gtk_text_iter_forward_char (&startword); /* advance one char */
    word = gtk_text_buffer_get_slice (buffer, &startword, &endword, FALSE);
    if (word && islink (word))
    {
      gdk_window_set_cursor (gtk_text_view_get_window 
        (text_view, GTK_TEXT_WINDOW_TEXT), hand_cursor);
      gtk_text_buffer_apply_tag_by_name (buffer, "url", &startword, &endword);
      gui->url = g_strdup(word);
      gdk_cursor_unref (hand_cursor);
    }
    else
    {
      gdk_window_set_cursor (gtk_text_view_get_window (text_view, 
        GTK_TEXT_WINDOW_TEXT), normal_cursor);
      gtk_text_buffer_get_bounds (buffer, &start, &end);
      gtk_text_buffer_remove_tag_by_name (buffer, "url", &start, &end);
      gui->url = g_strdup("");
      gdk_cursor_unref (normal_cursor);
    }
    if (word) g_free (word);
  }
}

/*
 * check if this tag is a url tag
 */
static GtkTextTag *
get_link_tag(GtkTextIter * iter)
{
  GtkTextTag *link_tag = NULL;
  GSList *list;
  GSList *tag_list = gtk_text_iter_get_tags(iter);
 
  for (list = tag_list; list; list = g_slist_next(list)) 
    {
      GtkTextTag *tag = list->data;
      gchar *name;
      g_object_get (G_OBJECT(tag), "name", &name, NULL);
      if (!strncmp(name, "url", 3))
        link_tag = tag_list->data;
      g_free(name);
    }
  g_slist_free(tag_list);

  return link_tag;
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

  if (get_link_tag (&iter))
  {
    if (g_strrstr (gui->url, "@"))
      openmail (gui->url);
    else
      openurl (gui->url);
  }

  return FALSE;
}

/*
 * grab mouse coordinates and modify cursor
 */
gboolean 
on_maintext_motion_notify_event (GtkWidget * widget, GdkEventMotion *event, 
  gpointer user_data)
{
  GdkWindow *window;
  gint x, y;
  GdkModifierType state;

  if (event->is_hint)
    window = gdk_window_get_pointer (event->window, &x, &y, &state);
  else
  {
    x = event->x;
    y = event->y;
    state = event->state;
  }
    
  gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (widget), 
    GTK_TEXT_WINDOW_WIDGET, event->x, event->y, &x, &y);
  set_cursor (GTK_TEXT_VIEW (widget), x, y);

  return FALSE;
}
