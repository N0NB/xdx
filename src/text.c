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
 * text.c - private functions for looking up and displaying text, either in a 
 * treeview or a textview
 */

#include <gtk/gtk.h>
#include "types.h"
#include "utils.h"
#include "text.h"
#include "net.h"
#include "gui.h"

gint last = 0;
gboolean info = FALSE;

/* find callsign and keep a counter for the start of frequency field */
static gchar *
findcall (gchar * str)
{
  gchar *end, *j;
  gboolean found = FALSE;

  end = str + strlen (str);
  for (j = str; j < end; ++j)
    {
      last++;
      switch (*j)
	{
	case ':':
	  *j = '\0';
	  found = TRUE;
	  break;
	}
      if (found)
	break;
    }
  return (str);
}

/* find the end of frequency and DX-call field */
static gchar *
findspace (gchar * str)
{
  gchar *end, *j;
  gboolean found = FALSE;

  end = str + strlen (str);
  for (j = str; j < end; ++j)
    {
      switch (*j)
	{
	case ' ':
	  *j = '\0';
	  found = TRUE;
	  break;
	}
      if (found)
	break;
    }
  return (str);
}

/* end of remarks field found if there is a space and the next character is a digit */
static gchar *
findrem (gchar * str)
{
  gchar *end, *j;
  gboolean found = FALSE;

  end = str + strlen (str);
  for (j = str; j < end; ++j)
    {
      last++;
      switch (*j)
	{
	case ' ':
	  if ((j > str + 28) && g_ascii_isdigit (*(j + 1)))
	    {
	      *j = '\0';
	      found = TRUE;
	    }
	  break;
	}
      if (found)
	break;
    }
  return (str);

}

/* search for the end of time field and check if it is followed by an info field */
static gchar *
findtime (gchar * str)
{
  gchar *end, *j;
  gboolean found = FALSE;

  end = str + strlen (str);
  for (j = str; j < end; ++j)
    {
      last++;
      switch (*j)
	{
	case 'Z':
	  *(j + 1) = '\0';
	  if (g_ascii_isalpha (*(j + 2)) || g_ascii_isdigit (*(j + 2)))
	    {
	      info = TRUE;
	    }
	  found = TRUE;
	  break;
	}
      if (found)
	break;
    }
  return (str);
}

/* search for the end of the locator field */
static gchar *
findinfo (gchar * str)
{
  gchar *end, *j;
  gboolean found = FALSE;

  end = str + strlen (str);
  for (j = str; j < end; ++j)
    {
      switch (*j)
	{
	case '\r':
	  *j = '\0';
	  found = TRUE;
	  break;
	}
      if (found)
	break;
    }
  return (str);
}

/* look for ascii bell, if found: beep and replace with cr/lf */
static gchar *
findbeep (gchar * str, gint l)
{
  gchar *end, *j;
  gboolean found = FALSE;

  end = str + l;
  for (j = str; j < end; ++j)
    {
      switch (*j)
	{
	case '\a':
	  gdk_beep ();
	  *j = '\r';
	  *(j + 1) = '\n';
	  *(j + 2) = '\0';
	  found = TRUE;
	  break;
	}
      if (found)
	break;
    }
  return (str);
}

/*
 * add text to the text widget and dx messages to the list
 */

void
maintext_add (gchar msg[], gint len, gint messagetype)
{
  GtkWidget *maintext, *treeview;
  GtkTextIter start, end;
  GtkTextMark *mark;
  GtkTreeIter iter;
  GtkTreePath *path;
  GtkTreeStore *model;
  GtkTextBuffer *buffer;
  gchar *call, *freq, *dx, *rem, *time, *inf = NULL;

  if (len < 1024) msg[len] = '\0';

  maintext = (GtkWidget *)g_object_get_data (G_OBJECT (gui->window), "maintext");
  model = (GtkTreeStore *)g_object_get_data(G_OBJECT(gui->window), "model");
  treeview = (GtkWidget *)g_object_get_data(G_OBJECT(gui->window), "treeview");
  buffer = (GtkTextBuffer *)g_object_get_data(G_OBJECT(gui->window), "buffer");
  gtk_text_buffer_get_bounds (buffer, &start, &end);

  if (messagetype == MESSAGE_RX)
    {
      msg = findbeep (msg, len);
      if (!g_ascii_strncasecmp (msg, "DX de ", 6))
	{
	  call = g_strdup (msg + 6);
	  call = findcall (call);

	  freq = g_strdup (msg + 6 + last + 1);
	  last = 0;
	  g_strstrip (freq);
	  freq = findspace (freq);

	  dx = g_strdup (msg + 26);
	  dx = findspace (dx);

	  rem = g_strdup (msg + 39);
	  rem = findrem (rem);

	  time = g_strdup (msg + 39 + last);
	  time = findtime (time);

	  if (info)
	    {
	      inf = g_strdup (msg + 39 + last);
	      inf = findinfo (inf);
	      info = FALSE;
	    }
	  last = 0;

	  gtk_tree_store_append (model, &iter, NULL);
	  gtk_tree_store_set (model, &iter, FROM_COLUMN, call, FREQ_COLUMN,
			      freq, DX_COLUMN, dx, REM_COLUMN, rem,
			      TIME_COLUMN, time, INFO_COLUMN, inf, -1);
	  path = gtk_tree_model_get_path (GTK_TREE_MODEL (model), &iter);
	  gtk_tree_view_set_cursor (GTK_TREE_VIEW (treeview), path, NULL,
				    FALSE);
	  gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW (treeview), path,
					NULL, TRUE, 0.0, 1.0);
	  gtk_tree_path_free (path);
	}
      else
	{
          gtk_text_buffer_place_cursor(buffer, &end);

	  if (!g_ascii_strncasecmp (msg, "WWV de ", 6)
	      || !g_ascii_strncasecmp (msg, "WCY de ", 6))
	    gtk_text_buffer_insert_with_tags_by_name (buffer, &end, msg, len,
						      "blue_foreground",
						      NULL);
	  else
	    gtk_text_buffer_insert (buffer, &end, msg, len);

	    mark = gtk_text_buffer_get_mark (buffer, "insert");
	    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(maintext), mark, 0.0, FALSE, 0.0, 1.0);
	}
    }
    else if (messagetype == MESSAGE_TX)
      gtk_text_buffer_insert_with_tags_by_name (buffer, &end, msg, len,
					      "red_foreground", NULL);
}
