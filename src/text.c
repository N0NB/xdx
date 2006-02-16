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
#include <stdlib.h>
#include "types.h"
#include "utils.h"
#include "text.h"
#include "net.h"
#include "gui.h"
#include "preferences.h"
#include "save.h"
#include "gtksourceiter.h"

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
static gchar* extractinfo(gchar *msg)
{
  gchar *dxmsg, *info, *ret;
  gint l;

  dx = new_dx();
  info = g_strdup(msg);

  if ((dxmsg = strstr(info, "DX de ")) && (info[0] == 'D'))
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
    ret = strstr(dx->toall, "\n");
    if (ret) *ret = '\0';
    dx->dx = FALSE;
    dx->nodx = TRUE;
  }
  
  g_free(info);

  ret = strstr(msg, "\n");
  if (ret)
    return (ret + 1); 
  else
    return NULL;
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
static gchar *
contains_highlights (gchar *str)
{
        gchar *ret = g_strdup ("00000000");
	if (g_ascii_strcasecmp (preferences.highword1, "?")
	&& g_utf8_strcasestr (str, preferences.highword1))
		ret[0] = '1';
	if (g_ascii_strcasecmp (preferences.highword2, "?")
	&& g_utf8_strcasestr (str, preferences.highword2))
		ret[1] = '1';
	if (g_ascii_strcasecmp (preferences.highword3, "?")
	&& g_utf8_strcasestr (str, preferences.highword3))
		ret[2] = '1';
	if (g_ascii_strcasecmp (preferences.highword4, "?")
	&& g_utf8_strcasestr (str, preferences.highword4))
		ret[3] = '1';
	if (g_ascii_strcasecmp (preferences.highword5, "?")
	&& g_utf8_strcasestr (str, preferences.highword5))
		ret[4] = '1';
	if (g_ascii_strcasecmp (preferences.highword6, "?")
	&& g_utf8_strcasestr (str, preferences.highword6))
		ret[5] = '1';
	if (g_ascii_strcasecmp (preferences.highword7, "?")
	&& g_utf8_strcasestr (str, preferences.highword7))
		ret[6] = '1';
	if (g_ascii_strcasecmp (preferences.highword8, "?")
	&& g_utf8_strcasestr (str, preferences.highword8))
		ret[7] = '1';
	return ret;
}

static gboolean 
findp (gunichar ch, gpointer user_data)
{
  switch (ch)
  {
    case ':':
    case 'e':
      return TRUE;
    default:
      return FALSE;
  }
}

/*
 * add text to the text widget and dx messages to the list
 */
void
maintext_add (gchar msg[], gint len, gint messagetype)
{
  GtkWidget *maintext, *treeview;
  GtkTextIter start, end, smatch, ematch;
  GtkTextMark *startmark, *endmark;
  GtkTreeIter iter;
  GtkTreePath *path;
  GtkTreeStore *model;
  GtkTextBuffer *buffer;
  GtkTextChildAnchor *anchor;
  GtkWidget *swidget;
  smiley *s;
  gchar *utf8, *high, *tagname, *p, *temp;
  guint i;

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
      while ((msg = extractinfo(msg)))
      {
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

	if (!GTK_WIDGET_HAS_FOCUS(treeview))
        {
          path = gtk_tree_model_get_path (GTK_TREE_MODEL (model), &iter);
          gtk_tree_view_set_cursor (GTK_TREE_VIEW (treeview), path, NULL, FALSE);
          gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW (treeview), path,
            NULL, TRUE, 0.0, 1.0);
          gtk_tree_path_free (path);
        }

        g_free(dx->spotter);
        g_free(dx->freq);
        g_free(dx->dxcall);
        g_free(dx->remark);
        g_free(dx->time);
        g_free(dx->info);
      }
      if (dx->nodx)  
      {
        if ((!g_ascii_strncasecmp (dx->toall, "WWV de ", 6)
            || !g_ascii_strncasecmp (dx->toall, "WCY de ", 6))
            && (utf8 = try_utf8(dx->toall)))
        {
          gtk_text_buffer_insert_with_tags_by_name
            (buffer, &end, utf8, -1, "wwv", NULL);
          if (preferences.savewwv) savewwv (dx->toall);
        }
        else if (!g_ascii_strncasecmp (dx->toall, "WX de ", 5)
          && (utf8 = try_utf8(dx->toall)))
        {
          gtk_text_buffer_insert_with_tags_by_name
            (buffer, &end, utf8, -1, "wx", NULL);
          if (preferences.savewx) savewx (dx->toall);
        }
        else
        {
          if (dx->toall && dx->toall[0] && (utf8 = try_utf8(dx->toall)))
          {
            if (preferences.savetoall) savetoall (dx->toall);

            /* use textmark to find begin and end of added line */
            startmark = gtk_text_buffer_create_mark (buffer, NULL, &end, TRUE);
            gtk_text_buffer_insert (buffer, &end, utf8, -1);
            gtk_text_buffer_get_bounds (buffer, &start, &end);
            endmark = gtk_text_buffer_create_mark (buffer, NULL, &end, TRUE);

            /* check for DX-cluster prompt and colorize it */
            if (g_utf8_strlen(utf8, -1) > 10)
            {
              temp = g_strdup (utf8);
              *(temp + 10) = '\0';
            if (!strcmp (temp, "To ALL de "))
            {
              gtk_text_buffer_get_iter_at_mark (buffer, &start, startmark);
              gtk_text_buffer_get_iter_at_mark (buffer, &end, startmark);
              if (gtk_text_iter_forward_find_char (&end, findp, NULL, NULL))
              {
                gtk_text_iter_forward_char (&end);
                gtk_text_buffer_apply_tag_by_name (buffer, "prompt", &start, &end);
                start = end;
              }
              if (gtk_text_iter_forward_find_char (&end, findp, NULL, NULL))
              {
                gtk_text_buffer_apply_tag_by_name (buffer, "call", &start, &end);
              }
            }
            }

            /* check for ON4KST prompt and colorize it */
            if (g_utf8_strlen(utf8, -1) > 5)
            {
              temp = g_strdup (utf8);
              if ((*(temp + 5) == ' ') && (*(temp + 4) == 'Z'))
              {
                *(temp + 4) = '\0';
                if ((atoi(temp) != 0) ||(!strcmp (temp, "0000")))
                {
                }
              }
            }

            high = contains_highlights (utf8);
            if (g_ascii_strcasecmp (high, "00000000"))
            for (i = 0; i < 8; i++)
            {
              if (high[i] == '1' && preferences.highmenu[i] == '1')
              {
                /* set starting point for search */
                gtk_text_buffer_get_iter_at_mark (buffer, &start, startmark);
                tagname = g_strdup_printf ("highcolor%d", i + 1);
                if (i == 0) p = g_strdup(preferences.highword1);
                else if (i == 1) p = g_strdup(preferences.highword2);
                else if (i == 2) p = g_strdup(preferences.highword3);
                else if (i == 3) p = g_strdup(preferences.highword4);
                else if (i == 4) p = g_strdup(preferences.highword5);
                else if (i == 5) p = g_strdup(preferences.highword6);
                else if (i == 6) p = g_strdup(preferences.highword7);
                else if (i == 7) p = g_strdup(preferences.highword8);
                else p = g_strdup ("???");
                /* search for highlights and apply tag */
                while (gtk_source_iter_forward_search (&start, p,
                  GTK_SOURCE_SEARCH_CASE_INSENSITIVE, &smatch, &ematch, NULL))
                {
                  gtk_text_buffer_apply_tag_by_name (buffer, tagname, &smatch, &ematch);
                  start = ematch;
                }
                g_free (p);
                g_free (tagname);
              }
            }   
            g_free (high);

            /* search backward for smileys, so we don't go past the end of buffer */
            if (contains_smileys (utf8))
	    {
              if (!smileylist) create_smiley_list ();
              while (smileylist)
              {
                s = (smiley *) smileylist->data;
                gtk_text_buffer_get_iter_at_mark (buffer, &end, endmark);
                while (gtk_text_iter_backward_search (&end, s->str,
                  GTK_TEXT_SEARCH_VISIBLE_ONLY|GTK_TEXT_SEARCH_TEXT_ONLY,
                  &smatch, &ematch, NULL))
                {      
                  swidget = gtk_image_new_from_file (s->file);
                  gtk_text_buffer_delete (buffer, &smatch, &ematch);
                  anchor = gtk_text_buffer_create_child_anchor (buffer, &smatch);
                  gtk_text_view_add_child_at_anchor (GTK_TEXT_VIEW (maintext),
                    GTK_WIDGET(swidget), anchor);
                  gtk_widget_show (swidget);
                  end = smatch;
                }
                smileylist = smileylist->next;
              }
            }
            gtk_text_buffer_delete_mark (buffer, startmark);
            gtk_text_buffer_delete_mark (buffer, endmark);
            g_free (utf8);
          }
        }
	if (!GTK_WIDGET_HAS_FOCUS(maintext))
	{
          gtk_text_buffer_place_cursor(buffer, &end);
          startmark = gtk_text_buffer_get_mark (buffer, "insert");
          gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(maintext), startmark, 0.0, FALSE, 
            0.0, 1.0);
        }
        g_free(dx->toall);
      }
      g_free(dx);
      }
    }
    else if (messagetype == MESSAGE_TX)
    {
      if (msg && msg[0] && (utf8 = try_utf8(msg)))
      {
          gtk_text_buffer_insert_with_tags_by_name 
            (buffer, &end, utf8, len, "sent", NULL);
          g_free (utf8);
      }
    }
}
