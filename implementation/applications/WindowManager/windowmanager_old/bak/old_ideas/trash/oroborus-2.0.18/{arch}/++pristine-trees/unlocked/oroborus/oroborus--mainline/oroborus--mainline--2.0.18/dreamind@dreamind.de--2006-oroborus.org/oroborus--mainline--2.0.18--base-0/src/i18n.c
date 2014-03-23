/* 
 * Oroborus Window Manager
 * X11 I18N Support Library
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
#include <X11/Xatom.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "i18n.h"

int use_fontset;

int
getFont (Display * dpy, MyFont * font, char *name)
{
  char **missing_list;
  int missing_count;
  char *def_string;
  XFontStruct **fonts;
  char **font_names;
  int i, ascent, descent, fnum;
  char *basename;

#ifdef DEBUG
  printf ("entering getFont\n");
  printf ("font=%s\n", name);
#endif

  font->font = NULL;
  font->fontset = NULL;

  if (use_fontset)
    {
      basename = malloc (strlen (name) + 3);
      if (basename)
	sprintf (basename, "%s,*", name);
      else
	basename = name;

      font->fontset =
	XCreateFontSet (dpy, basename, &missing_list, &missing_count,
			&def_string);
      if (!font->fontset)
	{
	  if (basename != name)
	    free (basename);
	  return False;
	}

      if (basename != name)
	free (basename);

      for (i = 0; i < missing_count; i++)
	fprintf (stderr, "Warning! Missing charset %s\n", missing_list[i]);

      fnum = XFontsOfFontSet (font->fontset, &fonts, &font_names);
      for (i = 0, ascent = 0, descent = 0; i < fnum; i++)
	{
	  if (ascent < fonts[i]->ascent)
	    ascent = fonts[i]->ascent;
	  if (descent < fonts[i]->descent)
	    descent = fonts[i]->descent;
	}

      font->height = ascent + descent;
      font->ascent = ascent;
      font->descent = descent;
    }
  else
    {
      font->font = XLoadQueryFont (dpy, name);
      if (!font->font)
	{
	  font->font = XLoadQueryFont (dpy, "fixed");
	  if (!font->font)
	    return False;
	}

      font->height = font->font->ascent + font->font->descent;
      font->ascent = font->font->ascent;
      font->descent = font->font->descent;
    }

  return True;
}

void
freeFont (Display * dpy, MyFont * font)
{
#ifdef DEBUG
  printf ("entering freeFont\n");
#endif

  if (use_fontset)
    XFreeFontSet (dpy, font->fontset);
  else
    XFreeFont (dpy, font->font);
}

void
drawString (Display * dpy, Drawable d, MyFont * font, GC gc, int x, int y,
	    char *str)
{
#ifdef DEBUG
  printf ("entering drawString\n");
#endif

  if (!str)
    return;

  if (use_fontset)
    XmbDrawString (dpy, d, font->fontset, gc, x, y, str, strlen (str));
  else
    XDrawString (dpy, d, gc, x, y, str, strlen (str));
}

int
getTextWidth (char *str, MyFont * font)
{
  int width;
  XRectangle ink, logical;

#ifdef DEBUG
  printf ("entering getTextWidth\n");
#endif

  if (!str)
    return 0;

  if (use_fontset)
    {
      XmbTextExtents (font->fontset, str, strlen (str), &ink, &logical);
      width = logical.width;
    }
  else
    width = XTextWidth (font->font, str, strlen (str));

  return width;
}

void
getWindowName (Display * dpy, Window w, char **name)
{
  int status, num;
  char **list;
  XTextProperty text_prop;

#ifdef DEBUG
  printf ("entering getWindowName\n");
#endif

  *name = NULL;
  status = XGetWMName (dpy, w, &text_prop);
  if (!status || !text_prop.value || !text_prop.nitems)
    return;
  if (text_prop.encoding == XA_STRING)
    {
      if (text_prop.value)
	{
	  *name = strdup ((char *)text_prop.value);
	  XFree (text_prop.value);
	}
    }
  else
    {
      XmbTextPropertyToTextList (dpy, &text_prop, &list, &num);
      if (num && *list)
	{
	  XFree (text_prop.value);
	  *name = strdup (*list);
	  XFreeStringList (list);
	}
    }
  return;
}

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: i18n handling for oroborus-wm                                                *
 ******************************************************************************************/
