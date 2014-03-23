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
#include <X11/extensions/shape.h>
#include <X11/Xatom.h>
#include <X11/xpm.h>

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/select.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "client.h"
#include "events.h"
#include "frame.h"
#include "getopt.h"
#include "hints.h"
#include "i18n.h"
#include "keyboard.h"
#include "misc.h"
#include "oroborus.h"
#include "pixmap.h"
#include "session.h"
#include "workspaces.h"

#include "globals.h"

Bool raise_current_window = False;

void
handleKeyPress (XKeyEvent * ev)
{
  client_t *c;
  int state, key;
  XEvent e;
  XWindowChanges wc;

#ifdef DEBUG
  printf ("entering handleKeyEvent\n");
#endif

  c = clientGetFocus ();
  state = ev->state & (Mod1Mask | Mod4Mask | ControlMask | ShiftMask);
  for (key = 0; key < KEY_ENUM_MAX; key++)
    if (keys[key].keycode == ev->keycode && keys[key].modifier == state)
      break;

  if (c)
    {
      switch (key)
	{
	case KEY_QUIT:
	  quit = True;
	  break;
	case KEY_MOVE_UP:
	case KEY_MOVE_DOWN:
	case KEY_MOVE_LEFT:
	case KEY_MOVE_RIGHT:
	  clientMove (c, (XEvent *) ev);
	  clientWarpMouse (c);
	  break;
	case KEY_TOP_LEFT:
	  wc.x = frameLeft (c) + margins[MARGIN_LEFT];
	  wc.y = frameTop (c) + margins[MARGIN_TOP];
	  clientConfigure (c, &wc, CWX | CWY);
	  clientWarpMouse (c);
	  break;
	case KEY_TOP_RIGHT:
	  wc.x =
	    XDisplayWidth (dpy,
			   screen) - c->width - frameRight (c) -
	    margins[MARGIN_RIGHT];
	  wc.y = frameTop (c) + margins[MARGIN_TOP];
	  clientConfigure (c, &wc, CWX | CWY);
	  clientWarpMouse (c);
	  break;
	case KEY_BOTTOM_LEFT:
	  wc.x = frameLeft (c) + margins[MARGIN_LEFT];
	  wc.y =
	    XDisplayHeight (dpy,
			    screen) - c->height - frameBottom (c) -
	    margins[MARGIN_BOTTOM];
	  clientConfigure (c, &wc, CWX | CWY);
	  clientWarpMouse (c);
	  break;
	case KEY_BOTTOM_RIGHT:
	  wc.x =
	    XDisplayWidth (dpy,
			   screen) - c->width - frameRight (c) -
	    margins[MARGIN_RIGHT];
	  wc.y =
	    XDisplayHeight (dpy,
			    screen) - c->height - frameBottom (c) -
	    margins[MARGIN_BOTTOM];
	  clientConfigure (c, &wc, CWX | CWY);
	  clientWarpMouse (c);
	  break;
	case KEY_RESIZE_UP:
	case KEY_RESIZE_DOWN:
	case KEY_RESIZE_LEFT:
	case KEY_RESIZE_RIGHT:
	  clientResize (c, (XEvent *) ev);
	  clientWarpMouse (c);
	  break;
	case KEY_CYCLE_WINDOWS:
	  clientCycle (c);
	  break;
	case KEY_CLOSE_WINDOW:
	  clientClose (c);
	  break;
	case KEY_HIDE_WINDOW:
	  clientHide (c, True);
	  break;
	case KEY_TOGGLE_FULLSCREEN_WINDOW:
	  clientToggleFullscreen (c);
	  break;
	case KEY_MAXIMIZE_WINDOW:
	  clientToggleMaximized (c, WIN_STATE_MAXIMIZED);
	  clientWarpMouse (c);
	  break;
	case KEY_MAXIMIZE_VERT:
	  clientToggleMaximized (c, WIN_STATE_MAXIMIZED_VERT);
	  clientWarpMouse (c);
	  break;
	case KEY_MAXIMIZE_HORIZ:
	  clientToggleMaximized (c, WIN_STATE_MAXIMIZED_HORIZ);
	  clientWarpMouse (c);
	  break;
	case KEY_SHADE_WINDOW:
	  clientToggleShaded (c);
	  break;
	case KEY_RAISE_WINDOW_LAYER:
	  clientSetLayer (c, c->win_layer + 1);
	  break;
	case KEY_LOWER_WINDOW_LAYER:
	  clientSetLayer (c, c->win_layer - 1);
	  break;
	case KEY_NEXT_WORKSPACE:
	  workspaceSwitch (workspace + 1, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_PREV_WORKSPACE:
	  workspaceSwitch (workspace - 1, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_ADD_WORKSPACE:
	  workspaceSetCount (workspace_count + 1);
	  setNetWorkarea (margins, workspace_count);
	  break;
	case KEY_DEL_WORKSPACE:
	  workspaceSetCount (workspace_count - 1);
	  setNetWorkarea (margins, workspace_count);
	  break;
	case KEY_STICK_WINDOW:
	  clientToggleSticky (c);
	  break;
	case KEY_WORKSPACE_1:
	  workspaceSwitch (0, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_2:
	  workspaceSwitch (1, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_3:
	  workspaceSwitch (2, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_4:
	  workspaceSwitch (3, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_5:
	  workspaceSwitch (4, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_6:
	  workspaceSwitch (5, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_7:
	  workspaceSwitch (6, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_8:
	  workspaceSwitch (7, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_9:
	  workspaceSwitch (8, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_MOVE_NEXT_WORKSPACE:
	  workspaceSwitch (workspace + 1, c);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_MOVE_PREV_WORKSPACE:
	  workspaceSwitch (workspace - 1, c);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_MOVE_WORKSPACE_1:
	  workspaceSwitch (0, c);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_MOVE_WORKSPACE_2:
	  workspaceSwitch (1, c);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_MOVE_WORKSPACE_3:
	  workspaceSwitch (2, c);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_MOVE_WORKSPACE_4:
	  workspaceSwitch (3, c);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_MOVE_WORKSPACE_5:
	  workspaceSwitch (4, c);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_MOVE_WORKSPACE_6:
	  workspaceSwitch (5, c);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_MOVE_WORKSPACE_7:
	  workspaceSwitch (6, c);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_MOVE_WORKSPACE_8:
	  workspaceSwitch (7, c);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_MOVE_WORKSPACE_9:
	  workspaceSwitch (8, c);
	  clientWarpMouse (clientGetFocus ());
	  break;
	}
    }
  else
    {
      switch (key)
	{
	case KEY_QUIT:
	  quit = True;
	  break;
	case KEY_CYCLE_WINDOWS:
	  if (clients)
	    clientCycle (clients->prev);
	  break;
	case KEY_NEXT_WORKSPACE:
	  workspaceSwitch (workspace + 1, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_PREV_WORKSPACE:
	  workspaceSwitch (workspace - 1, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_ADD_WORKSPACE:
	  workspaceSetCount (workspace_count + 1);
	  setNetWorkarea (margins, workspace_count);
	  break;
	case KEY_DEL_WORKSPACE:
	  workspaceSetCount (workspace_count - 1);
	  setNetWorkarea (margins, workspace_count);
	  break;
	case KEY_WORKSPACE_1:
	  workspaceSwitch (0, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_2:
	  workspaceSwitch (1, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_3:
	  workspaceSwitch (2, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_4:
	  workspaceSwitch (3, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_5:
	  workspaceSwitch (4, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_6:
	  workspaceSwitch (5, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_7:
	  workspaceSwitch (6, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_8:
	  workspaceSwitch (7, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	case KEY_WORKSPACE_9:
	  workspaceSwitch (8, NULL);
	  clientWarpMouse (clientGetFocus ());
	  break;
	}
    }

  while (XCheckTypedEvent (dpy, EnterNotify, &e));
}

void
handleButtonPress (XButtonEvent * ev)
{
  client_t *c;
  Window win;
  int state, replay = False;
  static Time last_button_time;

#ifdef DEBUG
  printf ("entering handleButtonPress\n");
#endif

  while (XCheckTypedEvent (dpy, ButtonPress, (XEvent *) ev));

  state = ev->state & (Mod1Mask | Mod4Mask | ShiftMask | ControlMask);

  c = clientGetFromWindow (ev->window, FRAME);
  if (c)
    {
      state = ev->state & (Mod1Mask | Mod4Mask | ShiftMask | ControlMask);
      win = getMouseWindow (dpy, c->frame);

      clientSetFocus (c, True);

      if (win == c->buttons[HIDE_BUTTON] ||
	  win == c->buttons[CLOSE_BUTTON] ||
	  win == c->buttons[MAXIMIZE_BUTTON] ||
	  win == c->buttons[SHADE_BUTTON])
	{
	  clientRaise (c);
	  clientButtonPress (c, win, ev);
	}
      else if (((win == c->title || win == c->sides[SIDE_BOTTOM] ||
		 win == c->sides[SIDE_LEFT] || win == c->sides[SIDE_RIGHT]) &&
		ev->button == Button1 && state == 0) || (ev->button == Button1
							 && state ==
							 keys[KEY_WINDOW_OPS].
							 modifier))
	{
	  clientRaise (c);
	  if (ev->time - last_button_time <= 250 && last_button_time != 0)
	    {
	      switch (double_click_action)
		{
		case ACTION_MAXIMIZE:
		  clientToggleMaximized (c, WIN_STATE_MAXIMIZED);
		  break;
		case ACTION_SHADE:
		  clientToggleShaded (c);
		  break;
		case ACTION_HIDE:
		  clientHide (c, True);
		  break;
		}
	      last_button_time = 0;
	    }
	  else
	    {
	      clientMove (c, (XEvent *) ev);
	      last_button_time = ev->time;
	    }
	}
      else if ((win == c->corners[CORNER_TOP_LEFT] ||
		win == c->corners[CORNER_TOP_RIGHT] ||
		win == c->corners[CORNER_BOTTOM_LEFT] ||
		win == c->corners[CORNER_BOTTOM_RIGHT]) &&
	       ev->button == Button1 && state == 0)
	{
	  clientRaise (c);
	  clientResize (c, (XEvent *) ev);
	}
      else if (win != c->window && ev->button == Button2 && state == 0
	       && !typeDesktop (c->window))
	{
	  clientRaise (c);
	  clientToggleSticky (c);
	}
      else if (ev->button == Button2
	       && state == keys[KEY_WINDOW_OPS].modifier
	       && !typeDesktop (c->window))
	{
	  clientRaise (c);
	  clientResize (c, (XEvent *) ev);
	}
      else if (((win != c->window && ev->button == Button3 && state == 0) ||
		(ev->button == Button3
		 && state == keys[KEY_WINDOW_OPS].modifier))
	       && !typeDesktop (c->window))
	clientLower (c);
      else if (win != c->window && ev->button == Button1 && state == Mod1Mask)
	clientSetLayer (c, c->win_layer + 1);
      else if (win != c->window && ev->button == Button3 && state == Mod1Mask)
	clientSetLayer (c, c->win_layer - 1);
      else
	{
	  if (raise_on_click)
	    clientRaise (c);
	  if (win == c->window)
	    replay = True;
	}

      if (replay)
	XAllowEvents (dpy, ReplayPointer, CurrentTime);
      else
	XAllowEvents (dpy, SyncPointer, CurrentTime);

      switch_timer(False);
    }
  else
    {
      XUngrabPointer (dpy, CurrentTime);
      XSendEvent (dpy, gnome_win, False, SubstructureNotifyMask,
		  (XEvent *) ev);
    }
}

void
handleButtonRelease (XButtonEvent * ev)
{
#ifdef DEBUG
  printf ("entering handleButtonRelease\n");
#endif

  XSendEvent (dpy, gnome_win, False, SubstructureNotifyMask, (XEvent *) ev);
}

void
handleDestroyNotify (XDestroyWindowEvent * ev)
{
  client_t *c;

#ifdef DEBUG
  printf ("entering handleDestroyNotify\n");
#endif

  c = clientGetFromWindow (ev->window, WINDOW);
  if (c)
    {
      clientUnframe (c, False);
      if (clients)
	{
	  c = clientGetTopMostShaded (WIN_LAYER_NORMAL, True);
	  if (!c)
	    c = clientGetTopMost (WIN_LAYER_NORMAL);

	  clientSetFocus (c, True);
	}
      else
	clientSetFocus (NULL, True);
    }
}

void
handleUnmapNotify (XUnmapEvent * ev)
{
  client_t *c;

#ifdef DEBUG
  printf ("entering handleUnmapNotify\n");
#endif

  c = clientGetFromWindow (ev->window, WINDOW);
  if (c)
    {
      if (c->ignore_unmap)
	c->ignore_unmap--;
      else
	{
	  clientUnframe (c, False);
	  if (clients)
	    {
	      c = clientGetTopMostShaded (WIN_LAYER_NORMAL, True);
	      if (!c)
		c = clientGetTopMost (WIN_LAYER_NORMAL);
	      clientSetFocus (c, True);
	    }
	  else
	    clientSetFocus (NULL, True);
	}
    }
}

void
handleMapRequest (XMapRequestEvent * ev)
{
  client_t *c;

#ifdef DEBUG
  printf ("entering handleMapRequest\n");
#endif

  c = clientGetFromWindow (ev->window, WINDOW);
  if (c)
    clientShow (c, True);
  else
    clientFrame (ev->window);
}

void
handleConfigureRequest (XConfigureRequestEvent * ev)
{
  client_t *c;
  XWindowChanges wc;

#ifdef DEBUG
  printf ("entering handleConfigureRequest\n");
#endif

  wc.x = ev->x;
  wc.y = ev->y;
  wc.width = ev->width;
  wc.height = ev->height;
  wc.sibling = ev->above;
  wc.stack_mode = ev->detail;
  wc.border_width = ev->border_width;

  c = clientGetFromWindow (ev->window, WINDOW);

  if (c)
    {
      if (! (c->above || c->was_above))
	{
	  wc.x += frameLeft(c);
	  wc.y += frameTop(c);
	}
      clientConfigure (c, &wc, ev->value_mask);
    }
  else
    XConfigureWindow (dpy, ev->window, ev->value_mask, &wc);
}

void
handleEnterNotify (XCrossingEvent * ev)
{
  client_t *c;

#ifdef DEBUG
  printf ("entering handleEnterNotify\n");
#endif

  while (XCheckTypedEvent (dpy, EnterNotify, (XEvent *) ev));

  c = clientGetFromWindow (ev->window, FRAME);
  if (c && !click_to_focus)
    clientSetFocus (c, True);

}

void
handleFocusIn (XFocusChangeEvent * ev)
{
  client_t *c;

#ifdef DEBUG
  printf ("entering handleFocusIn\n");
#endif

  c = clientGetFromWindow (ev->window, WINDOW);
  if (c)
    frameDraw (c);
  else if (clients)
    clientSetFocus (clientGetNext (clients->prev, 0), True);
  else
    clientSetFocus (NULL, True);

  if (raise_on_focus)
    switch_timer(True);
}

void
handleFocusOut (XFocusChangeEvent * ev)
{
  client_t *c;

#ifdef DEBUG
  printf ("entering handleFocusOut\n");
#endif

  c = clientGetFromWindow (ev->window, WINDOW);
  if (c)
    frameDraw (c);
}

void
handlePropertyNotify (XPropertyEvent * ev)
{
  client_t *c;
  long dummy;

#ifdef DEBUG
  printf ("entering handlePropertyNotify\n");
#endif

  c = clientGetFromWindow (ev->window, WINDOW);
  if (c)
    {
      if (ev->atom == XA_WM_NORMAL_HINTS)
	XGetWMNormalHints (dpy, c->window, c->size, &dummy);
      else if (ev->atom == XA_WM_NAME)
	{
	  if (c->name)
	    free (c->name);
	  getWindowName (dpy, c->window, &c->name);
	  frameDraw (c);
	}
      else if (ev->atom == win_hints)
	getGnomeHint (c->window, win_hints, &c->win_hints);
      else if (ev->atom == net_atoms[NET_WM_STRUT])
	{
	  delNetWMStrut (margins, c->margins);
	  getNetWMStrut (c->window, margins, c->margins);
	}
      else if (ev->atom == win_layer)
	{
	  getGnomeHint (c->window, win_layer, &dummy);
	  clientSetLayer (c, dummy);
	}
      else if (ev->atom == win_workspace)
	{
	  getGnomeHint (c->window, win_workspace, &dummy);
	  clientSetWorkspace (c, dummy);
	}
    }
  else
    {
      if (ev->atom == win_workspace_count)
	{
	  getGnomeHint (root, win_workspace_count, &dummy);
	  workspaceSetCount (dummy);
	  setNetWorkarea (margins, workspace_count);
	}
      else if (ev->atom == gnome_panel_desktop_area)
	{
	  getGnomeDesktopMargins (margins);
	  setNetWorkarea (margins, workspace_count);
	}
    }
}

void
handleClientMessage (XClientMessageEvent * ev)
{
  client_t *c;

#ifdef DEBUG
  printf ("entering handleClientMessage\n");
#endif

  c = clientGetFromWindow (ev->window, WINDOW);
  if (c)
    {
      if (ev->message_type == wm_change_state && ev->format == 32 &&
	  ev->data.l[0] == IconicState)
	clientHide (c, True);
      else if (ev->message_type == win_layer && ev->format == 32)
	{
	  setGnomeHint (c->window, ev->data.l[0], win_hints);
	  clientSetLayer (c, ev->data.l[0]);
	  if (ev->data.l[0] == 6)
	    clientToggleAbove (c);
	  clientRaise (c);
	}
      else if (ev->message_type == win_state && ev->format == 32)
	clientSetState (c, ev->data.l[0], ev->data.l[1]);
      else
	if ((ev->message_type == win_workspace
	     || ev->message_type == net_atoms[NET_WM_DESKTOP])
	    && ev->format == 32)
	clientSetWorkspace (c, ev->data.l[0]);
      else if (ev->message_type == net_atoms[NET_ACTIVE_WINDOW]
	       && ev->format == 32)
	{
	  if (getWMState (c->window) == IconicState)
	    clientShow (c, False);
	  clientSetFocus (c, True);
	  clientRaise (c);
	  setWMState (c->window, NormalState);
	}
      else if (ev->message_type == net_atoms[NET_CLOSE_WINDOW]
	       && ev->format == 32)
	clientClose (c);
      else if (ev->message_type == net_atoms[NET_WM_STATE]
	       && ev->format == 32)
	{
	  switch (ev->data.l[0])
	    {
	    case NET_WM_STATE_ADD:
	    case NET_WM_STATE_REMOVE:
	    case NET_WM_STATE_TOGGLE:
	      if (ev->data.l[1] == net_atoms[NET_WM_STATE_ABOVE]
		  || ev->data.l[2] == net_atoms[NET_WM_STATE_ABOVE])
		clientToggleAbove (c);
	      if (ev->data.l[1] == net_atoms[NET_WM_STATE_FULLSCREEN]
		  || ev->data.l[2] == net_atoms[NET_WM_STATE_FULLSCREEN])
		clientToggleFullscreen(c);
	      break;
	    }
	}
    }
  else
    {
      if ((ev->message_type == win_workspace
	   || ev->message_type == net_atoms[NET_WM_DESKTOP]
	   || ev->message_type == net_atoms[NET_CURRENT_DESKTOP])
	  && ev->format == 32)
	workspaceSwitch (ev->data.l[0], NULL);
      else
	if ((ev->message_type == win_workspace_count
	     || ev->message_type == net_atoms[NET_NUMBER_OF_DESKTOPS])
	    && ev->format == 32)
	{
	  workspaceSetCount (ev->data.l[0]);
	  setNetWorkarea (margins, workspace_count);
	}
    }
}

void
handleShape (XShapeEvent * ev)
{
  client_t *c;

#ifdef DEBUG
  printf ("entering handleShape\n");
#endif

  c = clientGetFromWindow (ev->window, WINDOW);
  if (c)
    frameDraw (c);
}

void
handleEvent (XEvent * ev)
{
#ifdef DEBUG
  printf ("entering handleEvent\n");
#endif

  switch (ev->type)
    {
    case KeyPress:
#ifdef DEBUG
      printf ("KeyPress\n");
#endif
      handleKeyPress ((XKeyEvent *) ev);
      break;
    case ButtonPress:
#ifdef DEBUG
      printf ("ButtonPress\n");
#endif
      handleButtonPress ((XButtonEvent *) ev);
      break;
    case ButtonRelease:
#ifdef DEBUG
      printf ("ButtonRelease\n");
#endif
      handleButtonRelease ((XButtonEvent *) ev);
      break;
    case DestroyNotify:
#ifdef DEBUG
      printf ("DestroyNotify\n");
#endif
      handleDestroyNotify ((XDestroyWindowEvent *) ev);
      break;
    case UnmapNotify:
#ifdef DEBUG
      printf ("UnmapNotify\n");
#endif
      handleUnmapNotify ((XUnmapEvent *) ev);
      break;
    case MapRequest:
#ifdef DEBUG
      printf ("MapRequest\n");
#endif
      handleMapRequest ((XMapRequestEvent *) ev);
      break;
    case ConfigureRequest:
#ifdef DEBUG
      printf ("ConfigureRequest\n");
#endif
      handleConfigureRequest ((XConfigureRequestEvent *) ev);
      break;
    case EnterNotify:
#ifdef DEBUG
      printf ("EnterNotify\n");
#endif
      handleEnterNotify ((XCrossingEvent *) ev);
      break;
    case FocusIn:
#ifdef DEBUG
      printf ("FocusIn\n");
#endif
      handleFocusIn ((XFocusChangeEvent *) ev);
      break;
    case FocusOut:
#ifdef DEBUG
      printf ("FocusOut\n");
#endif
      handleFocusOut ((XFocusChangeEvent *) ev);
      break;
    case PropertyNotify:
#ifdef DEBUG
      printf ("PropertyNotify\n");
#endif
      handlePropertyNotify ((XPropertyEvent *) ev);
      break;
    case ClientMessage:
#ifdef DEBUG
      printf ("clientMessage\n");
#endif
      handleClientMessage ((XClientMessageEvent *) ev);
      break;
    default:
#ifdef DEBUG
      printf ("default\n");
#endif
      if (shape && ev->type == shape_event)
	handleShape ((XShapeEvent *) ev);
    }
}

void
eventLoop ()
{
  fd_set readset;
  int x_fd;
  XEvent ev;
  int nfds;

#ifdef DEBUG
  printf ("entering eventLoop\n");
#endif

  /* first get the X11 fd */
  x_fd = XConnectionNumber (dpy);

  while (1)
    {
      /* then zero the readset and add the x_fd to it */
      FD_ZERO (&readset);
      FD_SET (x_fd, &readset);

      /* if the ice_fd exists, add it to the readset too. */
      if (ice_fd != -1)
	FD_SET (ice_fd, &readset);

      /* compute the number of the highest fd for the select call */
      nfds = (x_fd > ice_fd) ? x_fd+1 : ice_fd+1;
      
      /* did we catch a -1 (interrupted / error) in the select call? */
INT:  if (-1 == select (nfds, &readset, NULL, NULL, NULL))
	{
	  if (errno == EINTR)
	    {
	      /* do we need to reload oroborus? */
	      if (reload)
		{
		  cleanUp ();
#ifdef DEBUG
		  printf ("executing: %s\n", progname);
#endif
		  /* exec oroborus itself */
		  execvp (progname, stat_argv);
		}

	      /* do we need to raise the current window?
	       * raise_current_window is set in the SIGALRM handler.
	       */
	      if (raise_current_window)
		{
		  client_t *c = clientGetFocus();

		  raise_current_window = False;
		  /* is there a client, then raise it */
		  if (c)
		    {
#ifdef DEBUG
		      printf ("raising client (%#lx)\n", c->window);
#endif
		      clientRaise (c);
		      XFlush(dpy);
		    }
		}

	      /* shall we quit? */
	      if (quit)
		return;

	      /* EINTR means, we just can re-execute the select call.
	       * Sorry, but in this case just the best is a goto.
	       */
	      goto INT;
	    }

	  /* now comes the case, select fails, and errno is not EINTR. */
	  else
	    return;

	}
      else
	{
	  /* now lets check the fd's */
	  if (FD_ISSET (x_fd, &readset))
	    /* process all pending requests */
	    while (XPending (dpy))
	      {
		XNextEvent (dpy, &ev);
		handleEvent (&ev);
	      }
	  else if (FD_ISSET (ice_fd, &readset))
	    /* process the ice messages */
	    process_ice_msgs ();
	}

    }

#ifdef DEBUG
  printf ("leaving eventLoop\n");
#endif
}

void
switch_timer (Bool enable)
{
  struct itimerval itimer;

#ifdef DEBUG
  if (enable)
    printf ("TIMER: enabling\n");
  else
    printf ("TIMER: disabling\n");
#endif

  /* we don't need an interval */
  itimer.it_interval.tv_usec = 0;
  itimer.it_interval.tv_sec = 0;

  if (enable)
    /* insert the timer value here */
    itimer.it_value.tv_usec = raise_delay*1000;
  else
    itimer.it_value.tv_usec = 0;

  itimer.it_value.tv_sec = 0;

  setitimer (ITIMER_REAL, &itimer, NULL);
}

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: event handling for oroborus-wm                                               *
 ******************************************************************************************/
