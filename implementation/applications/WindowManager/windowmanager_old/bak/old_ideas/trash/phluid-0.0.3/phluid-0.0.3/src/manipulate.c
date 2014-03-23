#include "phluid.h"
#include "debug.h"

void
drag_client(Client * c, int mouse_x, int mouse_y)
{
  XEvent ev;
  Client *exposed;
  int startx, starty;

  startx = c->x;
  starty = c->y;

  /* XGrabServer(disp); */
#ifdef DEBUG
  fprintf(stdout, "in drag_client: %d,%d\n", mouse_x, mouse_y);
#endif

  for (;;) {
    XMaskEvent(disp, ButtonReleaseMask | PointerMotionMask | ExposureMask, &ev);
    switch (ev.type) {
      case MotionNotify:
#ifdef DEBUG
        fprintf(stdout, "got MotionNotify: %d,%d\n", ev.xmotion.x_root,
                ev.xmotion.y_root);
#endif
        c->x = startx + (ev.xmotion.x_root - mouse_x);
        c->y = starty + (ev.xmotion.y_root - mouse_y);
#ifdef DEBUG
        fprintf(stdout, "c->x, c->y: %d,%d\n", c->x, c->y);
#endif

        if (!MOVE_AFTER_DRAG) {
          XMoveWindow(disp, c->frame, c->x, c->y);
          XFlush(disp);
          if (!SEND_CONFIGURE_AFTER_DRAG)
            icccm_send_configure_event(c);
        }
        break;
      case ButtonRelease:
        /* XUngrabServer(disp); */
        return;
      case Expose:
        exposed = find_client_by_frame(ev.xexpose.window);
        if (exposed) {
          exposed->updates =
              imlib_update_append_rect(exposed->updates, ev.xexpose.x,
                                       ev.xexpose.y, ev.xexpose.width,
                                       ev.xexpose.height);
          redraw_client(exposed);
        }
        break;
    }
  }
}

void
shade_client(Client * c)
{
  if(!c->shaded) {
    c->shaded = 1;
    XResizeWindow(disp, c->frame, c->width, TITLEBAR_HEIGHT);
    c->ignore_unmap++;
    XUnmapWindow(disp, c->window);
    icccm_set_state(c, IconicState);
  } else {
    c->shaded = 0;
    XMapWindow(disp, c->window);
    XResizeWindow(disp, c->frame, c->width, c->height + TITLEBAR_HEIGHT);
    icccm_set_state(c, NormalState);
  }
}

void
iconify_client(Client * c)
{
  if(c->visible) {
    c->visible = 0;
    c->ignore_unmap += 2;
    XUnmapWindow(disp, c->frame);
    XUnmapWindow(disp, c->window);
    icccm_set_state(c, IconicState);
  } else {
    c->visible = 1;
    XMapWindow(disp, c->window);
    XMapWindow(disp, c->frame);
    icccm_set_state(c, NormalState);
  }
}
