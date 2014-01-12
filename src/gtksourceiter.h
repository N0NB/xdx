/*
 * xdx - GTK+ DX-cluster client for amateur radio
 * Copyright (C) 2002-2006 Joop Stakenborg <pg4i@amsat.org>
 *
 * Taken from the gtksourceview source tree with the following copyrights:
 * Copyright (C) 2000 - 2005 Paolo Maggi
 * Copyright (C) 2002, 2003 Jeroen Zwartepoorte
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


#ifndef __GTK_SOURCE_ITER_H__
#define __GTK_SOURCE_ITER_H__

G_BEGIN_DECLS

typedef enum
{
	GTK_SOURCE_SEARCH_VISIBLE_ONLY		 = 1 << 0,
	GTK_SOURCE_SEARCH_TEXT_ONLY		 = 1 << 1,
	GTK_SOURCE_SEARCH_CASE_INSENSITIVE	 = 1 << 2
	/* Possible future plans: SEARCH_REGEXP */
} GtkSourceSearchFlags;

const gchar *
g_utf8_strcasestr (const gchar *haystack, const gchar *needle);

gboolean gtk_source_iter_forward_search 	(const GtkTextIter   *iter,
						 const gchar         *str,
						 GtkSourceSearchFlags flags,
						 GtkTextIter         *match_start,
						 GtkTextIter         *match_end,
						 const GtkTextIter   *limit);

gboolean gtk_source_iter_backward_search 	(const GtkTextIter   *iter,
						 const gchar         *str,
						 GtkSourceSearchFlags flags,
						 GtkTextIter         *match_start,
						 GtkTextIter         *match_end,
						 const GtkTextIter   *limit);

gboolean gtk_source_iter_find_matching_bracket 	(GtkTextIter         *iter);

G_END_DECLS

#endif /* __GTK_SOURCE_ITER_H__ */
