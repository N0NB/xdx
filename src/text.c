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
 * text.c - private functions for looking up and displaying text, either in a 
 * treeview or a textview
 */

#include <gtk/gtk.h>
#include <string.h>
#include "types.h"
#include "utils.h"
#include "text.h"
#include "net.h"
#include "gui.h"
#include "preferences.h"
#include "save.h"

extern preferencestype preferences;

typedef struct dxinfo 
{
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

typedef struct 
{
  gchar *str;
  gchar *file;
} smiley;
 

static dxinfo *dx;
GSList *smileylist = NULL;

/*
 * create a new smiley struct containing text appearance and path to the pixmap
 */
smiley *new_smiley(void)
{
  smiley *sm = g_new0(smiley, 1);
  sm->str = NULL;
  sm->file = NULL;
  return(sm);
}

/*
 * create a list of supported smileys
 */
static void
create_smiley_list (void)
{
  smiley *s;

  s = new_smiley ();
  s->str = ":-((";
  s->file = PACKAGE_DATA_DIR "/pixmaps/cry.png";
  smileylist = g_slist_append(smileylist, s);
  s = new_smiley ();
  s->str = ":((";
  s->file = PACKAGE_DATA_DIR "/pixmaps/cry.png";
  smileylist = g_slist_append(smileylist, s);
  s = new_smiley ();
  s->str = ":))";
  s->file = PACKAGE_DATA_DIR "/pixmaps/bigsmile.png";
  smileylist = g_slist_append(smileylist, s);
  s->str = ":-))";
  s->file = PACKAGE_DATA_DIR "/pixmaps/bigsmile.png";
  smileylist = g_slist_append(smileylist, s);
  s = new_smiley ();
  s->str = ":)";
  s->file = PACKAGE_DATA_DIR "/pixmaps/smile.png";
  smileylist = g_slist_append(smileylist, s);
  s = new_smiley ();
  s->str = ":-)";
  s->file = PACKAGE_DATA_DIR "/pixmaps/smile.png";
  smileylist = g_slist_append(smileylist, s);
  s = new_smiley ();
  s->str = ";)";
  s->file = PACKAGE_DATA_DIR "/pixmaps/wink.png";
  smileylist = g_slist_append(smileylist, s);
  s = new_smiley ();
  s->str = ";-)";
  s->file = PACKAGE_DATA_DIR "/pixmaps/wink.png";
  smileylist = g_slist_append(smileylist, s);
  s = new_smiley ();
  s->str = ":(";
  s->file = PACKAGE_DATA_DIR "/pixmaps/sad.png";
  smileylist = g_slist_append(smileylist, s);
  s = new_smiley ();
  s->str = ":-(";
  s->file = PACKAGE_DATA_DIR "/pixmaps/sad.png";
  smileylist = g_slist_append(smileylist, s);
//  s = new_smiley (); s->str = "is"; s->file = PACKAGE_DATA_DIR "/pixmaps/sad.png"; smileylist = g_slist_append(smileylist, s);
}

/* 
 * extract call from dxmessage and return call and length of call 
 */
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
        case ' ': /* bug in dx-spider for calls > 6 ? */
        *j = '\0';
        found = TRUE;
        break;
      }
      if (found)
      break;
    }
  return (str);
}

/* 
 * find the end of frequency and DX-call field 
 */
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

/* 
 * find the end of frequency field 
 */
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

/* 
 * end of remarks field found if there is a space 
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

/* 
 * search for the end of time field 
 */
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

/* 
 * search for the end of the locator field 
 */
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

/*
 * create a new dxinfo struct
 */
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

  if ((dxmsg = g_strrstr(info, "DX de ")) && (info[0] == 'D'))
  {
    if (preferences.savedx) savedx (msg);
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
  if ((len = msg - dxmsg) > 0) 
  { /* dx and other messages on one line */
    dx->toall = g_strndup(msg, len);
    dx->nodx = TRUE;
  }
  
  g_free(info);
  return(dx);
}

/*
 * check for any of the supported smileys
 */
static gboolean
contains_smileys (gchar *str)
{
  if (g_strrstr (str, ":)"))
    return TRUE;
  else if (g_strrstr (str, ":-)"))
    return TRUE;
  else if (g_strrstr (str, ":("))
    return TRUE;
  else if (g_strrstr (str, ":-("))
    return TRUE;
  else if (g_strrstr (str, ";)"))
    return TRUE;
  else if (g_strrstr (str, ";-)"))
    return TRUE;
//  else if (g_strrstr (str, "is")) return TRUE;
  return FALSE;
}

/*
 * check if there is something to highlight
 */
static gboolean
contains_highlights (gchar *str)
{
	if (g_ascii_strcasecmp (preferences.callsign, "?")
	&& g_strrstr (str, preferences.highword1))
		return TRUE;
	if (g_ascii_strcasecmp (preferences.callsign, "?")
	&& g_strrstr (str, preferences.highword2))
		return TRUE;
	if (g_ascii_strcasecmp (preferences.callsign, "?")
	&& g_strrstr (str, preferences.highword3))
		return TRUE;
	if (g_ascii_strcasecmp (preferences.callsign, "?")
	&& g_strrstr (str, preferences.highword4))
		return TRUE;
	if (g_ascii_strcasecmp (preferences.callsign, "?")
	&& g_strrstr (str, preferences.highword5))
		return TRUE;
	if (g_ascii_strcasecmp (preferences.callsign, "?")
	&& g_strrstr (str, preferences.highword6))
		return TRUE;
	if (g_ascii_strcasecmp (preferences.callsign, "?")
	&& g_strrstr (str, preferences.highword7))
		return TRUE;
	if (g_ascii_strcasecmp (preferences.callsign, "?")
	&& g_strrstr (str, preferences.highword8))
		return TRUE;
	return FALSE;
}
/*
 * insert text and when it has a smiley, replace it with a pixmap
 */
static void insert_with_smileys
(GtkTextView *textview, gchar *str, gchar *tag)
{
  GtkTextIter istart, end, iend;
  GtkTextBuffer *buf;
  GtkTextMark *mark_start;
  GtkTextChildAnchor *anchor;
  GtkWidget *swidget;
  smiley *s;

  buf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));
  gtk_text_buffer_get_end_iter (buf, &end);

  if (tag)
    gtk_text_buffer_insert_with_tags_by_name (buf, &end, str, -1, tag, NULL);
  else
    gtk_text_buffer_insert (buf, &end, str, -1);

  mark_start = gtk_text_buffer_get_insert (buf);
  gtk_text_buffer_get_iter_at_mark (buf, &istart, mark_start);
  gtk_text_buffer_get_start_iter (buf, &iend);
  gtk_text_iter_backward_char (&istart);

  if (!smileylist) create_smiley_list ();
  while (smileylist)
  {
    s = (smiley *) smileylist->data;
    while (gtk_text_iter_backward_search
      (&istart, s->str, GTK_TEXT_SEARCH_VISIBLE_ONLY, &istart, &iend, NULL))
    {               
      swidget = gtk_image_new_from_file (s->file);
      gtk_text_buffer_delete (buf, &istart, &iend);
      anchor = gtk_text_buffer_create_child_anchor (buf, &istart);
      gtk_text_view_add_child_at_anchor (GTK_TEXT_VIEW (textview), 
        GTK_WIDGET(swidget), anchor);
      gtk_widget_show (swidget);
    }
    smileylist = smileylist->next;
  }
}

/*
 * add text to the text widget and dx messages to the list
 */
void
maintext_add (gchar msg[], gint len, gint messagetype)
{
  GtkWidget *maintext, *treeview;
  GtkTextIter start, end, istart, iend, cstart, cend;
  GtkTextMark *mark;
  GtkTreeIter iter;
  GtkTreePath *path;
  GtkTreeStore *model;
  GtkTextBuffer *buffer;
  GtkTextChildAnchor *anchor;
  GtkWidget *swidget;
  smiley *s;
  gchar *utf8;

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
  		    dx->freq, DX_COLUMN, dx->dxcall, TIME_COLUMN, dx->time, INFO_COLUMN, 
          dx->info, -1);
        /* remark field may contain foreign language characters */
        if (dx->remark && dx->remark[0] && (utf8 = try_utf8(dx->remark)))
        {
          gtk_tree_store_set (model, &iter, REM_COLUMN, dx->remark, -1);
          g_free (utf8);
        }
        path = gtk_tree_model_get_path (GTK_TREE_MODEL (model), &iter);
        gtk_tree_view_set_cursor (GTK_TREE_VIEW (treeview), path, NULL, FALSE);
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
        {
          gtk_text_buffer_insert_with_tags_by_name (buffer, &end, dx->toall, 
            len, "wwv", NULL); /* should be utf-8 clean */
          if (preferences.savewwv) savewwv (dx->toall);
        }
        else if (!g_ascii_strncasecmp (dx->toall, "WX de ", 5))
        {
          gtk_text_buffer_insert_with_tags_by_name (buffer, &end, dx->toall, 
            len, "wx", NULL); /* should be utf-8 clean */
          if (preferences.savewx) savewx (dx->toall);
        }
        else
        {
          if (dx->toall && dx->toall[0] && (utf8 = try_utf8(dx->toall)))
          {
            if (preferences.savetoall) savetoall (dx->toall);
            iend = end;
            gtk_text_buffer_insert (buffer, &end, utf8, -1);
	    istart = end;
	    gtk_text_iter_backward_char (&istart);
            if (contains_highlights (utf8))
            {
	      if (g_ascii_strcasecmp (preferences.highword1, "?"))
              {
                cstart = istart;
                cend = iend;
                while (gtk_text_iter_backward_search (&cstart, preferences.highword1,
			GTK_TEXT_SEARCH_VISIBLE_ONLY, &cstart, &cend, NULL))
                  gtk_text_buffer_apply_tag_by_name (buffer, "highcolor1", &cstart, &cend);
              }
	      if (g_ascii_strcasecmp (preferences.highword2, "?"))
              {
                cstart = istart;
                cend = iend;
                while (gtk_text_iter_backward_search (&cstart, preferences.highword2,
			GTK_TEXT_SEARCH_VISIBLE_ONLY, &cstart, &cend, NULL))
		  gtk_text_buffer_apply_tag_by_name (buffer, "highcolor2", &cstart, &cend);
              }
	      if (g_ascii_strcasecmp (preferences.highword3, "?"))
              {
                cstart = istart;
                cend = iend;
                while (gtk_text_iter_backward_search (&cstart, preferences.highword3,
			GTK_TEXT_SEARCH_VISIBLE_ONLY, &cstart, &cend, NULL))
		  gtk_text_buffer_apply_tag_by_name (buffer, "highcolor3", &cstart, &cend);
              }
	      if (g_ascii_strcasecmp (preferences.highword4, "?"))
              {
                cstart = istart;
                cend = iend;
                while (gtk_text_iter_backward_search (&cstart, preferences.highword4,
			GTK_TEXT_SEARCH_VISIBLE_ONLY, &cstart, &cend, NULL))
		  gtk_text_buffer_apply_tag_by_name (buffer, "highcolor4", &cstart, &cend);
              }
	      if (g_ascii_strcasecmp (preferences.highword5, "?"))
              {
                cstart = istart;
                cend = iend;
                while (gtk_text_iter_backward_search (&cstart, preferences.highword5,
			GTK_TEXT_SEARCH_VISIBLE_ONLY, &cstart, &cend, NULL))
		  gtk_text_buffer_apply_tag_by_name (buffer, "highcolor5", &cstart, &cend);
              }
	      if (g_ascii_strcasecmp (preferences.highword6, "?"))
              {
                cstart = istart;
                cend = iend;
                while (gtk_text_iter_backward_search (&cstart, preferences.highword6,
			GTK_TEXT_SEARCH_VISIBLE_ONLY, &cstart, &cend, NULL))
		  gtk_text_buffer_apply_tag_by_name (buffer, "highcolor6", &cstart, &cend);
              }
	      if (g_ascii_strcasecmp (preferences.highword7, "?"))
              {
                cstart = istart;
                cend = iend;
                while (gtk_text_iter_backward_search (&cstart, preferences.highword7,
			GTK_TEXT_SEARCH_VISIBLE_ONLY, &cstart, &cend, NULL))
		  gtk_text_buffer_apply_tag_by_name (buffer, "highcolor7", &cstart, &cend);
              }
	      if (g_ascii_strcasecmp (preferences.highword8, "?"))
              {
                cstart = istart;
                cend = iend;
                while (gtk_text_iter_backward_search (&cstart, preferences.highword8,
			GTK_TEXT_SEARCH_VISIBLE_ONLY, &cstart, &cend, NULL))
		  gtk_text_buffer_apply_tag_by_name (buffer, "highcolor8", &cstart, &cend);
              }
            }
            if (contains_smileys (utf8))
	    {
              if (!smileylist) create_smiley_list ();
              cstart = istart;
              cend = iend;
              while (smileylist)
              {
                s = (smiley *) smileylist->data;
                while (gtk_text_iter_backward_search(&cstart, s->str, GTK_TEXT_SEARCH_VISIBLE_ONLY, &cstart, &cend, NULL))
                {               
                  swidget = gtk_image_new_from_file (s->file);
                  gtk_text_buffer_delete (buffer, &cstart, &cend);
                  anchor = gtk_text_buffer_create_child_anchor (buffer, &cstart);
                  gtk_text_view_add_child_at_anchor (GTK_TEXT_VIEW (maintext), GTK_WIDGET(swidget), anchor);
                  gtk_widget_show (swidget);
                }
                smileylist = smileylist->next;
              }
            }
            g_free (utf8);
          }
        }
        mark = gtk_text_buffer_get_mark (buffer, "insert");
        gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(maintext), mark, 0.0, FALSE, 
          0.0, 1.0);
        g_free(dx->toall);
      }
      g_free(dx);
    }
    else if (messagetype == MESSAGE_TX)
    {
      if (msg && msg[0] && (utf8 = try_utf8(msg)))
      {
        if (contains_smileys (utf8))
          insert_with_smileys (GTK_TEXT_VIEW(maintext), utf8, "sent");
        else
          gtk_text_buffer_insert_with_tags_by_name 
            (buffer, &end, utf8, len, "sent", NULL);
        g_free (utf8);
      }
    }
}
