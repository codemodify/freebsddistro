#include "phluid.h"
#include "debug.h"

static Decal *new_decal(char *file, int x, int y, int fn1, int fn2, int fn3);
static void add_decal_to_client(Client * c, Decal * d);

void
do_decal_fn(Client * c, Decal * d, int button)
{
  int function;

  function = d->b1_fn;
  if (button == Button2)
    function = d->b2_fn;
  if (button == Button3)
    function = d->b3_fn;

  switch (function) {
    case DecalFnDelete:
      PH_DELETE_WIN(c);
      break;
    case DecalFnDestroy:
      XKillClient(disp, c->window);
      break;
    case DecalFnShade:
      shade_client(c);
      break;
    case DecalFnNone:
    default:
      break;
  }
}

void
add_new_decal_to_client(Client * c, char *file, int x, int y, int fn1, int fn2,
                        int fn3)
{
  Decal *tmp;
  tmp = new_decal(file, x, y, fn1, fn2, fn3);
  add_decal_to_client(c, tmp);
}

void
render_decals(Client * c)
{
  Decal *this;
  imlib_context_set_image(c->title_bar);
  for (this = c->decals_to_render; this; this = this->next) {
    int x, y;
    x = this->x;
    y = this->y;
    if(x < 0)
      x = c->width + x;
    if(y < 0)
      y = c->height + TITLEBAR_HEIGHT + y;
    imlib_blend_image_onto_image(this->image, 0, 0, 0, this->width,
                                 this->height, x, y, this->width,
                                 this->height);
  }
  c->decals_to_render = NULL;
}

Decal *
decal_at(Client * c, int x, int y)
{
  Decal *this;
  for (this = c->decals; this; this = this->next) {
    int dx, dy;
    dx = this->x;
    dy = this->y;
    if(dx < 0)
      dx = c->width + dx;
    if(dy < 0)
      dy = c->height + TITLEBAR_HEIGHT + dy;
    if (x >= dx && x <= dx + this->width &&
        y >= dy && y <= dy + this->height)
      return this;
  }
  return NULL;
}

static Decal *
new_decal(char *file, int x, int y, int fn1, int fn2, int fn3)
{
  Decal *d;

  d = malloc(sizeof(*d));
  d->x = x;
  d->y = y;
  d->b1_fn = fn1;
  d->b2_fn = fn2;
  d->b3_fn = fn3;
  d->image = imlib_load_image(file);
  imlib_context_set_image(d->image);
  d->width = imlib_image_get_width();
  d->height = imlib_image_get_height();

  return d;
}

static void
add_decal_to_client(Client * c, Decal * d)
{
  d->next = c->decals;
  c->decals = d;
}

void
clear_decals(Decal * d)
{
  Decal *this;
  for (this = d; this; this = this->next)
    free(d);
}
