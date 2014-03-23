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
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <X11/xpm.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "client.h"
#include "config.h"
#include "hints.h"
#include "i18n.h"
#include "keyboard.h"
#include "pixmap.h"
#include "workspaces.h"

#include "globals.h"

void
workspaceSwitch (int new_ws, client_t * c2)
{
  client_t *c, *f = NULL;
  int i;

#ifdef DEBUG
  printf ("entering workspaceSwitch\n");
#endif

  if (new_ws < 0 && wrap_workspaces)
    new_ws = workspace_count - 1;
  if (new_ws > workspace_count - 1 && wrap_workspaces)
    new_ws = 0;
  if (new_ws < 0 || new_ws > workspace_count - 1 || new_ws == workspace)
    return;

  workspace = new_ws;
  f = clientGetFocus ();
  if (f)
    f->focus = True;
  else if (!f && !c2)
    f = clientGetNext (clients, 0);


  if (c2)
    {
      setGnomeHint (c2->window, win_workspace, new_ws);
      setGnomeHint (c2->window, net_atoms[NET_WM_DESKTOP], new_ws);
      c2->win_workspace = new_ws;
    }

  for (c = clients, i = 0; i < client_count; c = c->next, i++)
    {
      if (c->win_state & WIN_STATE_STICKY)
	clientSetWorkspace (c, new_ws);
      else
	{
	  if (c->win_workspace == new_ws &&
	      getWMState (c->window) != IconicState)
	    {
	      clientShow (c, False);
	      if (c->focus)
		f = c;
	      c->focus = False;
	    }
	  else
	    clientHide (c, False);
	}
    }

  setGnomeHint (root, win_workspace, new_ws);
  setGnomeHint (root, net_atoms[NET_CURRENT_DESKTOP], new_ws);

  if (c2)
    f = c2;
  clientSetFocus (f, True);
  clientRaise (f);
}

void
workspaceSetCount (int count)
{
  client_t *c;
  int i;

#ifdef DEBUG
  printf ("entering workspaceSetCount\n");
#endif

  if (count < 1)
    count = 1;
  if (count == workspace_count)
    return;

  setGnomeHint (root, win_workspace_count, count);
  setGnomeHint (root, net_atoms[NET_NUMBER_OF_DESKTOPS], count);
  workspace_count = count;

  for (c = clients, i = 0; i < client_count; c = c->next, i++)
    if (c->win_workspace > count - 1)
      clientSetWorkspace (c, count - 1);

  if (workspace > count - 1)
    workspaceSwitch (count - 1, NULL);
}

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: workspace handling for oroborus-wm                                           *
 ******************************************************************************************/
