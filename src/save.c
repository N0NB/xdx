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
 * save.c - saving cluster information to harddisk
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <time.h>
#include "save.h"
#include "gui.h"


static gchar *getdate (void)
{
  time_t current;
  struct tm *timestruct = NULL;
  gchar datenow[20];
 
  time (&current);
  timestruct = localtime (&current);
  strftime (datenow, 20, "%Y-%m-%d", timestruct);
  return (g_strdup(datenow));
}


static gchar *gettime (void)
{
  time_t current;
  struct tm *timestruct = NULL;
  gchar stimenow[20];

  time (&current);
  timestruct = localtime (&current);
  strftime (stimenow, 20, "%T", timestruct);
  return (g_strdup (stimenow));
}


void savedx (gchar *dx)
{
  gchar *savedxfile, *d, *t;
  FILE *fp;

  savedxfile = g_strdup_printf ("%s/dxspots", gui->preferencesdir);
  fp = fopen (savedxfile, "a");
  if (fp)
  {
    d = getdate ();
    t = gettime ();
    fprintf (fp, "%s %s GMT - %s", d, t, dx);
    g_free (t);
    g_free (d);
    fclose (fp);
  }
  g_free (savedxfile);
}

void savewwv (gchar *wwv)
{
  gchar *wwvfile, *d, *t;
  FILE *fp;

  wwvfile = g_strdup_printf ("%s/wwv", gui->preferencesdir);
  fp = fopen (wwvfile, "a");
  if (fp)
  {
    d = getdate ();
    t = gettime ();
    fprintf (fp, "%s %s GMT - %s", d, t, wwv);
    g_free (t);
    g_free (d);
    fclose (fp);
  }
  g_free (wwvfile);
}
