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
 * gui_settingsdialog.c
 */

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "gui_settingsdialog.h"
#include "utils.h"
#include "gui.h"
#include "preferences.h"
#include "gtksourceiter.h"

GtkWidget *preferencesdialog;
extern preferencestype preferences;

static void
on_pautologincheckbutton_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkWidget *ploginhseparator, *pcommandslabel, *pcommandsentry;
  gboolean state;

  ploginhseparator = g_object_get_data (G_OBJECT (preferencesdialog), 
    "ploginhseparator");
  pcommandslabel = g_object_get_data (G_OBJECT (preferencesdialog), 
    "pcommandslabel");
  pcommandsentry = g_object_get_data (G_OBJECT (preferencesdialog), 
    "pcommandsentry");

  state = gtk_toggle_button_get_active (togglebutton);
  if (state)
  {
    gtk_widget_set_sensitive (ploginhseparator, TRUE);
    gtk_widget_set_sensitive (pcommandslabel, TRUE);
    gtk_widget_set_sensitive (pcommandsentry, TRUE);
  }
  else
  {
    gtk_widget_set_sensitive (ploginhseparator, FALSE);
    gtk_widget_set_sensitive (pcommandslabel, FALSE);
    gtk_widget_set_sensitive (pcommandsentry, FALSE);
  }
}


static void
on_phamlibcheckbutton_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkWidget *phamlibhseparator, *priglabel, *prigentry;
  gboolean state;

  phamlibhseparator = g_object_get_data (G_OBJECT (preferencesdialog), 
    "phamlibhseparator");
  priglabel = g_object_get_data (G_OBJECT (preferencesdialog), 
    "priglabel");
  prigentry = g_object_get_data (G_OBJECT (preferencesdialog), 
    "prigentry");

  state = gtk_toggle_button_get_active (togglebutton);
  if (state)
  {
    gtk_widget_set_sensitive (phamlibhseparator, TRUE);
    gtk_widget_set_sensitive (priglabel, TRUE);
    gtk_widget_set_sensitive (prigentry, TRUE);
  }
  else
  {
    gtk_widget_set_sensitive (phamlibhseparator, FALSE);
    gtk_widget_set_sensitive (priglabel, FALSE);
    gtk_widget_set_sensitive (prigentry, FALSE);
  }
}

static void
on_fontbutton_clicked (GtkButton *button, gpointer user_data)
{
  GtkWidget *fontselectiondialog, *pfontsdxentry, *pfontsallentry;
  GdkPixbuf *fontselectiondialog_icon_pixbuf;
  gchar *font, *path;
  gint response;

  fontselectiondialog = gtk_font_selection_dialog_new
    (_("xdx - Select a font"));
  path = g_build_filename (PACKAGE_DATA_DIR, "pixmaps", "xdx.png", NULL);
  fontselectiondialog_icon_pixbuf = gdk_pixbuf_new_from_file (path, NULL);
  g_free (path);
  if (fontselectiondialog_icon_pixbuf)
  {
    gtk_window_set_icon (GTK_WINDOW (fontselectiondialog),
      fontselectiondialog_icon_pixbuf);
    g_object_unref (fontselectiondialog_icon_pixbuf);
  }
  gtk_widget_destroy (GTK_FONT_SELECTION_DIALOG
    (fontselectiondialog)->apply_button);
  gtk_font_selection_dialog_set_preview_text (GTK_FONT_SELECTION_DIALOG
    (fontselectiondialog), _("How about this font?"));
  if (GPOINTER_TO_INT(user_data) == 1)
    gtk_font_selection_dialog_set_font_name
      (GTK_FONT_SELECTION_DIALOG(fontselectiondialog), preferences.dxfont);
  else
    gtk_font_selection_dialog_set_font_name
      (GTK_FONT_SELECTION_DIALOG(fontselectiondialog), preferences.allfont);
  gtk_widget_show_all (fontselectiondialog);
  response = gtk_dialog_run (GTK_DIALOG(fontselectiondialog));
  if (response == GTK_RESPONSE_OK)
  {
    font = gtk_font_selection_dialog_get_font_name
      (GTK_FONT_SELECTION_DIALOG (fontselectiondialog));
    if (GPOINTER_TO_INT(user_data) == 1)
    {
      pfontsdxentry = g_object_get_data (G_OBJECT (preferencesdialog), 
        "pfontsdxentry");
      gtk_entry_set_text (GTK_ENTRY (pfontsdxentry), font);
    }
	else
    {
      pfontsallentry = g_object_get_data (G_OBJECT (preferencesdialog), 
        "pfontsallentry");
      gtk_entry_set_text (GTK_ENTRY (pfontsallentry), font);
    }
    g_free (font);
  }
  gtk_widget_destroy (fontselectiondialog);
}

/*
 * called from the menu
 */
void on_settings_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  GtkWidget *pdialog_vbox, *pvbox1, *pvbox2, *pvbox3, *pvbox4,

    *pnotebook, *plabel1, *plabel2, *plabel3, *plabel4,

    *pcallsignframe, *pcallsignhbox, *pcallsignlabel, *pcallsignentry,
    *pcallsignframelabel, *ploginframe, *ploginvbox, *pautologincheckbutton,
    *ploginhseparator, *pcommandshbox, *pcommandslabel, *pcommandsentry,
    *ploginframelabel,

    *psavingframe, *psavingvbox, *psavedxcheckbutton, *psavewwvcheckbutton,
    *psavetoallcheckbutton, *psavewxcheckbutton, *psavinglabel, *psavinghbox,
    *psavingvbox2, *psavingvbox3,

    *phamlibframe, *phamlibvbox, *phamlibcheckbutton, *phamlibhseparator, 
    *phamlibhbox, *priglabel, *prigentry, *phamliblabel, 

    *pprogframe, *pprogvbox, *pproghbox1, *pprogbrowserlabel, 
    *pprogbrowserentry, *pproghbox2, *pprogmaillabel,
    *pprogmailentry, *pproglabel, *pproghbox3, *pprogsoundlabel,
    *pprogsoundentry,

    *pechoframe, *pechocheckbutton, *pechovbox, *pecholabel, *plivecheckbutton,

    *pcolumnsframe, *pcolumnsvbox, *pcolumnsvboxlabel, *pcolumnslabel,
    *pcolumnshseparator, *pspottercheckbutton, *pqrgcheckbutton,
    *pdxcheckbutton, *premarkscheckbutton, *ptimecheckbutton, 
    *pinfocheckbutton, *pcolumnshbox, *pcolumnsvbox2, *pcolumnsvbox3,

    *pfontsframe, *pfontslabel, *pfontsvbox, *pfontsdxlabel, *pfontsalllabel,
    *pfontsdxentry, *pfontsallentry, *pfontsdxbutton, *pfontsallbutton,
    *pfontshseparator, *pfontshbox1, *pfontshbox2,

    *phighframe, *phighframelabel, *phighvbox, *phighlabel, *phighseparator,
    *phighhbox, *phighvbox1, *phighhbox2, *phighlabel1, *colorbutton1,
    *phighbox3, *phighlabel2, *colorbutton2, *phighbox4, *phighlabel3,
    *colorbutton3, *phighbox5, *phighlabel4, *colorbutton4, *vseparator1,
    *phighvbox2, *phighbox6, *phighlabel5, *colorbutton5, *phighhbox7,
    *phighlabel6, *colorbutton6, *phighhbox8, *phighlabel7, *colorbutton7,
    *phighhbox9, *phighlabel8, *colorbutton8,

    *pcolorsframe, *pcolorsframelabel, *pcolorsvbox, *pcolorshbox,
    *promptcolorlabel, *promptcolorbutton, *sentcolorlabel,
    *sentcolorbutton, *wwvcolorlabel, *wwvcolorbutton, *wxcolorlabel,
    *wxcolorbutton;

  GtkTreeViewColumn *column;
  GtkWidget *treeview, *maintext, *mainentry;
  GtkWidget *highentry1, *highentry2, *highentry3, *highentry4, *highentry5,
    *highentry6, *highentry7, *highentry8; 
  PangoFontDescription *font_description;
  gint response, pango_size;
  gboolean state;
  gchar *str;
  GtkTextBuffer *buffer;
  GtkTextTagTable *table;
  GdkColor color;

  gtk_widget_set_sensitive (gui->window, 0);
  preferencesdialog = gtk_dialog_new_with_buttons (_("xdx - preferences"),
					    GTK_WINDOW (gui->window),
					    GTK_DIALOG_MODAL |
					    GTK_DIALOG_DESTROY_WITH_PARENT,
					    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					    GTK_STOCK_OK,
					    GTK_RESPONSE_OK, NULL);

  
  pdialog_vbox = GTK_DIALOG (preferencesdialog)->vbox;

  pnotebook = gtk_notebook_new ();
  pvbox1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pnotebook), pvbox1);
  pvbox2 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pnotebook), pvbox2);
  pvbox3 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pnotebook), pvbox3);
  pvbox4 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pnotebook), pvbox4);
  plabel1 = gtk_label_new (_("General"));
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (pnotebook), 
    gtk_notebook_get_nth_page (GTK_NOTEBOOK (pnotebook), 0), plabel1);
  plabel2 = gtk_label_new (_("Output"));
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (pnotebook), 
    gtk_notebook_get_nth_page (GTK_NOTEBOOK (pnotebook), 1), plabel2);
  plabel3 = gtk_label_new (_("Fonts"));
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (pnotebook), 
    gtk_notebook_get_nth_page (GTK_NOTEBOOK (pnotebook), 2), plabel3);
  plabel4 = gtk_label_new (_("Colors"));
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (pnotebook), 
    gtk_notebook_get_nth_page (GTK_NOTEBOOK (pnotebook), 3), plabel4);
  gtk_box_pack_start (GTK_BOX (pdialog_vbox), pnotebook, TRUE, TRUE, 0);


  pcallsignframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox1), pcallsignframe, TRUE, TRUE, 0);
  pcallsignhbox = gtk_hbox_new (TRUE, 0);
  gtk_container_add (GTK_CONTAINER (pcallsignframe), pcallsignhbox);
  pcallsignlabel = gtk_label_new (_("Your callsign"));
  gtk_box_pack_start (GTK_BOX (pcallsignhbox), pcallsignlabel, FALSE, FALSE, 0);
  pcallsignentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (pcallsignhbox), pcallsignentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (pcallsignentry), 15);

  ploginframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox1), ploginframe, TRUE, TRUE, 0);
  ploginvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (ploginframe), ploginvbox);
  pautologincheckbutton = gtk_check_button_new_with_label (_("Enable autologin"));
  gtk_box_pack_start (GTK_BOX (ploginvbox), pautologincheckbutton, FALSE, FALSE, 0);
  ploginhseparator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (ploginvbox), ploginhseparator, TRUE, TRUE, 0);

  pcommandshbox = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (ploginvbox), pcommandshbox, TRUE, TRUE, 0);
  pcommandslabel = gtk_label_new (_("Commands"));
  gtk_box_pack_start (GTK_BOX (pcommandshbox), pcommandslabel, FALSE, FALSE, 0);
  pcommandsentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (pcommandshbox), pcommandsentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (pcommandsentry), 80);
  gtk_widget_set_tooltip_text(pcommandsentry, 
    _("Comma separated list of commands to send at login"));
  gtk_widget_set_tooltip_text(pcallsignentry, 
    _("Callsign to be used for login"));

  ploginframelabel = gtk_label_new (_("Login"));
  gtk_frame_set_label_widget (GTK_FRAME (ploginframe), ploginframelabel);
  pcallsignframelabel = gtk_label_new (_("Callsign"));
  gtk_frame_set_label_widget (GTK_FRAME (pcallsignframe), pcallsignframelabel);

  if (preferences.autologin == 1)
  {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pautologincheckbutton), TRUE);
    gtk_widget_set_sensitive (ploginhseparator, TRUE);
    gtk_widget_set_sensitive (pcommandslabel, TRUE);
    gtk_widget_set_sensitive (pcommandsentry, TRUE);
  }
  else
  {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pautologincheckbutton), FALSE);
    gtk_widget_set_sensitive (ploginhseparator, FALSE);
    gtk_widget_set_sensitive (pcommandslabel, FALSE);
    gtk_widget_set_sensitive (pcommandsentry, FALSE);
  }
  if (g_ascii_strcasecmp (preferences.callsign, "?"))
    gtk_entry_set_text (GTK_ENTRY(pcallsignentry), preferences.callsign);
  if (g_ascii_strcasecmp (preferences.commands, "?"))
    gtk_entry_set_text (GTK_ENTRY(pcommandsentry), preferences.commands);

  phamlibframe = gtk_frame_new (NULL);
  gtk_widget_show (phamlibframe);
  gtk_box_pack_start (GTK_BOX (pvbox1), phamlibframe, TRUE, TRUE, 0);
  phamlibvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (phamlibframe), phamlibvbox);
  phamlibcheckbutton = gtk_check_button_new_with_label (_("Enable hamlib"));
  gtk_box_pack_start (GTK_BOX (phamlibvbox), phamlibcheckbutton, FALSE, FALSE, 0);
  phamlibhseparator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (phamlibvbox), phamlibhseparator, TRUE, TRUE, 0);
  phamlibhbox = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (phamlibvbox), phamlibhbox, TRUE, TRUE, 0);
  priglabel = gtk_label_new (_("Command for rigctl"));
  gtk_box_pack_start (GTK_BOX (phamlibhbox), priglabel, FALSE, FALSE, 0);
  prigentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (phamlibhbox), prigentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (prigentry), 80);
  phamliblabel = gtk_label_new (_("Hamlib"));
  gtk_frame_set_label_widget (GTK_FRAME (phamlibframe), phamliblabel);
  gtk_widget_set_tooltip_text(prigentry, _(
    "When double clicking on a dx-spot this will set the frequency of your "
    "rig using rigctl (%d = the frequency retrieved from the DX spot)"));

  if (preferences.hamlib == 1)
  {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(phamlibcheckbutton), TRUE);
    gtk_widget_set_sensitive (phamlibhseparator, TRUE);
    gtk_widget_set_sensitive (priglabel, TRUE);
    gtk_widget_set_sensitive (prigentry, TRUE);
  }
  else
  {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(phamlibcheckbutton), FALSE);
    gtk_widget_set_sensitive (phamlibhseparator, FALSE);
    gtk_widget_set_sensitive (priglabel, FALSE);
    gtk_widget_set_sensitive (prigentry, FALSE);
  }
  if (g_ascii_strcasecmp (preferences.rigctl, "?"))
    gtk_entry_set_text (GTK_ENTRY(prigentry), preferences.rigctl);
  
  pprogframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox1), pprogframe, TRUE, TRUE, 0);
  pprogvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pprogframe), pprogvbox);
  pproghbox1 = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (pprogvbox), pproghbox1, TRUE, TRUE, 0);
  pprogbrowserlabel = gtk_label_new (_("Web browser"));
  gtk_box_pack_start (GTK_BOX (pproghbox1), pprogbrowserlabel, FALSE, FALSE, 0);
  pprogbrowserentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (pproghbox1), pprogbrowserentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (pprogbrowserentry), 80);
  pproghbox2 = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (pprogvbox), pproghbox2, TRUE, TRUE, 0);
  pprogmaillabel = gtk_label_new (_("Mail program"));
  gtk_box_pack_start (GTK_BOX (pproghbox2), pprogmaillabel, FALSE, FALSE, 0);
  pprogmailentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (pproghbox2), pprogmailentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (pprogmailentry), 80);
  pproghbox3 = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (pprogvbox), pproghbox3, TRUE, TRUE, 0);
  pprogsoundlabel = gtk_label_new (_("Sound playing"));
  gtk_box_pack_start (GTK_BOX (pproghbox3), pprogsoundlabel, FALSE, FALSE, 0);
  pprogsoundentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (pproghbox3), pprogsoundentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (pprogmailentry), 80);
  pproglabel = gtk_label_new (_("Programs"));
  gtk_frame_set_label_widget (GTK_FRAME (pprogframe), pproglabel);
  gtk_widget_set_tooltip_text(pprogbrowserentry, 
    _("Web browser to start after clicking on a url (%s = url)"));
  gtk_widget_set_tooltip_text(pprogmailentry, 
    _("Mail program to start after clicking on a mail url (%s = mail url)"));
  gtk_widget_set_tooltip_text(pprogsoundentry, 
    _("Program used to play sound (%s = sound file)"));

  if (g_ascii_strcasecmp (preferences.browserapp, "?"))
    gtk_entry_set_text (GTK_ENTRY(pprogbrowserentry), preferences.browserapp);
  if (g_ascii_strcasecmp (preferences.mailapp, "?"))
    gtk_entry_set_text (GTK_ENTRY(pprogmailentry), preferences.mailapp);
  if (g_ascii_strcasecmp (preferences.soundapp, "?"))
    gtk_entry_set_text (GTK_ENTRY(pprogsoundentry), preferences.soundapp);

  pechoframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox2), pechoframe, TRUE, TRUE, 0);
  pechovbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pechoframe), pechovbox);
  pechocheckbutton = gtk_check_button_new_with_label
    (_("Echo sent text to the screen"));
  gtk_box_pack_start (GTK_BOX (pechovbox), pechocheckbutton, FALSE, FALSE, 0);
  plivecheckbutton = gtk_check_button_new_with_label
    (_("Send keepalive packets (read the manual)"));
  gtk_box_pack_start (GTK_BOX (pechovbox), plivecheckbutton, FALSE, FALSE, 0);
  pecholabel = gtk_label_new (_("General"));
  gtk_frame_set_label_widget (GTK_FRAME (pechoframe), pecholabel);
  if (preferences.localecho == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pechocheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pechocheckbutton), FALSE);
  if (preferences.keepalive == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(plivecheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(plivecheckbutton), FALSE);

  pcolumnsframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox2), pcolumnsframe, TRUE, TRUE, 0);
  pcolumnsvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pcolumnsframe), pcolumnsvbox);
  pcolumnslabel = gtk_label_new (_("Columns"));
  gtk_frame_set_label_widget (GTK_FRAME (pcolumnsframe), pcolumnslabel);
  pcolumnsvboxlabel = gtk_label_new (_("Columns to show on the screen"));
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox), pcolumnsvboxlabel, FALSE, FALSE, 0);
  pcolumnshseparator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox), pcolumnshseparator, FALSE, FALSE, 0);
  pcolumnshbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox), pcolumnshbox, FALSE, FALSE, 0);
  pcolumnsvbox2 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (pcolumnshbox), pcolumnsvbox2, TRUE, TRUE, 0);
  pspottercheckbutton = gtk_check_button_new_with_label (_("Spotter"));
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox2), pspottercheckbutton, FALSE, FALSE, 0);
  pqrgcheckbutton = gtk_check_button_new_with_label ("QRG");
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox2), pqrgcheckbutton, FALSE, FALSE, 0);
  pdxcheckbutton = gtk_check_button_new_with_label ("DX");
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox2), pdxcheckbutton, FALSE, FALSE, 0);
  pcolumnsvbox3 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (pcolumnshbox), pcolumnsvbox3, TRUE, TRUE, 0);
  premarkscheckbutton = gtk_check_button_new_with_label (_("Remarks"));
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox3), premarkscheckbutton, FALSE, FALSE, 0);
  ptimecheckbutton = gtk_check_button_new_with_label (_("Time"));
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox3), ptimecheckbutton, FALSE, FALSE, 0);
  pinfocheckbutton = gtk_check_button_new_with_label (_("Info"));
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox3), pinfocheckbutton, FALSE, FALSE, 0);
  if (preferences.col0visible == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pspottercheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pspottercheckbutton), FALSE);
  if (preferences.col1visible == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pqrgcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pqrgcheckbutton), FALSE);
  if (preferences.col2visible == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pdxcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pdxcheckbutton), FALSE);
  if (preferences.col3visible == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(premarkscheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(premarkscheckbutton), FALSE);
  if (preferences.col4visible == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(ptimecheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(ptimecheckbutton), FALSE);
  if (preferences.col5visible == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pinfocheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pinfocheckbutton), FALSE);


  psavingframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox2), psavingframe, TRUE, TRUE, 0);
  psavingvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (psavingframe), psavingvbox);
  psavinghbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (psavingvbox), psavinghbox);
  psavingvbox2 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (psavinghbox), psavingvbox2);
  psavedxcheckbutton = gtk_check_button_new_with_label (_("Save DX spots"));
  gtk_box_pack_start (GTK_BOX (psavingvbox2), psavedxcheckbutton, FALSE, FALSE, 0);
  psavewwvcheckbutton = gtk_check_button_new_with_label (_("Save WCY/WWV"));
  gtk_box_pack_start (GTK_BOX (psavingvbox2), psavewwvcheckbutton, FALSE, FALSE, 0);
  psavingvbox3 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (psavinghbox), psavingvbox3);
  psavetoallcheckbutton = gtk_check_button_new_with_label (_("Save \"To all\""));
  gtk_box_pack_start (GTK_BOX (psavingvbox3), psavetoallcheckbutton, FALSE, FALSE, 0);
  psavewxcheckbutton = gtk_check_button_new_with_label (_("Save WX"));
  gtk_box_pack_start (GTK_BOX (psavingvbox3), psavewxcheckbutton, FALSE, FALSE, 0);
  psavinglabel = gtk_label_new (_("Saving"));
  gtk_frame_set_label_widget (GTK_FRAME (psavingframe), psavinglabel);

  pfontsframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox3), pfontsframe, FALSE, FALSE, 0);
  pfontslabel = gtk_label_new (_("Fonts"));
  gtk_frame_set_label_widget (GTK_FRAME (pfontsframe), pfontslabel);
  pfontsvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pfontsframe), pfontsvbox);
  pfontsdxlabel = gtk_label_new (_("Font for DX messages"));
  pfontshbox1 = gtk_hbox_new (FALSE, 0);
  pfontsdxentry = gtk_entry_new ();
  pfontsdxbutton = gtk_button_new_with_mnemonic (_("Select _DX Font"));
  gtk_box_pack_start (GTK_BOX (pfontsvbox), pfontsdxlabel, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (pfontsvbox), pfontshbox1, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (pfontshbox1), pfontsdxentry, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (pfontshbox1), pfontsdxbutton, FALSE, FALSE, 0);
  pfontshseparator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (pfontsvbox), pfontshseparator, FALSE, FALSE, 0);
  pfontsalllabel = gtk_label_new (_("Font for other messages"));
  pfontshbox2 = gtk_hbox_new (FALSE, 0);
  pfontsallentry = gtk_entry_new ();
  pfontsallbutton = gtk_button_new_with_mnemonic (_("Select _Other Fonts"));
  gtk_box_pack_start (GTK_BOX (pfontsvbox), pfontsalllabel, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (pfontsvbox), pfontshbox2, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (pfontshbox2), pfontsallentry, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (pfontshbox2), pfontsallbutton, FALSE, FALSE, 0);
  gtk_entry_set_text (GTK_ENTRY (pfontsdxentry), preferences.dxfont);
  gtk_entry_set_text (GTK_ENTRY (pfontsallentry), preferences.allfont);
  gtk_editable_set_editable (GTK_EDITABLE (pfontsdxentry), FALSE);
  gtk_editable_set_editable (GTK_EDITABLE (pfontsallentry), FALSE);

  phighframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox3), phighframe, FALSE, FALSE, 0);
  phighframelabel = gtk_label_new (_("Highlighting"));
  gtk_frame_set_label_widget (GTK_FRAME (phighframe), phighframelabel);
  phighvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (phighframe), phighvbox);
  phighlabel = gtk_label_new (_("Colors to use for highlighting"));
  gtk_label_set_line_wrap (GTK_LABEL (phighlabel), TRUE);
  gtk_box_pack_start (GTK_BOX (phighvbox), phighlabel, FALSE, FALSE, 10);
  phighseparator =  gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (phighvbox), phighseparator, FALSE, FALSE, 0);
  phighhbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (phighvbox), phighhbox, FALSE, FALSE, 0);
  phighvbox1 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (phighhbox), phighvbox1, TRUE, TRUE, 0);

  phighhbox2 = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (phighvbox1), phighhbox2, TRUE, TRUE, 0);
  str = g_strdup_printf (_("Color %d"), 1);
  phighlabel1 = gtk_label_new (str);
  gtk_box_pack_start (GTK_BOX (phighhbox2), phighlabel1, TRUE, TRUE, 0);
  colorbutton1 = gtk_color_button_new ();
  gtk_box_pack_start (GTK_BOX (phighhbox2), colorbutton1, FALSE, FALSE, 0);

  phighbox3 = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (phighvbox1), phighbox3, TRUE, TRUE, 0);
  str = g_strdup_printf (_("Color %d"), 2);
  phighlabel2 = gtk_label_new (str);
  gtk_box_pack_start (GTK_BOX (phighbox3), phighlabel2, TRUE, TRUE, 0);
  colorbutton2 = gtk_color_button_new ();
  gtk_box_pack_start (GTK_BOX (phighbox3), colorbutton2, FALSE, FALSE, 0);

  phighbox4 = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (phighvbox1), phighbox4, TRUE, TRUE, 0);
  str = g_strdup_printf (_("Color %d"), 3);
  phighlabel3 = gtk_label_new (str);
  gtk_box_pack_start (GTK_BOX (phighbox4), phighlabel3, TRUE, TRUE, 0);
  colorbutton3 = gtk_color_button_new ();
  gtk_box_pack_start (GTK_BOX (phighbox4), colorbutton3, FALSE, FALSE, 0);

  phighbox5 = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (phighvbox1), phighbox5, TRUE, TRUE, 0);
  str = g_strdup_printf (_("Color %d"), 4);
  phighlabel4 = gtk_label_new (str);
  gtk_box_pack_start (GTK_BOX (phighbox5), phighlabel4, TRUE, TRUE, 0);
  colorbutton4 = gtk_color_button_new ();
  gtk_box_pack_start (GTK_BOX (phighbox5), colorbutton4, FALSE, FALSE, 0);

  vseparator1 = gtk_vseparator_new ();
  gtk_box_pack_start (GTK_BOX (phighhbox), vseparator1, TRUE, TRUE, 0);
  phighvbox2 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (phighhbox), phighvbox2, TRUE, TRUE, 0);

  phighbox6 = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (phighvbox2), phighbox6, TRUE, TRUE, 0);
  str = g_strdup_printf (_("Color %d"), 5);
  phighlabel5 = gtk_label_new (str);
  gtk_box_pack_start (GTK_BOX (phighbox6), phighlabel5, TRUE, TRUE, 0);
  colorbutton5 = gtk_color_button_new ();
  gtk_box_pack_start (GTK_BOX (phighbox6), colorbutton5, FALSE, FALSE, 0);

  phighhbox7 = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (phighvbox2), phighhbox7, TRUE, TRUE, 0);
  str = g_strdup_printf (_("Color %d"), 6);
  phighlabel6 = gtk_label_new (str);
  gtk_box_pack_start (GTK_BOX (phighhbox7), phighlabel6, TRUE, TRUE, 0);
  colorbutton6 = gtk_color_button_new ();
  gtk_box_pack_start (GTK_BOX (phighhbox7), colorbutton6, FALSE, FALSE, 0);

  phighhbox8 = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (phighvbox2), phighhbox8, TRUE, TRUE, 0);
  str = g_strdup_printf (_("Color %d"), 7);
  phighlabel7 = gtk_label_new (str);
  gtk_box_pack_start (GTK_BOX (phighhbox8), phighlabel7, TRUE, TRUE, 0);
  colorbutton7 = gtk_color_button_new ();
  gtk_box_pack_start (GTK_BOX (phighhbox8), colorbutton7, FALSE, FALSE, 0);

  phighhbox9 = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (phighvbox2), phighhbox9, TRUE, TRUE, 0);
  str = g_strdup_printf (_("Color %d"), 8);
  phighlabel8 = gtk_label_new (str);
  gtk_box_pack_start (GTK_BOX (phighhbox9), phighlabel8, TRUE, TRUE, 0);
  colorbutton8 = gtk_color_button_new ();
  gtk_box_pack_start (GTK_BOX (phighhbox9), colorbutton8, FALSE, FALSE, 0);

  gtk_color_button_set_color (GTK_COLOR_BUTTON(colorbutton1), &preferences.highcolor1);
  gtk_color_button_set_color (GTK_COLOR_BUTTON(colorbutton2), &preferences.highcolor2);
  gtk_color_button_set_color (GTK_COLOR_BUTTON(colorbutton3), &preferences.highcolor3);
  gtk_color_button_set_color (GTK_COLOR_BUTTON(colorbutton4), &preferences.highcolor4);
  gtk_color_button_set_color (GTK_COLOR_BUTTON(colorbutton5), &preferences.highcolor5);
  gtk_color_button_set_color (GTK_COLOR_BUTTON(colorbutton6), &preferences.highcolor6);
  gtk_color_button_set_color (GTK_COLOR_BUTTON(colorbutton7), &preferences.highcolor7);
  gtk_color_button_set_color (GTK_COLOR_BUTTON(colorbutton8), &preferences.highcolor8);

  pcolorsframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox4), pcolorsframe, FALSE, FALSE, 0);
  pcolorsframelabel = gtk_label_new (_("Colors for the chat window"));
  gtk_frame_set_label_widget (GTK_FRAME (pcolorsframe), pcolorsframelabel);
  pcolorsvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pcolorsframe), pcolorsvbox);

  pcolorshbox = gtk_hbox_new (TRUE, 0);
  gtk_container_add (GTK_CONTAINER (pcolorsvbox), pcolorshbox);
  promptcolorlabel = gtk_label_new (_("Prompt"));
  gtk_box_pack_start (GTK_BOX (pcolorshbox), promptcolorlabel, FALSE, FALSE, 0);
  promptcolorbutton = gtk_color_button_new ();
  gtk_box_pack_start (GTK_BOX (pcolorshbox), promptcolorbutton, FALSE, FALSE, 0);
  gtk_color_button_set_color
    (GTK_COLOR_BUTTON(promptcolorbutton), &preferences.promptcolor);

  pcolorshbox = gtk_hbox_new (TRUE, 0);
  gtk_container_add (GTK_CONTAINER (pcolorsvbox), pcolorshbox);
  sentcolorlabel = gtk_label_new (_("Sent text"));
  gtk_box_pack_start (GTK_BOX (pcolorshbox), sentcolorlabel, FALSE, FALSE, 0);
  sentcolorbutton = gtk_color_button_new ();
  gtk_box_pack_start (GTK_BOX (pcolorshbox), sentcolorbutton, FALSE, FALSE, 0);
  gtk_color_button_set_color
    (GTK_COLOR_BUTTON(sentcolorbutton), &preferences.sentcolor);

  pcolorshbox = gtk_hbox_new (TRUE, 0);
  gtk_container_add (GTK_CONTAINER (pcolorsvbox), pcolorshbox);
  wwvcolorlabel = gtk_label_new ("WWV / WCY");
  gtk_box_pack_start (GTK_BOX (pcolorshbox), wwvcolorlabel, FALSE, FALSE, 0);
  wwvcolorbutton = gtk_color_button_new ();
  gtk_box_pack_start (GTK_BOX (pcolorshbox), wwvcolorbutton, FALSE, FALSE, 0);
  gtk_color_button_set_color
    (GTK_COLOR_BUTTON(wwvcolorbutton), &preferences.wwvcolor);

  pcolorshbox = gtk_hbox_new (TRUE, 0);
  gtk_container_add (GTK_CONTAINER (pcolorsvbox), pcolorshbox);
  wxcolorlabel = gtk_label_new ("WX");
  gtk_box_pack_start (GTK_BOX (pcolorshbox), wxcolorlabel, FALSE, FALSE, 0);
  wxcolorbutton = gtk_color_button_new ();
  gtk_box_pack_start (GTK_BOX (pcolorshbox), wxcolorbutton, FALSE, FALSE, 0);
  gtk_color_button_set_color
    (GTK_COLOR_BUTTON(wxcolorbutton), &preferences.wxcolor);

  if (preferences.savedx == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavedxcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavedxcheckbutton), FALSE);
  if (preferences.savewwv == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavewwvcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavewwvcheckbutton), FALSE);
  if (preferences.savetoall == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavetoallcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavetoallcheckbutton), FALSE);
  if (preferences.savewx == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavewxcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavewxcheckbutton), FALSE);
 
  g_signal_connect ((gpointer) pautologincheckbutton, "toggled",
                    G_CALLBACK (on_pautologincheckbutton_toggled), NULL);
  g_signal_connect ((gpointer) phamlibcheckbutton, "toggled",
                    G_CALLBACK (on_phamlibcheckbutton_toggled), NULL);
  g_signal_connect ((gpointer) pfontsdxbutton, "clicked",
                    G_CALLBACK (on_fontbutton_clicked), GINT_TO_POINTER(1));
  g_signal_connect ((gpointer) pfontsallbutton, "clicked",
                    G_CALLBACK (on_fontbutton_clicked), GINT_TO_POINTER(2));

  g_object_set_data (G_OBJECT (preferencesdialog), "ploginhseparator", 
    ploginhseparator);
  g_object_set_data (G_OBJECT (preferencesdialog), "pcallsignlabel", 
    pcallsignlabel);
  g_object_set_data (G_OBJECT (preferencesdialog), "pcallsignentry", 
    pcallsignentry);
  g_object_set_data (G_OBJECT (preferencesdialog), "pcommandslabel", 
    pcommandslabel);
  g_object_set_data (G_OBJECT (preferencesdialog), "pcommandsentry", 
    pcommandsentry);

  g_object_set_data (G_OBJECT (preferencesdialog), "phamlibhseparator", 
    phamlibhseparator);
  g_object_set_data (G_OBJECT (preferencesdialog), "priglabel", 
    priglabel);
  g_object_set_data (G_OBJECT (preferencesdialog), "prigentry", 
    prigentry);

  g_object_set_data (G_OBJECT (preferencesdialog), "pfontsdxentry", 
    pfontsdxentry);
  g_object_set_data (G_OBJECT (preferencesdialog), "pfontsallentry", 
    pfontsallentry);

  gtk_widget_show_all (pnotebook);
  response = gtk_dialog_run (GTK_DIALOG (preferencesdialog));

  if (response == GTK_RESPONSE_OK)
  {
    /* callsign frame */
    str = gtk_editable_get_chars (GTK_EDITABLE (pcallsignentry), 0, -1);
    if (strlen(str) == 0)
      preferences.callsign = g_strdup ("N0CALL");
    else
      preferences.callsign = g_strdup (str);

    /* login frame */
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(pautologincheckbutton));
    if (state) 
      preferences.autologin = 1; 
    else 
      preferences.autologin = 0;
    str = gtk_editable_get_chars (GTK_EDITABLE (pcommandsentry), 0, -1);
    if (strlen(str) == 0)
      preferences.commands = g_strdup ("?");
    else
      preferences.commands = g_strdup (str);

    /* saving frame */
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(psavedxcheckbutton));
    if (state) 
      preferences.savedx = 1; 
    else 
      preferences.savedx = 0;
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(psavewwvcheckbutton));
    if (state) 
      preferences.savewwv = 1; 
    else 
      preferences.savewwv = 0;
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(psavetoallcheckbutton));
    if (state) 
      preferences.savetoall = 1; 
    else 
      preferences.savetoall = 0;
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(psavewxcheckbutton));
    if (state) 
      preferences.savewx = 1; 
    else 
      preferences.savewx = 0;

    /* hamlib frame */
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(phamlibcheckbutton));
    if (state) 
      preferences.hamlib = 1; 
    else 
      preferences.hamlib = 0;
    str = gtk_editable_get_chars (GTK_EDITABLE (prigentry), 0, -1);
    if (strlen(str) == 0)
      preferences.rigctl = g_strdup ("?");
    else
      preferences.rigctl = g_strdup (str);

    /* programs frame */
    str = gtk_editable_get_chars (GTK_EDITABLE (pprogbrowserentry), 0, -1);
    if (strlen(str) == 0)
      preferences.browserapp = g_strdup ("?");
    else
      preferences.browserapp = g_strdup (str);
    str = gtk_editable_get_chars (GTK_EDITABLE (pprogmailentry), 0, -1);
    if (strlen(str) == 0)
      preferences.mailapp = g_strdup ("?");
    else
      preferences.mailapp = g_strdup (str);
    str = gtk_editable_get_chars (GTK_EDITABLE (pprogsoundentry), 0, -1);
    if (strlen(str) == 0)
      preferences.soundapp = g_strdup ("?");
    else
      preferences.soundapp = g_strdup (str);

    /* general frame */
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(pechocheckbutton));
    if (state) 
      preferences.localecho = 1;
    else
      preferences.localecho = 0;
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(plivecheckbutton));
    if (state) 
      preferences.keepalive = 1;
    else
      preferences.keepalive = 0;

    /* columns frame */
    treeview = g_object_get_data (G_OBJECT (gui->window), "treeview");
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(pspottercheckbutton));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 0);
    if (state)
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), TRUE);
      if (preferences.col0visible == 0)
        gtk_tree_view_column_set_fixed_width (column, COL0WIDTH);
      preferences.col0visible = 1;
    }	    
    else
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
      preferences.col0visible = 0;
    }
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(pqrgcheckbutton));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 1);
    if (state)
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), TRUE);
      if (preferences.col1visible == 0)
        gtk_tree_view_column_set_fixed_width (column, COL1WIDTH);
      preferences.col1visible = 1;
    }	    
    else
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
      preferences.col1visible = 0;
    }
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(pdxcheckbutton));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 2);
    if (state)
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), TRUE);
      if (preferences.col2visible == 0)
        gtk_tree_view_column_set_fixed_width (column, COL2WIDTH);
      preferences.col2visible = 1;
    }	    
    else
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
      preferences.col2visible = 0;
    }
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(premarkscheckbutton));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 3);
    if (state)
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), TRUE);
      if (preferences.col3visible == 0)
        gtk_tree_view_column_set_fixed_width (column, COL3WIDTH);
      preferences.col3visible = 1;
    }	    
    else
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
      preferences.col3visible = 0;
    }
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(ptimecheckbutton));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 4);
    if (state)
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), TRUE);
      if (preferences.col4visible == 0)
        gtk_tree_view_column_set_fixed_width (column, COL4WIDTH);
      preferences.col4visible = 1;
    }	    
    else
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
      preferences.col4visible = 0;
    }
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(pinfocheckbutton));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 5);
    if (state)
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), TRUE);
      if (preferences.col5visible == 0)
        gtk_tree_view_column_set_fixed_width (column, COL5WIDTH);
      preferences.col5visible = 1;
    }	    
    else
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
      preferences.col5visible = 0;
    }

    /* fonts frame */
    str = gtk_editable_get_chars (GTK_EDITABLE (pfontsdxentry), 0, -1);
    font_description = pango_font_description_from_string (str);
    gtk_widget_modify_font (GTK_WIDGET(treeview), font_description);
    pango_font_description_free (font_description);
    preferences.dxfont = g_strdup (str);

    str = gtk_editable_get_chars (GTK_EDITABLE (pfontsallentry), 0, -1);
    font_description = pango_font_description_from_string (str);
    maintext = g_object_get_data (G_OBJECT (gui->window), "maintext");
    gtk_widget_modify_font (GTK_WIDGET(maintext), font_description);
    pango_size = pango_font_description_get_size (font_description);
    /* line spacing is half character size */
    g_object_set (G_OBJECT(maintext), "pixels-below-lines",
      PANGO_PIXELS (pango_size) / 2, NULL);
    pango_font_description_free (font_description);
    preferences.allfont = g_strdup (str);

    /* highlights frame*/
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (maintext));
    table = gtk_text_buffer_get_tag_table (buffer);

    gtk_color_button_get_color (GTK_COLOR_BUTTON(colorbutton1), &color);
    if (! gdk_color_equal(&color, &preferences.highcolor1))
    {
      str = g_strdup_printf ("#%02X%02X%02X",
	color.red * 255 / 65535, color.green * 255 / 65535, color.blue * 255 / 65535);
      gui->high1tagname = g_strdup_printf ("%d", rand ());
      gtk_text_buffer_create_tag (buffer, gui->high1tagname, "foreground",
	str, NULL);
      highentry1 = g_object_get_data (G_OBJECT (gui->window), "highentry1");
      gtk_widget_modify_text (highentry1, GTK_STATE_NORMAL, &color);
      preferences.highcolor1 = color;
    }
    gtk_color_button_get_color (GTK_COLOR_BUTTON(colorbutton2), &color);
    if (! gdk_color_equal(&color, &preferences.highcolor2))
    {
      str = g_strdup_printf ("#%02X%02X%02X",
	color.red * 255 / 65535, color.green * 255 / 65535, color.blue * 255 / 65535);
      gui->high2tagname = g_strdup_printf ("%d", rand ());
      gtk_text_buffer_create_tag (buffer, gui->high2tagname, "foreground",
	str, NULL);
      highentry2 = g_object_get_data (G_OBJECT (gui->window), "highentry2");
      gtk_widget_modify_text (highentry2, GTK_STATE_NORMAL, &color);
      preferences.highcolor2 = color;
    }
    gtk_color_button_get_color (GTK_COLOR_BUTTON(colorbutton3), &color);
    if (! gdk_color_equal(&color, &preferences.highcolor3))
    {
      str = g_strdup_printf ("#%02X%02X%02X",
	color.red * 255 / 65535, color.green * 255 / 65535, color.blue * 255 / 65535);
      gui->high3tagname = g_strdup_printf ("%d", rand ());
      gtk_text_buffer_create_tag (buffer, gui->high3tagname, "foreground",
	str, NULL);
      highentry3 = g_object_get_data (G_OBJECT (gui->window), "highentry3");
      gtk_widget_modify_text (highentry3, GTK_STATE_NORMAL, &color);
      preferences.highcolor3 = color;
    }
    gtk_color_button_get_color (GTK_COLOR_BUTTON(colorbutton4), &color);
    if (! gdk_color_equal(&color, &preferences.highcolor4))
    {
      str = g_strdup_printf ("#%02X%02X%02X",
	color.red * 255 / 65535, color.green * 255 / 65535, color.blue * 255 / 65535);
      gui->high4tagname = g_strdup_printf ("%d", rand ());
      gtk_text_buffer_create_tag (buffer, gui->high4tagname, "foreground",
	str, NULL);
      highentry4 = g_object_get_data (G_OBJECT (gui->window), "highentry4");
      gtk_widget_modify_text (highentry4, GTK_STATE_NORMAL, &color);
      preferences.highcolor4 = color;
    }
    gtk_color_button_get_color (GTK_COLOR_BUTTON(colorbutton5), &color);
    if (! gdk_color_equal(&color, &preferences.highcolor5))
    {
      str = g_strdup_printf ("#%02X%02X%02X",
	color.red * 255 / 65535, color.green * 255 / 65535, color.blue * 255 / 65535);
      gui->high5tagname = g_strdup_printf ("%d", rand ());
      gtk_text_buffer_create_tag (buffer, gui->high5tagname, "foreground",
	str, NULL);
      highentry5 = g_object_get_data (G_OBJECT (gui->window), "highentry5");
      gtk_widget_modify_text (highentry5, GTK_STATE_NORMAL, &color);
      preferences.highcolor5 = color;
    }
    gtk_color_button_get_color (GTK_COLOR_BUTTON(colorbutton6), &color);
    if (! gdk_color_equal(&color, &preferences.highcolor6))
    {
      str = g_strdup_printf ("#%02X%02X%02X",
	color.red * 255 / 65535, color.green * 255 / 65535, color.blue * 255 / 65535);
      gui->high6tagname = g_strdup_printf ("%d", rand ());
      gtk_text_buffer_create_tag (buffer, gui->high6tagname, "foreground",
	str, NULL);
      highentry6 = g_object_get_data (G_OBJECT (gui->window), "highentry6");
      gtk_widget_modify_text (highentry6, GTK_STATE_NORMAL, &color);
      preferences.highcolor6 = color;
    }
    gtk_color_button_get_color (GTK_COLOR_BUTTON(colorbutton7), &color);
    if (! gdk_color_equal(&color, &preferences.highcolor7))
    {
      str = g_strdup_printf ("#%02X%02X%02X",
	color.red * 255 / 65535, color.green * 255 / 65535, color.blue * 255 / 65535);
      gui->high7tagname = g_strdup_printf ("%d", rand ());
      gtk_text_buffer_create_tag (buffer, gui->high7tagname, "foreground",
	str, NULL);
      highentry7 = g_object_get_data (G_OBJECT (gui->window), "highentry7");
      gtk_widget_modify_text (highentry7, GTK_STATE_NORMAL, &color);
      preferences.highcolor7 = color;
    }
    gtk_color_button_get_color (GTK_COLOR_BUTTON(colorbutton8), &color);
    if (! gdk_color_equal(&color, &preferences.highcolor8))
    {
      str = g_strdup_printf ("#%02X%02X%02X",
	color.red * 255 / 65535, color.green * 255 / 65535, color.blue * 255 / 65535);
      gui->high8tagname = g_strdup_printf ("%d", rand ());
      gtk_text_buffer_create_tag (buffer, gui->high8tagname, "foreground",
	str, NULL);
      highentry8 = g_object_get_data (G_OBJECT (gui->window), "highentry8");
      gtk_widget_modify_text (highentry8, GTK_STATE_NORMAL, &color);
      preferences.highcolor8 = color;
    }

    /* colors frame */
    gtk_color_button_get_color (GTK_COLOR_BUTTON(promptcolorbutton), &color);
    if (! gdk_color_equal(&color, &preferences.promptcolor))
    {
      str = g_strdup_printf ("#%02X%02X%02X",
	color.red * 255 / 65535, color.green * 255 / 65535, color.blue * 255 / 65535);
      gui->prompttagname = g_strdup_printf ("%d", rand ());
      gtk_text_buffer_create_tag
        (buffer, gui->prompttagname, "foreground", str, NULL);
      gui->calltagname = g_strdup_printf ("%d", rand ());
      gtk_text_buffer_create_tag
        (buffer, gui->calltagname, "foreground", str, "weight", PANGO_WEIGHT_BOLD, NULL);
      preferences.promptcolor = color;
    }

    gtk_color_button_get_color (GTK_COLOR_BUTTON(sentcolorbutton), &color);
    if (! gdk_color_equal(&color, &preferences.sentcolor))
    {
      str = g_strdup_printf ("#%02X%02X%02X",
	color.red * 255 / 65535, color.green * 255 / 65535, color.blue * 255 / 65535);
      gui->senttagname = g_strdup_printf ("%d", rand ());
      gtk_text_buffer_create_tag
        (buffer, gui->senttagname, "foreground", str, NULL);
      preferences.sentcolor = color;
    }

    gtk_color_button_get_color (GTK_COLOR_BUTTON(wwvcolorbutton), &color);
    if (! gdk_color_equal(&color, &preferences.wwvcolor))
    {
      str = g_strdup_printf ("#%02X%02X%02X",
	color.red * 255 / 65535, color.green * 255 / 65535, color.blue * 255 / 65535);
      gui->wwvtagname = g_strdup_printf ("%d", rand ());
      gtk_text_buffer_create_tag
        (buffer, gui->wwvtagname, "foreground", str, NULL);
      preferences.wwvcolor = color;
    }

    gtk_color_button_get_color (GTK_COLOR_BUTTON(wxcolorbutton), &color);
    if (! gdk_color_equal(&color, &preferences.wxcolor))
    {
      str = g_strdup_printf ("#%02X%02X%02X",
	color.red * 255 / 65535, color.green * 255 / 65535, color.blue * 255 / 65535);
      gui->wxtagname = g_strdup_printf ("%d", rand ());
      gtk_text_buffer_create_tag
        (buffer, gui->wxtagname, "foreground", str, NULL);
      preferences.wxcolor = color;
    }
    g_free (str);
  }

  gtk_widget_destroy (preferencesdialog);
  mainentry = g_object_get_data (G_OBJECT (gui->window), "mainentry");
  gtk_widget_set_sensitive (gui->window, 1);
  gtk_widget_grab_focus (GTK_WIDGET (mainentry));

}
