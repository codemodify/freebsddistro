#include "phluid.h"
#include "debug.h"

static void render_name(Client *);

void
make_client(Window * w)
{
  Client *c;
  XWindowAttributes attr;
  XSetWindowAttributes pattr;

  XGetWindowAttributes(disp, *w, &attr);
  /* popup window, don't make a client */
  if (attr.override_redirect)
    return;

  c = malloc(sizeof *c);

  XFetchName(disp, *w, &c->name);
  c->shaded = 0;
  c->window = *w;
  c->x = attr.x;
  c->y = attr.y;
  c->width = attr.width;
  c->height = attr.height;
  c->ignore_unmap = 0;
  c->next = client_list_head;
  client_list_head = c;
  c->decals = NULL;
  c->decals_to_render = NULL;

/* XGrabServer(disp); *//* is this necessary? */

  /* set attributes for window decoration window */
  pattr.override_redirect = True;
  pattr.background_pixel = bg.pixel;
  pattr.border_pixel = bd.pixel;
  pattr.event_mask =
      StructureNotifyMask | SubstructureRedirectMask | SubstructureNotifyMask |
      ButtonPressMask | ButtonReleaseMask | PointerMotionMask | ExposureMask |
      EnterWindowMask;

  /* make window decoration window */
  c->frame =
      XCreateWindow(disp, root, c->x, c->y, c->width,
                    c->height + TITLEBAR_HEIGHT, 1, DefaultDepth(disp,
                                                                 screen),
                    CopyFromParent, DefaultVisual(disp, screen),
                    CWOverrideRedirect | CWBackPixel | CWBorderPixel |
                    CWEventMask, &pattr);

  XAddToSaveSet(disp, c->window);
  XSelectInput(disp, c->window, PropertyChangeMask);
  XSetWindowBorderWidth(disp, c->window, 0);
  XMoveWindow(disp, c->window, c->x, c->y + TITLEBAR_HEIGHT);

  /* going to get an UnmapNotify from XReparentWindow() */
  if (attr.map_state == IsViewable)
    c->ignore_unmap++;
  XReparentWindow(disp, c->window, c->frame, 0, TITLEBAR_HEIGHT);

  if (attr.map_state == IsViewable) {
    XWMHints *wm_hints;
    wm_hints = XAllocWMHints();
    wm_hints = XGetWMHints(disp, c->window);
    if (wm_hints->initial_state == IconicState) {
      c->visible = 0;
    } else {
      CARD32 data[2];
      data[0] = NormalState;
      data[1] = None;
      XMapWindow(disp, c->frame);
      c->visible = 1;
      XChangeProperty(disp, c->window, XInternAtom(disp, "WM_STATE", False),
                      XInternAtom(disp, "WM_STATE", False), 32, PropModeReplace,
                      (unsigned char *) data, 2);
    }

    XFree(wm_hints);
  }

  /* XUngrabServer(disp); */

  /* start adding nift with Imlib2 */

  /* make background image */
  c->bg =
      imlib_load_image(PACKAGE_DATA_DIR "/themes/default/images/title_bg.png");

  /* add decals */
  add_new_decal_to_client(c,
                          PACKAGE_DATA_DIR
                          "/themes/default/images/close_normal.png", 1, 1,
                          DecalFnDelete, DecalFnNone, DecalFnDestroy);

  c->updates = imlib_updates_init();
  render_title_bar(c);

  DEBUG_CLIENT_LIST();
}

void
delete_client(Client * c)
{
  Client *tmp;
  if (!c)
    return;

  XGrabServer(disp);
  /* The upcoming XReparentWindow and XMoveWindow calls might generate
     BadWindow errors if the window already got destroyed.  I don't know
     how to avoid this so I'll set a handler to take care of it(*cough* 
     ignore it *cough*) gracefully. */
  XSetErrorHandler(handle_xerror);

  XReparentWindow(disp, c->window, root, c->x, c->y);
  XRemoveFromSaveSet(disp, c->window);
  XDestroyWindow(disp, c->frame);
  XMoveWindow(disp, c->window, c->x, c->y);
  XSync(disp, False);

  /* set back to the default error handler */
  XSetErrorHandler(NULL);
  XUngrabServer(disp);

  if (client_list_head == c)
    client_list_head = c->next;
  else
    for (tmp = client_list_head; tmp && tmp->next; tmp = tmp->next)
      if (tmp->next == c)
        tmp->next = c->next;

  /* free up memory */
  if (c->name)
    XFree(c->name);
  imlib_context_set_image(c->title_bar);
  imlib_free_image();
  imlib_context_set_image(c->bg);
  imlib_free_image();
  free(c);

  DEBUG_CLIENT_LIST();
}

Client *
find_client_by_frame(Window w)
{
  Client *c;
  for (c = client_list_head; c; c = c->next)
    if (c->frame == w)
      return c;
  return NULL;
}

Client *
find_client_by_child(Window w)
{
  Client *c;
  for (c = client_list_head; c; c = c->next)
    if (c->window == w)
      return c;
  return NULL;
}

void
render_title_bar(Client * c)
{
  int i_w, i_h;
  imlib_context_set_image(c->bg);
  i_w = imlib_image_get_width();
  i_h = imlib_image_get_height();
  c->title_bar = imlib_create_image(c->width, TITLEBAR_HEIGHT);
  if (c->bg) {
    imlib_context_set_image(c->title_bar);
    imlib_blend_image_onto_image(c->bg, 0, 0, 0, i_w, i_h, 0, 0,
                                 c->width, TITLEBAR_HEIGHT);
  }

  c->decals_to_render = c->decals;
  render_decals(c);

  imlib_context_set_image(c->title_bar);
  imlib_context_set_blend(0);
  imlib_context_set_drawable(c->frame);
  imlib_render_image_on_drawable(0, 0);
  imlib_context_set_blend(1);

  /* render text */
  c->name_change = 1;
  c->name_width = 0;
  c->name_height = 0;
  redraw_client(c);
}

static void
render_name(Client * c)
{
  Imlib_Font font;
  int text_w, text_h;

  DEBUG_PRINT("in render_name\n");
  text_w = c->name_width;
  text_h = c->name_height;
  imlib_add_path_to_font_path(PACKAGE_DATA_DIR "/ttfonts");
  font = imlib_load_font("tahomabd/10");
  if (font) {
    int i_w, i_h;

    /* cover up old text */
    imlib_context_set_image(c->bg);
    i_w = imlib_image_get_width();
    i_h = imlib_image_get_height();
    imlib_context_set_image(c->title_bar);
    imlib_blend_image_onto_image(c->bg, 0, 0, 0, i_w, i_h, TEXT_X, 0,
                                 c->name_width, TITLEBAR_HEIGHT);

    /* render new text */
    imlib_context_set_font(font);
    imlib_context_set_color(60, 60, 60, 200);
    imlib_get_text_size(c->name, &c->name_width, &c->name_height);
    imlib_context_set_blend(1); /* be sure of blending */
    imlib_text_draw(TEXT_X, TEXT_Y, c->name);
    imlib_free_font();
  }
  /* update changed portion (the union of the old and new text areas) */
  if (c->name_width > text_w)
    text_w = c->name_width;
  if (c->name_height > text_h)
    text_h = c->name_height;
  c->updates =
      imlib_update_append_rect(c->updates, TEXT_X, TEXT_Y, text_w, text_h);
}

void
redraw_client(Client * c)
{
  Imlib_Image buffer;
  int i_w, i_h;
  Imlib_Updates current_update;
  int up_x, up_y, up_w, up_h;   /* geometry of updated portions */

  if (c->name_change > 0) {
    render_name(c);             /* update the name text */
    c->name_change--;
  }

  if (!c->updates)
    return;

  c->updates =
      imlib_updates_merge_for_rendering(c->updates, c->width, TITLEBAR_HEIGHT);

  for (current_update = c->updates; current_update;
       current_update = imlib_updates_get_next(current_update)) {
    imlib_updates_get_coordinates(current_update, &up_x, &up_y, &up_w, &up_h);

    imlib_context_set_image(c->title_bar);
    i_w = imlib_image_get_width();
    i_h = imlib_image_get_height();

    buffer = imlib_create_image(up_w, up_h);
    imlib_context_set_image(buffer);

    /* blend image onto buffer */
    imlib_blend_image_onto_image(c->title_bar, 0, 0, 0, i_w, i_h, -up_x,
                                 -up_y, c->width, TITLEBAR_HEIGHT);

    imlib_context_set_image(buffer);
    imlib_context_set_blend(0);
    imlib_context_set_drawable(c->frame);
    imlib_render_image_on_drawable(up_x, up_y);
    imlib_free_image();
    imlib_context_set_blend(1);
  }
  /* clean up old updates and init new */
  if (c->updates)
    imlib_updates_free(c->updates);
  c->updates = imlib_updates_init();
  XFlush(disp);
}
