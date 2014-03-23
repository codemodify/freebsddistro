/* 
 * Oroborus Window Manager
 *
 * Copyright (C) 2005 Stefan Pfetzing
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

#ifndef __CLIENT_H__
#define __CLIENT_H__

/*
 * Defines
 */

#define WINDOW				0
#define FRAME				1

#define APPLY				1
#define REMOVE				-1

#define PLACEMENT_MOUSE			0
#define PLACEMENT_ROOT			1

#define INCLUDE_HIDDEN			(1<<0)
#define INCLUDE_SKIP_FOCUS		(1<<1)
#define INCLUDE_ALL_WORKSPACES		(1<<2)

#define ACTIVE				0
#define INACTIVE			1
#define PRESSED				2

#define SIDE_LEFT			0
#define SIDE_RIGHT			1
#define SIDE_BOTTOM			2

#define MARGIN_LEFT			0
#define MARGIN_RIGHT			1
#define MARGIN_TOP			2
#define MARGIN_BOTTOM			3

#define CORNER_TOP_LEFT			0
#define CORNER_TOP_RIGHT		1
#define CORNER_BOTTOM_LEFT		2
#define CORNER_BOTTOM_RIGHT		3

#define TITLE_1				0
#define TITLE_2				1
#define TITLE_3				2
#define TITLE_4				3
#define TITLE_5				4

#define ALIGN_LEFT			0
#define ALIGN_RIGHT			1
#define ALIGN_CENTER			2

#define ACTION_NONE			0
#define ACTION_MAXIMIZE			1
#define ACTION_SHADE			2
#define ACTION_HIDE			3

enum
{
  NET_WM_STATE_REMOVE,		/* remove/unset property */
  NET_WM_STATE_ADD,		/* add/set property */
  NET_WM_STATE_TOGGLE		/* toggle property  */
};

enum
{
  HIDE_BUTTON,
  SHADE_BUTTON,
  MAXIMIZE_BUTTON,
  CLOSE_BUTTON,
  BUTTON_ENUM_MAX
};

/* The Type used for clients */
typedef struct client client_t;

/**
 * Client structure.
 * This structure represents a client with all its attributes.
 */
struct client
{
  Window window,	/**< pointer to the window itself. */
  	frame,		/**< pointer to the frame window. */
	title, 		/**< pointer to the title window. */
	sides[3], 	/**< pointer to the left, bottom and right side window. */
	corners[4], 	/**< pointer to each corner window */
	buttons[BUTTON_ENUM_MAX];
			/**< pointer to the button windows in the title bar */
  int	x,		/**< top left corner of the client. */
  	y, 		/**< top left corner of the client. */
  	width, height,	/**< dimensions of the client. */
	border_width;	/**< the width of the border */
  int 	old_x,		/**< remember the old position (when maximizing). */
  	old_y,		/**< remember the old position (when maximizing). */
  	old_width,	/**< remember the old dimensions (when maximizing). */
	old_height;	/**< remember the old dimensions (when maximizing). */
  int 	fs_x,		/**< remember the old position (when switching to fullscreen). */
  	fs_y,		/**< remember the old position (when switching to fullscreen). */
  	fs_width,	/**< remember the old dimensions (when switching to fullscreen). */
	fs_height,	/**< remember the old dimensions (when switching to fullscreen). */
	fs_win_layer;	/**< remember the old layer (when switching to fullscreen). */
  CARD32 margins[4];	/**< the space, the client wants to "reserve" at the desktop border,
			  for example the gnome panel does this. */
  int button_pressed[BUTTON_ENUM_MAX];
  			/**< to indicate if a button was pressed (and which). */
  char *name;		/**< the clients name */
  XSizeHints *size;	/**< client size hints, see man XSizeHints. */
  int has_border;	/**< has the client a border (can be forced to none with the borderless list). */
  int	above,		/**< is this client above each other client on the current workspace? */
  	was_above;	/**< remember if the client was above each other client on the current workspace. */
  long win_hints;	/**< the window hints specified as long value. see manpage for details. */
  long win_state;	/**< the current window state, also specified as long value. see manpage for details. */
  long win_layer;	/**< the current window layer. this is used to set a group of clients for example above another group of clients.
			  for example the gnome desktop specifies a window layer which is typically below each other client. */
  long win_workspace;	/**< the current workspace of the client */
  int ignore_unmap,	/**< ignore the unmap request. */
  	focus,		/**< the client has the focus. */
	non_focusing;	/**< client is non-focusing, like it does not want the focus. */
  client_t *next;	/**< pointer to the next client. */
  client_t *prev;	/**< pointer to the previous client */
  client_t *transientFor;
  			/**< pointer to the client, for which the current client is transient, ie its a dialog or popup window. */
};

/*
 *
 * Function prototypes
 *
 */

void clientToggleFullscreen (client_t *c);
void clientGravitate (client_t *, int);
void clientConfigure (client_t *, XWindowChanges *, int);

void clientFrame (Window);
void clientUnframe (client_t *, int);
void clientFrameAll ();
void clientUnframeAll ();

void clientGrabKeys (client_t *);
void clientUngrabKeys (client_t *);

client_t *clientGetFromWindow (Window, int);

void clientShow (client_t *, int);
void clientHide (client_t *, int);

void clientClose (client_t *);

void clientRaise (client_t *);
void clientLower (client_t *);
void clientSetLayer (client_t *, int);
client_t *clientGetTopMost (int);
client_t *clientGetTopMostShaded (int, int);

void clientSetWorkspace (client_t *, int);

void clientSetState (client_t *, CARD32, CARD32);

void clientToggleShaded (client_t *);
void clientToggleSticky (client_t *);
void clientToggleMaximized (client_t *, int);
void clientToggleAbove (client_t *);

void clientSetFocus (client_t *, int);
client_t *clientGetFocus ();

void clientMove (client_t *, XEvent *);
void clientResize (client_t *, XEvent *);
client_t *clientGetNext (client_t *, int);

void clientCycle (client_t *);
void clientButtonPress (client_t *, Window, XButtonEvent *);
void clientWarpMouse (client_t *);

Bool clientInBorderlessList (client_t *);


#endif /* __CLIENT_H_ */

/**This must remain at the end of the file.**********
 * vim600:set sw=2 ts=8 fdm=marker fmr=«««,»»»:     *
 * vim600:set cindent cinoptions={1s,>2s,^-1s,n-1s: *
 ****************************************************/

