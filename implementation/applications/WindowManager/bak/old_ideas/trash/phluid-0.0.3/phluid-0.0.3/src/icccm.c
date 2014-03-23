#include "phluid.h"
#include "debug.h"

void
icccm_send_client_message(Client *c, Atom msg) {
  int i, n;
  Atom *protocols;
  if (XGetWMProtocols(disp, c->window, &protocols, &n)) {
    for (i = 0; i < n; i++)
      if (protocols[i] == msg) {
        XEvent xcm;

        xcm.type = ClientMessage;
        xcm.xclient.window = c->window;
        xcm.xclient.message_type = XInternAtom(disp, "WM_PROTOCOLS", False);
        xcm.xclient.format = 32;
        xcm.xclient.data.l[0] = msg;
        xcm.xclient.data.l[1] = CurrentTime;

        XSendEvent(disp, c->window, False, NoEventMask, &xcm);
      }
    XFree(protocols);
  }
}

void
icccm_send_configure_event(Client * c)
{
  XConfigureEvent ce;
  ce.type = ConfigureNotify;
  ce.event = c->window;
  ce.window = c->window;
  ce.x = c->x;
  ce.y = c->y + TITLEBAR_HEIGHT;
  ce.width = c->width;
  ce.height = c->height;
  ce.border_width = 0;
  ce.above = None;
  ce.override_redirect = 0;
  XSendEvent(disp, c->window, False, StructureNotifyMask, (XEvent *) & ce);
}

void icccm_set_state(Client *c, int state) {
  CARD32 data[2];

  data[0] = state;
  data[1] = None;

  XChangeProperty(disp, c->window, XInternAtom(disp, "WM_STATE", False),
                  XInternAtom(disp, "WM_STATE", False), 32, PropModeReplace,
                  (unsigned char *) data, 2);
}
