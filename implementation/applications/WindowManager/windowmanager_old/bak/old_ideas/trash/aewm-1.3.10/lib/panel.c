/* aewm - Copyright 1998-2006 Decklin Foster <decklin@red-bean.com>.
 * This program is free software; please see LICENSE for details. */

#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "common.h"
#include "atom.h"
#include "menu.h"
#include "panel.h"

Atom net_wm_strut;
Atom net_wm_strut_partial;
Atom net_wm_wintype;
Atom net_wm_wintype_dock;

void setup_panel_atoms()
{
    net_wm_strut = XInternAtom(dpy, "_NET_WM_STRUT", False);
    net_wm_strut_partial = XInternAtom(dpy, "_NET_WM_STRUT_PARTIAL", False);
    net_wm_wintype = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
    net_wm_wintype_dock = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
}

void sig_handler(int signal)
{
    if (signal == SIGCHLD)
        wait(NULL);
}

void set_strut(Window w, strut_t *s)
{
    unsigned long data[4];

    data[0] = s->left;
    data[1] = s->right;
    data[2] = s->top;
    data[3] = s->bottom;

    XChangeProperty(dpy, w, net_wm_strut, XA_CARDINAL,
        32, PropModeReplace, (unsigned char *)data, 4);
}
