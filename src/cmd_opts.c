/*
 *  xdx - GTK+ DX-cluster client for amateur radio
 *  Copyright (C) 2014 Nate Bargmann N0NB <n0nb@n0nb.us>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
 *  cmd_opts.c - Parse command line options.
 */


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <getopt.h>
#include <stdlib.h>

#include <glib/gprintf.h>

#include "cmd_opts.h"


void
usage(FILE  *stream,
      gchar *my_name)
{
    g_fprintf(stream, "Usage: %s [options]\n\n", my_name);
    g_fprintf(stream,
              "  -c --cty_dat cty.dat      Path to cty.dat file\n"
              "  -h --help                 Display this usage information.\n"
              "  -V --version              Print %s version\n",
              PACKAGE_NAME);
}


void
version(FILE *stream)
{
    g_fprintf(stream, "%s DX Cluster client\n"
              "Copyright (C) 2002-2006 Joop Stakenborg <pg4i@amsat.org>\n"
              "Copyright (C) 2014 Nate Bargmann <n0nb@n0nb.us>\n\n"
              "%s comes with ABSOLUTELY NO WARRANTY.\n"
              "This is free software, and you are welcome to redistribute it\n"
              "under certain conditions.  See the file COPYING for details.\n\n",
              PACKAGE_NAME, PACKAGE_NAME);
}


void
parse_opts(int    *argc,
           char ***argv)
{
    int next_opt = 0;
    gchar *my_name;
    extern gchar *opt_cty_path;

    /* Valid short options. */
    const char *const s_opts = "c:hV";

    /* Valid long options. */
    const struct option l_opts[] = {
        { "cty_dat",    required_argument,  NULL, 'c' },
        { "help",       no_argument,        NULL, 'h' },
        { "verbose",    no_argument,        NULL, 'V' },
        { NULL,         0,                  NULL, 0 }
    };

    my_name = g_strdup_printf("%s", *argv[0]);

    while (next_opt != -1) {
        next_opt = getopt_long(*argc, *argv, s_opts, l_opts, NULL);

        switch (next_opt) {
            case 'c':
                opt_cty_path = g_strdup_printf("%s", optarg);
                break;

            case 'h':
                usage(stdout, my_name);
                exit(EXIT_SUCCESS);

            case 'V':
                version(stdout);
                exit(EXIT_SUCCESS);

            case '?':
                version(stderr);
                usage(stderr, my_name);
                exit(EXIT_FAILURE);

            case -1:
                break;

            default:
                abort();        /* Major Oops! */
        }
    }

    g_free(my_name);
}
