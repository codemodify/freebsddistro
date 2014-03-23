#include "phluid.h"
#include "debug.h"

Display *disp;
Window root;
Visual *vis;
Colormap cm;
Imlib_Image bg_image;
Client *client_list_head = NULL;
XColor fg, bg, bd, dummyc;
int depth;
int screen;
int bw = 1;

int
main(void)
{
  XEvent ev;
  Window dummyw1, dummyw2, *wins;
  XWindowAttributes attr;
  unsigned int nwins, i;

  init_signal();
  init_x();
  init_imlib();

  XQueryTree(disp, root, &dummyw1, &dummyw2, &wins, &nwins);
  for (i = 0; i < nwins; i++) {
    XGetWindowAttributes(disp, wins[i], &attr);
    {
      char *name, *dummy;
      XFetchName(disp, wins[i], &name);
      if (name && strcmp(name, "gkrellm") == 0) {
        XFree(name);
        continue;
      }
      dummy = malloc(sizeof(char) * 5);
      dummy[0] = '\0';
      if (name && strcmp(strncat(dummy, name, 4), "XMMS") == 0) {
        XFree(name);
        continue;
      }
    }
    if (!attr.override_redirect && attr.map_state == IsViewable)
      make_client(&wins[i]);
  }
  XFree(wins);

  XFlush(disp);

  /* main event loop */
  for (;;) {
    Client *c, *exposed;
    XNextEvent(disp, &ev);
    switch (ev.type) {
      case ButtonPress:
        button_press(ev);
        break;
      case MapRequest:
        DEBUG_PRINT("got MapRequest\n");
        {
          char *name, *dummy;
          XFetchName(disp, ev.xmaprequest.window, &name);
          DEBUG_VAR("name: %s\n", name);
          if (name && strcmp(name, "gkrellm") == 0) {
            XFree(name);
            break;
          }
          dummy = malloc(sizeof(char) * 5);
          dummy[0] = '\0';
          if (name && strcmp(strncat(dummy, name, 4), "XMMS") == 0) {
            XFree(name);
            break;
          }
        }
        c = find_client_by_frame(ev.xmap.window);
        if (c)
          break; /* ignore frame being mapped (we just did it!) */

        c = find_client_by_child(ev.xmap.window);
        if (c)
          break; /* we already have a client for this window */

        /* otherwise, make a new client */
        {
          int x, y;
          get_mouse_x_y(&x, &y);
          XMoveWindow(disp, ev.xmap.window, x-30, y-8);
          XMapWindow(disp, ev.xmaprequest.window);
          XSync(disp, False);
          make_client(&ev.xmap.window);
        }
        break;
      case DestroyNotify:
        DEBUG_PRINT("got DestroyNotify\n");
        /* fall through (handle the same as an Unmap */
      case UnmapNotify:
        DEBUG_PRINT("got UnmapNotify\n");
        c = find_client_by_child(ev.xunmap.window);
        if (c) {
          if (c->ignore_unmap > 0)
            c->ignore_unmap--;
          else
            delete_client(c);
        }
        break;
      case Expose:
        /* was the exposed window a frame? */
        exposed = find_client_by_frame(ev.xexpose.window);
        if (exposed) {
          /* yes, so ask imlib to update the exposed portion */
          exposed->updates =
              imlib_update_append_rect(exposed->updates, ev.xexpose.x,
                                       ev.xexpose.y, ev.xexpose.width,
                                       ev.xexpose.height);
        }
        break;
      case EnterNotify:
        DEBUG_PRINT("got EnterNotify\n");
        c = find_client_by_frame(ev.xcrossing.window);
        if (c) {
          if(!c->shaded)
            XSetInputFocus(disp, c->window, RevertToPointerRoot, CurrentTime);
        }
        break;
      case PropertyNotify:
        DEBUG_PRINT("got PropertyNotify\n");
        c = find_client_by_child(ev.xproperty.window);
        /* name change? */
        if (c && ev.xproperty.atom == XA_WM_NAME) {
          DEBUG_PRINT("PropertyNotify is a name change\n");
          DEBUG_VAR("old name was: %s\n", c->name);
          if (c->name)
            XFree(c->name);
          XFetchName(disp, c->window, &c->name);
          DEBUG_VAR("new name is: %s\n", c->name);
          c->name_change++;
          redraw_client(c);
        }
        break;
      case ConfigureRequest:
        {
          XWindowChanges changes;
          int mask, width, height;
          DEBUG_PRINT("got ConfigureRequest\n");
          mask = ev.xconfigurerequest.value_mask;
          width = ev.xconfigurerequest.width;
          height = ev.xconfigurerequest.height;
          c = find_client_by_child(ev.xconfigurerequest.window);
          if(c) {
            if(mask & (CWWidth|CWHeight)) {
              c->width = width;
              c->height = height;
              icccm_send_configure_event(c);
              XResizeWindow(disp, c->frame, width, height+TITLEBAR_HEIGHT);
            }
            if(mask & CWWidth) {
              /* width changed redraw titlebar */
              render_title_bar(c);
            }
            changes.x = 0;
            changes.y = TITLEBAR_HEIGHT;
          } else {
            changes.x = ev.xconfigurerequest.x;
            changes.y = ev.xconfigurerequest.y;
          }
          changes.width = width;
          changes.height = height;
          changes.sibling = ev.xconfigurerequest.above;
          changes.stack_mode = ev.xconfigurerequest.detail;
          XConfigureWindow(disp, ev.xconfigurerequest.window, mask, &changes);
        }
        break;
      default:
        break;
    }

    if (!XPending(disp)) {
      for (c = client_list_head; c; c = c->next) {
        redraw_client(c);
      }
    }
    XSync(disp, False);
  }

  return 0;
}
