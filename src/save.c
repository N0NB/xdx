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
 * save.c - saving cluster information to harddisk
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "save.h"
#include "gui.h"
#include "utils.h"

void savedx (gchar *dx)
{
  gchar *savedxfile, *d, *t;
  FILE *fp;

  savedxfile = g_strdup_printf ("%s/dxspots", gui->preferencesdir);
  fp = fopen (savedxfile, "a");
  if (fp)
  {
    d = getdate (TRUE);
    t = gettime (TRUE);
    fprintf (fp, "%s %s GMT - %s", d, t, dx);
    g_free (t);
    g_free (d);
    fclose (fp);
  }
  g_free (savedxfile);
}

/* in lines that look like this:
  "SFI=75, A=2, K=2, R= 13" 
  "K=0 expK=0 A=4 R=13 SFI=75"
  extract info after the '=' and save it
*/
static void appendwwvinfo (FILE *fpointer, gchar *item, gchar *line)
{
  gchar *copy = g_strdup (line);
  gchar *j, *tmp = NULL;
  gint i = 0;

  copy = my_strreplace (copy, "= ", "=");
  tmp = strstr (copy, item);
  if (tmp)
  {
    for (j = tmp; ; ++j)
    {
      i++;
      if (*j == ',')
      {
        *j = '\0';
        break;
      }
      else 
      if (*j == ' ')
      {
        *j = '\0';
        break;
      }
    }
    /* use atoi to nuke spaces */
    fprintf (fpointer, "\t%s", tmp + strlen(item));
  }
}

void savewwv (gchar *wwv)
{
  gchar *wwvfile, *d, *t, *tmp, *ind;
  FILE *fp;

  wwvfile = g_strdup_printf ("%s/wwv", gui->preferencesdir);
  fp = fopen (wwvfile, "a");
  if (fp)
  {
    d = getdate (TRUE);
    t = gettime (TRUE);
    fprintf (fp, "%s %s GMT - %s", d, t, wwv);
    g_free (t);
    g_free (d);
    fclose (fp);
  }
  /* extract wwv hostname and save to seperate file for every host */
  tmp = g_strdup (wwv + 7);
  ind = index (tmp, ' ');
  *ind = '\0';
  wwvfile = g_strdup_printf ("%s/%s.tsv", gui->preferencesdir, tmp);
  g_free (tmp);
  fp = fopen (wwvfile, "a");
  if (fp)
  {
   /* non-formatted use for saving tsv wwv info */
    d = getdate (FALSE);
    t = gettime (FALSE);
    fprintf (fp, "%s%s", d, t);
    g_free (t);
    g_free (d);
    appendwwvinfo (fp, "SFI=", wwv);
    appendwwvinfo (fp, "A=", wwv);
    appendwwvinfo (fp, "K=", wwv);
    appendwwvinfo (fp, "R=", wwv);
    fprintf (fp, "\n");
    fclose (fp);
  }
  g_free (wwvfile);
}

void savetoall (gchar *toall)
{
  gchar *toallfile, *d, *t;
  FILE *fp;

  toallfile = g_strdup_printf ("%s/toall", gui->preferencesdir);
  fp = fopen (toallfile, "a");
  if (fp)
  {
    d = getdate (TRUE);
    t = gettime (TRUE);
    fprintf (fp, "%s %s GMT - %s", d, t, toall);
    g_free (t);
    g_free (d);
    fclose (fp);
  }
  g_free (toallfile);
}

void savewx (gchar *wx)
{
  gchar *wxfile, *d, *t;
  FILE *fp;

  wxfile = g_strdup_printf ("%s/wx", gui->preferencesdir);
  fp = fopen (wxfile, "a");
  if (fp)
  {
    d = getdate (TRUE);
    t = gettime (TRUE);
    fprintf (fp, "%s %s GMT - %s", d, t, wx);
    g_free (t);
    g_free (d);
    fclose (fp);
  }
  g_free (wxfile);
}
