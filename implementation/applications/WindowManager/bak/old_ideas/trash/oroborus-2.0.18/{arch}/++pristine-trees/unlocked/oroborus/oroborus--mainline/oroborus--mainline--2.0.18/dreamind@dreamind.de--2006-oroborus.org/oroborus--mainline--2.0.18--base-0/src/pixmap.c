/* 
 * Oroborus Window Manager
 * Pixmap Utility Library
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
#include <X11/xpm.h>
#include <stdlib.h>
#include <stdio.h>

#include "pixmap.h"

int
loadPixmap (Display * dpy, MyPixmap * pm, char *dir, char *file,
	    XpmColorSymbol * cs, int n)
{
  char filename[512];
  XpmAttributes attr;

#ifdef DEBUG
  printf ("entering loadPixmap\n");
#endif

  pm->pixmap = None;
  pm->mask = None;
  pm->width = 1;
  pm->height = 1;
  snprintf (filename, sizeof (filename), "%s/%s", dir, file);
  attr.colorsymbols = cs;
  attr.numsymbols = n;
  attr.valuemask = XpmSize;
  if (n > 0 && cs)
    attr.valuemask = attr.valuemask | XpmColorSymbols;
  if (XpmReadFileToPixmap
      (dpy, XDefaultRootWindow (dpy), filename, &pm->pixmap, &pm->mask,
       &attr))
    return False;
  pm->width = attr.width;
  pm->height = attr.height;
  XpmFreeAttributes (&attr);
  return True;
}

void
freePixmap (Display * dpy, MyPixmap * pm)
{
#ifdef DEBUG
  printf ("entering freePixmap\n");
#endif

  if (pm->pixmap != None)
    XFreePixmap (dpy, pm->pixmap);
  if (pm->mask != None)
    XFreePixmap (dpy, pm->mask);
}

void
scalePixmap (Display * dpy, MyPixmap * src, MyPixmap * dst, int width,
	     int height)
{
  XpmImage xi_src, xi_dst;
  int x, y, sx, sy;
  unsigned int *src_data, *dst_data;

#ifdef DEBUG
  printf ("entering scalePixmap\n");
#endif

  /* don't ask me why but sometimes this function gets _REALLY_ big imagesizes
   * so there must be something totally wrong somewhere... */
  if (width > 20000)
    return;
  if (height > 20000)
    return;
  if (width < 1)
    return;
  if (height < 1)
    return;
  /* I currently don't know exactly but it _IS_ a bug in here */

  XpmCreateXpmImageFromPixmap (dpy, src->pixmap, src->mask, &xi_src, NULL);
  dst->width = width;
  dst->height = height;
  xi_dst.width = width;
  xi_dst.height = height;
  xi_dst.cpp = xi_src.cpp;
  xi_dst.ncolors = xi_src.ncolors;
  xi_dst.colorTable = xi_src.colorTable;
  xi_dst.data = malloc (sizeof (int) * (xi_dst.width * xi_dst.height));
  dst_data = xi_dst.data;
  src_data = xi_src.data;
#ifdef DEBUG
  printf ("xi_dst.width %i\n", xi_dst.width);
  printf ("xi_dst.height %i\n", xi_dst.height);
  printf ("xi_src.width %i\n", xi_src.width);
  printf ("xi_src.height %i\n", xi_src.height);
#endif
  for (y = 0; y < xi_dst.height; y++)
    {
      dst_data = xi_dst.data + (y * xi_dst.width);
      for (x = 0; x < xi_dst.width; x++)
	{
	  sx = (x * xi_src.width) / xi_dst.width;
	  sy = (y * xi_src.height) / xi_dst.height;
	  *dst_data = *(src_data + sx + (sy * xi_src.width));
	  dst_data++;
	}
    }
  XpmCreatePixmapFromXpmImage (dpy, DefaultRootWindow (dpy), &xi_dst,
			       &dst->pixmap, &dst->mask, NULL);
  free (xi_dst.data);
  XpmFreeXpmImage (&xi_src);
}

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: pixmap handling for oroborus-wm                                              *
 ******************************************************************************************/
