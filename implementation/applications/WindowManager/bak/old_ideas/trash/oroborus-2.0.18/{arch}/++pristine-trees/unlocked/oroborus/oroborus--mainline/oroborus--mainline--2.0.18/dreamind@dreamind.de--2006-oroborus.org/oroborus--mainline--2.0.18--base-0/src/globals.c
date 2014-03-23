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
#include <X11/xpm.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "client.h"
#include "i18n.h"
#include "keyboard.h"
#include "pixmap.h"

#include "globals.h"

char *progname;
char **stat_argv;

char *display = NULL;
Display *dpy;

Window root, gnome_win;
Colormap cmap;
int screen;
int depth;
long workspace;
CARD32 margins[4];
Bool quit = False, reload = False;
int shape, shape_event;
Cursor resize_cursor[4], move_cursor, root_cursor;

int want_verbose = 0;
char *rcfile = NULL;

client_t *clients = NULL;
CARD32 *client_list = NULL;
unsigned int client_count = 0;

MyKey keys[KEY_ENUM_MAX];

MyFont title_font;
int title_alignment,
    full_width_title,
    button_spacing,
    button_offset;

GC title_gc[2],
   box_gc;

MyPixmap sides[3][2],
	 corners[4][2],
	 buttons[4][3],
	 title[5][2];

char button_layout[6];
int double_click_action;
int box_move,
    box_resize;
int cycle_workspaces;

int click_to_focus,
    focus_new,
    raise_on_focus,
    raise_delay,
    raise_on_click;

int window_placement,
    snap_to_border,
    snap_to_windows,
    snap_width;

int workspace_count = -1,
    wrap_workspaces;

int button_state[4];

char *borderless_windows = NULL;

/**This must remain at the end of the file.**********
 * vim600:set sw=2 ts=8 fdm=marker fmr=«««,»»»:     *
 * vim600:set cindent cinoptions={1s,>2s,^-1s,n-1s: *
 ****************************************************/

