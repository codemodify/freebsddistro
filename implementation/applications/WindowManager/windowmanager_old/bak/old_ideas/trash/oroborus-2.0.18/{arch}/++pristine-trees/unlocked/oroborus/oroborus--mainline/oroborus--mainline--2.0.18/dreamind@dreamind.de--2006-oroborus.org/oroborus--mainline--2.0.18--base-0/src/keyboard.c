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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <string.h>
#include "keyboard.h"

#ifdef DEBUG
#include <stdio.h>
#endif

void
parseKeyString (Display * dpy, MyKey * key, char *str)
{
  char *k;

#ifdef DEBUG
  printf ("entering parseKeyString\n");
  printf ("key string=%s\n", str);
#endif

  key->keycode = 0;
  key->modifier = 0;

  if (!strcmp (str, "none"))
    return;

  k = strrchr (str, '+');
  if (k)
    {
      key->keycode = XKeysymToKeycode (dpy, XStringToKeysym (k + 1));
      if (strstr (str, "Shift"))
	key->modifier = key->modifier | ShiftMask;
      if (strstr (str, "Control"))
	key->modifier = key->modifier | ControlMask;
      if (strstr (str, "Mod1"))
	key->modifier = key->modifier | Mod1Mask;
      if (strstr (str, "Mod2"))
	key->modifier = key->modifier | Mod2Mask;
      if (strstr (str, "Mod3"))
	key->modifier = key->modifier | Mod3Mask;
      if (strstr (str, "Mod4"))
	key->modifier = key->modifier | Mod4Mask;
      if (strstr (str, "Mod5"))
	key->modifier = key->modifier | Mod5Mask;
    }
}

void
grabKey (Display * dpy, MyKey * key, Window w)
{
#ifdef DEBUG
  printf ("entering grabKey\n");
#endif

  if (key->keycode)
    {
      XGrabKey (dpy, key->keycode, key->modifier, w, False, GrabModeAsync,
		GrabModeAsync);
      XGrabKey (dpy, key->keycode, key->modifier | NumLockMask, w, False,
		GrabModeAsync, GrabModeAsync);
      XGrabKey (dpy, key->keycode, key->modifier | CapsLockMask, w, False,
		GrabModeAsync, GrabModeAsync);
      XGrabKey (dpy, key->keycode, key->modifier | ScrollLockMask, w, False,
		GrabModeAsync, GrabModeAsync);
      XGrabKey (dpy, key->keycode,
		key->modifier | CapsLockMask | ScrollLockMask, w, False,
		GrabModeAsync, GrabModeAsync);
      XGrabKey (dpy, key->keycode, key->modifier | NumLockMask | CapsLockMask,
		w, False, GrabModeAsync, GrabModeAsync);
      XGrabKey (dpy, key->keycode,
		key->modifier | NumLockMask | ScrollLockMask, w, False,
		GrabModeAsync, GrabModeAsync);
      XGrabKey (dpy, key->keycode,
		key->modifier | NumLockMask | CapsLockMask | ScrollLockMask,
		w, False, GrabModeAsync, GrabModeAsync);
    }
}

void
ungrabKeys (Display * dpy, Window w)
{
#ifdef DEBUG
  printf ("entering ungrabKeys\n");
#endif

  XUngrabKey (dpy, AnyKey, AnyModifier, w);
}

void
init_keyboard (Display * dpy)
{
  XModifierKeymap *xmk = NULL;
  KeyCode *map;
  int m, k;

#ifdef DEBUG
  printf ("init_keyboard\n");
#endif

  xmk = XGetModifierMapping (dpy);
  if (xmk)
    {
      map = xmk->modifiermap;
      for (m = 0; m < 8; m++)
	for (k = 0; k < xmk->max_keypermod; k++, map++)
	  {
	    if (*map == XKeysymToKeycode (dpy, XK_Num_Lock))
	      NumLockMask = (1 << m);
	    if (*map == XKeysymToKeycode (dpy, XK_Caps_Lock))
	      CapsLockMask = (1 << m);
	    if (*map == XKeysymToKeycode (dpy, XK_Scroll_Lock))
	      ScrollLockMask = (1 << m);
	  }
      XFreeModifiermap (xmk);
    }
}

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: keyboard handling for oroborus-wm                                            *
 ******************************************************************************************/
