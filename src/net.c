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
 * net.c - private functions for sending and receiving data, connecting and 
 * disconnecting.
 */

#if STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#else
# if HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif

#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#if HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif
#ifndef WEXITSTATUS
# define WEXITSTATUS(stat_val) ((unsigned)(stat_val) >> 8)
#endif
#ifndef WIFEXITED
# define WIFEXITED(stat_val) (((stat_val) & 255) == 0)
#endif

#if HAVE_STRING_H
# if !STDC_HEADERS && HAVE_MEMORY_H
#  include <memory.h>
# endif
# include <string.h>
#endif

#if HAVE_STRINGS_H
# include <strings.h>
#endif

#include <errno.h>
#include <resolv.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <gtk/gtk.h>
#include "gui.h"
#include "net.h"
#include "utils.h"
#include "text.h"


/* create a new struct for the server */
servertype *new_cluster(void)
{
        servertype *server = g_new0(servertype, 1);
        server->host = NULL;
        server->port = NULL;
	server->rxchannel = NULL;
        server->source_id = 0;
	server->sockethandle = -1;
        return(server);
}

/* 
 * resolve, connect and create the io channel for reading
 */

gboolean
clresolve (servertype *cluster)
{
  gint ret, opt;
  GString *msg = g_string_new ("");
  struct sockaddr_in claddress;
  struct hostent *clhostent;
  GError *err = NULL;
  GIOStatus res = G_IO_STATUS_NORMAL;

  g_string_printf (msg, _("Resolving %s..."), cluster->host);
  updatestatusbar (msg);

  clhostent = gethostbyname (cluster->host);
  if (clhostent == NULL)
    {
      g_string_printf (msg, _("Resolve failed: %s"), hstrerror (h_errno));
      updatestatusbar (msg);
      g_string_free (msg, TRUE);
      return FALSE;
    }
  else
    {
      g_string_printf (msg, _("Connecting to: %s"),
		       inet_ntoa (*((struct in_addr *) clhostent->h_addr)));
      updatestatusbar (msg);
    }

  if ((cluster->sockethandle = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      msg = g_string_new (strerror (errno));
      updatestatusbar (msg);
      g_string_free (msg, TRUE);
      return FALSE;
    }

  /* send keepalive probes */
  setsockopt (cluster->sockethandle, SOL_SOCKET, SO_KEEPALIVE, (gchar *) & opt,
	      sizeof (opt));
  /* make connection setup non-blocking */
  fcntl (cluster->sockethandle, F_SETFL, O_NONBLOCK);

  claddress.sin_family = AF_INET;
  claddress.sin_port = htons (atoi (cluster->port));
  bcopy (clhostent->h_addr, &claddress.sin_addr, clhostent->h_length);
  bzero (&(claddress.sin_zero), 8);

  ret = connect (cluster->sockethandle, (struct sockaddr *) &claddress,
		 sizeof (struct sockaddr));

  if (ret == -1 && errno != EINPROGRESS)
    {
      msg = g_string_new (strerror (errno));
      updatestatusbar (msg);
      g_string_free (msg, TRUE);
      return FALSE;
    }

  g_string_printf (msg, _("Connected to %s"), cluster->host);
  updatestatusbar (msg);
  g_string_free (msg, TRUE);

  menu_set_sensitive (gui->item_factory, "/Host/Open", FALSE);
  menu_set_sensitive (gui->item_factory, "/Host/Close", TRUE);

  cluster->rxchannel = g_io_channel_unix_new (cluster->sockethandle);
  /* some hams like to type non-ascii characters on the cluster. We could never guess what the
         locale is, so we make the channel binary */
  res = g_io_channel_set_encoding (cluster->rxchannel, NULL, &err);
  if ((res != G_IO_STATUS_NORMAL) && err)
  {
    g_string_printf (msg, _("Error on setting channel encoding: %s"), err->message);
    updatestatusbar (msg);
    g_string_free (msg, TRUE);
    g_error_free (err);
    err = NULL;
  }
  cluster->source_id = g_io_add_watch (cluster->rxchannel, G_IO_IN, rx, cluster);
  return TRUE;
}


/*
 * disconnect routine
 */

void
cldisconnect (gboolean updatemessagebar)
{
  GString *msg = g_string_new ("");
  servertype *cluster;

  cluster = g_object_get_data(G_OBJECT(gui->window), "cluster");
  g_io_channel_unref(cluster->rxchannel);
  cluster->rxchannel = NULL;
  g_source_remove(cluster->source_id);
  g_free(cluster->host);
  g_free(cluster->port);

  close(cluster->sockethandle);
  cluster->sockethandle = -1;

  if (updatemessagebar)
  {
    g_string_printf (msg, _("Connection closed"));
    updatestatusbar (msg);
    g_string_free (msg, TRUE);
  }

  menu_set_sensitive (gui->item_factory, "/Host/Open", TRUE);
  menu_set_sensitive (gui->item_factory, "/Host/Close", FALSE);
}

/*
 * a message is received here
 */

gboolean
rx (GIOChannel * channel, GIOCondition cond, gpointer data)
{
  gchar buf[1024];
  gsize numbytes;
  GString *msg = g_string_new ("");
  GIOStatus res = G_IO_STATUS_NORMAL;
  GError *err = NULL;
  gboolean ret;
  servertype *cluster = (servertype *)data;

  do
    res = g_io_channel_read_chars (cluster->rxchannel, buf, 1024, &numbytes, &err);
  while (res == G_IO_STATUS_AGAIN);

  switch (res)
    {
    case G_IO_STATUS_ERROR: /*connection refused */
      g_string_printf (msg, ("%s"), err->message);
      updatestatusbar (msg);
      g_string_free (msg, TRUE);
      g_error_free (err);
      err = NULL;
      cldisconnect (FALSE);
      return FALSE;
      break;
    case G_IO_STATUS_NORMAL:
      ret = TRUE;
      break;
    case G_IO_STATUS_EOF: /* remote end has closed connection */
      cldisconnect (TRUE);
      return FALSE;
      break;
    default:
      break;
    }

  if ((cond & G_IO_IN) && G_IO_STATUS_NORMAL)
    {
      if (numbytes == 0) /* remote end has closed connection */
	{
	  cldisconnect (TRUE);
	  ret = FALSE;
	}
      else
	maintext_add (buf, numbytes, MESSAGE_RX);
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
	  updatestatusbar (errmsg);
          g_string_free (errmsg, TRUE);
	  return;
	}
      else
        {
	maintext_add (txmsg->str, txmsg->len, MESSAGE_TX);
	}
    }
  else
    {
  g_string_printf (errmsg, _("Nothing to send, you are not connected"));
  updatestatusbar (errmsg);
  g_string_free (errmsg, TRUE);
    }
}
