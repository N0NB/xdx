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
 * history.c - private functions for handling connect history
 */

#include <stdio.h>
#include <gtk/gtk.h>
#include "utils.h"
#include "history.h"
#include "gui.h"

#define TXHISTORY 10

/*
 * recall history and copy into the appropriate GList
 */

void
loadhistory (void)
{
  gchar *historyfile, history[128], **histsplit;
  FILE *fp;

  historyfile = g_strdup_printf ("%s/history", gui->preferencesdir);
  fp = fopen (historyfile, "r");
  if (fp == NULL) return;

  while (!feof (fp))
    {
      if (fscanf (fp, "%s", history) == EOF)
	break;
      histsplit = g_strsplit(history, ":", -1);
      if (!g_ascii_strncasecmp (history, "ho", 2))
	gui->hostnamehistory =
	  g_list_append (gui->hostnamehistory, g_strdup(histsplit[1]));
      else if (!g_ascii_strncasecmp (history, "po", 2))
	gui->porthistory = g_list_append (gui->porthistory, g_strdup(histsplit[1]));
      g_strfreev(histsplit);
    }
  fclose (fp);
  g_free(historyfile);
}

/*
 * save history to ~/.xdx/history for the hostname combobox and port combobox
 */

void
savehistory (void)
{
  gchar *historyfile;
  FILE *fp;
  guint i, n;
  GList *link;

  historyfile = g_strdup_printf ("%s/history", gui->preferencesdir);
  fp = fopen (historyfile, "w");
  if (fp == NULL) return;

      if ((n = g_list_length (gui->hostnamehistory)) > 0)
	{
	  for (i = 0; i < n; i++)
	    {
	      link = g_list_nth (gui->hostnamehistory, i);
	      if (link) fprintf (fp, "ho:%s\n", (gchar *)link->data);
	    }
	}

      if ((n = g_list_length (gui->porthistory)) > 0)
	{
	  for (i = 0; i < n; i++)
	    {
	      link = g_list_nth (gui->porthistory, i);
	      if (link) fprintf (fp, "po:%s\n", (gchar *)link->data);
	    }
	}

  fclose (fp);
  g_free(historyfile);
}

void tx_save(GString *txmsg)
{
  gui->txhistory = g_list_append(gui->txhistory, g_strdup(txmsg->str));
  if (g_list_length(gui->txhistory) > TXHISTORY)
    gui->txhistory = g_list_remove(gui->txhistory, g_list_first(gui->txhistory)->data);
  else gui->txitem++; 
  gui->updown = 0;
}

void tx_previous(void)
{
  GtkWidget *mainentry;
  GtkTextBuffer *entrybuffer;
  GtkTextIter end;
  gchar *str;

  if (gui->updown < gui->txitem) gui->updown++;
  if (gui->txhistory)
  {
    str = g_list_nth_data(gui->txhistory, (gui->txitem) - (gui->updown));
    mainentry = g_object_get_data (G_OBJECT (gui->window), "mainentry");
    entrybuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (mainentry));
    gtk_text_buffer_set_text (entrybuffer, str, -1);
    gtk_text_buffer_get_end_iter (entrybuffer, &end);
    gtk_text_buffer_place_cursor (entrybuffer, &end);
  }
}

void tx_next(void)
{
  GtkWidget *mainentry;
  GtkTextBuffer *entrybuffer;
  GtkTextIter end;
  gchar *str;

  if (gui->updown > 0) gui->updown--;

  mainentry = g_object_get_data (G_OBJECT (gui->window), "mainentry");
  entrybuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (mainentry));

  if (gui->updown == 0)
    gtk_text_buffer_set_text (entrybuffer, "", 0);
  else
  {
    str = g_list_nth_data(gui->txhistory, (gui->txitem) - (gui->updown));
    gtk_text_buffer_set_text (entrybuffer, str, -1);
    gtk_text_buffer_get_end_iter (entrybuffer, &end);
    gtk_text_buffer_place_cursor (entrybuffer, &end);
  }
}
