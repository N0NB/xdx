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
 *  text.h
 */


#ifndef XDX_TEXT_H
#define XDX_TEXT_H

typedef struct
{
	gchar *countryname;
	guchar cq;              /* guchar max=255 */
	guchar itu;
	gchar *continent;
	gint latitude;
	gint longitude;
	gshort timezone;
	gchar *px;
	gchar *exceptions;
	guint worked;
	guint confirmed;
}
dxcc_data;

struct info
{
	gint country;
	gint cq;
	gint itu;
};

void cleanup_dxcc (void);
gint readctydata (void);
struct info lookupcountry_by_callsign (gchar * callsign);

void maintext_add (gchar * msg, gint len, gint messagetype);

#endif /* XDX_TEXT_H */
