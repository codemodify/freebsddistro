/* 
 * Oroborus Window Manager
 * Rc File Parsing Library
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
#include <stdio.h>
#include <string.h>
#include "parserc.h"

int
parseRc (char *file, char *dir, char *rc[][2])
{
  char filename[512], buf[512], *lvalue, *rvalue;
  FILE *fp;
  int i;

#ifdef DEBUG
  printf ("entering parseRc\n");
#endif

  if (dir)
    snprintf (filename, sizeof (filename), "%s/%s", dir, file);
  else
    strncpy (filename, file, sizeof (filename));
  fp = fopen (filename, "r");
  if (!fp)
    return False;

  while (fgets (buf, sizeof (buf), fp))
    {
      lvalue = strtok (buf, "=");
      if (lvalue)
	{
	  for (i = 0; rc[i][OPTION]; i++)
	    {
	      if (!strcmp (lvalue, rc[i][OPTION]))
		{
		  rvalue = strtok (NULL, "\n");
		  if (rvalue)
		    {
		      if (rc[i][VALUE])
			free (rc[i][VALUE]);
		      rc[i][VALUE] = strdup (rvalue);
#ifdef DEBUG
		      printf ("%s=%s\n", rc[i][OPTION], rc[i][VALUE]);
#endif
		    }
		}
	    }
	}
    }
  fclose (fp);
  return True;
}

int
checkRc (char *rc[][2])
{
  int i, rval = True;

#ifdef DEBUG
  printf ("entering checkRc\n");
#endif

  for (i = 0; rc[i][OPTION]; i++)
    {
      if (!rc[i][VALUE])
	{
	  fprintf (stderr, "missing value for option %s\n", rc[i][OPTION]);
	  rval = False;
	}
    }
  return rval;
}

char *
getValue (char *option, char *rc[][2])
{
  int i;

#ifdef DEBUG
  printf ("entering getValue\n");
#endif

  for (i = 0; rc[i][OPTION]; i++)
    if (!strcmp (option, rc[i][OPTION]))
      break;
  return rc[i][VALUE];
}

void
freeRc (char *rc[][2])
{
  int i;

#ifdef DEBUG
  printf ("entering freeRc\n");
#endif

  for (i = 0; rc[i][OPTION]; i++)
    free (rc[i][VALUE]);
}

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: configuration parsing for oroborus-wm                                        *
 ******************************************************************************************/
