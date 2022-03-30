/*
 *  xdx - GTK+ DX-cluster client for amateur radio
 *  Copyright (C) 2002-2006 Joop Stakenborg <pg4i@amsat.org>
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
 * utils.h
 */


#ifndef XDX_UTILS_H
#define XDX_UTILS_H

void add_pixmap_directory(const gchar *directory);
void updatestatusbar(GString *statusmessage, gboolean timeout);

gboolean openurl(const char *url);
gboolean openmail(const char *url);
void opensound(const char *file);
gchar *try_utf8(const gchar *str);
gchar *xdxgetdate(gboolean formatted);
gchar *xdxgettime(gboolean formatted);
void logconnection(GString *logstr);
gchar *my_strreplace(const char     *str,
                     const char     *delimiter,
                     const char     *replacement);

#endif /* XDX_UTILS_H */
