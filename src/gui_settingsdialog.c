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

void
on_pautologincheckbutton_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

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
  GtkWidget *preferencesdialog, *pdialog_vbox, *pvbox, *ploginframe,
    *ploginvbox, *pautologincheckbutton, *ploginhseparator, *ploginhbox,
    *pcallsignlabel, *pcallsignentry, *preconnectcheckbutton, *ploginlabel,
    *psavingframe, *psavingvbox, *psavedxcheckbutton, *psavewwvcheckbutton,
    *psavetoallcheckbutton, *psavewxcheckbutton, *psavinglabel, *phamlibframe,
    *phamlibvbox, *phamlibcheckbutton, *phamlibhseparator, *phamlibhbox,
    *priglabel, *prigentry, *phamliblabel, *pprogframe, *pprogvbox, *pproghbox1,
    *pprogbrowserlabel, *pprogbrowserentry, *pproghbox2, *pprogmaillabel,
    *pprogmailentry, *pproglabel, *pdialog_action_area;
  gint response;

  gtk_widget_set_sensitive (gui->window, 0);
  preferencesdialog = gtk_dialog_new_with_buttons (_("xdx - preferences"),
					    GTK_WINDOW (gui->window),
					    GTK_DIALOG_MODAL |
					    GTK_DIALOG_DESTROY_WITH_PARENT,
					    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					    GTK_STOCK_OK,
					    GTK_RESPONSE_OK, NULL);

  pdialog_vbox = GTK_DIALOG (preferencesdialog)->vbox;
  gtk_widget_show (pdialog_vbox);

  pvbox = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (pvbox);
  gtk_box_pack_start (GTK_BOX (pdialog_vbox), pvbox, TRUE, TRUE, 0);

  ploginframe = gtk_frame_new (NULL);
  gtk_widget_show (ploginframe);
  gtk_box_pack_start (GTK_BOX (pvbox), ploginframe, TRUE, TRUE, 0);

  ploginvbox = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (ploginvbox);
  gtk_container_add (GTK_CONTAINER (ploginframe), ploginvbox);

  pautologincheckbutton = gtk_check_button_new_with_mnemonic (_("Enable autologin"));
  gtk_widget_show (pautologincheckbutton);
  gtk_box_pack_start (GTK_BOX (ploginvbox), pautologincheckbutton, FALSE, FALSE, 0);

  ploginhseparator = gtk_hseparator_new ();
  gtk_widget_show (ploginhseparator);
  gtk_box_pack_start (GTK_BOX (ploginvbox), ploginhseparator, TRUE, TRUE, 0);

  ploginhbox = gtk_hbox_new (TRUE, 0);
  gtk_widget_show (ploginhbox);
  gtk_box_pack_start (GTK_BOX (ploginvbox), ploginhbox, TRUE, TRUE, 0);

  pcallsignlabel = gtk_label_new (_("Callsign"));
  gtk_widget_show (pcallsignlabel);
  gtk_box_pack_start (GTK_BOX (ploginhbox), pcallsignlabel, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (pcallsignlabel), GTK_JUSTIFY_LEFT);

  pcallsignentry = gtk_entry_new ();
  gtk_widget_show (pcallsignentry);
  gtk_box_pack_start (GTK_BOX (ploginhbox), pcallsignentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (pcallsignentry), 10);

  preconnectcheckbutton = gtk_check_button_new_with_mnemonic (_("Enable auto-reconnect"));
  gtk_widget_show (preconnectcheckbutton);
  gtk_box_pack_start (GTK_BOX (ploginvbox), preconnectcheckbutton, FALSE, FALSE, 0);

  ploginlabel = gtk_label_new (_("Login"));
  gtk_widget_show (ploginlabel);
  gtk_frame_set_label_widget (GTK_FRAME (ploginframe), ploginlabel);
  gtk_label_set_justify (GTK_LABEL (ploginlabel), GTK_JUSTIFY_LEFT);

  psavingframe = gtk_frame_new (NULL);
  gtk_widget_show (psavingframe);
  gtk_box_pack_start (GTK_BOX (pvbox), psavingframe, TRUE, TRUE, 0);

  psavingvbox = gtk_vbox_new (TRUE, 0);
  gtk_widget_show (psavingvbox);
  gtk_container_add (GTK_CONTAINER (psavingframe), psavingvbox);

  psavedxcheckbutton = gtk_check_button_new_with_mnemonic (_("Save DX spots"));
  gtk_widget_show (psavedxcheckbutton);
  gtk_box_pack_start (GTK_BOX (psavingvbox), psavedxcheckbutton, FALSE, FALSE, 0);

  psavewwvcheckbutton = gtk_check_button_new_with_mnemonic (_("Save WWV"));
  gtk_widget_show (psavewwvcheckbutton);
  gtk_box_pack_start (GTK_BOX (psavingvbox), psavewwvcheckbutton, FALSE, FALSE, 0);

  psavetoallcheckbutton = gtk_check_button_new_with_mnemonic (_("Save \"To all\""));
  gtk_widget_show (psavetoallcheckbutton);
  gtk_box_pack_start (GTK_BOX (psavingvbox), psavetoallcheckbutton, FALSE, FALSE, 0);

  psavewxcheckbutton = gtk_check_button_new_with_mnemonic (_("Save WX"));
  gtk_widget_show (psavewxcheckbutton);
  gtk_box_pack_start (GTK_BOX (psavingvbox), psavewxcheckbutton, FALSE, FALSE, 0);

  psavinglabel = gtk_label_new (_("Saving"));
  gtk_widget_show (psavinglabel);
  gtk_frame_set_label_widget (GTK_FRAME (psavingframe), psavinglabel);
  gtk_label_set_justify (GTK_LABEL (psavinglabel), GTK_JUSTIFY_LEFT);

  phamlibframe = gtk_frame_new (NULL);
  gtk_widget_show (phamlibframe);
  gtk_box_pack_start (GTK_BOX (pvbox), phamlibframe, TRUE, TRUE, 0);

  phamlibvbox = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (phamlibvbox);
  gtk_container_add (GTK_CONTAINER (phamlibframe), phamlibvbox);

  phamlibcheckbutton = gtk_check_button_new_with_mnemonic (_("Enable hamlib"));
  gtk_widget_show (phamlibcheckbutton);
  gtk_box_pack_start (GTK_BOX (phamlibvbox), phamlibcheckbutton, FALSE, FALSE, 0);

  phamlibhseparator = gtk_hseparator_new ();
  gtk_widget_show (phamlibhseparator);
  gtk_box_pack_start (GTK_BOX (phamlibvbox), phamlibhseparator, TRUE, TRUE, 0);

  phamlibhbox = gtk_hbox_new (TRUE, 0);
  gtk_widget_show (phamlibhbox);
  gtk_box_pack_start (GTK_BOX (phamlibvbox), phamlibhbox, TRUE, TRUE, 0);

  priglabel = gtk_label_new (_("Command for rigctl"));
  gtk_widget_show (priglabel);
  gtk_box_pack_start (GTK_BOX (phamlibhbox), priglabel, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (priglabel), GTK_JUSTIFY_LEFT);

  prigentry = gtk_entry_new ();
  gtk_widget_show (prigentry);
  gtk_box_pack_start (GTK_BOX (phamlibhbox), prigentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (prigentry), 10);

  phamliblabel = gtk_label_new (_("Hamlib"));
  gtk_widget_show (phamliblabel);
  gtk_frame_set_label_widget (GTK_FRAME (phamlibframe), phamliblabel);
  gtk_label_set_justify (GTK_LABEL (phamliblabel), GTK_JUSTIFY_LEFT);

  pprogframe = gtk_frame_new (NULL);
  gtk_widget_show (pprogframe);
  gtk_box_pack_start (GTK_BOX (pvbox), pprogframe, TRUE, TRUE, 0);

  pprogvbox = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (pprogvbox);
  gtk_container_add (GTK_CONTAINER (pprogframe), pprogvbox);

  pproghbox1 = gtk_hbox_new (TRUE, 0);
  gtk_widget_show (pproghbox1);
  gtk_box_pack_start (GTK_BOX (pprogvbox), pproghbox1, TRUE, TRUE, 0);

  pprogbrowserlabel = gtk_label_new (_("Web browser"));
  gtk_widget_show (pprogbrowserlabel);
  gtk_box_pack_start (GTK_BOX (pproghbox1), pprogbrowserlabel, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (pprogbrowserlabel), GTK_JUSTIFY_LEFT);

  pprogbrowserentry = gtk_entry_new ();
  gtk_widget_show (pprogbrowserentry);
  gtk_box_pack_start (GTK_BOX (pproghbox1), pprogbrowserentry, TRUE, TRUE, 5);

  pproghbox2 = gtk_hbox_new (TRUE, 0);
  gtk_widget_show (pproghbox2);
  gtk_box_pack_start (GTK_BOX (pprogvbox), pproghbox2, TRUE, TRUE, 0);

  pprogmaillabel = gtk_label_new (_("Mail program"));
  gtk_widget_show (pprogmaillabel);
  gtk_box_pack_start (GTK_BOX (pproghbox2), pprogmaillabel, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (pprogmaillabel), GTK_JUSTIFY_LEFT);

  pprogmailentry = gtk_entry_new ();
  gtk_widget_show (pprogmailentry);
  gtk_box_pack_start (GTK_BOX (pproghbox2), pprogmailentry, TRUE, TRUE, 5);

  pproglabel = gtk_label_new (_("Programs"));
  gtk_widget_show (pproglabel);
  gtk_frame_set_label_widget (GTK_FRAME (pprogframe), pproglabel);
  gtk_label_set_justify (GTK_LABEL (pproglabel), GTK_JUSTIFY_LEFT);

  g_signal_connect ((gpointer) pautologincheckbutton, "toggled",
                    G_CALLBACK (on_pautologincheckbutton_toggled),
                    NULL);
  g_signal_connect ((gpointer) phamlibcheckbutton, "toggled",
                    G_CALLBACK (on_phamlibcheckbutton_toggled),
                    NULL);

  response = gtk_dialog_run (GTK_DIALOG (preferencesdialog));

  if (response == GTK_RESPONSE_OK)
  {
  }

  gtk_widget_destroy (preferencesdialog);
  gtk_widget_set_sensitive (gui->window, 1);

}
