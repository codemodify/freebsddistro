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

#ifndef __HINTS_H
#define __HINTS_H

#define MWM_HINTS_DECORATIONS		(1L<<1)
#define MWM_DECOR_ALL			(1L<<0)
#define MWM_DECOR_BORDER		(1L<<1)
#define MWM_DECOR_TITLE			(1L<<3)
#define MWM_DECOR_MAXIMIZE		(1L<<6)

#define WIN_STATE_STICKY		(1L<<0)
#define WIN_STATE_MAXIMIZED_VERT	(1L<<2)
#define WIN_STATE_MAXIMIZED_HORIZ	(1L<<3)
#define WIN_STATE_MAXIMIZED		((1L<<2)|(1L<<3))
#define WIN_STATE_SHADED		(1L<<5)
#define WIN_STATE_FULLSCREEN		(1L<<7)

#define WIN_HINTS_SKIP_FOCUS		(1L<<0)
#define WIN_HINTS_SKIP_TASKBAR		(1L<<2)

#define WIN_LAYER_DESKTOP		0
#define WIN_LAYER_BELOW			2
#define WIN_LAYER_NORMAL		4
#define WIN_LAYER_DOCK			5
#define WIN_LAYER_ONTOP			6
#define WIN_LAYER_FULLSCREEN		8

enum
{
  NET_CLIENT_LIST,
  NET_CLIENT_LIST_STACKING,
  NET_WM_WINDOW_TYPE,
  NET_WM_WINDOW_TYPE_DESKTOP,
  NET_WM_WINDOW_TYPE_DOCK,
  NET_WM_WINDOW_TYPE_TOOLBAR,
  NET_WM_WINDOW_TYPE_MENU,
  NET_WM_WINDOW_TYPE_UTILITY,
  NET_WM_WINDOW_TYPE_SPLASH,
  NET_WM_WINDOW_TYPE_DIALOG,
  NET_WM_WINDOW_TYPE_NORMAL,
  NET_WM_STATE,
  NET_WM_STATE_MODAL,
  NET_WM_STATE_STICKY,
  NET_WM_STATE_SHADED,
  NET_WM_STATE_SKIP_TASKBAR,
  NET_WM_STATE_SKIP_PAGER,
  NET_WM_STATE_HIDDEN,
  NET_WM_STATE_ABOVE,
  NET_WM_STATE_FULLSCREEN,
  NET_WM_NAME,
  NET_WM_DESKTOP,
  NET_NUMBER_OF_DESKTOPS,
  NET_CURRENT_DESKTOP,
  NET_ACTIVE_WINDOW,
  NET_SUPPORTING_WM_CHECK,
  NET_WM_WORKAREA,
  NET_WORKAREA,
  NET_CLOSE_WINDOW,
  NET_WM_STRUT,
  NET_WM_PID,
  NET_ATOM_COUNT
};


typedef struct
{
  CARD32 flags;
  CARD32 functions;
  CARD32 decorations;
  INT32 inputMode;
  CARD32 status;
}
PropMwmHints;

extern Atom wm_state, wm_change_state, wm_delete_window, wm_protocols;
extern Atom motif_wm_hints;
extern Atom win_hints, win_state, win_client_list, win_layer, win_workspace,
  win_workspace_count, win_desktop_button_proxy, win_supporting_wm_check,
  gnome_panel_desktop_area;
extern Atom net_atoms[NET_ATOM_COUNT];

void initHints (Display *);
void initICCCMHints ();
void initGnomeHints ();
void initNETHints ();

long getWMState (Window);
void setWMState (Window, long state);
void initMotifHints ();
PropMwmHints *getMotifHints (Window);
int getGnomeHint (Window, Atom, long *);
void setGnomeHint (Window, Atom, long);
void setNetWmName (Window, Atom, char *);
void setSupportingWmCheck (Window, Window);
void getGnomeDesktopMargins (CARD32 *);
int getNetWMPid (Window w);
void getNetWMStrut (Window, CARD32 *, CARD32 *);
void delNetWMStrut (CARD32 *, CARD32 *);
int typeDesktop (Window);
int typeDock (Window);
int typeAbove (Window);
int typeEWMH (Window, char *);
void setNetWorkarea (CARD32 *, int);
void *getPropData (Window, Atom, Atom, unsigned long *);
void getEWMHState (Window, long *);
void setFocusHint (Window);
int equals (char *left, char *right);

#endif /* __HINTS_H */

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: x11_hints header for oroborus-wm                                             *
 ******************************************************************************************/
