/* 
 * Oroborus Window Manager
 *
 * Copyright (C) 2002-2005 Stefan Pfetzing
 *
 * Based on stuff from Felix Bellaby <felix@pooh.u-net.com>
 *
 * Session management support for a server to support
 * gnome-session start up, close down and re-spawning.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_X11_SM_SMLIB_H
#include <X11/SM/SMlib.h>
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <X11/Xpoll.h>
#include <X11/xpm.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pwd.h>

#include "client.h"
#include "frame.h"
#include "getopt.h"
#include "i18n.h"
#include "keyboard.h"
#include "oroborus.h"
#include "pixmap.h"
#include "session.h"

#include "globals.h"

#ifdef HAVE_X11_SM_SMLIB_H
#define GnomePriority "_GSM_Priority"

/* Useful if you want to change the SM properties outside this file. */
SmcConn sm_conn = NULL;

/* static stuff: */

static IceConn ice_conn = NULL;
int ice_fd = -1;
static int set_props = 0;
static char *sm_client_id = "";
static char *command = NULL;

static void
callback_die (SmcConn smc_conn, SmPointer client_data)
{
#ifdef DEBUG
  printf ("exiting\n");
#endif
  SmcCloseConnection (smc_conn, 0, NULL);

  /* this graceful closedown stuff needs to be kept in sync: */
  cleanUp ();

  exit (0);
}

static void
callback_save_yourself (SmcConn smc_conn, SmPointer client_data,
			int save_style, Bool shutdown, int interact_style,
			Bool fast)
{
  if (set_props)
    {
      struct
      {
	SmPropValue program[1];
	SmPropValue user[1];
	SmPropValue hint[1];
	SmPropValue priority[1];
	SmPropValue restart[3];
      }
      vals;
      SmProp prop[] = {
	{SmProgram, SmLISTofARRAY8, 1, vals.program},
	{SmUserID, SmLISTofARRAY8, 1, vals.user},
	{SmRestartStyleHint, SmCARD8, 1, vals.hint},
	{GnomePriority, SmCARD8, 1, vals.priority},
	{SmCloneCommand, SmLISTofARRAY8, 0, vals.restart},
	{SmRestartCommand, SmLISTofARRAY8, 0, vals.restart}
      };
      SmProp *props[] = {
	&prop[0],
	&prop[1],
	&prop[2],
	&prop[3],
	&prop[4],
	&prop[5]
      };
      char priority = 5;
      char restart_style = SmRestartImmediately;
      struct passwd *pw = getpwuid (getuid ());
      int n = 0, i;

      vals.program->value = command;
      vals.program->length = strlen (vals.program->value);

      vals.user->value = pw ? pw->pw_name : "";
      vals.user->length = strlen (vals.user->value);

      vals.hint->value = &restart_style;
      vals.hint->length = 1;

      vals.priority->value = &priority;
      vals.priority->length = 1;

      /* This stuff MUST be kept in sync with esd.c: */
      vals.restart[n++].value = command;
      if (rcfile)
	{
	  vals.restart[n++].value = "-r";
	  vals.restart[n++].value = rcfile;
	}
      /* end of stuff to keep in sync */

      prop[4].num_vals = n;

      vals.restart[n++].value = "--sm-client-id";
      vals.restart[n++].value = sm_client_id;

      prop[5].num_vals = n;

      for (i = 0; i < n; i++)
	vals.restart[i].length = strlen (vals.restart[i].value);

      SmcSetProperties (smc_conn, sizeof (props) / sizeof (SmProp *), props);

      set_props = 0;

    }
  /* Nothing to save */
  SmcSaveYourselfDone (smc_conn, 1);
}

static void
callback_shutdown_cancelled (SmcConn smc_conn, SmPointer client_data)
{
  /* We are not really interested in this message. */
}

static void
callback_save_complete (SmcConn smc_conn, SmPointer client_data)
{
  /* We are not really interested in this message. */
}

static void
ice_io_error_handler (IceConn connection)
{
  /* The less we do here the better - the default handler does an
     exit(1) instead of closing the losing connection. */
}

void
process_ice_msgs (void)
{
  IceProcessMessagesStatus status;

  status = IceProcessMessages (ice_conn, NULL, NULL);

  if (status == IceProcessMessagesIOError)
    {
      SmcCloseConnection (sm_conn, 0, NULL);
      ice_fd = -1;
      ice_conn = NULL;
      sm_conn = NULL;
    }
}

void
session_init (char *argv0, char *id)
{
  SmcCallbacks callbacks;

  callbacks.save_yourself.callback = callback_save_yourself;
  callbacks.die.callback = callback_die;
  callbacks.save_complete.callback = callback_save_complete;
  callbacks.shutdown_cancelled.callback = callback_shutdown_cancelled;

  callbacks.save_yourself.client_data =
    callbacks.die.client_data =
    callbacks.save_complete.client_data =
    callbacks.shutdown_cancelled.client_data = (SmPointer) NULL;

  command = argv0;
  sm_client_id = id;

  IceSetIOErrorHandler (ice_io_error_handler);

  if (getenv ("SESSION_MANAGER"))
    {
      char error_string_ret[4096] = "";
      char *client_id = NULL;

      if (sm_client_id)
	client_id = strdup (sm_client_id);

      sm_conn = SmcOpenConnection (NULL, NULL, SmProtoMajor, SmProtoMinor,
				   SmcSaveYourselfProcMask | SmcDieProcMask |
				   SmcSaveCompleteProcMask |
				   SmcShutdownCancelledProcMask,
				   &callbacks, client_id, &sm_client_id,
				   4096, error_string_ret);

      if (!client_id)
	set_props = 1;
      else
	{
	  set_props = strcmp (client_id, sm_client_id);
	  free (client_id);
	}

      if (error_string_ret[0])
	fprintf (stderr, "While connecting to session manager:\n%s.",
		 error_string_ret);
    }
  if (sm_conn)
    {
      ice_conn = SmcGetIceConnection (sm_conn);
      ice_fd = IceConnectionNumber (ice_conn);

      /* Not really needed... */
      fcntl (ice_fd, F_SETFD, fcntl (ice_fd, F_GETFD, 0) | FD_CLOEXEC);
#ifdef DEBUG
      printf ("Connection established!\n");
#endif
    }
}

void
add_ice_fd (fd_set * rd_fds, int *max_fd)
{
  if (ice_fd != -1)
    {
      FD_SET (ice_fd, rd_fds);
      if (ice_fd > *max_fd)
	*max_fd = ice_fd;
    }
}

#else /* HAVE_X11_SM_SMLIB_H */

void
session_init (char *argv0, char *id)
{
  /* Nothing to do */
}

void
add_ice_fd (fd_set *rd_fds, int *max_fd)
{
  /* Nothing to do */
}

#endif /* HAVE_X11_SM_SMLIB_H */

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: session handling for oroborus-wm                                             *
 ******************************************************************************************/
