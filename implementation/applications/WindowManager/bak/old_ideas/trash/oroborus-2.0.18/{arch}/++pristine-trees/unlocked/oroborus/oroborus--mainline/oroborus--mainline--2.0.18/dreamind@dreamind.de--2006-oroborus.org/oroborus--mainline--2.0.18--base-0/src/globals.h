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

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

extern char *progname;
extern char **stat_argv;

extern char *display;
extern Display *dpy;

extern Window root, gnome_win;
extern Colormap cmap;
extern int screen;
extern int depth;
extern long workspace;
extern CARD32 margins[4];
extern Bool quit, reload;
extern int shape, shape_event;
extern Cursor resize_cursor[4], move_cursor, root_cursor;

extern int want_verbose;
extern char *rcfile;

extern client_t *clients;
extern CARD32 *client_list;
extern unsigned int client_count;

extern MyKey keys[KEY_ENUM_MAX];

extern MyFont title_font;
extern int title_alignment, full_width_title, button_spacing, button_offset;

extern GC title_gc[2], box_gc;

extern MyPixmap sides[3][2],
		corners[4][2],
		buttons[4][3],
		title[5][2];

extern char button_layout[6];
extern int double_click_action;
extern int box_move,
	   box_resize;
extern int cycle_workspaces;

extern int click_to_focus,
	   focus_new,
	   raise_on_focus,
	   raise_delay,
	   raise_on_click;

extern int window_placement,
	   snap_to_border,
	   snap_to_windows,
	   snap_width;

extern int workspace_count,
	   wrap_workspaces;

extern int button_state[4];

extern char *borderless_windows;

#endif /* __GLOBALS_H_ */

/**This must remain at the end of the file.**********
 * vim600:set sw=2 ts=8 fdm=marker fmr=«««,»»»:     *
 * vim600:set cindent cinoptions={1s,>2s,^-1s,n-1s: *
 ****************************************************/

