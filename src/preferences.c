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
 * preferences.c - private functions for saving and recalling xdx preferences.
 */

#include <gtk/gtk.h>
#include <stdio.h>
#if HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
#include "preferences.h"
#include "utils.h"
#include "gui.h"

preferencestype preferences;

/*
 * check if ~/.xdx directory exists
 */

void
dircheck ()
{
  struct stat statdir;

  gui->preferencesdir = g_strdup_printf ("%s/.%s", g_get_home_dir (), PACKAGE);
  if (stat (gui->preferencesdir, &statdir) == -1)
    {
      if (mkdir (gui->preferencesdir, S_IRUSR | S_IWUSR | S_IXUSR) == -1)
	g_error (_("Creating ~/.%s directory."), PACKAGE);
    }
  else if (!S_ISDIR (statdir.st_mode))
    g_error (_("~/.%s is not a directory."), PACKAGE);
}

/*
 * look up settings in ~/.xdx/preferences
 */

void
loadpreferences (void)
{
  gchar *preferencesfile, label[100], value[100];
  FILE *fp;

  /* defaults */
  preferences.x = 10;
  preferences.y = 30;
  preferences.width = 700;
  preferences.height = 550;
  preferences.columnwidths = g_strdup("70,70,70,360,60,60,");
  preferences.autologin = 0;
  preferences.autoreconnect = 0;
  preferences.callsign = g_strdup("N0CALL");
  preferences.savedx = 0;

  /* open preferences file */
  preferencesfile = g_strdup_printf ("%s/preferences", gui->preferencesdir);
  fp = fopen (preferencesfile, "r");
  if (fp)
  {
    while (!feof (fp))
      {
        if (fscanf (fp, "%s %s", label, value) == EOF) break;
	      if (!g_strcasecmp(label, "x")) 
          preferences.x = atoi(value);
        else if (!g_strcasecmp(label, "y")) 
          preferences.y = atoi(value);
        else if (!g_strcasecmp(label, "width")) 
          preferences.width = atoi(value);
        else if (!g_strcasecmp(label, "height")) 
          preferences.height = atoi(value);
        else if (!g_strcasecmp(label, "columnwidths")) 
          preferences.columnwidths = g_strdup(value);
        else if (!g_strcasecmp(label, "autologin")) 
          preferences.autologin = atoi(value);
        else if (!g_strcasecmp(label, "autoreconnect")) 
          preferences.autoreconnect = atoi(value);
        else if (!g_strcasecmp(label, "callsign")) 
          preferences.callsign = g_strdup(value);
        else if (!g_strcasecmp(label, "savedx")) 
          preferences.savedx = atoi(value);
      }
    fclose (fp);
  }
  g_free(preferencesfile);
}

/*
 * save preferences in ~/.xdx/preferences
 */

void
savepreferences (void)
{
  gchar *preferencesfile, *str;
  FILE *fp;

  /* open preferences file */
  preferencesfile = g_strdup_printf ("%s/preferences", gui->preferencesdir);
  fp = fopen (preferencesfile, "w");
  if (fp)
  {
    fprintf (fp, "version %s\n", VERSION);
    str = g_strdup_printf("%d", preferences.x);
    fprintf(fp, "x %s\n", str);
    str = g_strdup_printf("%d", preferences.y);
    fprintf(fp, "y %s\n", str);
    str = g_strdup_printf("%d", preferences.width);
    fprintf(fp, "width %s\n", str);
    str = g_strdup_printf("%d", preferences.height);
    fprintf(fp, "height %s\n", str);
    str = g_strdup_printf("%s", preferences.columnwidths);
    fprintf(fp, "columnwidths %s\n", str);
    str = g_strdup_printf("%d", preferences.autologin);
    fprintf(fp, "autologin %s\n", str);
    str = g_strdup_printf("%d", preferences.autoreconnect);
    fprintf(fp, "autoreconnect %s\n", str);
    str = g_strdup_printf("%s", preferences.callsign);
    fprintf(fp, "callsign %s\n", str);
    str = g_strdup_printf("%d", preferences.savedx);
    fprintf(fp, "savedx %s\n", str);
    g_free(str);
    fclose (fp);
  }
  g_free(preferencesfile);
}
