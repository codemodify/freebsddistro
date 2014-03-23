#include "phluid.h"
#include "debug.h"

#ifdef DEBUG

void
debug_client_list(void)
{
  Client *tmp;
  DEBUG_PRINT("client list: ");
  for (tmp = client_list_head; tmp; tmp = tmp->next) {
    DEBUG_VAR("%s -> ", tmp->name);
  }
  DEBUG_PRINT("(null)\n");
}

#endif
