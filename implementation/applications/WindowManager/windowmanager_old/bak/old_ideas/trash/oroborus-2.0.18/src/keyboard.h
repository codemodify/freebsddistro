/* 
 * Oroborus Window Manager
 * X11 Keyboard Utility Library
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

#ifndef __KEYBOARD_H
#define __KEYBOARD_H

enum
{
  KEY_MOVE_UP,
  KEY_MOVE_DOWN,
  KEY_MOVE_LEFT,
  KEY_MOVE_RIGHT,
  KEY_RESIZE_UP,
  KEY_RESIZE_DOWN,
  KEY_RESIZE_LEFT,
  KEY_RESIZE_RIGHT,
  KEY_CYCLE_WINDOWS,
  KEY_CLOSE_WINDOW,
  KEY_HIDE_WINDOW,
  KEY_TOGGLE_FULLSCREEN_WINDOW,
  KEY_MAXIMIZE_WINDOW,
  KEY_MAXIMIZE_VERT,
  KEY_MAXIMIZE_HORIZ,
  KEY_SHADE_WINDOW,
  KEY_RAISE_WINDOW_LAYER,
  KEY_LOWER_WINDOW_LAYER,
  KEY_NEXT_WORKSPACE,
  KEY_PREV_WORKSPACE,
  KEY_ADD_WORKSPACE,
  KEY_DEL_WORKSPACE,
  KEY_STICK_WINDOW,
  KEY_WORKSPACE_1,
  KEY_WORKSPACE_2,
  KEY_WORKSPACE_3,
  KEY_WORKSPACE_4,
  KEY_WORKSPACE_5,
  KEY_WORKSPACE_6,
  KEY_WORKSPACE_7,
  KEY_WORKSPACE_8,
  KEY_WORKSPACE_9,
  KEY_MOVE_NEXT_WORKSPACE,
  KEY_MOVE_PREV_WORKSPACE,
  KEY_MOVE_WORKSPACE_1,
  KEY_MOVE_WORKSPACE_2,
  KEY_MOVE_WORKSPACE_3,
  KEY_MOVE_WORKSPACE_4,
  KEY_MOVE_WORKSPACE_5,
  KEY_MOVE_WORKSPACE_6,
  KEY_MOVE_WORKSPACE_7,
  KEY_MOVE_WORKSPACE_8,
  KEY_MOVE_WORKSPACE_9,
  KEY_TOP_LEFT,
  KEY_TOP_RIGHT,
  KEY_BOTTOM_LEFT,
  KEY_BOTTOM_RIGHT,
  KEY_QUIT,
  KEY_WINDOW_OPS,
  KEY_ENUM_MAX
};


typedef struct
{
  KeyCode keycode;
  int modifier;
}
MyKey;

void parseKeyString (Display *, MyKey *, char *);
void grabKey (Display *, MyKey *, Window);
void ungrabKeys (Display *, Window);
void init_keyboard (Display * dpy);

int NumLockMask, CapsLockMask, ScrollLockMask;

#endif /* __KEYBOARD_H */

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: keyboard header for oroborus-wm                                              *
 ******************************************************************************************/
