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

typedef struct dxinfo {
  gchar *spotter;
  gchar *freq;
  gchar *dxcall;
  gchar *remark;
  gchar *time;
  gchar *info;
  gchar *toall;
  gboolean dx;
  gboolean nodx;
} dxinfo;

static dxinfo *dx;

/* extract call from dxmessage and return call and length of call */
static gchar *
findcall (gchar * str, gint * spotterlen)
{
  gchar *end, *j;
  gboolean found = FALSE;

  end = str + strlen (str);
  *spotterlen = 0;
  for (j = str; j < end; ++j)
    {
      *spotterlen = *spotterlen + 1;
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

/* find the end of frequency field */
static gchar *
findfreq (gchar * str)
{
  gchar *end, *j;
  gboolean found = FALSE;

  end = str + strlen (str);
  for (j = str; j < end; ++j)
    {
      switch (*j)
	{
	case '.':
	  *(j + 2) = '\0';
	  found = TRUE;
	  break;
	}
      if (found)
	break;
    }
  return (str);
}

/* end of remarks field found if there is a space 
 * and the next 2 characters are digits 
 */
static gchar *
findrem (gchar * str, gint * remlen)
{
  gchar *end, *j;
  gboolean found = FALSE;

  end = str + strlen (str);
  *remlen = 0;
  for (j = str; j < end; ++j)
    {
      *remlen = *remlen + 1;
      switch (*j)
	{
	case ' ':
	  if ((j > str + 28) && g_ascii_isdigit (*(j + 1))
            && g_ascii_isdigit (*(j + 2)))
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

/* search for the end of time field */
static gchar *
findtime (gchar * str)
{
  gchar *end, *j;
  gboolean found = FALSE;

  end = str + strlen (str);
  for (j = str; j < end; ++j)
    {
      switch (*j)
	{
	case 'Z':
	  *(j + 1) = '\0';
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
  gint len = 0;

  end = str + strlen (str);
  for (j = str; j < end; ++j)
    {
      len++;
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
  if (len < 2) str = NULL;
  return (str);
}

dxinfo *new_dx(void)
{
        dxinfo *dx = g_new0(dxinfo, 1);
        dx->spotter = NULL;
        dx->freq = NULL;
        dx->dxcall = NULL;
        dx->remark = NULL;
        dx->time = NULL;
        dx->info = NULL;
        dx->toall = NULL;
	dx->dx = FALSE;
        dx->nodx = FALSE;
        return(dx);
}

/*
 * DX~de~JA0AOQ:~~~~~1822.5~~RA3DOX~~~~~~~cq..~loud~~~~~~~~~~~~~~~~~~~~~~1923Z
 * 01234567890123456789012345678901234567890123456789012345678901234567890123456789
 *       -                   -            -     <-- fixed positions
 */
static dxinfo *
extractinfo(gchar *msg)
{
  gchar *dxmsg, *info;
  gint l, len;

  dx = new_dx();
  info = g_strdup(msg);

  if (dxmsg = g_strrstr(info, "DX de "))
  {
    dx->spotter = g_strdup(findcall(dxmsg + 6, &l));
    dx->freq = g_strdup(findfreq(dxmsg + 6 + l));
    dx->dxcall = g_strdup(findspace(dxmsg + 26));
    dx->remark = g_strdup(findrem(dxmsg + 39, &l));
    dx->time = g_strdup(findtime(dxmsg + 39 + l));
    dx->info = g_strdup(findinfo(dxmsg + 45 + l));
    dx->toall = NULL;
    dx->dx = TRUE;
    dx->nodx = FALSE;
  }
  else
  {
    dx->spotter = NULL;
    dx->freq = NULL;
    dx->dxcall = NULL;
    dx->remark = NULL;
    dx->time = NULL;
    dx->info = NULL;
    dx->toall = g_strdup(msg);
    dx->dx = FALSE;
    dx->nodx = TRUE;
  }
  if (len = msg - dxmsg > 0) 
  { /* dx and other messages on one line */
    dx->toall = g_strndup(msg, len);
    dx->nodx = TRUE;
  }
  
  g_free(info);
  return(dx);
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

  if (len < 1024) msg[len] = '\0';

  maintext = (GtkWidget *)g_object_get_data (G_OBJECT (gui->window), "maintext");
  model = (GtkTreeStore *)g_object_get_data(G_OBJECT(gui->window), "model");
  treeview = (GtkWidget *)g_object_get_data(G_OBJECT(gui->window), "treeview");
  buffer = (GtkTextBuffer *)g_object_get_data(G_OBJECT(gui->window), "buffer");
  gtk_text_buffer_get_bounds (buffer, &start, &end);

  if (messagetype == MESSAGE_RX)
    { /* beep if there is a bell */
      if (g_strrstr(msg, "\a"))
      {
        gdk_beep();
        g_strdelimit(msg, "\a", ' ');
      }
      dx = extractinfo(msg);
      if (dx->dx)
      { 
          g_strstrip(dx->freq);
          g_strstrip(dx->remark);
	  gtk_tree_store_append (model, &iter, NULL);
	  gtk_tree_store_set (model, &iter, FROM_COLUMN, dx->spotter, FREQ_COLUMN,
			      dx->freq, DX_COLUMN, dx->dxcall, REM_COLUMN, dx->remark,
			      TIME_COLUMN, dx->time, INFO_COLUMN, dx->info, -1);
	  path = gtk_tree_model_get_path (GTK_TREE_MODEL (model), &iter);
	  gtk_tree_view_set_cursor (GTK_TREE_VIEW (treeview), path, NULL,
				    FALSE);
	  gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW (treeview), path,
					NULL, TRUE, 0.0, 1.0);
	  gtk_tree_path_free (path);

          g_free(dx->spotter);
          g_free(dx->freq);
          g_free(dx->dxcall);
          g_free(dx->remark);
          g_free(dx->time);
          g_free(dx->info);
      }
      if (dx->nodx)  
	{
          gtk_text_buffer_place_cursor(buffer, &end);

	  if (!g_ascii_strncasecmp (dx->toall, "WWV de ", 6)
	      || !g_ascii_strncasecmp (dx->toall, "WCY de ", 6))
	    gtk_text_buffer_insert_with_tags_by_name (buffer, &end, dx->toall, len,
                  "wwv", NULL); /* should be utf-8 clean */
	  else
    { /* try local language, if it fails fall back to ISO-8859-1 */
      if (!g_utf8_validate (dx->toall, -1, NULL ))
        dx->toall = g_locale_to_utf8 (dx->toall, -1, NULL, NULL, NULL);
      if (!g_utf8_validate (dx->toall, -1, NULL ))
        g_convert (dx->toall, strlen (dx->toall), "UTF-8", "ISO-8859-1", 
          NULL, NULL, NULL);
	    gtk_text_buffer_insert (buffer, &end, dx->toall, len);
    }
	  mark = gtk_text_buffer_get_mark (buffer, "insert");
	  gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(maintext), mark, 0.0, FALSE, 0.0, 1.0);
          g_free(dx->toall);
	}
      g_free(dx);
    }
    else if (messagetype == MESSAGE_TX)
    { /* try local language, if it fails fall back to ISO-8859-1 */
      if (!g_utf8_validate (msg, -1, NULL ))
        msg = g_locale_to_utf8 (msg, -1, NULL, NULL, NULL);
      if (!g_utf8_validate (msg, -1, NULL ))
        g_convert (msg, strlen (msg), "UTF-8", "ISO-8859-1", 
          NULL, NULL, NULL);
      gtk_text_buffer_insert_with_tags_by_name (buffer, &end, msg, len,
					      "sent", NULL);
    }
}
