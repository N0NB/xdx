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
 * gui_settingsdialog.c
 */

#include <gtk/gtk.h>
#include "gui_settingsdialog.h"
#include "utils.h"
#include "gui.h"
#include "preferences.h"

GtkWidget *preferencesdialog;
extern preferencestype preferences;

void
on_pautologincheckbutton_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkWidget *pautologincheckbutton, *ploginhseparator, *pcallsignlabel,
    *pcallsignentry, *pcommandslabel, *pcommandsentry;
  gboolean state;

  pautologincheckbutton = g_object_get_data (G_OBJECT (preferencesdialog), 
    "pautologincheckbutton");
  ploginhseparator = g_object_get_data (G_OBJECT (preferencesdialog), 
    "ploginhseparator");
  pcallsignlabel = g_object_get_data (G_OBJECT (preferencesdialog), 
    "pcallsignlabel");
  pcallsignentry = g_object_get_data (G_OBJECT (preferencesdialog), 
    "pcallsignentry");
  pcommandslabel = g_object_get_data (G_OBJECT (preferencesdialog), 
    "pcommandslabel");
  pcommandsentry = g_object_get_data (G_OBJECT (preferencesdialog), 
    "pcommandsentry");

  state = gtk_toggle_button_get_active 
    (GTK_TOGGLE_BUTTON(pautologincheckbutton));
  if (state)
  {
    gtk_widget_set_sensitive (ploginhseparator, TRUE);
    gtk_widget_set_sensitive (pcallsignlabel, TRUE);
    gtk_widget_set_sensitive (pcallsignentry, TRUE);
    gtk_widget_set_sensitive (pcommandslabel, TRUE);
    gtk_widget_set_sensitive (pcommandsentry, TRUE);
  }
  else
  {
    gtk_widget_set_sensitive (ploginhseparator, FALSE);
    gtk_widget_set_sensitive (pcallsignlabel, FALSE);
    gtk_widget_set_sensitive (pcallsignentry, FALSE);
    gtk_widget_set_sensitive (pcommandslabel, FALSE);
    gtk_widget_set_sensitive (pcommandsentry, FALSE);
  }
}


void
on_phamlibcheckbutton_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}

/*
 * called from the menu
 */
void on_settings_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  GtkWidget *pdialog_vbox, *pvbox, 

    *ploginframe, *ploginvbox, *pautologincheckbutton, *ploginhseparator, 
    *ploginhbox, *pcallsignlabel, *pcallsignentry, *pcommandshbox, 
    *pcommandslabel, *pcommandsentry, *ploginlabel,

    *psavingframe, *psavingvbox, *psavedxcheckbutton, *psavewwvcheckbutton,
    *psavetoallcheckbutton, *psavewxcheckbutton, *psavinglabel, 

    *phamlibframe, *phamlibvbox, *phamlibcheckbutton, *phamlibhseparator, 
    *phamlibhbox, *priglabel, *prigentry, *phamliblabel, 

    *pprogframe, *pprogvbox, *pproghbox1, *pprogbrowserlabel, 
    *pprogbrowserentry, *pproghbox2, *pprogmaillabel,
    *pprogmailentry, *pproglabel, 

    *pdialog_action_area;
  gint response;
  gboolean state;
  gchar *str;
  GtkTooltips *tooltips;

  gtk_widget_set_sensitive (gui->window, 0);
  preferencesdialog = gtk_dialog_new_with_buttons (_("xdx - preferences"),
					    GTK_WINDOW (gui->window),
					    GTK_DIALOG_MODAL |
					    GTK_DIALOG_DESTROY_WITH_PARENT,
					    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					    GTK_STOCK_OK,
					    GTK_RESPONSE_OK, NULL);

  pdialog_vbox = GTK_DIALOG (preferencesdialog)->vbox;
  pvbox = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (pdialog_vbox), pvbox, TRUE, TRUE, 0);

  ploginframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox), ploginframe, TRUE, TRUE, 0);
  ploginvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (ploginframe), ploginvbox);
  pautologincheckbutton = gtk_check_button_new_with_mnemonic (_("Enable autologin"));
  gtk_box_pack_start (GTK_BOX (ploginvbox), pautologincheckbutton, FALSE, FALSE, 0);
  ploginhseparator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (ploginvbox), ploginhseparator, TRUE, TRUE, 0);
  ploginhbox = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (ploginvbox), ploginhbox, TRUE, TRUE, 0);
  pcallsignlabel = gtk_label_new (_("Callsign"));
  gtk_box_pack_start (GTK_BOX (ploginhbox), pcallsignlabel, FALSE, FALSE, 0);
  pcallsignentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (ploginhbox), pcallsignentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (pcallsignentry), 15);
  pcommandshbox = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (ploginvbox), pcommandshbox, TRUE, TRUE, 0);
  pcommandslabel = gtk_label_new (_("Commands"));
  gtk_box_pack_start (GTK_BOX (pcommandshbox), pcommandslabel, FALSE, FALSE, 0);
  pcommandsentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (pcommandshbox), pcommandsentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (pcommandsentry), 80);
  tooltips = gtk_tooltips_new ();
  gtk_tooltips_set_tip (tooltips, pcommandsentry, 
    _("Comma separated list of commands to send at login"), NULL);

  ploginlabel = gtk_label_new (_("Login"));
  gtk_frame_set_label_widget (GTK_FRAME (ploginframe), ploginlabel);

  if (preferences.autologin == 1)
  {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pautologincheckbutton), TRUE);
    gtk_widget_set_sensitive (ploginhseparator, TRUE);
    gtk_widget_set_sensitive (pcallsignlabel, TRUE);
    gtk_widget_set_sensitive (pcallsignentry, TRUE);
    gtk_widget_set_sensitive (pcommandslabel, TRUE);
    gtk_widget_set_sensitive (pcommandsentry, TRUE);
  }
  else
  {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pautologincheckbutton), FALSE);
    gtk_widget_set_sensitive (ploginhseparator, FALSE);
    gtk_widget_set_sensitive (pcallsignlabel, FALSE);
    gtk_widget_set_sensitive (pcallsignentry, FALSE);
    gtk_widget_set_sensitive (pcommandslabel, FALSE);
    gtk_widget_set_sensitive (pcommandsentry, FALSE);
  }
  if (g_ascii_strcasecmp (preferences.callsign, "?"))
    gtk_entry_set_text (GTK_ENTRY(pcallsignentry), preferences.callsign);
  if (g_ascii_strcasecmp (preferences.commands, "?"))
    gtk_entry_set_text (GTK_ENTRY(pcommandsentry), preferences.commands);

  psavingframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox), psavingframe, TRUE, TRUE, 0);
  psavingvbox = gtk_vbox_new (TRUE, 0);
  gtk_container_add (GTK_CONTAINER (psavingframe), psavingvbox);
  psavedxcheckbutton = gtk_check_button_new_with_mnemonic (_("Save DX spots"));
  gtk_box_pack_start (GTK_BOX (psavingvbox), psavedxcheckbutton, FALSE, FALSE, 0);
  psavewwvcheckbutton = gtk_check_button_new_with_mnemonic (_("Save WCY/WWV"));
  gtk_box_pack_start (GTK_BOX (psavingvbox), psavewwvcheckbutton, FALSE, FALSE, 0);
  psavetoallcheckbutton = gtk_check_button_new_with_mnemonic (_("Save \"To all\""));
  gtk_box_pack_start (GTK_BOX (psavingvbox), psavetoallcheckbutton, FALSE, FALSE, 0);
  psavewxcheckbutton = gtk_check_button_new_with_mnemonic (_("Save WX"));
  gtk_box_pack_start (GTK_BOX (psavingvbox), psavewxcheckbutton, FALSE, FALSE, 0);
  psavinglabel = gtk_label_new (_("Saving"));
  gtk_frame_set_label_widget (GTK_FRAME (psavingframe), psavinglabel);

  if (preferences.savedx == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavedxcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavedxcheckbutton), FALSE);
  if (preferences.savewwv == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavewwvcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavewwvcheckbutton), FALSE);

  phamlibframe = gtk_frame_new (NULL);
  gtk_widget_show (phamlibframe);
  gtk_box_pack_start (GTK_BOX (pvbox), phamlibframe, TRUE, TRUE, 0);
  phamlibvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (phamlibframe), phamlibvbox);
  phamlibcheckbutton = gtk_check_button_new_with_mnemonic (_("Enable hamlib"));
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

  pprogframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox), pprogframe, TRUE, TRUE, 0);
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
  pproglabel = gtk_label_new (_("Programs"));
  gtk_frame_set_label_widget (GTK_FRAME (pprogframe), pproglabel);

  g_signal_connect ((gpointer) pautologincheckbutton, "toggled",
                    G_CALLBACK (on_pautologincheckbutton_toggled),
                    NULL);
  g_signal_connect ((gpointer) phamlibcheckbutton, "toggled",
                    G_CALLBACK (on_phamlibcheckbutton_toggled),
                    NULL);

  g_object_set_data (G_OBJECT (preferencesdialog), "pautologincheckbutton", 
    pautologincheckbutton);
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

  g_object_set_data (G_OBJECT (preferencesdialog), "phamlibcheckbutton", 
    phamlibcheckbutton);

  gtk_widget_show_all (pvbox);
  response = gtk_dialog_run (GTK_DIALOG (preferencesdialog));

  if (response == GTK_RESPONSE_OK)
  {
    /* login frame */
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(pautologincheckbutton));
    if (state) 
      preferences.autologin = 1; 
    else 
      preferences.autologin = 0;
    str = gtk_editable_get_chars (GTK_EDITABLE (pcallsignentry), 0, -1);
    if (strlen(str) == 0)
      preferences.callsign = g_strdup ("?");
    else
      preferences.callsign = g_strdup (str);
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
    g_free (str);
  }

  gtk_widget_destroy (preferencesdialog);
  gtk_widget_set_sensitive (gui->window, 1);

}
