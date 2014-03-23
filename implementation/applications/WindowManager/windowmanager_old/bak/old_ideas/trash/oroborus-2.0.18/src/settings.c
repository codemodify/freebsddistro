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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "client.h"
#include "hints.h"
#include "i18n.h"
#include "keyboard.h"
#include "misc.h"
#include "parserc.h"
#include "pixmap.h"
#include "settings.h"

#include "globals.h"

void
loadSettings ()
{
  char *rc[][2] = {
    {"theme", NULL},
    {"scheme", NULL},
    {"font", NULL},
    {"title_alignment", NULL},
    {"full_width_title", NULL},
    {"button_layout", NULL},
    {"button_spacing", NULL},
    {"button_offset", NULL},
    {"active_text_color", NULL},
    {"inactive_text_color", NULL},
    {"active_border_color", NULL},
    {"inactive_border_color", NULL},
    {"active_color_1", NULL},
    {"inactive_color_1", NULL},
    {"active_shadow_1", NULL},
    {"inactive_shadow_1", NULL},
    {"active_hilight_1", NULL},
    {"inactive_hilight_1", NULL},
    {"active_color_2", NULL},
    {"inactive_color_2", NULL},
    {"active_shadow_2", NULL},
    {"inactive_shadow_2", NULL},
    {"active_hilight_2", NULL},
    {"inactive_hilight_2", NULL},
    {"double_click_action", NULL},
    {"box_move", NULL},
    {"box_resize", NULL},
    {"click_to_focus", NULL},
    {"focus_new", NULL},
    {"raise_on_focus", NULL},
    {"raise_delay", NULL},
    {"raise_on_click", NULL},
    {"window_placement", NULL},
    {"snap_to_border", NULL},
    {"snap_to_windows", NULL},
    {"snap_width", NULL},
    {"workspace_count", NULL},
    {"wrap_workspaces", NULL},
    {"cycle_workspaces", NULL},
    {"close_window_key", NULL},
    {"hide_window_key", NULL},
    {"toggle_fullscreen_window_key", NULL},
    {"maximize_window_key", NULL},
    {"maximize_vert_key", NULL},
    {"maximize_horiz_key", NULL},
    {"shade_window_key", NULL},
    {"lower_window_layer_key", NULL},
    {"raise_window_layer_key", NULL},
    {"cycle_windows_key", NULL},
    {"move_window_up_key", NULL},
    {"move_window_down_key", NULL},
    {"move_window_left_key", NULL},
    {"move_window_right_key", NULL},
    {"resize_window_up_key", NULL},
    {"resize_window_down_key", NULL},
    {"resize_window_left_key", NULL},
    {"resize_window_right_key", NULL},
    {"next_workspace_key", NULL},
    {"prev_workspace_key", NULL},
    {"add_workspace_key", NULL},
    {"del_workspace_key", NULL},
    {"stick_window_key", NULL},
    {"workspace_1_key", NULL},
    {"workspace_2_key", NULL},
    {"workspace_3_key", NULL},
    {"workspace_4_key", NULL},
    {"workspace_5_key", NULL},
    {"workspace_6_key", NULL},
    {"workspace_7_key", NULL},
    {"workspace_8_key", NULL},
    {"workspace_9_key", NULL},
    {"move_window_next_workspace_key", NULL},
    {"move_window_prev_workspace_key", NULL},
    {"move_window_workspace_1_key", NULL},
    {"move_window_workspace_2_key", NULL},
    {"move_window_workspace_3_key", NULL},
    {"move_window_workspace_4_key", NULL},
    {"move_window_workspace_5_key", NULL},
    {"move_window_workspace_6_key", NULL},
    {"move_window_workspace_7_key", NULL},
    {"move_window_workspace_8_key", NULL},
    {"move_window_workspace_9_key", NULL},
    {"move_top_left_key", NULL},
    {"move_top_right_key", NULL},
    {"move_bottom_left_key", NULL},
    {"move_bottom_right_key", NULL},
    {"window_ops_modifiers", NULL},
    {"quit_key", NULL},
    {"borderless_windows", NULL},
    {NULL, NULL}
  };
  char *theme, *scheme;
  XpmColorSymbol colsym[16];

#ifdef DEBUG
  printf ("entering settingsLoad\n");
#endif

  if (!parseRc ("defaults", DATADIR, rc))
    {
      fprintf (stderr, "%s: Missing defaults file\n", progname);
      exit (1);
    }
  if (!checkRc (rc))
    {
      fprintf (stderr, "%s: Missing values in defaults file\n", progname);
      exit (1);
    }

  if (rcfile)
    parseRc (rcfile, NULL, rc);
  else
    parseRc (".oroborusrc", getenv ("HOME"), rc);

  scheme = getValue ("scheme", rc);
  parseRc (scheme, NULL, rc);

  theme = getValue ("theme", rc);
  parseRc ("themerc", theme, rc);

  if (rcfile)
    parseRc (rcfile, NULL, rc);
  else
    parseRc (".oroborusrc", getenv ("HOME"), rc);

  if (!getFont (dpy, &title_font, getValue ("font", rc)))
    {
      fprintf (stderr, "%s: Cannot load default font\n", progname);
      exit (1);
    }

  colsym[0].name = "active_text_color";
  colsym[0].value = getValue ("active_text_color", rc);
  colsym[1].name = "inactive_text_color";
  colsym[1].value = getValue ("inactive_text_color", rc);
  colsym[2].name = "active_border_color";
  colsym[2].value = getValue ("active_border_color", rc);
  colsym[3].name = "inactive_border_color";
  colsym[3].value = getValue ("inactive_border_color", rc);
  colsym[4].name = "active_color_1";
  colsym[4].value = getValue ("active_color_1", rc);
  colsym[5].name = "inactive_color_1";
  colsym[5].value = getValue ("inactive_color_1", rc);
  colsym[6].name = "active_shadow_1";
  colsym[6].value = getValue ("active_shadow_1", rc);
  colsym[7].name = "inactive_shadow_1";
  colsym[7].value = getValue ("inactive_shadow_1", rc);
  colsym[8].name = "active_hilight_1";
  colsym[8].value = getValue ("active_hilight_1", rc);
  colsym[9].name = "inactive_hilight_1";
  colsym[9].value = getValue ("inactive_hilight_1", rc);
  colsym[10].name = "active_color_2";
  colsym[10].value = getValue ("active_color_2", rc);
  colsym[11].name = "inactive_color_2";
  colsym[11].value = getValue ("inactive_color_2", rc);
  colsym[12].name = "active_shadow_2";
  colsym[12].value = getValue ("active_shadow_2", rc);
  colsym[13].name = "inactive_shadow_2";
  colsym[13].value = getValue ("inactive_shadow_2", rc);
  colsym[14].name = "active_hilight_2";
  colsym[14].value = getValue ("active_hilight_2", rc);
  colsym[15].name = "inactive_hilight_2";
  colsym[15].value = getValue ("inactive_hilight_2", rc);

  title_gc[ACTIVE] =
    createGC (dpy, cmap, getValue ("active_text_color", rc), GXcopy,
	      title_font.font, False);
  title_gc[INACTIVE] =
    createGC (dpy, cmap, getValue ("inactive_text_color", rc), GXcopy,
	      title_font.font, False);

  loadPixmap (dpy, &sides[SIDE_LEFT][ACTIVE], theme, "left-active.xpm",
	      colsym, 16);
  loadPixmap (dpy, &sides[SIDE_LEFT][INACTIVE], theme, "left-inactive.xpm",
	      colsym, 16);
  loadPixmap (dpy, &sides[SIDE_RIGHT][ACTIVE], theme, "right-active.xpm",
	      colsym, 16);
  loadPixmap (dpy, &sides[SIDE_RIGHT][INACTIVE], theme, "right-inactive.xpm",
	      colsym, 16);
  loadPixmap (dpy, &sides[SIDE_BOTTOM][ACTIVE], theme, "bottom-active.xpm",
	      colsym, 16);
  loadPixmap (dpy, &sides[SIDE_BOTTOM][INACTIVE], theme,
	      "bottom-inactive.xpm", colsym, 16);
  loadPixmap (dpy, &corners[CORNER_TOP_LEFT][ACTIVE], theme,
	      "top-left-active.xpm", colsym, 16);
  loadPixmap (dpy, &corners[CORNER_TOP_LEFT][INACTIVE], theme,
	      "top-left-inactive.xpm", colsym, 16);
  loadPixmap (dpy, &corners[CORNER_TOP_RIGHT][ACTIVE], theme,
	      "top-right-active.xpm", colsym, 16);
  loadPixmap (dpy, &corners[CORNER_TOP_RIGHT][INACTIVE], theme,
	      "top-right-inactive.xpm", colsym, 16);
  loadPixmap (dpy, &corners[CORNER_BOTTOM_LEFT][ACTIVE], theme,
	      "bottom-left-active.xpm", colsym, 16);
  loadPixmap (dpy, &corners[CORNER_BOTTOM_LEFT][INACTIVE], theme,
	      "bottom-left-inactive.xpm", colsym, 16);
  loadPixmap (dpy, &corners[CORNER_BOTTOM_RIGHT][ACTIVE], theme,
	      "bottom-right-active.xpm", colsym, 16);
  loadPixmap (dpy, &corners[CORNER_BOTTOM_RIGHT][INACTIVE], theme,
	      "bottom-right-inactive.xpm", colsym, 16);
  loadPixmap (dpy, &buttons[HIDE_BUTTON][ACTIVE], theme, "hide-active.xpm",
	      colsym, 16);
  loadPixmap (dpy, &buttons[HIDE_BUTTON][INACTIVE], theme,
	      "hide-inactive.xpm", colsym, 16);
  loadPixmap (dpy, &buttons[HIDE_BUTTON][PRESSED], theme, "hide-pressed.xpm",
	      colsym, 16);
  loadPixmap (dpy, &buttons[CLOSE_BUTTON][ACTIVE], theme, "close-active.xpm",
	      colsym, 16);
  loadPixmap (dpy, &buttons[CLOSE_BUTTON][INACTIVE], theme,
	      "close-inactive.xpm", colsym, 16);
  loadPixmap (dpy, &buttons[CLOSE_BUTTON][PRESSED], theme,
	      "close-pressed.xpm", colsym, 16);
  loadPixmap (dpy, &buttons[MAXIMIZE_BUTTON][ACTIVE], theme,
	      "maximize-active.xpm", colsym, 16);
  loadPixmap (dpy, &buttons[MAXIMIZE_BUTTON][INACTIVE], theme,
	      "maximize-inactive.xpm", colsym, 16);
  loadPixmap (dpy, &buttons[MAXIMIZE_BUTTON][PRESSED], theme,
	      "maximize-pressed.xpm", colsym, 16);
  loadPixmap (dpy, &buttons[SHADE_BUTTON][ACTIVE], theme, "shade-active.xpm",
	      colsym, 16);
  loadPixmap (dpy, &buttons[SHADE_BUTTON][INACTIVE], theme,
	      "shade-inactive.xpm", colsym, 16);
  loadPixmap (dpy, &buttons[SHADE_BUTTON][PRESSED], theme,
	      "shade-pressed.xpm", colsym, 16);
  loadPixmap (dpy, &title[TITLE_1][ACTIVE], theme, "title-1-active.xpm",
	      colsym, 16);
  loadPixmap (dpy, &title[TITLE_1][INACTIVE], theme, "title-1-inactive.xpm",
	      colsym, 16);
  loadPixmap (dpy, &title[TITLE_2][ACTIVE], theme, "title-2-active.xpm",
	      colsym, 16);
  loadPixmap (dpy, &title[TITLE_2][INACTIVE], theme, "title-2-inactive.xpm",
	      colsym, 16);
  loadPixmap (dpy, &title[TITLE_3][ACTIVE], theme, "title-3-active.xpm",
	      colsym, 16);
  loadPixmap (dpy, &title[TITLE_3][INACTIVE], theme, "title-3-inactive.xpm",
	      colsym, 16);
  loadPixmap (dpy, &title[TITLE_4][ACTIVE], theme, "title-4-active.xpm",
	      colsym, 16);
  loadPixmap (dpy, &title[TITLE_4][INACTIVE], theme, "title-4-inactive.xpm",
	      colsym, 16);
  loadPixmap (dpy, &title[TITLE_5][ACTIVE], theme, "title-5-active.xpm",
	      colsym, 16);
  loadPixmap (dpy, &title[TITLE_5][INACTIVE], theme, "title-5-inactive.xpm",
	      colsym, 16);

  if (!strcmp ("left", getValue ("title_alignment", rc)))
    title_alignment = ALIGN_LEFT;
  else if (!strcmp ("right", getValue ("title_alignment", rc)))
    title_alignment = ALIGN_RIGHT;
  else
    title_alignment = ALIGN_CENTER;
  full_width_title = !strcmp ("true", getValue ("full_width_title", rc));

  strncpy (button_layout, getValue ("button_layout", rc), 5);
  button_spacing = atoi (getValue ("button_spacing", rc));
  button_offset = atoi (getValue ("button_offset", rc));

  box_gc = createGC (dpy, cmap, "#FFFFFF", GXxor, title_font.font, True);
  box_resize = !strcmp ("true", getValue ("box_resize", rc));
  box_move = !strcmp ("true", getValue ("box_move", rc));

  click_to_focus = !strcmp ("true", getValue ("click_to_focus", rc));
  focus_new = !strcmp ("true", getValue ("focus_new", rc));
  raise_on_focus = !strcmp ("true", getValue ("raise_on_focus", rc));
  raise_delay = abs (atoi (getValue ("raise_delay", rc)));
  raise_on_click = !strcmp ("true", getValue ("raise_on_click", rc));

  if (!strcmp ("center_root", getValue ("window_placement", rc)))
    window_placement = PLACEMENT_ROOT;
  else
    window_placement = PLACEMENT_MOUSE;
  snap_to_border = !strcmp ("true", getValue ("snap_to_border", rc));
  snap_width = abs (atoi (getValue ("snap_width", rc)));
  snap_to_windows = !strcmp ("true", getValue ("snap_to_windows", rc));

  if (!strcmp ("shade", getValue ("double_click_action", rc)))
    double_click_action = ACTION_SHADE;
  else if (!strcmp ("hide", getValue ("double_click_action", rc)))
    double_click_action = ACTION_HIDE;
  else if (!strcmp ("maximize", getValue ("double_click_action", rc)))
    double_click_action = ACTION_MAXIMIZE;
  else
    double_click_action = ACTION_NONE;

  if (workspace_count < 0)
    {
      workspace_count = abs (atoi (getValue ("workspace_count", rc)));
      setGnomeHint (root, win_workspace_count, workspace_count);
      setGnomeHint (root, net_atoms[NET_NUMBER_OF_DESKTOPS], workspace_count);
    }
  wrap_workspaces = !strcmp ("true", getValue ("wrap_workspaces", rc));

  cycle_workspaces = !strcmp ("true", getValue ("cycle_workspaces", rc));

  parseKeyString (dpy, &keys[KEY_MOVE_UP],
		  getValue ("move_window_up_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_DOWN],
		  getValue ("move_window_down_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_LEFT],
		  getValue ("move_window_left_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_RIGHT],
		  getValue ("move_window_right_key", rc));
  parseKeyString (dpy, &keys[KEY_RESIZE_UP],
		  getValue ("resize_window_up_key", rc));
  parseKeyString (dpy, &keys[KEY_RESIZE_DOWN],
		  getValue ("resize_window_down_key", rc));
  parseKeyString (dpy, &keys[KEY_RESIZE_LEFT],
		  getValue ("resize_window_left_key", rc));
  parseKeyString (dpy, &keys[KEY_RESIZE_RIGHT],
		  getValue ("resize_window_right_key", rc));
  parseKeyString (dpy, &keys[KEY_CYCLE_WINDOWS],
		  getValue ("cycle_windows_key", rc));
  parseKeyString (dpy, &keys[KEY_CLOSE_WINDOW],
		  getValue ("close_window_key", rc));
  parseKeyString (dpy, &keys[KEY_HIDE_WINDOW],
		  getValue ("hide_window_key", rc));
  parseKeyString (dpy, &keys[KEY_TOGGLE_FULLSCREEN_WINDOW],
		  getValue ("toggle_fullscreen_window_key", rc));
  parseKeyString (dpy, &keys[KEY_MAXIMIZE_WINDOW],
		  getValue ("maximize_window_key", rc));
  parseKeyString (dpy, &keys[KEY_MAXIMIZE_VERT],
		  getValue ("maximize_vert_key", rc));
  parseKeyString (dpy, &keys[KEY_MAXIMIZE_HORIZ],
		  getValue ("maximize_horiz_key", rc));
  parseKeyString (dpy, &keys[KEY_SHADE_WINDOW],
		  getValue ("shade_window_key", rc));
  parseKeyString (dpy, &keys[KEY_LOWER_WINDOW_LAYER],
		  getValue ("lower_window_layer_key", rc));
  parseKeyString (dpy, &keys[KEY_RAISE_WINDOW_LAYER],
		  getValue ("raise_window_layer_key", rc));
  parseKeyString (dpy, &keys[KEY_NEXT_WORKSPACE],
		  getValue ("next_workspace_key", rc));
  parseKeyString (dpy, &keys[KEY_PREV_WORKSPACE],
		  getValue ("prev_workspace_key", rc));
  parseKeyString (dpy, &keys[KEY_ADD_WORKSPACE],
		  getValue ("add_workspace_key", rc));
  parseKeyString (dpy, &keys[KEY_DEL_WORKSPACE],
		  getValue ("del_workspace_key", rc));
  parseKeyString (dpy, &keys[KEY_STICK_WINDOW],
		  getValue ("stick_window_key", rc));
  parseKeyString (dpy, &keys[KEY_WORKSPACE_1],
		  getValue ("workspace_1_key", rc));
  parseKeyString (dpy, &keys[KEY_WORKSPACE_2],
		  getValue ("workspace_2_key", rc));
  parseKeyString (dpy, &keys[KEY_WORKSPACE_3],
		  getValue ("workspace_3_key", rc));
  parseKeyString (dpy, &keys[KEY_WORKSPACE_4],
		  getValue ("workspace_4_key", rc));
  parseKeyString (dpy, &keys[KEY_WORKSPACE_5],
		  getValue ("workspace_5_key", rc));
  parseKeyString (dpy, &keys[KEY_WORKSPACE_6],
		  getValue ("workspace_6_key", rc));
  parseKeyString (dpy, &keys[KEY_WORKSPACE_7],
		  getValue ("workspace_7_key", rc));
  parseKeyString (dpy, &keys[KEY_WORKSPACE_8],
		  getValue ("workspace_8_key", rc));
  parseKeyString (dpy, &keys[KEY_WORKSPACE_9],
		  getValue ("workspace_9_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_NEXT_WORKSPACE],
		  getValue ("move_window_next_workspace_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_PREV_WORKSPACE],
		  getValue ("move_window_prev_workspace_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_WORKSPACE_1],
		  getValue ("move_window_workspace_1_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_WORKSPACE_2],
		  getValue ("move_window_workspace_2_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_WORKSPACE_3],
		  getValue ("move_window_workspace_3_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_WORKSPACE_4],
		  getValue ("move_window_workspace_4_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_WORKSPACE_5],
		  getValue ("move_window_workspace_5_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_WORKSPACE_6],
		  getValue ("move_window_workspace_6_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_WORKSPACE_7],
		  getValue ("move_window_workspace_7_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_WORKSPACE_8],
		  getValue ("move_window_workspace_8_key", rc));
  parseKeyString (dpy, &keys[KEY_MOVE_WORKSPACE_9],
		  getValue ("move_window_workspace_9_key", rc));
  parseKeyString (dpy, &keys[KEY_WINDOW_OPS],
		  getValue ("window_ops_modifiers", rc));
  parseKeyString (dpy, &keys[KEY_TOP_LEFT],
		  getValue ("move_top_left_key", rc));
  parseKeyString (dpy, &keys[KEY_TOP_RIGHT],
		  getValue ("move_top_right_key", rc));
  parseKeyString (dpy, &keys[KEY_BOTTOM_LEFT],
		  getValue ("move_bottom_left_key", rc));
  parseKeyString (dpy, &keys[KEY_BOTTOM_RIGHT],
		  getValue ("move_bottom_right_key", rc));
  parseKeyString (dpy, &keys[KEY_QUIT], getValue ("quit_key", rc));

  borderless_windows =
    malloc (sizeof (char) *
	    (strlen (getValue ("borderless_windows", rc)) + 1));
  snprintf (borderless_windows,
	    sizeof (char) * (strlen (getValue ("borderless_windows", rc)) +
			     1), "%s", getValue ("borderless_windows", rc));

  ungrabKeys (dpy, gnome_win);
  grabKey (dpy, &keys[KEY_CYCLE_WINDOWS], gnome_win);
  grabKey (dpy, &keys[KEY_NEXT_WORKSPACE], gnome_win);
  grabKey (dpy, &keys[KEY_PREV_WORKSPACE], gnome_win);
  grabKey (dpy, &keys[KEY_ADD_WORKSPACE], gnome_win);
  grabKey (dpy, &keys[KEY_NEXT_WORKSPACE], gnome_win);
  grabKey (dpy, &keys[KEY_WORKSPACE_1], gnome_win);
  grabKey (dpy, &keys[KEY_WORKSPACE_2], gnome_win);
  grabKey (dpy, &keys[KEY_WORKSPACE_3], gnome_win);
  grabKey (dpy, &keys[KEY_WORKSPACE_4], gnome_win);
  grabKey (dpy, &keys[KEY_WORKSPACE_5], gnome_win);
  grabKey (dpy, &keys[KEY_WORKSPACE_6], gnome_win);
  grabKey (dpy, &keys[KEY_WORKSPACE_7], gnome_win);
  grabKey (dpy, &keys[KEY_WORKSPACE_8], gnome_win);
  grabKey (dpy, &keys[KEY_WORKSPACE_9], gnome_win);
  grabKey (dpy, &keys[KEY_QUIT], gnome_win);
  freeRc (rc);
}

void
unloadSettings ()
{
  int i;
#ifdef DEBUG
  printf ("entering unloadSettings\n");
#endif
  for (i = 0; i < 3; i++)
    {
      freePixmap (dpy, &sides[i][ACTIVE]);
      freePixmap (dpy, &sides[i][INACTIVE]);
    }
  for (i = 0; i < 4; i++)
    {
      freePixmap (dpy, &corners[i][ACTIVE]);
      freePixmap (dpy, &corners[i][INACTIVE]);
      freePixmap (dpy, &buttons[i][ACTIVE]);
      freePixmap (dpy, &buttons[i][INACTIVE]);
      freePixmap (dpy, &buttons[i][PRESSED]);
    }
  for (i = 0; i < 5; i++)
    {
      freePixmap (dpy, &title[i][ACTIVE]);
      freePixmap (dpy, &title[i][INACTIVE]);
    }
  XFreeGC (dpy, title_gc[ACTIVE]);
  XFreeGC (dpy, title_gc[INACTIVE]);
  XFreeGC (dpy, box_gc);
}

void
reloadSettings ()
{
  client_t *c;
  int i;
  XWindowChanges wc;
#ifdef DEBUG
  printf ("entering reloadSettings\n");
#endif
  for (c = clients, i = 0; i < client_count; c = c->next, i++)
    {
      clientGravitate (c, REMOVE);
      clientUngrabKeys (c);
    }
  unloadSettings ();
  loadSettings ();
  for (c = clients, i = 0; i < client_count; c = c->next, i++)
    {
      clientGravitate (c, APPLY);
      wc.x = c->x;
      wc.y = c->y;
      wc.width = c->width;
      wc.height = c->height;
      clientConfigure (c, &wc, CWX | CWY | CWWidth | CWHeight);
      clientGrabKeys (c);
    }
}

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 * arch-tag: settings handling for oroborus-wm                                            *
 ******************************************************************************************/
