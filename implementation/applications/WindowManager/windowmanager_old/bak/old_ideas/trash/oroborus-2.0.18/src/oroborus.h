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

/** \file oroborus.h
 * This file contains all definitions of the main function set used by oroborus.
 *
 * For a more exact description, look at each functions definition. */

#ifndef __OROBORUS_H
#define __OROBORUS_H

/*
 * getopt_long return codes
 */
enum
{
  SM_ID = 130
};

/* the structure of all long options for getopt */
static struct option const long_options[] = {
  {"verbose", no_argument, 0, 'v'},
  {"help", no_argument, 0, 'h'},
  {"version", no_argument, 0, 'V'},
  {"file", required_argument, 0, 'r'},
  {"display", required_argument, 0, 'd'},
  {"sm-client-id", required_argument, 0, SM_ID},
  {NULL, 0, NULL, 0}
};

/*
 * Function prototypes
 */

void cleanUp ();

#endif /* __OROBORUS_H */

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: main header file of oroborus-wm                                              *
 ******************************************************************************************/
