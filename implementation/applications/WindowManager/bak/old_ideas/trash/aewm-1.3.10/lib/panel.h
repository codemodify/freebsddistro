/* aewm - Copyright 1998-2006 Decklin Foster <decklin@red-bean.com>.
 * This program is free software; please see LICENSE for details. */

#ifndef AEWM_CLIENTS_PANEL_H
#define AEWM_CLIENTS_PANEL_H

#include <X11/X.h>

typedef struct client client_t;

struct client {
    client_t *next;
    Window   win;
    void     *widget;
    int      save;
};

extern void setup_panel_atoms();
extern void sig_handler(int);
extern void set_strut(Window, strut_t *);

#endif /* AEWM_CLIENTS_PANEL_H */
