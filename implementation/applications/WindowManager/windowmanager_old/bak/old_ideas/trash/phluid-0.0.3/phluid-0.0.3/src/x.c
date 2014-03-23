#include "phluid.h"
#include "debug.h"

void
init_x(void)
{
  char *fg_str = "white";
  char *bg_str = "slategrey";
  char *bd_str = "black";

  disp = XOpenDisplay(NULL);
  root = DefaultRootWindow(disp);
  screen = DefaultScreen(disp);
  vis = DefaultVisual(disp, screen);
  depth = DefaultDepth(disp, screen);
  cm = DefaultColormap(disp, screen);

  XAllocNamedColor(disp, cm, fg_str, &fg, &dummyc);
  XAllocNamedColor(disp, cm, bg_str, &bg, &dummyc);
  XAllocNamedColor(disp, cm, bd_str, &bd, &dummyc);

  XSelectInput(disp, root, SubstructureRedirectMask|SubstructureNotifyMask|ButtonPressMask);
}

void
init_imlib(void)
{
  bg_image = imlib_load_image("/home/matt/gradient.jpg");

  /* imlib settings */
  imlib_set_cache_size(2048 * 1024);
  imlib_set_color_usage(128);
  imlib_context_set_dither(1);
  imlib_context_set_blend(1);
  imlib_context_set_display(disp);
  imlib_context_set_visual(vis);
  imlib_context_set_colormap(cm);
}

Window x_get_top_window(void)
{
  Window dummy, *children, top;
  unsigned int n;
  XQueryTree(disp, root, &dummy, &dummy, &children, &n);
  top = children[n - 1];
  XFree(children);
  return top;
}

int
handle_xerror(Display * disp, XErrorEvent * e)
{
  if (e->error_code == BadWindow) {
    /* Probably got an unmap event and called delete_client, but
       the window got destroyed before we could do anything about it.
       Just ignore this case.  I'll print out a debug message anyway. */
    DEBUG_PRINT("*** got a BadWindow error in remove_client().  ");
    DEBUG_PRINT("probably safe to ignore ***\n");
  }
  return 0;
}
