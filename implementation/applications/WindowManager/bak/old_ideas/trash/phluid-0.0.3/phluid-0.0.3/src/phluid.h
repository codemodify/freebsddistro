/*
 * phluid.h
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmd.h>
#include <Imlib2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "decal.h"

/* for convenience */
#define PH_DELETE_WIN(c) icccm_send_client_message(c, XInternAtom( \
  disp, "WM_DELETE_WINDOW", False));


/* some constants */
#define TITLEBAR_HEIGHT 17

#define TEXT_X 25
#define TEXT_Y 1

#define MOVE_AFTER_DRAG 0
#define SEND_CONFIGURE_AFTER_DRAG 1

/* client structure */
typedef struct _Client Client;

struct _Client
{
  Window window, frame;
  int x, y, width, height;
  char *name;
  int name_change;
  int name_width, name_height;
  Client *next;
  Imlib_Image bg;
  Imlib_Image title_bar;
  Imlib_Updates updates;
  unsigned int ignore_unmap;
  int shaded;
  int visible;
  Decal *decals;
  Decal *decals_to_render;
};

/* phluid.c */
extern Display *disp;
extern Window root;
extern Visual *vis;
extern Colormap cm;
extern Imlib_Image bg_image;
extern Client *client_list_head;
extern XColor fg, bg, bd, dummyc;
extern int depth;
extern int screen;
extern int bw;

/* client.c */
extern void make_client(Window * w);
extern void render_title_bar(Client * c);
extern void delete_client(Client * c);
extern void redraw_client(Client * c);
extern Client *find_client_by_frame(Window w);
extern Client *find_client_by_child(Window w);

/* decal.c */
extern void add_new_decal_to_client(Client * c, char *file, int x, int y,
                                    int fn1, int fn2, int fn3);
extern Decal *decal_at(Client * c, int x, int y);
extern void do_decal_fn(Client * c, Decal * d, int button);
extern void render_decals(Client *);
extern void clear_decals(Decal *);

/* mouse.c */
extern void get_mouse_x_y(int *, int *);

/* manipulate.c */
extern void drag_client(Client *, int, int);
extern void shade_client(Client *);
extern void unshade_client(Client *);
extern void iconify_client(Client *);

/* x.c */
extern void init_x(void);
extern void init_imlib(void);
extern Window x_get_top_window(void);
extern int handle_xerror(Display *, XErrorEvent *);

/* icccm.c */
extern void icccm_send_client_message(Client *, Atom);
extern void icccm_send_configure_event(Client *);
extern void icccm_set_state(Client *, int);

/* signal.c */
void init_signal(void);

/* event_handlers.c */
extern void button_press(XEvent);
