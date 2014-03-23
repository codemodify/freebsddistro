#include "phluid.h"
#include "debug.h"

void
button_press(XEvent ev)
{
  Client *c;
  int mouse_x, mouse_y;
  DEBUG_PRINT("got ButtonPress\n");
  get_mouse_x_y(&mouse_x, &mouse_y);
  if(ev.xbutton.window == root) {
    switch(ev.xbutton.button) {
      case Button2:
        system("gtk-switch &");
        break;
      default:
        system("gtk-panel &");
        break;
    }
  }
  c = find_client_by_frame(ev.xbutton.window);
  if (c) {
    Decal *decal;
    XEvent peek;

    decal = decal_at(c, ev.xbutton.x, ev.xbutton.y);
    XPeekEvent(disp, &peek);

    if (decal) {
      Decal *tmp_decal;
      XEvent tmp_ev;
      XMaskEvent(disp, ButtonReleaseMask, &tmp_ev);
      tmp_decal = decal_at(c, tmp_ev.xbutton.x, tmp_ev.xbutton.y);
      if(tmp_decal == decal)
        do_decal_fn(c, decal, ev.xbutton.button);
      return;
    }

    if (ev.xbutton.button == Button2) {
      shade_client(c);
      XSync(disp, False);
      icccm_send_configure_event(c);
      return;
    }

    if (ev.xbutton.button == Button3) {
      return;
    }

    /* the user let go immediately, so treat as click not drag */
    if (peek.type == ButtonRelease) {
      Window top;
      Client *tmp;
      char *name;

      top = x_get_top_window();
      XFetchName(disp, top, &name);
      DEBUG_VAR("top window: %s\n", name);
      XFree(name);

      tmp = find_client_by_frame(top);
      if (!tmp)
        DEBUG_PRINT("top window not found as frame\n");

      if (c->frame == top) {
        DEBUG_PRINT("client is on top\n");
        XLowerWindow(disp, c->frame);
      } else {
        DEBUG_PRINT("client is NOT on top\n");
        XRaiseWindow(disp, c->frame);
      }
    }

    /* motion before release detected, so drag and move */
    if (peek.type == MotionNotify) {
      drag_client(c, mouse_x, mouse_y);
      if (MOVE_AFTER_DRAG) {
        XMoveWindow(disp, c->frame, c->x, c->y);
      }
      XSync(disp,False);
      if (SEND_CONFIGURE_AFTER_DRAG) {
        icccm_send_configure_event(c);
      }
    }
  }
}
