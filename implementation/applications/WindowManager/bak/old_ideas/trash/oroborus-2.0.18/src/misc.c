/* 
 * Oroborus Window Manager
 * Miscellaneous Utility Functions
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
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "misc.h"

#ifdef DEBUG
#include <stdio.h>
#endif

void
getMouseXY (Display * dpy, Window w, int *x2, int *y2)
{
  Window w1, w2;
  int x1, y1;
  unsigned int m;

#ifdef DEBUG
  printf ("entering getMouseXY\n");
#endif

  XQueryPointer (dpy, w, &w1, &w2, &x1, &y1, x2, y2, &m);
}

Window
getMouseWindow (Display * dpy, Window w)
{
  Window w1, w2;
  int x1, y1, x2, y2;
  unsigned int m;

#ifdef DEBUG
  printf ("entering getMouseWindow\n");
#endif

  XQueryPointer (dpy, w, &w1, &w2, &x1, &y1, &x2, &y2, &m);
  return w2;
}

GC
createGC (Display * dpy, Colormap cmap, char *col, int func,
	  XFontStruct * font, int inc_sw)
{
  XGCValues gv;
  XColor xc1, xc2;
  GC gc;
  int mask;

#ifdef DEBUG
  printf ("entering createGC\n");
  printf ("color=%s\n", col);
#endif

  mask = GCForeground | GCFunction;
  XAllocNamedColor (dpy, cmap, col, &xc1, &xc2);
  gv.foreground = xc2.pixel;
  gv.function = func;
  if (font)
    {
      gv.font = font->fid;
      mask = mask | GCFont;
    }
  if (inc_sw)
    {
      gv.subwindow_mode = IncludeInferiors;
      mask = mask | GCSubwindowMode;
    }
  gc = XCreateGC (dpy, XDefaultRootWindow (dpy), mask, &gv);
  return gc;
}

void
sendClientMessage (Display * dpy, Window w, Atom a, long x, int mask)
{
  XEvent ev;

#ifdef DEBUG
  printf ("entering sendClientMessage\n");
#endif

  ev.type = ClientMessage;
  ev.xclient.window = w;
  ev.xclient.message_type = a;
  ev.xclient.format = 32;
  ev.xclient.data.l[0] = x;
  ev.xclient.data.l[1] = CurrentTime;
  XSendEvent (dpy, w, False, mask, &ev);
}

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: misc stuff for oroborus-wm                                                   *
 ******************************************************************************************/
