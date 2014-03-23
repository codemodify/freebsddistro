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

#ifndef __I18N_H
#define __I18N_H

typedef struct
{
  XFontStruct *font;
  XFontSet fontset;
  int height;
  int ascent;
  int descent;
}
MyFont;

extern int use_fontset;
int getFont (Display *, MyFont *, char *);
void freeFont (Display *, MyFont *);
void drawString (Display *, Drawable, MyFont *, GC, int, int, char *);
int getTextWidth (char *, MyFont *);
void getWindowName (Display *, Window, char **);

#endif /* __I18N_H */

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: i18n header for oroborus-wm                                                  *
 ******************************************************************************************/
