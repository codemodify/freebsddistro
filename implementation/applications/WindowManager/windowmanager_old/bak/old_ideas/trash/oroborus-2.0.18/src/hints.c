/* 
 * Oroborus Window Manager
 * X11 Hints Utility Library 
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
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <X11/Xatom.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "hints.h"

Atom wm_state, wm_change_state, wm_delete_window, wm_protocols;
Atom motif_wm_hints;
Atom win_hints, win_state, win_client_list, win_layer, win_workspace,
  win_workspace_count, win_desktop_button_proxy, win_supporting_wm_check,
  gnome_panel_desktop_area;
Atom net_atoms[NET_ATOM_COUNT];
Display *dpy;

void
initHints (Display * d)
{
  dpy = d;
  initICCCMHints ();
  initMotifHints ();
  initGnomeHints ();
  initNETHints ();
}

void
initICCCMHints ()
{
#ifdef DEBUG
  printf ("entering initICCCMHints\n");
#endif

  wm_state = XInternAtom (dpy, "WM_STATE", False);
  wm_change_state = XInternAtom (dpy, "WM_CHANGE_STATE", False);
  wm_delete_window = XInternAtom (dpy, "WM_DELETE_WINDOW", False);
  wm_protocols = XInternAtom (dpy, "WM_PROTOCOLS", False);
}

void
initNETHints ()
{
  Window root;
#ifdef DEBUG
  printf ("entering initNETHints\n");
#endif

  root = XDefaultRootWindow (dpy);

  net_atoms[NET_CLIENT_LIST] = XInternAtom (dpy, "_NET_CLIENT_LIST", False);
  net_atoms[NET_CLIENT_LIST_STACKING] =
    XInternAtom (dpy, "_NET_CLIENT_LIST_STACKING", False);
  net_atoms[NET_WM_WINDOW_TYPE] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE", False);
  net_atoms[NET_WM_WINDOW_TYPE_DESKTOP] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
  net_atoms[NET_WM_WINDOW_TYPE_DOCK] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
  net_atoms[NET_WM_WINDOW_TYPE_TOOLBAR] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
  net_atoms[NET_WM_WINDOW_TYPE_MENU] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_MENU", False);
  net_atoms[NET_WM_WINDOW_TYPE_UTILITY] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_UTILITY", False);
  net_atoms[NET_WM_WINDOW_TYPE_SPLASH] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_SPLASH", False);
  net_atoms[NET_WM_WINDOW_TYPE_DIALOG] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
  net_atoms[NET_WM_WINDOW_TYPE_NORMAL] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_NORMAL", False);
  net_atoms[NET_WM_STATE] = XInternAtom (dpy, "_NET_WM_STATE", False);
  net_atoms[NET_WM_STATE_MODAL] =
    XInternAtom (dpy, "_NET_WM_STATE_MODAL", False);
  net_atoms[NET_WM_STATE_STICKY] =
    XInternAtom (dpy, "_NET_WM_STATE_STICKY", False);
  net_atoms[NET_WM_STATE_SHADED] =
    XInternAtom (dpy, "_NET_WM_STATE_SHADED", False);
  net_atoms[NET_WM_STATE_SKIP_TASKBAR] =
    XInternAtom (dpy, "_NET_WM_STATE_SKIP_TASKBAR", False);
  net_atoms[NET_WM_STATE_SKIP_PAGER] =
    XInternAtom (dpy, "_NET_WM_STATE_SKIP_PAGER", False);
  net_atoms[NET_WM_STATE_HIDDEN] =
    XInternAtom (dpy, "_NET_WM_STATE_HIDDEN", False);
  net_atoms[NET_WM_STATE_ABOVE] =
    XInternAtom (dpy, "_NET_WM_STATE_ABOVE", False);
  net_atoms[NET_WM_STATE_FULLSCREEN] =
    XInternAtom (dpy, "_NET_WM_STATE_FULLSCREEN", False);
  net_atoms[NET_WM_NAME] = XInternAtom (dpy, "_NET_WM_NAME", False);
  net_atoms[NET_WM_DESKTOP] = XInternAtom (dpy, "_NET_WM_DESKTOP", False);
  net_atoms[NET_NUMBER_OF_DESKTOPS] =
    XInternAtom (dpy, "_NET_NUMBER_OF_DESKTOPS", False);
  net_atoms[NET_CURRENT_DESKTOP] =
    XInternAtom (dpy, "_NET_CURRENT_DESKTOP", False);
  net_atoms[NET_ACTIVE_WINDOW] =
    XInternAtom (dpy, "_NET_ACTIVE_WINDOW", False);
  net_atoms[NET_SUPPORTING_WM_CHECK] =
    XInternAtom (dpy, "_NET_SUPPORTING_WM_CHECK", False);
  net_atoms[NET_WORKAREA] = XInternAtom (dpy, "_NET_WORKAREA", False);
  net_atoms[NET_WM_WORKAREA] = XInternAtom (dpy, "_NET_WM_WORKAREA", False);
  net_atoms[NET_CLOSE_WINDOW] = XInternAtom (dpy, "_NET_CLOSE_WINDOW", False);
  net_atoms[NET_WM_PID] = XInternAtom (dpy, "_NET_WM_PID", False);
  net_atoms[NET_WM_STRUT] = XInternAtom (dpy, "_NET_WM_STRUT", False);
  XChangeProperty (dpy, root, XInternAtom (dpy, "_NET_SUPPORTED", False),
		   XA_ATOM, 32, PropModeReplace, (unsigned char *) net_atoms,
		   NET_ATOM_COUNT);
}

void
initMotifHints ()
{
#ifdef DEBUG
  printf ("entering initMotifHints\n");
#endif

  motif_wm_hints = XInternAtom (dpy, "_MOTIF_WM_HINTS", False);
}

void
initGnomeHints ()
{
  Window root;

#ifdef DEBUG
  printf ("entering initGnomeHints\n");
#endif

  root = XDefaultRootWindow (dpy);
  win_hints = XInternAtom (dpy, "_WIN_HINTS", False);
  win_state = XInternAtom (dpy, "_WIN_STATE", False);
  win_client_list = XInternAtom (dpy, "_WIN_CLIENT_LIST", False);
  win_layer = XInternAtom (dpy, "_WIN_LAYER", False);
  win_workspace = XInternAtom (dpy, "_WIN_WORKSPACE", False);
  win_workspace_count = XInternAtom (dpy, "_WIN_WORKSPACE_COUNT", False);
  win_desktop_button_proxy =
    XInternAtom (dpy, "_WIN_DESKTOP_BUTTON_PROXY", False);
  win_supporting_wm_check =
    XInternAtom (dpy, "_WIN_SUPPORTING_WM_CHECK", False);
  gnome_panel_desktop_area =
    XInternAtom (dpy, "GNOME_PANEL_DESKTOP_AREA", False);
}

long
getWMState (Window w)
{
  unsigned long *data = NULL, state = WithdrawnState;

#ifdef DEBUG
  printf ("entering getWmState\n");
#endif

  data = getPropData (w, wm_state, wm_state, NULL);
  if (data)
    {
      state = *data;
      XFree (data);
    }
  return state;
}

void
setWMState (Window w, long state)
{
  CARD32 data[2];

#ifdef DEBUG
  printf ("entering setWmState\n");
#endif

  data[0] = state;
  data[1] = None;

  XChangeProperty (dpy, w, wm_state, wm_state, 32, PropModeReplace,
		   (unsigned char *) data, 2);
}

PropMwmHints *
getMotifHints (Window w)
{
#ifdef DEBUG
  printf ("entering getMotifHints\n");
#endif

  return getPropData (w, motif_wm_hints, motif_wm_hints, NULL);
}

int
getGnomeHint (Window w, Atom a, long *value)
{
  int success = False;
  long *data = NULL;

#ifdef DEBUG
  printf ("entering getGnomeHint\n");
#endif

  if (a == win_layer)
    *value = WIN_LAYER_NORMAL;
  else
    *value = 0;

  data = getPropData (w, a, XA_CARDINAL, NULL);
  if (data)
    {
      *value = *data;
      success = True;
      XFree (data);
    }
  return success;
}

void
setGnomeHint (Window w, Atom a, long value)
{
#ifdef DEBUG
  printf ("entering setGnomeHint\n");
#endif

  XChangeProperty (dpy, w, a, XA_CARDINAL, 32, PropModeReplace,
		   (unsigned char *) &value, 1);
}

void
setNetWmName (Window w, Atom a, char *value)
{
#ifdef DEBUG
  printf ("entering setNetWmName\n");
#endif

  XChangeProperty (dpy, w, a, XA_STRING, 8, PropModeReplace,
		   (unsigned char *) value, strlen (value));
}

void
setSupportingWmCheck (Window w, Window w2)
{
#ifdef DEBUG
  printf ("entering setSupportingWmCheck\n");
#endif

  XChangeProperty (dpy, w, net_atoms[NET_SUPPORTING_WM_CHECK], XA_WINDOW, 32,
		   PropModeReplace, (unsigned char *) &w2, 1);
}

void
getEWMHState (Window w, long *value)
{
  unsigned long *data;
  unsigned long items = 0;
  unsigned long i;

  data = getPropData (w, net_atoms[NET_WM_STATE], XA_ATOM, &items);
  if (data)
    {
      for (i = 0; i < items; i++)
	{
	  char *currentName = XGetAtomName (dpy, data[i]);
	  char *configuredName =
	    XGetAtomName (dpy, net_atoms[NET_WM_STATE_STICKY]);
	  if (equals (currentName, configuredName))
	    *value |= WIN_STATE_STICKY;
	  XFree (currentName);
	  XFree (configuredName);
	}
      XFree (data);
    }
}

void
getGnomeDesktopMargins (CARD32 * margins)
{
  unsigned long items_read;
  CARD32 *data = NULL;

#ifdef DEBUG
  printf ("entering getGnomeDesktopMargins\n");
#endif

  data =
    getPropData (XDefaultRootWindow (dpy), gnome_panel_desktop_area,
		 XA_CARDINAL, &items_read);
  if (data && items_read >= 4)
    {
      margins[0] = data[0];
      margins[1] = data[1];
      margins[2] = data[2];
      margins[3] = data[3];
      XFree (data);
    }
  else
    {
      margins[0] = 0;
      margins[1] = 0;
      margins[2] = 0;
      margins[3] = 0;
    }
}

int
getNetWMPid (Window w)
{
  int val;
  int *data = NULL;
  unsigned long items_read;

  data = getPropData (w, net_atoms[NET_WM_PID], XA_CARDINAL, &items_read);
  
  if (items_read >= 1)
    {
      val = *data;
      XFree(data);

      return val;
    }
  else
    return 0;
}

void
getNetWMStrut (Window win, CARD32 * margins, CARD32 * window_margins)
{
  unsigned long items_read;
  CARD32 *data = NULL;

#ifdef DEBUG
  printf ("entering getGnomeDesktopMargins\n");
#endif

  data = getPropData (win, net_atoms[NET_WM_STRUT], XA_CARDINAL, &items_read);
  if (data && items_read >= 4)
    {
      window_margins[0] = data[0];
      window_margins[1] = data[1];
      window_margins[2] = data[2];
      window_margins[3] = data[3];
      margins[0] += data[0];
      margins[1] += data[1];
      margins[2] += data[2];
      margins[3] += data[3];
      XFree (data);
    }
  else
    {
      window_margins[0] = 0;
      window_margins[1] = 0;
      window_margins[2] = 0;
      window_margins[3] = 0;
    }
}

void
delNetWMStrut (CARD32 * margins, CARD32 * data)
{
#ifdef DEBUG
  printf ("entering delNetWMStrut\n");
#endif

  if (data[0] || data[1] || data[2] || data[3])
    {
      margins[0] -= data[0];
      margins[1] -= data[1];
      margins[2] -= data[2];
      margins[3] -= data[3];
    }
}

void
setNetWorkarea (CARD32 * margins, int workspaces)
{
  struct workarea
  {
    CARD32 left, top, right, bottom;
  }
   *workarea;
  int i;

  if (workspaces <= 0)
    return;

  workarea = calloc (workspaces, sizeof (struct workarea));

  for (i = 0; i < workspaces; i++)
    {
      workarea[i].left = margins[0];
      workarea[i].top = margins[1];
      workarea[i].right =
	XDisplayWidth (dpy, XDefaultScreen (dpy)) - margins[1];
      workarea[i].bottom =
	XDisplayHeight (dpy, XDefaultScreen (dpy)) - margins[3];
    }

  XChangeProperty (dpy, XDefaultRootWindow (dpy),
		   net_atoms[NET_WORKAREA], XA_CARDINAL, 32,
		   PropModeReplace, (unsigned char *) workarea,
		   4 * workspaces);
  XChangeProperty (dpy, XDefaultRootWindow (dpy), net_atoms[NET_WM_WORKAREA],
		   XA_CARDINAL, 32, PropModeReplace,
		   (unsigned char *) workarea, 4 * workspaces);
  free (workarea);
}

int
typeDesktop (Window w)
{
  return typeEWMH (w, "_NET_WM_WINDOW_TYPE_DESKTOP");
}

int
typeDock (Window w)
{
  return typeEWMH (w, "_NET_WM_WINDOW_TYPE_DOCK");
}

int
typeAbove (Window w)
{
  return typeEWMH (w, "_NET_WM_STATE_ABOVE");
}

int
typeEWMH (Window w, char *type)
{
  unsigned long *data;
  unsigned long items = 0;
  unsigned long item = 0;
  int ret = 0;

#ifdef DEBUG
  printf ("entering typeEWMH\n");
#endif

  data = getPropData (w, net_atoms[NET_WM_WINDOW_TYPE], XA_ATOM, &items);
  while (data && items && item < items && !ret)
    {
      char *name;
      name = XGetAtomName (dpy, data[item]);
#ifdef DEBUG
      printf ("%s\n", name);
#endif
      ret = equals (name, type);
      XFree (name);
      item++;
    }
  if (data)
    XFree (data);

  return ret;
}

void *
getPropData (Window w, Atom prop, Atom type, unsigned long *items_ret)
{
  Atom type_ret;
  int format_ret;
  unsigned long after_ret;
  unsigned char *prop_data;

#ifdef DEBUG
  printf ("entering getPropData\n");
#endif
  prop_data = NULL;

  if (items_ret == NULL)
    {
      items_ret = (unsigned long *) malloc (sizeof (unsigned long));
      XGetWindowProperty (dpy, w, prop, 0, 0x7fffffff, False,
			  type, &type_ret, &format_ret, items_ret,
			  &after_ret, &prop_data);
      if (prop_data && *items_ret)
	{
	  free (items_ret);
	  return prop_data;
	}
      else
	{
	  free (items_ret);
	  return NULL;
	}
    }
  else
    {
      XGetWindowProperty (dpy, w, prop, 0, 0x7fffffff, False,
			  type, &type_ret, &format_ret, items_ret,
			  &after_ret, &prop_data);
      if (prop_data && *items_ret)
	return prop_data;
      else
	return NULL;
    }
}

void
setFocusHint (Window w)
{
#ifdef DEBUG
  printf ("entering setFocusHint\n");
#endif

  XChangeProperty (dpy, XDefaultRootWindow (dpy),
		   net_atoms[NET_ACTIVE_WINDOW], XA_WINDOW, 32,
		   PropModeReplace, (unsigned char *) &w, 1);
}

int
equals (char *left, char *right)
{
  if (left && right && (strcmp (left, right) == 0))
    return 1;
  else
    return 0;
}

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: x11_hints handling for oroborus-wm                                           *
 ******************************************************************************************/
