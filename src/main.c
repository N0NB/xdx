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
 *  main.c - start of gtk loop.
 */


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <gtk/gtk.h>

#include "cmd_opts.h"
#include "gui.h"
#include "history.h"
#include "locale.h"
#include "preferences.h"
#include "text.h"
#include "utils.h"


extern preferencestype preferences;
GdkVisual *visual;
gchar *prompttagname, *calltagname;
gchar *opt_cty_path = NULL;     /* For -c or --cty_path options. */
guitype *gui;
GValue grid_spacing = G_VALUE_INIT;

int
main(int argc, char *argv[])
{
    GtkWidget *treeview, *maintext, *vpaned, *sidemenu, *reconnectmenu,
              *highframe, *fkeysmenu, *func_grid,
              *highcheck1, *highcheck2, *highcheck3, *highcheck4, *highcheck5,
              *highcheck6, *highcheck7, *highcheck8, *soundcheck,
              *highentry1, *highentry2, *highentry3, *highentry4, *highentry5,
              *highentry6, *highentry7, *highentry8,
              *f1button, *f2button, *f3button, *f4button, *f5button, *f6button,
              *f7button, *f8button;
    GtkTreeViewColumn *column;
    gchar **wsplit, *colorstr, *str;
    GString *greeting = g_string_new("");
    PangoFontDescription *font_description;
    gint pango_size;
    GtkTextBuffer *buffer;

#ifdef ENABLE_NLS
    bindtextdomain(PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset(PACKAGE, "UTF-8");
    textdomain(PACKAGE);
#endif

    putenv("TZ=GMT");
    tzset();

    add_pixmap_directory(PACKAGE_DATA_DIR G_DIR_SEPARATOR_S "pixmaps");
    add_pixmap_directory(PACKAGE_SOURCE_DIR G_DIR_SEPARATOR_S "pixmaps");

    gtk_init(&argc, &argv);
    setlocale(LC_NUMERIC, "C");
    parse_opts(&argc, &argv);

    /* Application default spacing in pixels between children in grids. */
    g_value_init(&grid_spacing, G_TYPE_INT);
    g_value_set_int(&grid_spacing, 8);

    visual = gdk_screen_get_system_visual(gdk_screen_get_default());

    create_mainwindow();
    dircheck();
    loadpreferences();
    loadhistory();
    readctydata();

    maintext = g_object_get_data(G_OBJECT(gui->window), "maintext");
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(maintext));

    /* most of the tagnames are randomised when changed, so we can keep on
     * using the old tags without having to remove them from the tagtable
     */
    gui->high1tagname = g_strdup("highcolor1");
    gui->high2tagname = g_strdup("highcolor2");
    gui->high3tagname = g_strdup("highcolor3");
    gui->high4tagname = g_strdup("highcolor4");
    gui->high5tagname = g_strdup("highcolor5");
    gui->high6tagname = g_strdup("highcolor6");
    gui->high7tagname = g_strdup("highcolor7");
    gui->high8tagname = g_strdup("highcolor8");

    colorstr = g_strdup_printf("%s",
                               gdk_rgba_to_string(&preferences.highcolor1));
    gtk_text_buffer_create_tag(buffer,
                               gui->high1tagname,
                               "foreground-rgba",
                               colorstr,
                               NULL);

    colorstr = g_strdup_printf("%s",
                               gdk_rgba_to_string(&preferences.highcolor2));
    gtk_text_buffer_create_tag(buffer,
                               gui->high2tagname,
                               "foreground-rgba",
                               colorstr,
                               NULL);

    colorstr = g_strdup_printf("%s",
                               gdk_rgba_to_string(&preferences.highcolor3));
    gtk_text_buffer_create_tag(buffer,
                               gui->high3tagname,
                               "foreground-rgba",
                               colorstr,
                               NULL);

    colorstr = g_strdup_printf("%s",
                               gdk_rgba_to_string(&preferences.highcolor4));
    gtk_text_buffer_create_tag(buffer,
                               gui->high4tagname,
                               "foreground-rgba",
                               colorstr,
                               NULL);

    colorstr = g_strdup_printf("%s",
                               gdk_rgba_to_string(&preferences.highcolor5));
    gtk_text_buffer_create_tag(buffer,
                               gui->high5tagname,
                               "foreground-rgba",
                               colorstr,
                               NULL);

    colorstr = g_strdup_printf("%s",
                               gdk_rgba_to_string(&preferences.highcolor6));
    gtk_text_buffer_create_tag(buffer,
                               gui->high6tagname,
                               "foreground-rgba",
                               colorstr,
                               NULL);

    colorstr = g_strdup_printf("%s",
                               gdk_rgba_to_string(&preferences.highcolor7));
    gtk_text_buffer_create_tag(buffer,
                               gui->high7tagname,
                               "foreground-rgba",
                               colorstr,
                               NULL);

    colorstr = g_strdup_printf("%s",
                               gdk_rgba_to_string(&preferences.highcolor8));
    gtk_text_buffer_create_tag(buffer,
                               gui->high8tagname,
                               "foreground-rgba",
                               colorstr,
                               NULL);

    gui->prompttagname = g_strdup("prompt");
    gui->calltagname = g_strdup("call");
    gui->wwvtagname = g_strdup("wwv");
    gui->senttagname = g_strdup("sent");
    gui->wxtagname = g_strdup("wx");

    colorstr = g_strdup_printf("%s",
                               gdk_rgba_to_string(&preferences.promptcolor));
    gtk_text_buffer_create_tag(buffer,
                               gui->prompttagname,
                               "foreground-rgba",
                               colorstr,
                               NULL);
    gtk_text_buffer_create_tag(buffer,
                               gui->calltagname,
                               "foreground-rgba",
                               colorstr,
                               "weight",
                               PANGO_WEIGHT_BOLD,
                               NULL);

    colorstr = g_strdup_printf("%s",
                               gdk_rgba_to_string(&preferences.wwvcolor));
    gtk_text_buffer_create_tag(buffer,
                               gui->wwvtagname,
                               "foreground-rgba",
                               colorstr,
                               NULL);

    colorstr = g_strdup_printf("%s",
                               gdk_rgba_to_string(&preferences.wxcolor));
    gtk_text_buffer_create_tag(buffer,
                               gui->wxtagname,
                               "foreground-rgba",
                               colorstr,
                               NULL);

    colorstr = g_strdup_printf("%s",
                               gdk_rgba_to_string(&preferences.sentcolor));
    gtk_text_buffer_create_tag(buffer,
                               gui->senttagname,
                               "foreground-rgba",
                               colorstr,
                               NULL);

    g_free(colorstr);

    highcheck1 = g_object_get_data(G_OBJECT(gui->window), "highcheck1");
    highcheck2 = g_object_get_data(G_OBJECT(gui->window), "highcheck2");
    highcheck3 = g_object_get_data(G_OBJECT(gui->window), "highcheck3");
    highcheck4 = g_object_get_data(G_OBJECT(gui->window), "highcheck4");
    highcheck5 = g_object_get_data(G_OBJECT(gui->window), "highcheck5");
    highcheck6 = g_object_get_data(G_OBJECT(gui->window), "highcheck6");
    highcheck7 = g_object_get_data(G_OBJECT(gui->window), "highcheck7");
    highcheck8 = g_object_get_data(G_OBJECT(gui->window), "highcheck8");
    soundcheck = g_object_get_data(G_OBJECT(gui->window), "soundcheck");

    if (preferences.highmenu[0] == '1')
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(highcheck1), TRUE);

    if (preferences.highmenu[1] == '1')
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(highcheck2), TRUE);

    if (preferences.highmenu[2] == '1')
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(highcheck3), TRUE);

    if (preferences.highmenu[3] == '1')
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(highcheck4), TRUE);

    if (preferences.highmenu[4] == '1')
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(highcheck5), TRUE);

    if (preferences.highmenu[5] == '1')
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(highcheck6), TRUE);

    if (preferences.highmenu[6] == '1')
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(highcheck7), TRUE);

    if (preferences.highmenu[7] == '1')
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(highcheck8), TRUE);

    if (preferences.playsound == 1)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(soundcheck), TRUE);

    highentry1 = g_object_get_data(G_OBJECT(gui->window), "highentry1");
    highentry2 = g_object_get_data(G_OBJECT(gui->window), "highentry2");
    highentry3 = g_object_get_data(G_OBJECT(gui->window), "highentry3");
    highentry4 = g_object_get_data(G_OBJECT(gui->window), "highentry4");
    highentry5 = g_object_get_data(G_OBJECT(gui->window), "highentry5");
    highentry6 = g_object_get_data(G_OBJECT(gui->window), "highentry6");
    highentry7 = g_object_get_data(G_OBJECT(gui->window), "highentry7");
    highentry8 = g_object_get_data(G_OBJECT(gui->window), "highentry8");

    if (g_ascii_strcasecmp(preferences.highword1, "?"))
        gtk_entry_set_text(GTK_ENTRY(highentry1), preferences.highword1);

    if (g_ascii_strcasecmp(preferences.highword2, "?"))
        gtk_entry_set_text(GTK_ENTRY(highentry2), preferences.highword2);

    if (g_ascii_strcasecmp(preferences.highword3, "?"))
        gtk_entry_set_text(GTK_ENTRY(highentry3), preferences.highword3);

    if (g_ascii_strcasecmp(preferences.highword4, "?"))
        gtk_entry_set_text(GTK_ENTRY(highentry4), preferences.highword4);

    if (g_ascii_strcasecmp(preferences.highword5, "?"))
        gtk_entry_set_text(GTK_ENTRY(highentry5), preferences.highword5);

    if (g_ascii_strcasecmp(preferences.highword6, "?"))
        gtk_entry_set_text(GTK_ENTRY(highentry6), preferences.highword6);

    if (g_ascii_strcasecmp(preferences.highword7, "?"))
        gtk_entry_set_text(GTK_ENTRY(highentry7), preferences.highword7);

    if (g_ascii_strcasecmp(preferences.highword8, "?"))
        gtk_entry_set_text(GTK_ENTRY(highentry8), preferences.highword8);

/* Suppress deprecated warnings until a solution using pango attributes is
 * developed.
 */
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    gtk_widget_override_color(highentry1,
                           GTK_STATE_FLAG_NORMAL,
                           &preferences.highcolor1);

    gtk_widget_override_color(highentry2,
                           GTK_STATE_FLAG_NORMAL,
                           &preferences.highcolor2);

    gtk_widget_override_color(highentry3,
                           GTK_STATE_FLAG_NORMAL,
                           &preferences.highcolor3);

    gtk_widget_override_color(highentry4,
                           GTK_STATE_FLAG_NORMAL,
                           &preferences.highcolor4);

    gtk_widget_override_color(highentry5,
                           GTK_STATE_FLAG_NORMAL,
                           &preferences.highcolor5);

    gtk_widget_override_color(highentry6,
                           GTK_STATE_FLAG_NORMAL,
                           &preferences.highcolor6);

    gtk_widget_override_color(highentry7,
                           GTK_STATE_FLAG_NORMAL,
                           &preferences.highcolor7);

    gtk_widget_override_color(highentry8,
                           GTK_STATE_FLAG_NORMAL,
                           &preferences.highcolor8);
#pragma GCC diagnostic pop

    treeview = g_object_get_data(G_OBJECT(gui->window), "treeview");
    vpaned = g_object_get_data(G_OBJECT(gui->window), "vpaned");

    font_description = pango_font_description_from_string(preferences.dxfont);
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    gtk_widget_modify_font(GTK_WIDGET(treeview), font_description);
#pragma GCC diagnostic pop
    pango_font_description_free(font_description);

    font_description = pango_font_description_from_string(preferences.allfont);
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    gtk_widget_modify_font(GTK_WIDGET(maintext), font_description);
#pragma GCC diagnostic pop

    pango_size = pango_font_description_get_size(font_description);

    /* line spacing is half character size */
    g_object_set(G_OBJECT(maintext),
                 "pixels-below-lines",
                 PANGO_PIXELS(pango_size) / 2,
                 NULL);

    pango_font_description_free(font_description);

    wsplit = g_strsplit(preferences.columnwidths, ",", 0);
    column = gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0);

    if (!preferences.col0visible)
        gtk_tree_view_column_set_visible(GTK_TREE_VIEW_COLUMN(column), FALSE);
    else
        gtk_tree_view_column_set_fixed_width(column, atoi(wsplit[0]));

    column = gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 1);

    if (!preferences.col1visible)
        gtk_tree_view_column_set_visible(GTK_TREE_VIEW_COLUMN(column), FALSE);
    else
        gtk_tree_view_column_set_fixed_width(column, atoi(wsplit[1]));

    column = gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 2);

    if (!preferences.col2visible)
        gtk_tree_view_column_set_visible(GTK_TREE_VIEW_COLUMN(column), FALSE);
    else
        gtk_tree_view_column_set_fixed_width(column, atoi(wsplit[2]));

    column = gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 3);

    if (!preferences.col3visible)
        gtk_tree_view_column_set_visible(GTK_TREE_VIEW_COLUMN(column), FALSE);
    else
        gtk_tree_view_column_set_fixed_width(column, atoi(wsplit[3]));

    column = gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 4);

    if (!preferences.col4visible)
        gtk_tree_view_column_set_visible(GTK_TREE_VIEW_COLUMN(column), FALSE);
    else
        gtk_tree_view_column_set_fixed_width(column, atoi(wsplit[4]));

    column = gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 5);

    if (!preferences.col5visible)
        gtk_tree_view_column_set_visible(GTK_TREE_VIEW_COLUMN(column), FALSE);
    else
        gtk_tree_view_column_set_fixed_width(column, atoi(wsplit[5]));

    column = gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 6);

    if (!preferences.col6visible)
        gtk_tree_view_column_set_visible(GTK_TREE_VIEW_COLUMN(column), FALSE);
    else
        gtk_tree_view_column_set_fixed_width(column, atoi(wsplit[6]));

    g_strfreev(wsplit);

    f1button = g_object_get_data(G_OBJECT(gui->window), "f1button");
    f2button = g_object_get_data(G_OBJECT(gui->window), "f2button");
    f3button = g_object_get_data(G_OBJECT(gui->window), "f3button");
    f4button = g_object_get_data(G_OBJECT(gui->window), "f4button");
    f5button = g_object_get_data(G_OBJECT(gui->window), "f5button");
    f6button = g_object_get_data(G_OBJECT(gui->window), "f6button");
    f7button = g_object_get_data(G_OBJECT(gui->window), "f7button");
    f8button = g_object_get_data(G_OBJECT(gui->window), "f8button");

    if (strcmp(preferences.f1command, "^"))
        str = g_strdup_printf("F1: %s", preferences.f1command);
    else
        str = g_strdup("F1:");

    gtk_button_set_label(GTK_BUTTON(f1button), str);

    if (strcmp(preferences.f2command, "^"))
        str = g_strdup_printf("F2: %s", preferences.f2command);
    else
        str = g_strdup("F2:");

    gtk_button_set_label(GTK_BUTTON(f2button), str);

    if (strcmp(preferences.f3command, "^"))
        str = g_strdup_printf("F3: %s", preferences.f3command);
    else
        str = g_strdup("F3:");

    gtk_button_set_label(GTK_BUTTON(f3button), str);

    if (strcmp(preferences.f4command, "^"))
        str = g_strdup_printf("F4: %s", preferences.f4command);
    else
        str = g_strdup("F4:");

    gtk_button_set_label(GTK_BUTTON(f4button), str);

    if (strcmp(preferences.f5command, "^"))
        str = g_strdup_printf("F5: %s", preferences.f5command);
    else
        str = g_strdup("F5:");

    gtk_button_set_label(GTK_BUTTON(f5button), str);

    if (strcmp(preferences.f6command, "^"))
        str = g_strdup_printf("F6: %s", preferences.f6command);
    else
        str = g_strdup("F6:");

    gtk_button_set_label(GTK_BUTTON(f6button), str);

    if (strcmp(preferences.f7command, "^"))
        str = g_strdup_printf("F7: %s", preferences.f7command);
    else
        str = g_strdup("F7:");

    gtk_button_set_label(GTK_BUTTON(f7button), str);

    if (strcmp(preferences.f8command, "^"))
        str = g_strdup_printf("F8: %s", preferences.f8command);
    else
        str = g_strdup("F8:");

    gtk_button_set_label(GTK_BUTTON(f8button), str);
    g_free(str);

    gtk_widget_show_all(gui->window);

    gtk_window_move(GTK_WINDOW(gui->window),
                    preferences.x, preferences.y);

    gtk_window_resize(GTK_WINDOW(gui->window),
                      preferences.width, preferences.height);

    gtk_paned_set_position(GTK_PANED(vpaned), preferences.handlebarpos);

    func_grid = g_object_get_data(G_OBJECT(gui->window), "func_grid");
    fkeysmenu = g_object_get_data(G_OBJECT(gui->window), "keybar_item");

    if (preferences.fbox == 0) {
        gtk_widget_hide(func_grid);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(fkeysmenu),
                                       FALSE);
    } else {
        gtk_widget_show(func_grid);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(fkeysmenu),
                                       TRUE);
    }

    highframe = g_object_get_data(G_OBJECT(gui->window), "highframe");
    sidemenu = g_object_get_data(G_OBJECT(gui->window), "sidebar_item");

    if (preferences.sidebar == 0) {
        gtk_widget_hide(highframe);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(sidemenu),
                                       FALSE);
    } else {
        gtk_widget_show(highframe);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(sidemenu),
                                       TRUE);
    }

    reconnectmenu = g_object_get_data(G_OBJECT(gui->window), "reconnect_item");

    if (preferences.reconnect == 0) {
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(reconnectmenu),
                                       FALSE);
    } else {
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(reconnectmenu),
                                       TRUE);
    }

    gtk_widget_set_sensitive(GTK_WIDGET(
                             g_object_get_data(G_OBJECT(gui->window),
                                                        "close_item")),
                             FALSE);

    g_string_printf(greeting, _("Welcome to %s"), PACKAGE);
    updatestatusbar(greeting, FALSE);
    g_string_free(greeting, TRUE);

    gtk_main();

    return 0;
}
