#include "phluid.h"
#include "debug.h"

void
get_mouse_x_y(int *x, int *y)
{
  /* dummy vars */
  Window tmp;
  int i;
  unsigned int ui;

  XQueryPointer(disp, root, &tmp, &tmp, x, y, &i, &i, &ui);
}
