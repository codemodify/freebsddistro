/* 
 * Oroborus Window Manager
 *
 * Copyright (C) 2001 Ken Lynch
 * Copyright (C) 2002-2005 Stefan Pfetzing
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

#include <X11/Xlib.h>
#include <X11/Xmd.h>
#include <X11/cursorfont.h>
#include <X11/extensions/shape.h>
#include <X11/Xlocale.h>
#include <X11/xpm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#include "client.h"
#include "events.h"
#include "frame.h"
#include "getopt.h"
#include "hints.h"
#include "i18n.h"
#include "keyboard.h"
#include "misc.h"
#include "oroborus.h"
#include "parserc.h"
#include "pixmap.h"
#include "session.h"
#include "settings.h"
#include "workspaces.h"

#include "globals.h"

static char *sm_client_id = NULL;

int
handleXError (Display * dpy, XErrorEvent * err)
{
  switch (err->error_code)
    {
    case BadAccess:
      if (err->resourceid == root)
	{
	  fprintf (stderr, "%s: Another window manager is running\n",
		   progname);
	  exit (1);
	}
    }
  return 0;
}

void
cleanUp ()
{
  int i;

#ifdef DEBUG
  printf ("entering cleanUp\n");
#endif

  clientUnframeAll ();
  unloadSettings ();
  XFreeCursor (dpy, root_cursor);
  XFreeCursor (dpy, move_cursor);
  for (i = 0; i < 4; i++)
    XFreeCursor (dpy, resize_cursor[i]);
  XSetInputFocus (dpy, root, RevertToPointerRoot, CurrentTime);
  XCloseDisplay (dpy);
}

void
handleSignal (int sig)
{
  int *status = NULL;
#ifdef DEBUG
  printf ("entering handleSignal\n");
#endif

  switch (sig)
    {
    case SIGINT:
    case SIGTERM:
      quit = True;
      break;
    case SIGSEGV:
      fprintf (stderr, "%s: Segmentation fault\n", progname);
      cleanUp ();
      abort ();
      break;
    case SIGHUP:
      reload = True;
      break;
    case SIGCHLD:
      printf ("waiting for a child\n");
      waitpid (-1, status, WNOHANG);
      break;
    case SIGALRM:
      raise_current_window = True;
      break;
    }
}

void
usage (int status)
{
  printf ("%s - lightweight themeable window manager for X.\n", progname);
  printf ("Usage: %s [OPTION]... \n", progname);
  printf ("Options:\n");
  printf ("  -r, --file=FILE            use an alternative rcfile\n");
  printf ("                             instead of $HOME/.oroborusrc\n");
  printf ("  -v, --verbose              print more information\n");
  printf ("  -h, --help                 display this help and exit\n");
  printf ("  -V, --version              output version information and exit\n");
  printf ("  -d, --display=DISPLAY      use an alternative display instead of,\n");
  printf ("                             $DISPLAY\n");
  printf ("  --sm-client-id             use the specified session management id.\n");
  exit (status);
}

int
decode_switches (int argc, char **argv)
{
  int c;

  while ((c = getopt_long (argc, argv,  "v"	/* verbose */
			   		"h"	/* help */
			   		"V"	/* version */
			   		"r:"	/* file */
			   		"d:",	/* display */
			   long_options, (int *) 0)) != EOF)
    {
      switch (c)
	{
	case 'd':		/* --display */
	  display = optarg;
	  break;
	case 'v':		/* --verbose */
	  want_verbose = 1;
	  break;
	case 'r':		/* --file */
	  rcfile = optarg;
	  break;
	case 130:		/* --file */
	  sm_client_id = optarg;
	  break;
	case 'V':
	  printf ("%s %s\n", PACKAGE, VERSION);
	  exit (0);
	case 'h':
	  usage (0);
	default:
	  usage (EXIT_FAILURE);
	}
    }
  return optind;
}

/**
 * Initialize all variables.
 * This function initializes all variables, reads the config and so on.
 * It also displays a short informational (License, Author and so on) Line.
 */
void
initialize (int argc, char **argv)
{
  struct sigaction act;
  char *locale = "";
  int dummy;

#ifdef DEBUG
  printf ("entering initialize\n");
#endif

  progname = argv[0];
  if (strrchr (progname, '/'))
    progname = strrchr (progname, '/') + sizeof (char);

  decode_switches (argc, argv);

  if (want_verbose)
    {
      printf ("--- %s %s ---\n", PACKAGE, VERSION );
      printf ("Copyright (c) %d Stefan Pfetzing\n", YEAR);
      printf ("Distributed under the terms and conditions of the GPL\n");
    }

  locale = setlocale (LC_ALL, "");

  stat_argv = argv;

  if (!locale || !strcmp (locale, "C") || !strcmp (locale, "POSIX") ||
      !XSupportsLocale ())
    use_fontset = False;
  else
    use_fontset = True;

  dpy = XOpenDisplay (display);
  if (!dpy)
    {
      fprintf (stderr, "%s: failed to open display %s!\n", progname,
	       display ? display : getenv ("DISPLAY"));
      exit (1);
    }

  session_init (progname, sm_client_id);
  act.sa_handler = handleSignal;
  act.sa_flags = 0;
  if (sigaction (SIGINT, &act, NULL))
    fprintf (stderr, "%s: Warning, could not install SIGINT Handler!\n",
	     progname);
  if (sigaction (SIGTERM, &act, NULL))
    fprintf (stderr, "%s: Warning, could not install SIGTERM Handler!\n",
	     progname);
  if (sigaction (SIGHUP, &act, NULL))
    fprintf (stderr, "%s: Warning, could not install SIGHUP Handler!\n",
	     progname);
  if (sigaction (SIGSEGV, &act, NULL))
    fprintf (stderr, "%s: Warning, could not install SIGSEGV Handler!\n",
	     progname);
  if (sigaction (SIGCHLD, &act, NULL))
    fprintf (stderr, "%s: Warning, could not install SIGCHLD Handler!\n",
	     progname);
  if (sigaction (SIGALRM, &act, NULL))
    fprintf (stderr, "%s: Warning, could not install SIGALRM Handler!\n",
	     progname);

  root = XDefaultRootWindow (dpy);
  screen = XDefaultScreen (dpy);
  depth = DefaultDepth (dpy, screen);
  cmap = DefaultColormap (dpy, screen);

  XSetErrorHandler (handleXError);
  XSelectInput (dpy, root,
		SubstructureNotifyMask | SubstructureRedirectMask |
		ButtonPressMask | ButtonReleaseMask | FocusChangeMask |
		PropertyChangeMask);

  shape = XShapeQueryExtension (dpy, &shape_event, &dummy);

  initHints (dpy);
  gnome_win = XCreateSimpleWindow (dpy, root, -1000, -1000, 5, 5, 0, 0, 0);
  setGnomeHint (root, win_supporting_wm_check, gnome_win);
  setGnomeHint (gnome_win, win_supporting_wm_check, gnome_win);
  setSupportingWmCheck (root, gnome_win);
  setSupportingWmCheck (gnome_win, gnome_win);
  setNetWmName (gnome_win, net_atoms[NET_WM_NAME], "oroborus");
  setGnomeHint (root, win_desktop_button_proxy, gnome_win);
  setGnomeHint (gnome_win, win_desktop_button_proxy, gnome_win);
  getGnomeHint (root, win_workspace, &workspace);
  getGnomeDesktopMargins (margins);

  root_cursor = XCreateFontCursor (dpy, XC_left_ptr);
  move_cursor = XCreateFontCursor (dpy, XC_fleur);
  resize_cursor[CORNER_TOP_LEFT] =
    XCreateFontCursor (dpy, XC_top_left_corner);
  resize_cursor[CORNER_TOP_RIGHT] =
    XCreateFontCursor (dpy, XC_top_right_corner);
  resize_cursor[CORNER_BOTTOM_LEFT] =
    XCreateFontCursor (dpy, XC_bottom_left_corner);
  resize_cursor[CORNER_BOTTOM_RIGHT] =
    XCreateFontCursor (dpy, XC_bottom_right_corner);
  XDefineCursor (dpy, root, root_cursor);

  init_keyboard (dpy);

  loadSettings ();

  setNetWorkarea (margins, workspace_count);

  setGnomeHint (root, win_workspace, workspace);
  setGnomeHint (root, net_atoms[NET_CURRENT_DESKTOP], workspace);

  clientFrameAll ();

  XMapWindow (dpy, gnome_win);
  XSetInputFocus (dpy, gnome_win, RevertToNone, CurrentTime);
}

/** Main Oroborus Function.
 *  The C compiler typically starts here, and all of the work is done elsewhere.
 */
int
main (int argc, char **argv)
{
  initialize (argc, argv);
  eventLoop ();
  cleanUp ();
  return 0;
}

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: main file of oroborus-wm                                                     *
 ******************************************************************************************/
