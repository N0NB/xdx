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

GPtrArray *dxcc;
GHashTable *prefixes;
gint excitu, exccq, countries;

extern preferencestype preferences;

typedef struct dxinfo 
{
  gchar *spotter;
  gchar *freq;
  gchar *dxcall;
  gchar *remark;
  gchar *time;
  gchar *info;
  gchar *country;
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
  dx->country = NULL;
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
struct info lookup;
	
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
	  
    lookup = lookupcountry_by_callsign(dx->dxcall);
    dxcc_data *d = g_ptr_array_index (dxcc, lookup.country);
    dx->country = g_strdup(d->countryname);
	  
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
    dx->country = NULL;
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

/* used when colorizing DX-cluster prompt */
static gboolean 
findcolonprompt (gunichar ch, gpointer user_data)
{
  switch (ch)
  {
    case ':': /* marks end of prompt */
      return TRUE;
    default:
      return FALSE;
  }
}

/* used when colorizing ON4KST chat prompt */
static gboolean 
findrightarrowprompt (gunichar ch, gpointer user_data)
{
  switch (ch)
  {
    case '>': /* marks end of prompt */
      return TRUE;
    default:
      return FALSE;
  }
}

/* used when colorizing all prompts */
static gboolean 
findpromptspace (gunichar ch, gpointer user_data)
{
  switch (ch)
  {
    case ' ':
      return TRUE;
    default:
      return FALSE;
  }
}

/* play a sound when there is a highlight */
static void playsound (void)
{
  gchar *path = g_build_filename
    (PACKAGE_DATA_DIR, "sounds", "attention.wav", NULL);
  opensound (path);
  g_free (path);
}

/* prompt types */
#define NOTFOUND 0
#define DXCLUSTERNORMALPROMPT 1
#define DXCLUSTERPUBLICPROMPT 2
#define ON4KSTPROMPT 3

/*
 * add text to the text widget and dx messages to the list
 */
void
maintext_add (gchar msg[], gint len, gint messagetype)
{
  GtkWidget *maintext, *treeview;
  GtkTextIter start, end, smatch, ematch;
  GtkTextMark *startmark, *endmark, *promptmark;
  GtkTreeIter iter;
  GtkTreePath *path;
  GtkTreeStore *model;
  GtkTextBuffer *buffer;
  GtkTextChildAnchor *anchor;
  GtkWidget *swidget;
  smiley *s;
  gchar *utf8, *high, *tagname, *p, *temp, *mycall;
  guint i, prompttype = NOTFOUND;

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
          dx->info, COUNTRY_COLUMN, dx->country, -1);
        /* remark field may contain foreign language characters */
        if (dx->remark && dx->remark[0] && (utf8 = try_utf8(dx->remark)))
        {
          gtk_tree_store_set (model, &iter, REM_COLUMN, dx->remark, -1);
          g_free (utf8);
        }


        /* focusing the treeview will stop scrolling */ 
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
	g_free(dx->country);
        g_free(dx->info);
      }
      if (dx->nodx)  
      {
        if ((!g_ascii_strncasecmp (dx->toall, "WWV de ", 6)
            || !g_ascii_strncasecmp (dx->toall, "WCY de ", 6))
            && (utf8 = try_utf8(dx->toall)))
        {
          gtk_text_buffer_insert_with_tags_by_name
            (buffer, &end, utf8, -1, gui->wwvtagname, NULL);
          if (preferences.savewwv) savewwv (dx->toall);
        }
        else if (!g_ascii_strncasecmp (dx->toall, "WX de ", 5)
          && (utf8 = try_utf8(dx->toall)))
        {
          gtk_text_buffer_insert_with_tags_by_name
            (buffer, &end, utf8, -1, gui->wxtagname, NULL);
          if (preferences.savewx) savewx (dx->toall);
        }
        else
        {
          if (dx->toall && dx->toall[0] && (utf8 = try_utf8(dx->toall)))
          {
            if (preferences.savetoall) savetoall (dx->toall);

            /* use textmark to find begin and end of added line */
            startmark = gtk_text_buffer_create_mark (buffer, NULL, &end, TRUE);
            promptmark = gtk_text_buffer_create_mark (buffer, NULL, &end, TRUE);
            gtk_text_buffer_insert (buffer, &end, utf8, -1);
            gtk_text_buffer_get_bounds (buffer, &start, &end);
            endmark = gtk_text_buffer_create_mark (buffer, NULL, &end, TRUE);

            /* check for 'To ALL de PG4I:' DX-cluster prompt and colorize it */
            if (g_utf8_strlen(utf8, -1) > 10)
            {
              temp = g_strdup (utf8);
              *(temp + 10) = '\0';
              if (!strcmp (temp, "To ALL de ") || !strcmp (temp, "To LOCAL d"))
              {
                prompttype = DXCLUSTERPUBLICPROMPT;
                gtk_text_buffer_get_iter_at_mark (buffer, &start, startmark);
                gtk_text_buffer_get_iter_at_mark (buffer, &end, startmark);
                if (gtk_text_iter_forward_find_char (&end, findpromptspace, NULL, NULL))
                  if (gtk_text_iter_forward_find_char (&end, findpromptspace, NULL, NULL))
                    if (gtk_text_iter_forward_find_char (&end, findpromptspace, NULL, NULL))
                    {
                      gtk_text_buffer_apply_tag_by_name (buffer, gui->prompttagname, &start, &end);
                      start = end;
                    }
                 /* locally announce DX-spots have time in prompt between brackets */
                if (index (utf8, '(') && strstr (utf8, "):"))
                {
                  if (gtk_text_iter_forward_find_char (&end, findpromptspace, NULL, NULL))
                  {
                    gtk_text_buffer_apply_tag_by_name (buffer, gui->calltagname, &start, &end);
                    start = end;
                  }
                  if (gtk_text_iter_forward_find_char (&end, findcolonprompt, NULL, NULL))
                  {
                    gtk_text_iter_forward_char (&end); /* forward to colon */
                    gtk_text_buffer_apply_tag_by_name (buffer, gui->prompttagname, &start, &end);
                    /* in case highlighting starts at prompt */
                    promptmark = gtk_text_buffer_create_mark (buffer, NULL, &end, TRUE);
                  }
                }
                else if (gtk_text_iter_forward_find_char (&end, findcolonprompt, NULL, NULL))
                {
                  gtk_text_buffer_apply_tag_by_name (buffer, gui->calltagname, &start, &end);
                  start = end;
                  gtk_text_iter_forward_char (&end); /* forward to colon */
                  gtk_text_buffer_apply_tag_by_name (buffer, gui->prompttagname, &start, &end);
                  /* in case highlighting starts at prompt */
                  promptmark = gtk_text_buffer_create_mark (buffer, NULL, &end, TRUE);
                }
              }
              g_free (temp);
            }

            /*
                normal DX-cluster prompt is something like:
                PG4I de PI5EHV-8 27-Feb-2006 1709Z >
                so we look for 'mycall de ' here...
             */
            if (prompttype == NOTFOUND)
            {
             mycall = g_strdup_printf ("%s de ", preferences.callsign);
             temp = g_strdup (utf8);
             if (g_utf8_strlen (temp, -1) > strlen (preferences.callsign) + 4)
             {
              *(temp + strlen (preferences.callsign) + 4) = '\0';

              if (strcasecmp(temp, mycall) == 0)
              {
                prompttype = DXCLUSTERNORMALPROMPT;
                gtk_text_buffer_get_iter_at_mark (buffer, &start, startmark);
                gtk_text_buffer_get_iter_at_mark (buffer, &end, startmark);
                if (gtk_text_iter_forward_find_char (&end, findrightarrowprompt, NULL, NULL))
                {
                    gtk_text_iter_forward_char (&end);
                    gtk_text_buffer_apply_tag_by_name (buffer, gui->prompttagname, &start, &end);
                    promptmark = gtk_text_buffer_create_mark (buffer, NULL, &end, TRUE);
                }
              }
             }
             g_free (temp);
             g_free (mycall);
            }

            /* check for ON4KST prompt (starts with "1213Z ",
             * where 1213 is current time) and colorize it */
            if (prompttype == NOTFOUND)
            {
             if (g_utf8_strlen(utf8, -1) > 5)
             {
              temp = g_strdup (utf8);
              if ((*(temp + 5) == ' ') && (*(temp + 4) == 'Z'))
              {
                *(temp + 4) = '\0';
                if ((atoi(temp) != 0) ||(!strcmp (temp, "0000")))
                {
                  prompttype = ON4KSTPROMPT;
                  gtk_text_buffer_get_iter_at_mark (buffer, &start, startmark);
                  gtk_text_buffer_get_iter_at_mark (buffer, &end, startmark);
                  if (gtk_text_iter_forward_find_char (&end, findpromptspace, NULL, NULL))
                  {
                    gtk_text_buffer_apply_tag_by_name (buffer, gui->prompttagname, &start, &end);
                    start = end;
                  }
                  if (gtk_text_iter_forward_find_char (&end, findpromptspace, NULL, NULL))
                  {
                    gtk_text_buffer_apply_tag_by_name (buffer, gui->calltagname, &start, &end);
                    start = end;
                  }
                  if (gtk_text_iter_forward_find_char (&end, findrightarrowprompt, NULL, NULL))
                  {
                    gtk_text_iter_forward_char (&end);
                    gtk_text_buffer_apply_tag_by_name (buffer, gui->prompttagname, &start, &end);
                    promptmark = gtk_text_buffer_create_mark (buffer, NULL, &end, TRUE);
                  }
                }
              }
              g_free (temp);
             }
            }

            /* check for highlights, before or after the prompt */
            gtk_text_buffer_get_iter_at_mark (buffer, &start, startmark);
            gtk_text_buffer_get_iter_at_mark (buffer, &end, endmark);
            high = contains_highlights
              (gtk_text_buffer_get_text (buffer, &start, &end, FALSE));
            if (g_ascii_strcasecmp (high, "00000000"))
            {
             for (i = 0; i < 8; i++)
             {
              if (high[i] == '1')
              {
                /* lookup name of tag and word to be highlighted */
                if (i == 0)
                {
                  tagname = g_strdup (gui->high1tagname);
                  p = g_strdup(preferences.highword1);
                }
                else if (i == 1)
                {
                  tagname = g_strdup (gui->high2tagname);
                  p = g_strdup(preferences.highword2);
                }
                else if (i == 2)
                {
                  tagname = g_strdup (gui->high3tagname);
                  p = g_strdup(preferences.highword3);
                }
                else if (i == 3)
                {
                  tagname = g_strdup (gui->high4tagname);
                  p = g_strdup(preferences.highword4);
                }
                else if (i == 4)
                {
                  tagname = g_strdup (gui->high5tagname);
                  p = g_strdup(preferences.highword5);
                }
                else if (i == 5)
                {
                  tagname = g_strdup (gui->high6tagname);
                  p = g_strdup(preferences.highword6);
                }
                else if (i == 6)
                {
                  tagname = g_strdup (gui->high7tagname);
                  p = g_strdup(preferences.highword7);
                }
                else if (i == 7)
                {
                  tagname = g_strdup (gui->high8tagname);
                  p = g_strdup(preferences.highword8);
                }
                else
                {
                  p = g_strdup ("???");
                  tagname = g_strdup ("???");
                }
                /* set starting point for search */
		if (preferences.highmenu[i] == '0')
                  gtk_text_buffer_get_iter_at_mark (buffer, &start, promptmark);
                else
                  gtk_text_buffer_get_iter_at_mark (buffer, &start, startmark);
                /* search for highlights and apply tag */
                while (gtk_source_iter_forward_search (&start, p,
                  GTK_SOURCE_SEARCH_CASE_INSENSITIVE, &smatch, &ematch, NULL))
                {
                  /* we can't tag an already tagged textpart */
                  gtk_text_buffer_remove_all_tags (buffer, &smatch, &ematch);
                  gtk_text_buffer_apply_tag_by_name (buffer, tagname, &smatch, &ematch);
                  start = ematch;
                  if (preferences.playsound == 1) playsound ();
                }
                g_free (p);
                g_free (tagname);
              }
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
            gtk_text_buffer_delete_mark (buffer, promptmark);
            gtk_text_buffer_delete_mark (buffer, endmark);
            g_free (utf8);
          }
        }
        /* focusing (clicking) the textview will stop scrolling */
	if (!GTK_WIDGET_HAS_FOCUS(maintext))
	{
          gtk_text_buffer_get_bounds (buffer, &start, &end);
          gtk_text_buffer_place_cursor(buffer, &end);
          endmark = gtk_text_buffer_create_mark (buffer, NULL, &end, TRUE);
          gtk_text_view_scroll_to_mark
            (GTK_TEXT_VIEW(maintext), endmark, 0.0, FALSE, 0.0, 1.0);
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
            (buffer, &end, utf8, len, gui->senttagname, NULL);
          g_free (utf8);
      }
    }
}

/* ---- */


/* replace callsign area (K0AR/2 -> K2AR) so we can do correct lookups */
static gchar *
change_area (gchar *callsign, gint area)
{
	gchar *end, *j;

	end = callsign + strlen (callsign);
	for (j = callsign; j < end; ++j)
	{
		switch (*j)
		{
			case '0' ... '9':
			if ((j - callsign) > 1)
				*j = area + 48;
			break;
		}
	}

	return(g_strdup(callsign));
}

/*
   extract prefix from a callsign with a forward slash:
   - check if callsign has a '/'
   - replace callsign area's (K0AR/2 -> K2AR)
   - skip /mm, /am and /qrp
   - return string after slash if it is shorter than string before
 */
static gchar *
getpx (gchar *checkcall)
{

	gchar *pxstr = NULL, **split;

	/* characters after '/' might contain a country */
	if (strchr(checkcall, '/'))
	{
		split = g_strsplit(checkcall, "/", 2);
		if (split[1]) /* we might be typing */
		{
			if ((strlen(split[1]) > 1) && (strlen(split[1]) < strlen(split[0])))
			/* this might be a candidate */
			{
				if ((g_ascii_strcasecmp(split[1], "AM") == 0)
					|| (g_ascii_strcasecmp(split[1], "MM") == 0))
					pxstr = NULL; /* don't know location */
				else if (g_ascii_strcasecmp(split[1], "QRP") == 0)
					pxstr = g_strdup(split[0]);
				else pxstr = g_strdup(split[1]);
			}
			else if ((strlen(split[1]) == 1) &&
				split[1][0] >= '0' && split[1][0] <= '9')
			/* callsign area changed */
			{
				pxstr = change_area(split[0], atoi(split[1]));
			}
			else pxstr = g_strdup(split[0]);
		}
		else pxstr = g_strdup(split[0]);
		g_strfreev(split);
	}
	else
		pxstr = g_strdup(checkcall);

	return (pxstr);
}


/* parse an exception and extract the CQ and ITU zone */
static gchar *
findexc(gchar *exception)
{
	gchar *end, *j;

	excitu = 0;
	exccq = 0;
	end = exception + strlen (exception);
	for (j = exception; j < end; ++j)
	{
		switch (*j)
		{
			case '(':
				if (*(j+2) == 41)
					exccq = *(j+1) - 48;
				else if (*(j+3) == 41)
					exccq = ((*(j+1) - 48) * 10) + (*(j+2) - 48);
			case '[':
				if (*(j+2) == 93)
					excitu = *(j+1) - 48;
				else if (*(j+3) == 93)
					excitu = ((*(j+1) - 48) * 10) + (*(j+2) - 48);
			case ';':
				*j = '\0';
			break;
		}
	}
	return (exception);
}






/*
 * go through exception string and stop when end of prefix
 * is reached (BT3L(23)[33] -> BT3L)
 */
static gchar *
findpfx_in_exception (gchar * pfx)
{
	gchar *end, *j;

	g_strstrip (pfx);
	end = pfx + strlen (pfx);
	for (j = pfx; j < end; ++j)
	{
		switch (*j)
		{
		case '(':
		case '[':
		case ';':
			*j = '\0';
			break;
		}
	}
	return pfx;
}


/*
 * go through the hashtable with the current callsign and return the country number
 * cq zone and itu zone - this also goes through the exceptionlist
 */
struct info
lookupcountry_by_callsign (gchar * callsign)
{
	gint ipx, iexc;
	gchar *px;
	gchar **excsplit, *exc;
	gchar *searchpx = NULL;
	struct info lookup;

	lookup.country = 0;
	/* first check complete callsign */
	lookup.country = GPOINTER_TO_INT(g_hash_table_lookup (prefixes, callsign));

	if (lookup.country == 0 && (px = getpx (callsign)))
	{	/* start with full callsign and truncate it until a correct lookup */
		for (ipx = strlen (px); ipx > 0; ipx--)
		{
			searchpx = g_strndup (px, ipx);
			lookup.country = GPOINTER_TO_INT
				(g_hash_table_lookup (prefixes, searchpx));
			if (lookup.country > 0) break;
		}
		g_free (px);
	}
	else
		searchpx = g_strdup (callsign);

	dxcc_data *d = g_ptr_array_index (dxcc, lookup.country);
	lookup.itu = d -> itu;
	lookup.cq = d -> cq;

	/* look for CQ/ITU zone exceptions */
	if (strchr(d->exceptions, '(') || strchr(d->exceptions, '['))
	{
		excsplit = g_strsplit (d->exceptions, ",", -1);
		for (iexc = 0 ;; iexc++)
		{
			if (!excsplit[iexc]) break;
			exc = findexc (excsplit[iexc]);
			if (g_ascii_strcasecmp (searchpx, exc) == 0)
			{
				if (excitu > 0) lookup.itu = excitu;
				if (exccq > 0) lookup.cq = exccq;
			}
		}
		g_strfreev(excsplit);
	}
	return lookup;
}



/* add an item from cty.dat to the dxcc array */
static void
dxcc_add (gchar *c, gint w, gint i, gchar *cont, gint lat, gint lon,
	gint tz, gchar *p, gchar *e)
{
	dxcc_data *new_dxcc = g_new (dxcc_data, 1);

	new_dxcc -> countryname = g_strdup (c);
	new_dxcc -> cq = w;
	new_dxcc -> itu = i;
	new_dxcc -> continent = g_strdup (cont);
	new_dxcc -> latitude = lat;
	new_dxcc -> longitude = lon;
	new_dxcc -> timezone = tz;
	new_dxcc -> px = g_strdup (p);
	new_dxcc -> exceptions = g_strdup (e);
	g_ptr_array_add (dxcc, new_dxcc);
}

/* fill the hashtable with all of the prefixes from cty.dat */
gint
readctydata (void)
{

	gchar buf[4096], *cty_location, *pfx, **split, **pfxsplit, *excstr;
	gint ichar = 0, dxccitem = 0, ipfx = 0, ch = 0;
	FILE *fp;
	
	cty_location = g_strdup_printf ("%s%s%s", PACKAGE_DATA_DIR, G_DIR_SEPARATOR_S, "cty.dat");

	if ((fp = fopen (cty_location, "r")) == NULL)
	{
		printf("Cannot find cty.dat in %s !\n",cty_location);
		g_free (cty_location);
		return (1);
	}

	dxcc = g_ptr_array_new ();
	prefixes = g_hash_table_new_full (g_str_hash, g_str_equal,
		(GDestroyNotify)g_free, NULL);

	/* first field in case hash_table_lookup returns NULL */
	dxcc_add ("Unknown", 0, 0, "--", 0, 0, 0, "", "");
	countries = 1;

	while (!feof (fp))
	{
		while (ch != 59)
		{
			ch = fgetc (fp);
			if (ch == EOF) break;
			buf[ichar++] = ch;
		}
		if (ch == EOF)
		break;
		buf[ichar] = '\0';
		ichar = 0;
		ch = 0;

			/* split up the first line */
		split = g_strsplit (buf, ":", 9);

		if (!g_strrstr (split[7], "*")) /* ignore WAE countries */
		{
		for (dxccitem = 0; dxccitem < 9; dxccitem++)
			g_strstrip (split[dxccitem]);
		/* split up the second line */
		excstr = my_strreplace (split[8], "\r\n", "");
		excstr = my_strreplace (excstr, "    ", "");
		excstr = my_strreplace (excstr, ";", "");
		pfxsplit = g_strsplit (excstr, ",", 0);

		dxcc_add (split[0], atoi(split[1]), atoi(split[2]), split[3],
			(gint)(strtod(split[4], NULL) * 100), (gint)(strtod(split[5], NULL) * 100),
			(gint)(strtod(split[6], NULL) * 10), split[7], excstr);
		g_free (excstr);

		/* official prefix */
		g_hash_table_insert (prefixes, g_strdup (split[7]),
			GINT_TO_POINTER (countries));

		/* exception list */
		for (ipfx = 0;; ipfx++)
		{
			if (!pfxsplit[ipfx]) break;
			pfx = findpfx_in_exception (pfxsplit[ipfx]);
			if (g_ascii_strcasecmp(pfx, split[7]) != 0)
				g_hash_table_insert (prefixes, g_strdup (pfx),
			GINT_TO_POINTER (countries));
		}

		g_strfreev (pfxsplit);
		g_strfreev (split);
		countries++;
		}
	}
	fclose (fp);
	g_free (cty_location);
	return (0);
}

/* free memory used by the dxcc array */
void
cleanup_dxcc (void)
{
	gint i;

	/* free the dxcc array */
	if (dxcc)
	{
		for (i = 0; i < dxcc->len; i++)
		{
			dxcc_data *d = g_ptr_array_index (dxcc, i);
			g_free (d->countryname);
			g_free (d->continent);
			g_free (d->px);
			g_free (d->exceptions);
			g_free (d);
		}
		g_ptr_array_free (dxcc, TRUE);
	}
	if (prefixes) g_hash_table_destroy (prefixes);
}

