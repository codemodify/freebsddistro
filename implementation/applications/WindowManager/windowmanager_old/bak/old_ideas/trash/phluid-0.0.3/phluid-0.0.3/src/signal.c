#include "phluid.h"
#include "debug.h"

static void
quit_gracefully(void)
{
  Client *c;
  DEBUG_PRINT("*** caught SIGINT ***\n");
  for (c = client_list_head; c; c = c->next) {
    delete_client(c);
  }
  exit(0);
}

static void
sig_handler(int sig)
{
  switch (sig) {
    case SIGINT:
      quit_gracefully();
      break;
  }
}

void
init_signal(void)
{
  (void) signal(SIGINT, sig_handler);
}
