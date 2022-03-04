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
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
 * hyperlink.c - clicking on links
 */


#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "gui.h"
#include "hyperlink.h"
#include "utils.h"


/*
 * count number of dots in a link
 */
static gboolean
linkcontains2dots(gchar *link)
{
    gint dots = 0, i = 0;
    gchar *linktocheck, *end, *j, **split;
    gboolean toshort = FALSE;

    linktocheck = g_strdup(link);
    end = linktocheck + strlen(linktocheck);

    for (j = linktocheck; j < end; ++j) {
        switch (*j) {
            case '.':
            case '@':
            case '/':
                dots++;
                break;
        }
    }

    g_free(linktocheck);

    if (dots < 1) return FALSE;

    split = g_strsplit(link, ".", -1);

    for (;;) {
        if (split[i] == NULL) break;

        if (strlen(split[i]) < 2) toshort = TRUE;

        i++;
    }

    if (toshort) return FALSE;

    return TRUE;
}


/*
 * check if link
 */
static gboolean
islink(gchar *link)
{
    if (g_strrstr(link, "\r"))
        return FALSE;

    if (g_strrstr(link, " "))
        return FALSE;
    else if (!g_strrstr(link, "."))
        return FALSE;
    else if (g_strrstr(link, ".."))
        return FALSE;
    else if (!g_ascii_strncasecmp(link, "http://", 7))
        return TRUE;
    else if (!g_ascii_strncasecmp(link, "www.", 4))
        return TRUE;
    else if (!g_ascii_strncasecmp(link, "ftp://", 7))
        return TRUE;
    else if (!g_ascii_strncasecmp(link, "ftp.", 7))
        return TRUE;
    else if (linkcontains2dots(link))
        return TRUE;

    return FALSE;
}


/*
 * used by set_cursor to find begin/end of a word
 */
static gboolean
findw(gunichar ch,
      gpointer user_data)
{
    switch (ch) {
        case ' ':
        case ',':
        case ';':
        case '\n':
        case '\r':
            return TRUE;

        default:
            return FALSE;
    }
}


/*
 * change cursor depending on whether we have a link or not
 */
static void
set_cursor(GtkTextView *text_view,
           gint         x,
           gint         y)
{
    GtkTextBuffer *buffer;
    GtkTextIter iter, startword, endword, start, end;
    GdkCursor *hand_cursor, *normal_cursor;
    GdkDisplay *display;
    gchar *word;

    buffer = gtk_text_view_get_buffer(text_view);
    gtk_text_view_get_iter_at_location(text_view, &iter, x, y);

    display = gdk_display_get_default();
    hand_cursor = gdk_cursor_new_from_name(display, "pointer");
    normal_cursor = gdk_cursor_new_from_name(display, "text");

    startword = iter;
    endword = iter;

    if (gtk_text_iter_forward_find_char(&endword, findw, NULL, NULL) &&
            gtk_text_iter_backward_find_char(&startword, findw, NULL, NULL)) {
        gtk_text_iter_forward_char(&startword);  /* advance one char */
        word = gtk_text_buffer_get_slice(buffer, &startword, &endword, FALSE);

        if (word && islink(word)) {
            gdk_window_set_cursor(gtk_text_view_get_window
                                  (text_view, GTK_TEXT_WINDOW_TEXT), hand_cursor);
            gtk_text_buffer_apply_tag_by_name(buffer, "url", &startword, &endword);
            gui->url = g_strdup(word);
            g_object_unref(hand_cursor);
        } else {
            gdk_window_set_cursor(gtk_text_view_get_window(text_view,
                                  GTK_TEXT_WINDOW_TEXT), normal_cursor);
            gtk_text_buffer_get_bounds(buffer, &start, &end);
            gtk_text_buffer_remove_tag_by_name(buffer, "url", &start, &end);
            gui->url = g_strdup("");
            g_object_unref(normal_cursor);
        }

        if (word) g_free(word);
    }
}


/*
 * check if this tag is a url tag
 */
static GtkTextTag *
get_link_tag(GtkTextIter *iter)
{
    GtkTextTag *link_tag = NULL;
    GSList *list;
    GSList *tag_list = gtk_text_iter_get_tags(iter);

    for (list = tag_list; list; list = g_slist_next(list)) {
        GtkTextTag *tag = list->data;
        gchar *name;
        g_object_get(G_OBJECT(tag), "name", &name, NULL);

        if (!strncmp(name, "url", 3))
            link_tag = tag_list->data;

        g_free(name);
    }

    g_slist_free(tag_list);

    return link_tag;
}


/*
 * click on a link
 */
gboolean
on_maintext_event_after(GtkWidget      *widget,
                        GdkEventKey    *event,
                        gpointer        user_data)
{
    GtkTextIter start, end, iter;
    GtkTextBuffer *buffer;
    GdkEventButton *ev;
    gint x, y;
    gboolean ret;

    if (event->type != GDK_BUTTON_RELEASE) return FALSE;

    ev = (GdkEventButton *)event;

    if (ev->button == 1) {

        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));

        /* we shouldn't follow a link if the user has selected something */
        gtk_text_buffer_get_selection_bounds(buffer, &start, &end);

        if (gtk_text_iter_get_offset(&start) != gtk_text_iter_get_offset(&end))
            return FALSE;

        gtk_text_view_window_to_buffer_coords(GTK_TEXT_VIEW(widget),
                                              GTK_TEXT_WINDOW_WIDGET, ev->x, ev->y, &x, &y);
        gtk_text_view_get_iter_at_location(GTK_TEXT_VIEW(widget), &iter, x, y);

        if (get_link_tag(&iter)) {
            if (g_strrstr(gui->url, "@"))
                ret = openmail(gui->url);
            else
                ret = openurl(gui->url);

            /* When ret is FALSE, the link was not handled by openmail or openurl
             * so call gtk_show_uri to have the desktop defined default app handle
             * the link.
             */
            if (ret == FALSE)
                gtk_show_uri_on_window(GTK_WINDOW(gui->window),
                                       gui->url,
                                       GDK_CURRENT_TIME,
                                       NULL);
        }
    }

    return FALSE;
}


/*
 * grab mouse coordinates and modify cursor
 */
gboolean
on_maintext_motion_notify_event(GtkWidget       *widget,
                                GdkEventMotion  *event,
                                gpointer         user_data)
{
    GdkDisplay *display;
    GdkSeat *seat;
    GdkDevice *pointer;
    gint x, y;
    GdkModifierType state;

    /* Setting the display, seat, and pointer variables from:
     * https://stackoverflow.com/a/24847120
     * https://stackoverflow.com/a/62962920
     */
    display = gdk_display_get_default();
    seat = gdk_display_get_default_seat(display);
    pointer  = gdk_seat_get_pointer(seat);

    if (event->is_hint){
        gdk_window_get_device_position(event->window,
                                       pointer,
                                       &x,
                                       &y,
                                       &state);
    } else {
        x = event->x;
        y = event->y;
        state = event->state;
    }

    gtk_text_view_window_to_buffer_coords(GTK_TEXT_VIEW(widget),
                                          GTK_TEXT_WINDOW_WIDGET, event->x, event->y, &x, &y);
    set_cursor(GTK_TEXT_VIEW(widget), x, y);

    return FALSE;
}
