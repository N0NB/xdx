/*
 * xdx - GTK+ DX-cluster client for amateur radio
 * Copyright (C) 2002-2003 Joop Stakenborg <pg4i@amsat.org>
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
 * net.c - private functions for sending and receiving data, connecting and 
 * disconnecting.
 */


#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif
#ifndef WEXITSTATUS
# define WEXITSTATUS(stat_val) ((unsigned)(stat_val) >> 8)
#endif
#ifndef WIFEXITED
# define WIFEXITED(stat_val) (((stat_val) & 255) == 0)
#endif

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <resolv.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <gtk/gtk.h>
#include "gui.h"
#include "net.h"
#include "utils.h"
#include "text.h"
#include "preferences.h"
#include "history.h"

extern preferencestype preferences;

/* create a new struct for the server */
servertype *new_cluster(void)
{
  servertype *server = g_new0(servertype, 1);
  server->host = NULL;
  server->port = NULL;
  server->rxchannel = NULL;
  server->source_id = 0;
  server->sockethandle = -1;
  server->connected = FALSE;
  server->reconnecttimer = -1;
  server->reconnect = FALSE;
  server->lastcommand = NULL;
  server->keepalivetimer = -1;
  return(server);
}

/* 
 * resolve, connect and create the io channel for reading
 */

gboolean
clresolve (servertype *cluster)
{
  gint ret;
  GString *msg = g_string_new ("");
  struct sockaddr_in claddress;
  struct hostent *clhostent;
  GError *err = NULL;
  GIOStatus res = G_IO_STATUS_NORMAL;

  g_string_printf (msg, _("Resolving %s..."), cluster->host);
  updatestatusbar (msg, FALSE);

  clhostent = gethostbyname (cluster->host);
  if (clhostent == NULL)
    {
      g_string_printf (msg, _("Resolve failed: %s"), hstrerror (h_errno));
      updatestatusbar (msg, FALSE);
      g_string_free (msg, TRUE);
      return FALSE;
    }
  else
    {
      g_string_printf (msg, _("Connecting to: %s"),
		       inet_ntoa (*((struct in_addr *) clhostent->h_addr)));
      updatestatusbar (msg, FALSE);
    }

  if ((cluster->sockethandle = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      msg = g_string_new (g_strerror (errno));
      updatestatusbar (msg, FALSE);
      g_string_free (msg, TRUE);
      return FALSE;
    }

  claddress.sin_family = AF_INET;
  claddress.sin_port = htons (atoi (cluster->port));
  bcopy (clhostent->h_addr, &claddress.sin_addr, clhostent->h_length);
  bzero (&(claddress.sin_zero), 8);

  ret = connect (cluster->sockethandle, (struct sockaddr *) &claddress,
		 sizeof (struct sockaddr));

  if (ret == -1 && errno != EINPROGRESS)
    {
      if (preferences.reconnect == 1 && cluster->reconnect)
      {
        g_string_printf (msg, ("%s, trying reconnect in 10 seconds"), g_strerror (errno));
        updatestatusbar (msg, FALSE);
        logconnection (msg);
        cluster->reconnecttimer = g_timeout_add (10000, reconnect, NULL);
      }
      else
      {
        msg = g_string_new (g_strerror (errno));
        updatestatusbar (msg, FALSE);
        logconnection (msg);
        g_string_free (msg, TRUE);
      }
      return FALSE;
    }
  
  if (cluster->reconnect) cluster->reconnect = FALSE;
  g_string_printf (msg, _("Connected to %s"), cluster->host);
  updatestatusbar (msg, FALSE);
  logconnection (msg);
  g_string_free (msg, TRUE);

  menu_set_sensitive (gui->ui_manager, "/MainMenu/HostMenu/Open", FALSE);
  menu_set_sensitive (gui->ui_manager, "/MainMenu/HostMenu/Close", TRUE);

  cluster->rxchannel = g_io_channel_unix_new (cluster->sockethandle);
  g_io_channel_set_flags (cluster->rxchannel, G_IO_FLAG_NONBLOCK, &err);
  res = g_io_channel_set_encoding (cluster->rxchannel, NULL, &err);
  cluster->keepalivetimer = g_timeout_add (500000, send_keepalivepacket, NULL);
  cluster->source_id = g_io_add_watch
    (cluster->rxchannel, G_IO_IN, rx, cluster);

  return TRUE;
}


/*
 * disconnect routine, called from the disconnect dialog
 */
void
cldisconnect (GString *msg, gboolean timeout)
{
  servertype *cluster;

  cluster = g_object_get_data (G_OBJECT(gui->window), "cluster");

  if (cluster->rxchannel)
  {
    g_io_channel_shutdown (cluster->rxchannel, TRUE, NULL);
    g_io_channel_unref (cluster->rxchannel);
    cluster->rxchannel = NULL;
  }
  g_source_remove (cluster->source_id);
  g_source_remove (cluster->keepalivetimer);

  close (cluster->sockethandle);
  cluster->sockethandle = -1;
  cluster->connected = FALSE;

  if (msg) updatestatusbar (msg, timeout);

  menu_set_sensitive (gui->ui_manager, "/MainMenu/HostMenu/Open", TRUE);
  menu_set_sensitive (gui->ui_manager, "/MainMenu/HostMenu/Close", FALSE);
}

gint
send_keepalivepacket (gpointer data)
{
  if (preferences.keepalive == 1)
  {
    servertype *cluster = g_object_get_data(G_OBJECT(gui->window), "cluster");
    write (cluster->sockethandle, "\n", 1);
  }
  return TRUE;
}

gint
reconnect (gpointer data)
{
  servertype *cluster;

  cluster = g_object_get_data (G_OBJECT(gui->window), "cluster");
  g_source_remove (cluster->reconnecttimer);
  clresolve (cluster);
  return FALSE;
}

/*
 * a message is received here
 */
gboolean
rx (GIOChannel * channel, GIOCondition cond, gpointer data)
{
  gchar buf[1024], **sendsplit = NULL;
  gsize numbytes;
  GString *msg = g_string_new ("");
  GString *txstr = g_string_new ("");
  GIOStatus res = G_IO_STATUS_NORMAL;
  GError *err = NULL;
  gboolean ret = FALSE;
  gint i = 0;
  servertype *cluster = (servertype *)data;

  do
    res = g_io_channel_read_chars (cluster->rxchannel, buf, 1024, &numbytes, &err);
  while (res == G_IO_STATUS_AGAIN);

  switch (res)
    {
    case G_IO_STATUS_ERROR: /* connection refused ? */
      g_string_printf (msg, ("%s while connected"), err->message);
      cldisconnect (msg, FALSE);
      g_string_free (msg, TRUE);
      g_error_free (err);
      err = NULL;
      return FALSE;
      break;
    case G_IO_STATUS_NORMAL:
      ret = TRUE;
      break;
    case G_IO_STATUS_EOF: /* remote end has closed connection */
      if (preferences.reconnect == 1 &&
          g_ascii_strncasecmp(cluster->lastcommand, "/b", 2) != 0 &&
          g_ascii_strncasecmp(cluster->lastcommand, "/q", 2) != 0)
      {
        g_string_printf (msg, _("Connection closed, trying reconnect in 10 seconds"));
        cldisconnect (msg, FALSE);
        logconnection (msg);
        cluster->reconnecttimer = g_timeout_add (10000, reconnect, NULL);
        cluster->reconnect = TRUE;
      }
      else
      {
        g_string_printf (msg, _("Connection closed by remote host"));
        logconnection (msg);
        cldisconnect (msg, FALSE);
      }
      g_string_free (msg, TRUE);
      return FALSE;
      break;
    default:
      break;
    }

  if ((cond & G_IO_IN) && G_IO_STATUS_NORMAL)
    {
      if (numbytes == 0) /* remote end has closed connection ? */
      {
        g_string_printf (msg, _("Connection closed by remote host (0 bytes received)"));
        cldisconnect (msg, FALSE);
        g_string_free (msg, TRUE);
        ret = FALSE;
      }
      else
      {
        maintext_add (buf, numbytes, MESSAGE_RX);
        /* autologin */
        if (!cluster->connected && (preferences.autologin == 1) && 
          (g_ascii_strcasecmp (preferences.callsign, "?")))
        {
          g_string_printf (txstr, "%s", preferences.callsign);
          tx (txstr);
          g_string_free (txstr, TRUE);
          if (g_ascii_strcasecmp (preferences.commands, "?"))
          {
            sendsplit = g_strsplit (preferences.commands, ",", 0);
            while (sendsplit[i])
            {
              if (strlen(sendsplit[i]) > 0)
              {
                txstr = g_string_new (sendsplit[i]);
                tx (txstr);
                usleep (500000);
                while (gtk_events_pending()) gtk_main_iteration ();
                g_string_free (txstr, TRUE);
              }
              i++;
            }
            g_strfreev (sendsplit);
          }
          cluster->connected = TRUE;
        }
      }
    }
  return ret;
}

/*
 * send messages to the socket
 */

void
tx (GString * txmsg)
{
  gint numbytes;
  GString *errmsg = g_string_new ("");
  servertype *cluster;
	
  cluster = g_object_get_data(G_OBJECT(gui->window), "cluster");

  if ((cluster->rxchannel) && (cluster->sockethandle != -1))
  {
    if (txmsg->len > 0) tx_save(txmsg);
    txmsg = g_string_append (txmsg, "\n");
    numbytes = write (cluster->sockethandle, txmsg->str, txmsg->len);
    if (numbytes == -1)
    {
      g_string_printf (errmsg, _("Write failed: %s"), g_strerror (errno));
      updatestatusbar (errmsg, FALSE);
      g_string_free (errmsg, TRUE);
      return;
    }
    else if (preferences.localecho == 1)
    {
      maintext_add (txmsg->str, txmsg->len, MESSAGE_TX);
    }
    cluster->lastcommand = g_strdup (txmsg->str);
  }
  else
  {
    g_string_printf (errmsg, _("Nothing to send, you are not connected"));
    updatestatusbar (errmsg, FALSE);
    g_string_free (errmsg, TRUE);
  }
}
