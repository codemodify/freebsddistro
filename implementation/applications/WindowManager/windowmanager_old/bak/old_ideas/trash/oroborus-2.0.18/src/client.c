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
#include <X11/extensions/shape.h>
#include <X11/Xmd.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/xpm.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#include "client.h"
#include "events.h"
#include "frame.h"
#include "getopt.h"
#include "hints.h"
#include "i18n.h"
#include "keyboard.h"
#include "misc.h"
#include "pixmap.h"
#include "workspaces.h"

#include "globals.h"

void
clientInitPosition (client_t * c)
{
  int mx, my, min_x, min_y, max_x, max_y;

#ifdef DEBUG
  printf ("entering clientInitPosition\n");
#endif

  clientGravitate (c, APPLY);
  if (c->non_focusing || c->above)
    {
      min_x = 0;
      min_y = 0;
      max_x = XDisplayWidth (dpy, screen);
      max_y = XDisplayHeight (dpy, screen);
    }
  else
    {
      min_x = margins[MARGIN_LEFT];
      min_y = margins[MARGIN_TOP];
      max_x = XDisplayWidth (dpy, screen) - margins[MARGIN_RIGHT];
      max_y = XDisplayHeight (dpy, screen) - margins[MARGIN_BOTTOM];
    }

  if (!(c->size->flags & (PPosition | USPosition)))
    {
      getMouseXY (dpy, root, &mx, &my);
      c->x = mx - (frameWidth (c) / 2);
      c->y = my - (frameHeight (c) / 2);

      if (window_placement == PLACEMENT_ROOT)
	{
	  c->x = (max_x - min_x) / 2 - (frameWidth (c) / 2);
	  c->y = (max_y - min_y) / 2 - (frameHeight (c) / 2);
	}
    }

  if (c->x > max_x - frameWidth (c) + frameLeft (c))
    c->x = max_x - frameWidth (c) + frameLeft (c);
  if (c->y > max_y - frameHeight (c) + frameTop (c))
    c->y = max_y - frameHeight (c) + frameTop (c);
  if (c->x < min_x + frameLeft (c))
    c->x = min_x + frameLeft (c);
  if (c->y < min_y + frameTop (c))
    c->y = min_y + frameTop (c);
}

void
clientGrabKeys (client_t * c)
{
  int i;
#ifdef DEBUG
  printf ("entering clientGrabKeys\n");
  printf ("grabbing keys for client (%#lx)\n", c->window);
#endif

  /* do not grab the keys for panels. */
  if (c->non_focusing)
    {
      /* bind only these keys to "special" windows */
      int keynums[] = {
	KEY_CYCLE_WINDOWS,
	KEY_NEXT_WORKSPACE,
	KEY_PREV_WORKSPACE,
	KEY_ADD_WORKSPACE,
	KEY_DEL_WORKSPACE,
	KEY_WORKSPACE_1,
	KEY_WORKSPACE_2,
	KEY_WORKSPACE_3,
	KEY_WORKSPACE_4,
	KEY_WORKSPACE_5,
	KEY_WORKSPACE_6,
	KEY_WORKSPACE_7,
	KEY_WORKSPACE_8,
	KEY_WORKSPACE_9,
	KEY_QUIT
      };
      for (i = 0; i < 15; i++)
	grabKey (dpy, &keys[keynums[i]], c->window);
      return;
    }

  for (i = 0; i < KEY_ENUM_MAX; i++)
    grabKey (dpy, &keys[i], c->window);
}

void
clientUngrabKeys (client_t * c)
{
#ifdef DEBUG
  printf ("entering clientUngrabKeys\n");
  printf ("ungrabing keys for client (%#lx)\n", c->window);
#endif

  ungrabKeys (dpy, c->window);
}

void
clientGravitate (client_t * c, int mode)
{
  int gravity, dx = 0, dy = 0;

#ifdef DEBUG
  printf ("entering clientGravitate\n");
#endif

  gravity =
    c->size->flags & PWinGravity ? c->size->win_gravity : NorthWestGravity;
  switch (gravity)
    {
    case CenterGravity:
      dx = (c->border_width * 2) - ((frameLeft (c) + frameRight (c)) / 2);
      dy = (c->border_width * 2) - ((frameTop (c) + frameBottom (c)) / 2);
      break;
    case NorthGravity:
      dx = (c->border_width * 2) - ((frameLeft (c) + frameRight (c)) / 2);
      dy = frameTop (c);
      break;
    case SouthGravity:
      dx = (c->border_width * 2) - ((frameLeft (c) + frameRight (c)) / 2);
      dy = (c->border_width * 2) - frameBottom (c);
      break;
    case EastGravity:
      dx = (c->border_width * 2) - frameRight (c);
      dy = (c->border_width * 2) - ((frameTop (c) + frameBottom (c)) / 2);
      break;
    case WestGravity:
      dx = frameLeft (c);
      dy = (c->border_width * 2) - ((frameTop (c) + frameBottom (c)) / 2);
      break;
    case NorthWestGravity:
      dx = frameLeft (c);
      dy = frameTop (c);
      break;
    case NorthEastGravity:
      dx = (c->border_width * 2) - frameRight (c);
      dy = frameTop (c);
      break;
    case SouthWestGravity:
      dx = frameLeft (c);
      dy = (c->border_width * 2) - frameBottom (c);
      break;
    case SouthEastGravity:
      dx = (c->border_width * 2) - frameRight (c);
      dy = (c->border_width * 2) - frameBottom (c);
      break;
    }
  c->x = c->x + (dx * mode);
  c->y = c->y + (dy * mode);
}

void
clientAddToList (client_t * c)
{
  int i;
  CARD32 *new;

#ifdef DEBUG
  printf ("entering clientAddToList\n");
#endif

  new = (CARD32 *) malloc (sizeof (CARD32) * client_count);
  if (new)
    {
      for (i = 0; i < client_count - 1; i++)
	new[i] = client_list[i];
      new[i] = c->window;
      XChangeProperty (dpy, root, win_client_list, XA_CARDINAL, 32,
		       PropModeReplace, (unsigned char *) new, client_count);
      XChangeProperty (dpy, root, net_atoms[NET_CLIENT_LIST], XA_WINDOW, 32,
		       PropModeReplace, (unsigned char *) new, client_count);
      XChangeProperty (dpy, root, net_atoms[NET_CLIENT_LIST_STACKING],
		       XA_WINDOW, 32, PropModeReplace,
		       (unsigned char *) new, client_count);
      if (client_list)
	free (client_list);
      client_list = new;
    }
}

void
clientRemoveFromList (client_t * c)
{
  int i, j;
  CARD32 *new;

#ifdef DEBUG
  printf ("entering clientRemoveFromList\n");
#endif

  new = (CARD32 *) malloc (sizeof (CARD32) * client_count);
  if (new)
    {
      for (i = 0, j = 0; i < client_count; i++)
	if (client_list[i] != c->window)
	  new[j++] = client_list[i];
      XChangeProperty (dpy, root, win_client_list, XA_CARDINAL, 32,
		       PropModeReplace, (unsigned char *) new,
		       client_count - 1);
      XChangeProperty (dpy, root, net_atoms[NET_CLIENT_LIST], XA_WINDOW, 32,
		       PropModeReplace, (unsigned char *) new,
		       client_count - 1);
      XChangeProperty (dpy, root, net_atoms[NET_CLIENT_LIST_STACKING],
		       XA_WINDOW, 32, PropModeReplace,
		       (unsigned char *) new, client_count - 1);
      if (client_list)
	free (client_list);
      client_list = new;
    }
}

int
clientGetWidthInc (client_t * c)
{
  if (c->size->flags & PResizeInc)
    return c->size->width_inc;
  return 1;
}

int
clientGetHeightInc (client_t * c)
{
  if (c->size->flags & PResizeInc)
    return c->size->height_inc;
  return 1;
}

void
clientSetWidth (client_t * c, int w1)
{
  int w2;

  if (c->size->flags & PResizeInc)
    {
      if (c->size->width_inc == 0)
	c->size->width_inc = 1;
      w2 = (w1 - c->size->min_width) / c->size->width_inc;
      w1 = c->size->min_width + (w2 * c->size->width_inc);
    }
  if (c->size->flags & PMinSize)
    if (w1 < c->size->min_width)
      w1 = c->size->min_width;
  if (c->size->flags & PMaxSize)
    if (w1 > c->size->max_width)
      w1 = c->size->max_width;
  if (w1 < 1)
    w1 = 1;
  c->width = w1;
}

void
clientSetHeight (client_t * c, int h1)
{
  int h2;

  if (c->size->flags & PResizeInc)
    {
      if (c->size->height_inc == 0)
	c->size->height_inc = 1;
      h2 = (h1 - c->size->min_height) / c->size->height_inc;
      h1 = c->size->min_height + (h2 * c->size->height_inc);
    }
  if (c->size->flags & PMinSize)
    if (h1 < c->size->min_height)
      h1 = c->size->min_height;
  if (c->size->flags & PMaxSize)
    if (h1 > c->size->max_height)
      h1 = c->size->max_height;
  if (h1 < 1)
    h1 = 1;
  c->height = h1;
}

client_t *
clientGetTopMost (int layer)
{
  return clientGetTopMostShaded (layer, 0);
}

client_t *
clientGetTopMostShaded (int layer, int non_shaded)
{
  Window w1, w2, *wins;
  unsigned int i, count;
  XWindowAttributes attr;
  client_t *top = NULL, *c;

#ifdef DEBUG
  printf ("entering clientGetTopMost\n");
#endif

  XQueryTree (dpy, root, &w1, &w2, &wins, &count);
  for (i = 0; i < count; i++)
    {
      XGetWindowAttributes (dpy, wins[i], &attr);
      c = clientGetFromWindow (wins[i], FRAME);
      if (c && attr.map_state == IsViewable)
	{
	  if ((c->win_layer <= layer))
	    {
	      if (non_shaded && !(c->win_state & WIN_STATE_SHADED))
		top = c;
	      else if (!non_shaded)
		top = c;
	    }
	  else
	    break;
	}
    }
  if (wins)
    XFree (wins);
  return top;
}

client_t *
clientGetBottomMost (int layer)
{
  Window w1, w2, *wins;
  unsigned int i, count;
  XWindowAttributes attr;
  client_t *c = NULL;

#ifdef DEBUG
  printf ("entering clientGetBottomMost\n");
#endif

  XQueryTree (dpy, root, &w1, &w2, &wins, &count);
  for (i = 0; i < count; i++)
    {
      XGetWindowAttributes (dpy, wins[i], &attr);
      c = clientGetFromWindow (wins[i], FRAME);
      if (c && attr.map_state == IsViewable
	  && !(c->win_hints & WIN_HINTS_SKIP_FOCUS))
	{
	  if (c->win_layer >= layer)
	    break;
	}
    }
  if (wins)
    XFree (wins);
  return c;
}

void
clientConfigure (client_t * c, XWindowChanges * wc, int mask)
{
  XConfigureEvent ce;
  client_t *sibling = NULL;

#ifdef DEBUG
  printf ("entering clientConfigure\n");
  printf ("configuring client (%#lx)\n", c->window);
#endif

  if (mask & CWX)
    c->x = wc->x;
  if (mask & CWY)
    c->y = wc->y;
  if (mask & CWWidth)
    clientSetWidth (c, wc->width);
  if (mask & CWHeight)
    clientSetHeight (c, wc->height);
  if (mask & CWBorderWidth)
    c->border_width = wc->border_width;
  if (mask & CWStackMode)
    {
      switch (wc->stack_mode)
	{
	case Above:
	  sibling = clientGetTopMost (c->win_layer);
	  if (!sibling)
	    wc->stack_mode = Below;
	  break;
	case Below:
	  sibling = clientGetBottomMost (c->win_layer);
	  break;
	}
      if (sibling)
	{
	  if (sibling != c)
	    {
	      wc->sibling = sibling->frame;
	      mask = mask | CWSibling;
	    }
	  else
	    mask = mask & ~(CWSibling | CWStackMode);
	}
      else
	mask = mask & ~CWSibling;
    }

  wc->x = frameX (c);
  wc->y = frameY (c);
  wc->width = frameWidth (c);
  wc->height = frameHeight (c);
  wc->border_width = 0;
  XConfigureWindow (dpy, c->frame, mask, wc);
  wc->x = frameLeft (c);
  wc->y = frameTop (c);
  wc->width = c->width;
  wc->height = c->height;
  mask = mask & ~CWStackMode;
  mask = mask & ~CWSibling;
  XConfigureWindow (dpy, c->window, mask, wc);

  if (mask & (CWWidth | CWHeight))
    frameDraw (c);

  if (mask)
    {
      ce.type = ConfigureNotify;
      ce.event = c->window;
      ce.window = c->window;
      ce.x = c->x;
      ce.y = c->y;
      ce.width = c->width;
      ce.height = c->height;
      ce.border_width = 0;
      ce.above = None;
      ce.override_redirect = False;
      XSendEvent (dpy, c->window, False, StructureNotifyMask,
		  (XEvent *) & ce);
    }
}

void
clientFrame (Window w)
{
  client_t *c;
  XWindowAttributes attr;
  XWindowChanges wc;
  long dummy;
  PropMwmHints *mwm_hints;
  Window transientWindow;
  int i;

#ifdef DEBUG
  printf ("entering clientFrame\n");
  printf ("framing client (%#lx)\n", w);
#endif

  c = malloc (sizeof (*c));

  c->size = XAllocSizeHints ();
  XGetWMNormalHints (dpy, w, c->size, &dummy);
  XGetWindowAttributes (dpy, w, &attr);
  c->window = w;
  if (typeAbove (c->window))
    {
      c->above = True;
      c->was_above = True;
    }
  else
    {
      c->above = False;
      c->was_above = False;
    }
  c->x = c->old_x = attr.x;
  c->y = c->old_y = attr.y;
  c->width = c->old_width = attr.width;
  c->height = c->old_height = attr.height;
  c->border_width = attr.border_width;
  for (i = 0; i < BUTTON_ENUM_MAX; i++)
    c->button_pressed[i] = False;
  getNetWMStrut (w, margins, c->margins);
  if (!getGnomeHint (w, win_hints, &c->win_hints))
    c->win_hints = 0;
  if (!getGnomeHint (w, win_state, &c->win_state))
    c->win_state = 0;
  getEWMHState (w, &c->win_state);
  if (!getGnomeHint (w, win_layer, &c->win_layer))
    c->win_layer = WIN_LAYER_NORMAL;
  if (!getGnomeHint (w, win_workspace, &c->win_workspace))
    {
      setGnomeHint (w, win_workspace, workspace);
      setGnomeHint (w, net_atoms[NET_WM_DESKTOP], workspace);
      c->win_workspace = workspace;
    }
  if (c->win_workspace > workspace_count - 1)
    {
      setGnomeHint (w, win_workspace, workspace_count - 1);
      setGnomeHint (w, net_atoms[NET_WM_DESKTOP], workspace_count - 1);
      c->win_workspace = workspace_count - 1;
    }

  if (clientInBorderlessList (c))
    {
      c->has_border = False;
    }
  else
    c->has_border = True;

  if (typeDesktop (c->window))
    {
      c->win_state = c->win_state | WIN_STATE_STICKY;
      c->non_focusing = True;
      c->has_border = False;
      c->win_layer = WIN_LAYER_DESKTOP;
    }
  else if (typeDock (c->window))
    {
      c->win_state = c->win_state | WIN_STATE_STICKY;
      c->non_focusing = True;
      c->win_layer = WIN_LAYER_DOCK;
    }
  else
    c->non_focusing = False;

  c->ignore_unmap = attr.map_state == IsViewable ? 1 : 0;
  c->focus = False;

  getWindowName (dpy, c->window, &c->name);

  mwm_hints = getMotifHints (c->window);
  if (mwm_hints)
    {
      if (mwm_hints->flags & MWM_HINTS_DECORATIONS &&
	  !(mwm_hints->decorations & MWM_DECOR_ALL))
	c->has_border = mwm_hints->decorations & MWM_DECOR_BORDER;
      XFree (mwm_hints);
    }

  if (attr.map_state != IsViewable)
    clientInitPosition (c);
  else
    clientGravitate (c, APPLY);

  c->frame =
    XCreateSimpleWindow (dpy, root, frameX (c), frameY (c), frameWidth (c),
			 frameHeight (c), 0, 0, 0);
  XSelectInput (dpy, c->frame,
		SubstructureNotifyMask | SubstructureRedirectMask |
		EnterWindowMask);
  XSelectInput (dpy, c->window, FocusChangeMask | PropertyChangeMask);
  if (shape)
    XShapeSelectInput (dpy, c->window, ShapeNotifyMask);
  XSetWindowBorderWidth (dpy, w, 0);
  XReparentWindow (dpy, w, c->frame, frameLeft (c), frameTop (c));
  XGrabButton (dpy, AnyButton, AnyModifier, c->frame, False, ButtonPressMask,
	       GrabModeSync, GrabModeAsync, None, None);

  c->sides[SIDE_LEFT] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->sides[SIDE_RIGHT] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->sides[SIDE_BOTTOM] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->corners[CORNER_BOTTOM_LEFT] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->corners[CORNER_BOTTOM_RIGHT] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->title = XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->corners[CORNER_TOP_LEFT] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->corners[CORNER_TOP_RIGHT] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  for (i = 0; i < 4; i++)
    XDefineCursor (dpy, c->corners[i], resize_cursor[i]);

  for (i = 0; i < BUTTON_ENUM_MAX; i++)
    c->buttons[i] = XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);

  if (clients)
    {
      c->prev = clients->prev;
      c->next = clients;
      clients->prev->next = c;
      clients->prev = c;
    }
  else
    {
      clients = c;
      c->next = c;
      c->prev = c;
    }

  XGetTransientForHint (dpy, c->window, &transientWindow);
  c->transientFor = clientGetFromWindow (transientWindow, WINDOW);

  client_count++;
  clientAddToList (c);
  clientGrabKeys (c);
  frameDraw (c);
  wc.x = c->x;
  wc.y = c->y;
  wc.height = c->height;
  wc.width = c->width;
  wc.stack_mode = Above;
  clientConfigure (c, &wc, CWX | CWY | CWHeight | CWWidth | CWStackMode);

  if (getWMState (c->window) != IconicState)
    {
      clientShow (c, True);
      if (focus_new
	  && (!(c->win_hints & WIN_HINTS_SKIP_FOCUS) || !c->non_focusing))
	{
	  clientSetFocus (c, True);
	}
    }
  else
    setWMState (c->window, IconicState);

#ifdef DEBUG
  printf ("client_count=%d\n", client_count);
#endif
  if (c->above)
    clientSetFocus (c, 0);
}

void
clientUnframe (client_t * c, int remap)
{
#ifdef DEBUG
  printf ("entering clientUnframe\n");
  printf ("unframing client (%#lx)\n", c->window);
#endif

  delNetWMStrut (margins, c->margins);
  clientGravitate (c, REMOVE);
  clientUngrabKeys (c);
  XSetWindowBorderWidth (dpy, c->window, c->border_width);
  XReparentWindow (dpy, c->window, root, c->x, c->y);
  if (remap)
    XMapWindow (dpy, c->window);
  else
    setWMState (c->window, WithdrawnState);
  XDestroyWindow (dpy, c->frame);

  clientRemoveFromList (c);
  if (client_count == 1)
    clients = NULL;
  else
    {
      c->next->prev = c->prev;
      c->prev->next = c->next;
      if (c == clients)
	clients = clients->next;
    }

  if (c->name)
    free (c->name);
  if (c->size)
    XFree (c->size);
  free (c);
  client_count--;

#ifdef DEBUG
  printf ("client_count=%d\n", client_count);
#endif
}

void
clientFrameAll ()
{
  unsigned int count, i;
  Window w1, w2, *wins = NULL;
  XWindowAttributes attr;

#ifdef DEBUG
  printf ("entering clientFrameAll\n");
#endif

  XQueryTree (dpy, root, &w1, &w2, &wins, &count);
  for (i = 0; i < count; i++)
    {
      XGetWindowAttributes (dpy, wins[i], &attr);
      if (!attr.override_redirect && attr.map_state == IsViewable)
	clientFrame (wins[i]);
    }
  if (wins)
    XFree (wins);
}

void
clientUnframeAll ()
{
  client_t *c;
  unsigned int count, i;
  Window w1, w2, *wins = NULL;

#ifdef DEBUG
  printf ("entering clientUnframeAll\n");
#endif

  XQueryTree (dpy, root, &w1, &w2, &wins, &count);
  for (i = 0; i < count; i++)
    {
      c = clientGetFromWindow (wins[i], FRAME);
      if (c)
	clientUnframe (c, True);
    }
  if (wins)
    XFree (wins);
}

client_t *
clientGetFromWindow (Window w, int mode)
{
  client_t *c;
  int i;

#ifdef DEBUG
  printf ("entering clientGetFromWindow\n");
#endif

  for (c = clients, i = 0; i < client_count; c = c->next, i++)
    {
      if (c->window == w && mode == WINDOW)
	return c;
      if (c->frame == w && mode == FRAME)
	return c;
    }

#ifdef DEBUG
  printf ("no client found\n");
#endif

  return NULL;
}

void
clientShow (client_t * c, int change_state)
{
#ifdef DEBUG
  printf ("entering clientShow\n");
  printf ("showing client (%#lx)\n", c->window);
#endif

  if (c->win_workspace == workspace || c->win_state & WIN_STATE_STICKY)
    {
      XMapWindow (dpy, c->window);
      XMapWindow (dpy, c->frame);
    }
  if (change_state)
    setWMState (c->window, NormalState);
}

void
clientHide (client_t * c, int change_state)
{
#ifdef DEBUG
  printf ("entering clientHide\n");
  printf ("hiding client (%#lx)\n", c->window);
#endif

  XUnmapWindow (dpy, c->window);
  XUnmapWindow (dpy, c->frame);
  if (change_state)
    setWMState (c->window, IconicState);
  c->ignore_unmap++;
}

void
clientClose (client_t * c)
{
#ifdef DEBUG
  printf ("entering clientClose\n");
#endif

  sendClientMessage (dpy, c->window, wm_protocols, wm_delete_window,
		     NoEventMask);
}

void
clientKill (client_t * c)
{
  int pid = getNetWMPid (c->window);

#ifdef DEBUG
  printf ("entering clientKill\n");
#endif
  
  if (pid)
    kill (pid, SIGKILL);

  XKillClient (dpy, c->window);
}

void
clientRaise (client_t * c)
{
  XWindowChanges wc;
  client_t *c2;
  int i;

#ifdef DEBUG
  printf ("entering clientRaise\n");
#endif

  if (!c)
    return;

  if (c->win_hints & WIN_HINTS_SKIP_FOCUS || c->non_focusing)
    {
#ifdef DEBUG
      printf ("NOT raising client (%#lx)\n", c->window);
#endif
      return;
    }

#ifdef DEBUG
  printf ("raising client (%#lx)\n", c->window);
#endif

  wc.stack_mode = Above;
  clientConfigure (c, &wc, CWStackMode);

  for (c2 = clients, i = 0; i < client_count; c2 = c2->next, i++)
    if (c2->transientFor == c)
      clientRaise (c2);
}

void
clientLower (client_t * c)
{
  XWindowChanges wc;

#ifdef DEBUG
  printf ("entering clientLower\n");
  printf ("lowering client (%#lx)\n", c->window);
#endif

  wc.stack_mode = Below;
  clientConfigure (c, &wc, CWStackMode);
}

void
clientSetLayer (client_t * c, int l)
{
  int old_layer = c->win_layer;

#ifdef DEBUG
  printf ("entering clientSetLayer\n");
  printf ("setting client (%#lx) layer to %d\n", c->window, l);
#endif

  if (l == old_layer)
    return;

  setGnomeHint (c->window, win_layer, l);
  c->win_layer = l;
  if (l > old_layer)
    clientRaise (c);
  else
    clientLower (c);
}

void
clientSetWorkspace (client_t * c, int ws)
{
#ifdef DEBUG
  printf ("entering clientSetWorkspace\n");
  printf ("setting client (%#lx) to workspace %d\n", c->window, ws);
#endif

  if (c->win_workspace == ws)
    return;

  setGnomeHint (c->window, win_workspace, ws);
  setGnomeHint (c->window, net_atoms[NET_WM_DESKTOP], ws);
  c->win_workspace = ws;
  if (getWMState (c->window) != IconicState)
    {
      if (c->win_state & WIN_STATE_STICKY)
	clientShow (c, False);
      else
	{
	  if (ws == workspace)
	    clientShow (c, False);
	  else
	    clientHide (c, False);
	}
    }
}

void
clientSetState (client_t * c, CARD32 mask, CARD32 value)
{
  XWindowChanges wc;

#ifdef DEBUG
  printf ("entering clientSetState\n");
  printf ("changing state for client (%#lx)\n", c->window);
#endif

  c->win_state = (c->win_state & ~mask) | value;
  setGnomeHint (c->window, win_state, c->win_state);
  if (mask & ~WIN_STATE_STICKY)
    {
      wc.width = c->width;
      wc.height = c->height;
      clientConfigure (c, &wc, CWWidth | CWHeight);
    }
}

void
clientToggleShaded (client_t * c)
{
  XWindowChanges wc;

#ifdef DEBUG
  printf ("entering clientToggleShaded\n");
  printf ("shading/unshading client (%#lx)\n", c->window);
#endif

  c->win_state = c->win_state ^ WIN_STATE_SHADED;
  setGnomeHint (c->window, win_state, c->win_state);
  wc.width = c->width;
  wc.height = c->height;
  clientConfigure (c, &wc, CWWidth | CWHeight);
}

void
clientToggleAbove (client_t * c)
{
#ifdef DEBUG
  printf ("entering clientToggleAbove\n");
  printf ("toggling client (%#lx)\n", c->window);
#endif

  if(c->above)
    {
      c->above = False;
      c->win_layer = WIN_LAYER_NORMAL;
    }
  else
    {
      c->above = True;
      c->win_layer = WIN_LAYER_ONTOP;
      clientSetFocus(c, 0);
    }
  setGnomeHint(c->window, win_layer, c->win_layer);
  c->was_above = True;
}

void
clientToggleSticky (client_t * c)
{
#ifdef DEBUG
  printf ("entering clientToggleSticky\n");
  printf ("sticking/unsticking client (%#lx)\n", c->window);
#endif

  c->win_state = c->win_state ^ WIN_STATE_STICKY;
  setGnomeHint (c->window, win_state, c->win_state);
  clientSetWorkspace (c, workspace);
}

void
clientToggleFullscreen (client_t * c)
{
  XWindowChanges wc;

#ifdef DEBUG
  printf ("entering clientToggleFullscreen\n");
  printf ("fullscreening/unfullscreening client (%#lx)\n", c->window);
#endif

  /* check if the window is already at its maximum size. */
  if (c->size->flags & (PMinSize | PMaxSize) &&
      c->size->min_width == c->size->max_width &&
      c->size->min_height == c->size->max_height)
    return;

  
  if (c->win_state & WIN_STATE_FULLSCREEN)
    {
      wc.width = c->fs_width;
      wc.height = c->fs_height;
      wc.x = c->fs_x;
      wc.y = c->fs_y;
      c->win_state &= ~WIN_STATE_FULLSCREEN;
      clientSetLayer(c, c->fs_win_layer);
    }
  else
    {
      wc.x = wc.y = 0;
      c->fs_x = c->x;
      c->fs_y = c->y;
      c->fs_width = c->width;
      c->fs_height = c->height;
      c->win_state |= WIN_STATE_FULLSCREEN;
      c->fs_win_layer = c->win_layer;
      clientSetLayer(c, WIN_LAYER_FULLSCREEN);
      wc.width = XDisplayWidth (dpy, screen);
      wc.height = XDisplayHeight (dpy, screen);
    }

  clientConfigure (c, &wc, CWX | CWY | CWWidth | CWHeight);
  
  /* make sure the client is focussed and raised afterwards. */
  clientSetFocus (c, True);
  clientRaise (c);
}

void
clientToggleMaximized (client_t * c, int mode)
{
  XWindowChanges wc;

#ifdef DEBUG
  printf ("entering clientToggleMaximized\n");
  printf ("maximzing/unmaximizing client (%#lx)\n", c->window);
#endif

  if (c->size->flags & (PMinSize | PMaxSize) &&
      c->size->min_width == c->size->max_width &&
      c->size->min_height == c->size->max_height)
    return;

  if (c->win_state & WIN_STATE_FULLSCREEN)
    {
      clientToggleFullscreen(c);
      return;
    }

  if (c->win_state & WIN_STATE_MAXIMIZED)
    {
      wc.width = c->old_width;
      wc.height = c->old_height;
      wc.x = c->old_x;
      wc.y = c->old_y;
      c->win_state &= ~WIN_STATE_MAXIMIZED;
    }
  else
    {
      c->old_x = c->x;
      c->old_y = c->y;
      c->old_width = c->width;
      c->old_height = c->height;

      if (mode != WIN_STATE_MAXIMIZED_VERT)
	{
	  wc.x = frameLeft (c) + margins[MARGIN_LEFT];
	  wc.width =
	    XDisplayWidth (dpy,
			   screen) - frameLeft (c) - frameRight (c) -
	    margins[MARGIN_LEFT] - margins[MARGIN_RIGHT];
	  c->win_state |= WIN_STATE_MAXIMIZED_HORIZ;
	}
      else
	{
	  wc.x = c->x;
	  wc.width = c->width;
	}
      if (mode != WIN_STATE_MAXIMIZED_HORIZ)
	{
	  wc.y = frameTop (c) + margins[MARGIN_TOP];
	  wc.height =
	    XDisplayHeight (dpy,
			    screen) - frameTop (c) - frameBottom (c) -
	    margins[MARGIN_TOP] - margins[MARGIN_BOTTOM];
	  c->win_state |= WIN_STATE_MAXIMIZED_VERT;
	}
      else
	{
	  wc.y = c->y;
	  wc.height = c->height;
	}

    }
  setGnomeHint (c->window, win_state, c->win_state);
  clientConfigure (c, &wc, CWX | CWY | CWWidth | CWHeight);
}

void
clientSetFocus (client_t * c, int sort)
{
  client_t *tmp;
  int i;

#ifdef DEBUG
  printf ("entering clientSetFocus\n");
  if (c)
    printf ("setting focus to client (%#lx)\n", c->window);
  else
    printf ("setting focus to none\n");
#endif

  if (c)
    {
      if (c->win_hints & WIN_HINTS_SKIP_FOCUS)
	return;

      if (c->non_focusing)
	{
	  XSetInputFocus (dpy, c->window, RevertToNone, CurrentTime);
	  return;
	}

      if (sort)
	{
	  if (client_count > 2 && c != clients)
	    {
	      tmp = c;
	      c->prev->next = c->next;
	      c->next->prev = c->prev;

	      c->prev = clients->prev;
	      c->next = clients;
	      clients->prev->next = c;
	      clients->prev = c;
	    }
	  clients = c;
	}

      XSetInputFocus (dpy, c->window, RevertToNone, CurrentTime);
      setFocusHint (c->window);
      for (tmp = clients, i = 0; i < client_count; tmp = tmp->next, i++)
	if (c != tmp && tmp && tmp->win_workspace == workspace && tmp->focus)
	  {
	    if (tmp->focus)
	      {
		tmp->focus = False;
		frameDraw (tmp);
	      }
	  }
      c->focus = True;
    }
  else
    {
      XSetInputFocus (dpy, gnome_win, RevertToNone, CurrentTime);
      setFocusHint (None);
      for (tmp = clients, i = 0; i < client_count; tmp = tmp->next, i++)
	if (c != tmp && tmp && tmp->win_workspace == workspace && tmp->focus)
	  {
	    if (tmp->focus)
	      {
		tmp->focus = False;
		frameDraw (tmp);
	      }
	  }
    }
}

client_t *
clientGetFocus ()
{
  client_t *c;
  int i;

#ifdef DEBUG
  printf ("entering clientGetFocus\n");
#endif

  for (c = clients, i = 0; i < client_count; c = c->next, i++)
    if (c->focus && c->win_workspace == workspace)
      return c;
  return NULL;
}

void
clientDrawOutline (client_t * c)
{
#ifdef DEBUG
  printf ("entering clientDrawOutline\n");
#endif

  XDrawRectangle (dpy, root, box_gc, frameX (c), frameY (c),
		  frameWidth (c) - 1, frameHeight (c) - 1);
}

void
clientMove (client_t * c, XEvent * e)
{
  int mx, my, moving = True, grab = False, use_keys = False, g1, g2;
  client_t *closestx, *closesty, *c2;
  XWindowChanges wc;
  XEvent ev;
  int i;
  int ox, oy;
  int cx, cy, dx, dy;

#ifdef DEBUG
  printf ("entering clientDoMove\n");
  printf ("moving client (%#lx)\n", c->window);
#endif

  if ((c->win_state & WIN_STATE_MAXIMIZED) == WIN_STATE_MAXIMIZED
      || c->non_focusing)
    return;

  g1 = XGrabKeyboard (dpy, c->window, False, GrabModeAsync, GrabModeAsync,
		      CurrentTime);
  g2 = XGrabPointer (dpy, c->frame, False,
		     PointerMotionMask | ButtonReleaseMask, GrabModeAsync,
		     GrabModeAsync, None, move_cursor, CurrentTime);

  if (g1 != GrabSuccess || g2 != GrabSuccess)
    {
#ifdef DEBUG
      printf ("grab failed in clientMove\n");
#endif
      if (g1 == GrabSuccess)
	XUngrabKeyboard (dpy, CurrentTime);
      if (g2 == GrabSuccess)
	XUngrabPointer (dpy, CurrentTime);
      return;
    }

  if (e->type == KeyPress)
    {
      use_keys = True;
      XPutBackEvent (dpy, e);
    }

  getMouseXY (dpy, root, &mx, &my);
  ox = c->x;
  oy = c->y;

#ifdef DEBUG
  printf ("entering move loop\n");
#endif

  while (moving)
    {
      XNextEvent (dpy, &ev);

      if (ev.type == KeyPress)
	{
	  if (!grab && box_move)
	    {
	      XGrabServer (dpy);
	      grab = True;
	      clientDrawOutline (c);
	    }
	  if (box_move)
	    clientDrawOutline (c);
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_HORIZ))
	    {
	      if (ev.xkey.keycode == keys[KEY_MOVE_LEFT].keycode)
		c->x = c->x - 16;
	      if (ev.xkey.keycode == keys[KEY_MOVE_RIGHT].keycode)
		c->x = c->x + 16;
	    }
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_VERT))
	    {
	      if (ev.xkey.keycode == keys[KEY_MOVE_UP].keycode)
		c->y = c->y - 16;
	      if (ev.xkey.keycode == keys[KEY_MOVE_DOWN].keycode)
		c->y = c->y + 16;
	    }
	  if (box_move)
	    clientDrawOutline (c);
	  else
	    {
	      wc.x = c->x;
	      wc.y = c->y;
	      clientConfigure (c, &wc, CWX | CWY);
	    }
	}
      else if (use_keys && ev.type == KeyRelease)
	{
	  if (IsModifierKey (XKeycodeToKeysym (dpy, ev.xkey.keycode, 0)))
	    moving = False;
	}
      else if (ev.type == MotionNotify)
	{
	  int new_ws = 0;
	  int switch_ws = 0;

	  while (XCheckTypedEvent (dpy, MotionNotify, &ev));

	  if (!grab && box_move)
	    {
	      XGrabServer (dpy);
	      grab = True;
	      clientDrawOutline (c);
	    }
	  if (box_move)
	    clientDrawOutline (c);

	  if (workspace_count > 1 && cycle_workspaces)
	    {
	      int msx, msy;

	      getMouseXY (dpy, root, &msx, &msy);
	      if (msx == 0 && !(workspace == 0 && !wrap_workspaces))
		{
		  new_ws = workspace - 1;
		  switch_ws = 1;
		  XWarpPointer (dpy, None, root, 0, 0, 0, 0,
				XDisplayWidth (dpy, screen) - 11, msy);
		  ev.xmotion.x_root = XDisplayWidth (dpy, screen) - 11;
		}
	      else if (msx == XDisplayWidth (dpy, screen) - 1 &&
		       !(workspace == workspace_count - 1
			 && !wrap_workspaces))
		{
		  new_ws = workspace + 1;
		  switch_ws = 1;
		  XWarpPointer (dpy, None, root, 0, 0, 0, 0, 10, msy);
		  ev.xmotion.x_root = 10;
		}
	    }

	  if (!(c->win_state & WIN_STATE_MAXIMIZED_HORIZ))
	    {
	      c->x = ox + (ev.xmotion.x_root - mx);
	      if (snap_to_border)
		{
		  if (abs (frameX (c) - margins[MARGIN_LEFT]) < snap_width)
		    c->x = frameLeft (c) + margins[MARGIN_LEFT];
		  if (abs (frameX (c) - XDisplayWidth (dpy, screen) +
			   frameWidth (c) + margins[MARGIN_RIGHT]) <
		      snap_width)
		    c->x =
		      (XDisplayWidth (dpy, screen) - frameRight (c) -
		       c->width - margins[MARGIN_RIGHT]);
		}
	    }

	  if (!(c->win_state & WIN_STATE_MAXIMIZED_VERT))
	    {
	      c->y = oy + (ev.xmotion.y_root - my);
	      if (snap_to_border)
		{
		  if (abs (frameY (c) - margins[MARGIN_TOP]) < snap_width)
		    c->y = frameTop (c) + margins[MARGIN_TOP];
		  if (abs (frameY (c) - XDisplayHeight (dpy, screen) +
			   frameHeight (c) + margins[MARGIN_BOTTOM]) <
		      snap_width)
		    c->y =
		      (XDisplayHeight (dpy, screen) -
		       margins[MARGIN_BOTTOM] - frameHeight (c) +
		       frameTop (c));
		}

	      if (snap_to_windows)
		{
		  cx = cy = snap_width + 1;
		  closestx = closesty = NULL;

		  for (c2 = clients, i = 0; i < client_count;
		       c2 = c2->next, i++)
		    {
		      if (c2->win_workspace != c->win_workspace)
			continue;

		      if (c2->win_hints & WIN_HINTS_SKIP_FOCUS)
			continue;

		      /* Are we touching the bottom of a window? */
		      dy =
			abs (frameY (c) - (frameY (c2) + frameHeight (c2)));
		      if (dy < snap_width && dy < cy)
			{
			  cy = dy;
			  closesty = c2;
			}

		      /* Top? */
		      dy = abs (frameY (c2) - (frameY (c) + frameHeight (c)));
		      if (dy < snap_width && dy < cy)
			{
			  cy = dy;
			  closesty = c2;
			}

		      /* Left? */
		      dx = abs (frameX (c2) - (frameX (c) + frameWidth (c)));
		      if (dx < snap_width && dx < cx)
			{
			  cx = dx;
			  closestx = c2;
			}

		      /* Right */
		      dx = abs (frameX (c) - (frameX (c2) + frameWidth (c2)));
		      if (dx < snap_width && dx < cx)
			{
			  cx = dx;
			  closestx = c2;
			}
		    }

		  if (closesty)
		    {
		      if (cy ==
			  abs (frameY (c) -
			       (frameY (closesty) + frameHeight (closesty))))
			{
			  c->y =
			    frameY (closesty) + frameHeight (closesty) +
			    frameTop (c);
			}
		      else
			{
			  c->y =
			    frameY (closesty) - (frameHeight (c) -
						 frameTop (c));
			}
		    }

		  if (closestx)
		    {
		      if (cx ==
			  abs (frameX (c) -
			       (frameX (closestx) + frameWidth (closestx))))
			{
			  c->x =
			    frameX (closestx) + frameWidth (closestx) +
			    frameLeft (c);
			}
		      else
			{
			  c->x =
			    (frameX (closestx) - frameWidth (c)) +
			    frameRight (c);
			}
		    }
		}
	    }

	  if (box_move)
	    clientDrawOutline (c);
	  else
	    {
	      wc.x = c->x;
	      wc.y = c->y;
	      clientConfigure (c, &wc, CWX | CWY);
	    }
	  if (switch_ws)
	    {
	      workspaceSwitch (new_ws, c);
	      clientRaise (c);
	    }
	}
      else if (!use_keys && ev.type == ButtonRelease)
	moving = False;
      else if (ev.type == UnmapNotify && ev.xunmap.window == c->window)
	moving = False;
      else
	handleEvent (&ev);
    }
#ifdef DEBUG
  printf ("leaving move loop\n");
#endif

  XUngrabKeyboard (dpy, CurrentTime);
  XUngrabPointer (dpy, CurrentTime);

  if (grab && box_move)
    clientDrawOutline (c);

  wc.x = c->x;
  wc.y = c->y;
  clientConfigure (c, &wc, CWX | CWY);

  XUngrabServer (dpy);
}

void
clientResize (client_t * c, XEvent * e)
{
  int mx, my, oldw, oldh, resizing = True, grab = False, corner =
    CORNER_BOTTOM_RIGHT, use_keys = False, g1, g2;
  XEvent ev;
  XWindowChanges wc;

#ifdef DEBUG
  printf ("entering clientResize\n");
  printf ("resizing client (%#lx)\n", c->window);
#endif

  if ((c->win_state & WIN_STATE_SHADED) ||
      ((c->win_state & WIN_STATE_MAXIMIZED) == WIN_STATE_MAXIMIZED))
    return;

  getMouseXY (dpy, c->frame, &mx, &my);
  if (e->type == KeyPress)
    {
      corner = CORNER_BOTTOM_RIGHT;
      use_keys = True;
    }
  else
    {
      if (mx < frameWidth (c) / 2 && my < frameHeight (c) / 2)
	corner = CORNER_TOP_LEFT;
      else if (mx < frameWidth (c) / 2)
	corner = CORNER_BOTTOM_LEFT;
      else if (my < frameHeight (c) / 2)
	corner = CORNER_TOP_RIGHT;
    }

  g1 = XGrabKeyboard (dpy, c->window, False, GrabModeAsync, GrabModeAsync,
		      CurrentTime);
  g2 = XGrabPointer (dpy, c->frame, False,
		     PointerMotionMask | ButtonReleaseMask, GrabModeAsync,
		     GrabModeAsync, None, resize_cursor[corner], CurrentTime);

  if (g1 != GrabSuccess || g2 != GrabSuccess)
    {
#ifdef DEBUG
      printf ("grab failed in clientResize\n");
#endif
      if (g1 == GrabSuccess)
	XUngrabKeyboard (dpy, CurrentTime);
      if (g2 == GrabSuccess)
	XUngrabPointer (dpy, CurrentTime);
      return;
    }

  if (use_keys)
    XPutBackEvent (dpy, e);

  if (corner == CORNER_TOP_RIGHT || corner == CORNER_BOTTOM_RIGHT)
    mx = frameWidth (c) - mx;
  if (corner == CORNER_BOTTOM_LEFT || corner == CORNER_BOTTOM_RIGHT)
    my = frameHeight (c) - my;

#ifdef DEBUG
  printf ("entering resize loop\n");
#endif

  while (resizing)
    {
      XNextEvent (dpy, &ev);

      if (ev.type == KeyPress)
	{
	  if (!grab && box_resize)
	    {
	      XGrabServer (dpy);
	      grab = True;
	      clientDrawOutline (c);
	    }
	  if (box_resize)
	    clientDrawOutline (c);
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_VERT))
	    {
	      if (ev.xkey.keycode == keys[KEY_MOVE_UP].keycode)
		c->height =
		  c->height - (clientGetHeightInc (c) <
			       10 ? 10 : clientGetHeightInc (c));
	      if (ev.xkey.keycode == keys[KEY_MOVE_DOWN].keycode)
		c->height =
		  c->height + (clientGetHeightInc (c) <
			       10 ? 10 : clientGetHeightInc (c));
	    }
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_HORIZ))
	    {
	      if (ev.xkey.keycode == keys[KEY_MOVE_LEFT].keycode)
		c->width =
		  c->width - (clientGetWidthInc (c) <
			      10 ? 10 : clientGetWidthInc (c));
	      if (ev.xkey.keycode == keys[KEY_MOVE_RIGHT].keycode)
		c->width =
		  c->width + (clientGetWidthInc (c) <
			      10 ? 10 : clientGetWidthInc (c));
	    }
	  if (box_resize)
	    clientDrawOutline (c);
	  else
	    {
	      wc.x = c->x;
	      wc.y = c->y;
	      wc.width = c->width;
	      wc.height = c->height;
	      clientConfigure (c, &wc, CWX | CWY | CWWidth | CWHeight);
	    }
	}
      else if (use_keys && ev.type == KeyRelease)
	{
	  if (IsModifierKey (XKeycodeToKeysym (dpy, ev.xkey.keycode, 0)))
	    resizing = False;
	}
      else if (ev.type == MotionNotify)
	{
	  while (XCheckTypedEvent (dpy, MotionNotify, &ev));

	  if (!grab && box_resize)
	    {
	      XGrabServer (dpy);
	      grab = True;
	      clientDrawOutline (c);
	    }
	  if (box_resize)
	    clientDrawOutline (c);
	  oldw = c->width;
	  oldh = c->height;
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_HORIZ))
	    {
	      if (corner == CORNER_TOP_LEFT || corner == CORNER_BOTTOM_LEFT)
		c->width =
		  (c->x + c->width) - ev.xmotion.x_root + mx - frameLeft (c);
	      if (corner == CORNER_BOTTOM_RIGHT || corner == CORNER_TOP_RIGHT)
		c->width = (ev.xmotion.x_root - c->x) + mx - frameRight (c);
	    }
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_VERT))
	    {
	      if (corner == CORNER_TOP_LEFT || corner == CORNER_TOP_RIGHT)
		c->height =
		  (c->y + c->height) - ev.xmotion.y_root + my - frameTop (c);
	      if (corner == CORNER_BOTTOM_RIGHT ||
		  corner == CORNER_BOTTOM_LEFT)
		c->height = (ev.xmotion.y_root - c->y) + my - frameBottom (c);
	    }
	  clientSetWidth (c, c->width);
	  clientSetHeight (c, c->height);
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_HORIZ) &&
	      (corner == CORNER_TOP_LEFT || corner == CORNER_BOTTOM_LEFT))
	    c->x = c->x - (c->width - oldw);
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_VERT) &&
	      (corner == CORNER_TOP_LEFT || corner == CORNER_TOP_RIGHT))
	    c->y = c->y - (c->height - oldh);
	  if (box_resize)
	    clientDrawOutline (c);
	  else
	    {
	      wc.x = c->x;
	      wc.y = c->y;
	      wc.width = c->width;
	      wc.height = c->height;
	      clientConfigure (c, &wc, CWX | CWY | CWWidth | CWHeight);
	    }
	}
      else if (ev.type == ButtonRelease)
	resizing = False;
      else if (ev.type == UnmapNotify && ev.xunmap.window == c->window)
	resizing = False;
      else
	handleEvent (&ev);
    }
#ifdef DEBUG
  printf ("leaving resize loop\n");
#endif

  XUngrabKeyboard (dpy, CurrentTime);
  XUngrabPointer (dpy, CurrentTime);

  if (grab && box_resize)
    clientDrawOutline (c);

  wc.x = c->x;
  wc.y = c->y;
  wc.width = c->width;
  wc.height = c->height;
  clientConfigure (c, &wc, (CWX | CWY | CWHeight | CWWidth));

  XUngrabServer (dpy);
}

client_t *
clientGetNext (client_t * c, int mask)
{
  client_t *c2;
  unsigned int i, okay;

#ifdef DEBUG
  printf ("entering clientGetNext\n");
#endif

  if (c)
    {
      for (c2 = c->next, i = 0; i < client_count; c2 = c2->next, i++)
	{
	  okay = True;
	  if (c2->win_hints & WIN_HINTS_SKIP_FOCUS &&
	      !(mask & INCLUDE_SKIP_FOCUS))
	    okay = False;
	  if (getWMState (c2->window) == IconicState &&
	      !(mask & INCLUDE_HIDDEN))
	    okay = False;
	  if (c2->win_workspace != workspace &&
	      !(mask & INCLUDE_ALL_WORKSPACES))
	    okay = False;
	  if ((c2->win_state & WIN_STATE_SHADED) && !(mask & INCLUDE_HIDDEN))
	    okay = False;
	  if (c2->non_focusing)
	    okay = False;
	  if (okay)
	    return c2;
	}
      for (c2 = c->next, i = 0; i < client_count; c2 = c2->next, i++)
	{
	  okay = True;
	  if (c2->win_hints & WIN_HINTS_SKIP_FOCUS &&
	      !(mask & INCLUDE_SKIP_FOCUS))
	    okay = False;
	  if (getWMState (c2->window) == IconicState &&
	      !(mask & INCLUDE_HIDDEN))
	    okay = False;
	  if (c2->win_workspace != workspace &&
	      !(mask & INCLUDE_ALL_WORKSPACES))
	    okay = False;
	  if (c2->non_focusing)
	    okay = False;
	  if (okay)
	    return c2;
	}
    }
  return NULL;
}

void
clientCycle (client_t * c)
{
  client_t *c2 = c;
  int cycling = True, g1, g2;
  XEvent ev;

#ifdef DEBUG
  printf ("entering clientCycle\n");
#endif

  if (!c)
    return;

  g1 = XGrabKeyboard (dpy, gnome_win, False, GrabModeAsync, GrabModeAsync,
		      CurrentTime);
  g2 = XGrabPointer (dpy, gnome_win, False, NoEventMask, GrabModeAsync,
		     GrabModeAsync, None, None, CurrentTime);
  if (g1 != GrabSuccess || g2 != GrabSuccess)
    {
#ifdef DEBUG
      printf ("grab failed in clientCycle\n");
#endif
      if (g1 == GrabSuccess)
	XUngrabKeyboard (dpy, CurrentTime);
      if (g2 == GrabSuccess)
	XUngrabPointer (dpy, CurrentTime);
      return;
    }

  c2 = clientGetNext (c2, INCLUDE_HIDDEN);
  if (!c2)
    cycling = False;
  else
    {
      clientShow (c2, False);
      clientRaise (c2);
      clientSetFocus (c2, 0);
    }

#ifdef DEBUG
  printf ("entering cycle loop\n");
#endif

  while (cycling)
    {
      XNextEvent (dpy, &ev);
      if (ev.type == KeyPress)
	{
	  if (ev.xkey.keycode == keys[KEY_CYCLE_WINDOWS].keycode)
	    {
	      if (getWMState (c2->window) == IconicState)
		clientHide (c2, False);
	      c2 = clientGetNext (c2, INCLUDE_HIDDEN);
	      if (!c2)
		cycling = False;
	      else
		{
		  clientShow (c2, False);
		  clientRaise (c2);
		  clientSetFocus (c2, 0);
		}
	    }
	  else
	    {
	      XPutBackEvent (dpy, &ev);
	      cycling = False;
	    }
	}
      else if (ev.type == KeyRelease)
	{
	  if (IsModifierKey (XKeycodeToKeysym (dpy, ev.xkey.keycode, 0)))
	    cycling = False;
	}
      else
	handleEvent (&ev);
    }
#ifdef DEBUG
  printf ("leaving cycle loop\n");
#endif

  XUngrabKeyboard (dpy, CurrentTime);
  XUngrabPointer (dpy, CurrentTime);

  if (c2)
    {
      clientShow (c2, True);
      clientSetFocus (c2, True);
      clientWarpMouse (c2);
    }
  else
    c->focus = True;
}

void
clientButtonPress (client_t * c, Window w, XButtonEvent * bev)
{
  int pressed = True, b, g1, g2;
  XEvent ev;

#ifdef DEBUG
  printf ("entering clientButtonPress\n");
#endif

  for (b = 0; b < BUTTON_ENUM_MAX; b++)
    if (c->buttons[b] == w)
      break;

  g1 = XGrabKeyboard (dpy, c->window, False, GrabModeAsync, GrabModeAsync,
		      CurrentTime);
  g2 = XGrabPointer (dpy, w, False,
		     ButtonReleaseMask | EnterWindowMask | LeaveWindowMask,
		     GrabModeAsync, GrabModeAsync, None, None, CurrentTime);

  if (g1 != GrabSuccess || g2 != GrabSuccess)
    {
#ifdef DEBUG
      printf ("grab failed in clientButtonPress\n");
#endif
      if (g1 == GrabSuccess)
	XUngrabKeyboard (dpy, CurrentTime);
      if (g2 == GrabSuccess)
	XUngrabPointer (dpy, CurrentTime);
      return;
    }

#ifdef DEBUG
  printf ("entering button press loop\n");
#endif

  c->button_pressed[b] = True;
  frameDraw (c);

  while (pressed)
    {
      XNextEvent (dpy, &ev);

      if (ev.type == EnterNotify)
	{
	  c->button_pressed[b] = True;
	  frameDraw (c);
	}
      else if (ev.type == LeaveNotify)
	{
	  c->button_pressed[b] = False;
	  frameDraw (c);
	}
      else if (ev.type == ButtonRelease)
	pressed = False;
      else if (ev.type == UnmapNotify && ev.xunmap.window == c->window)
	{
	  pressed = False;
	  c->button_pressed[b] = False;
	}
      else if (ev.type == KeyPress || ev.type == KeyRelease)
	{
	}
      else
	handleEvent (&ev);
    }
#ifdef DEBUG
  printf ("leaving button press loop\n");
#endif

  XUngrabPointer (dpy, CurrentTime);
  XUngrabKeyboard (dpy, CurrentTime);

  if (c->button_pressed[b])
    {
      c->button_pressed[b] = False;
      frameDraw (c);

      switch (b)
	{
	case HIDE_BUTTON:
	  clientHide (c, True);
	  break;
	case CLOSE_BUTTON:
	  if (bev->button == Button3)
	    clientKill (c);
	  else
	    {
	      clientClose (c);
	    }
	  break;
	case MAXIMIZE_BUTTON:
	  if (bev->button == Button1)
	    clientToggleMaximized (c, WIN_STATE_MAXIMIZED);
	  else if (bev->button == Button2)
	    clientToggleMaximized (c, WIN_STATE_MAXIMIZED_VERT);
	  else if (bev->button == Button3)
	    clientToggleMaximized (c, WIN_STATE_MAXIMIZED_HORIZ);
	  break;
	case SHADE_BUTTON:
	  clientToggleShaded (c);
	  break;
	}
    }
}

void
clientWarpMouse (client_t * c)
{
  if (!c)
    return;
  else if (!click_to_focus && c->win_state & WIN_STATE_SHADED)
    XWarpPointer (dpy, None, c->window, 0, 0, 0, 0,
		  (frameWidth (c) / 2), (frameHeight (c) / 2) * -1);
  else if (!click_to_focus)
    XWarpPointer (dpy, None, c->window, 0, 0, 0, 0,
		  (frameWidth (c) / 2), (frameHeight (c) / 2));
}

Bool
clientInBorderlessList (client_t * c)
{
  XClassHint *class_res;
  char *win_class_name;
  char *tok;
  char *buf;
  size_t strSize;

#ifdef DEBUG
  printf ("entering clientInBorderlessList\n");
#endif

  class_res = XAllocClassHint ();

  if (!class_res)
    return False;

  if (XGetClassHint (dpy, c->window, class_res) && class_res->res_name)
    win_class_name = class_res->res_name;
  else
    {
      XFree (class_res->res_name);
      XFree (class_res->res_class);
      XFree (class_res);
      return False;
    }

  strSize = sizeof (char) * (strlen (borderless_windows) + 1);
  buf = malloc (strSize);
  snprintf (buf, strSize, "%s", borderless_windows);

  tok = strtok (buf, ";");

  while (tok)
    {
      if (strcasecmp (tok, win_class_name) == 0)
	{
	  XFree (class_res->res_name);
	  XFree (class_res->res_class);
	  XFree (class_res);
	  free (buf);
	  return True;
	}
      tok = strtok (NULL, ";");
    }

  XFree (class_res->res_name);
  XFree (class_res->res_class);
  XFree (class_res);
  free (buf);
  return False;
}

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: client related stuff for oroborus-wm                                         *
 ******************************************************************************************/
