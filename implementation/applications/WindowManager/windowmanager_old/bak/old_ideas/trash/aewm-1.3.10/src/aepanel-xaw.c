/* aewm - Copyright 1998-2006 Decklin Foster <decklin@red-bean.com>.
 * This program is free software; please see LICENSE for details. */

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <X11/Xatom.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include "common.h"
#include "atom.h"
#include "menu.h"
#include "panel.h"

#define NAME_SIZE 48

void *make_launchitem_cb(void *, char *, char *); 
void make_clientitem(Widget, Window);
void cleanup_clientitem(client_t *);
void check_event(Widget, XtPointer, XEvent *, Boolean *);
static void update_client_list(Widget);
int ignore_xerror(Display *, XErrorEvent *);
void popup_cb(Widget, XtPointer, XtPointer);
void fork_exec_cb(Widget, XtPointer, XtPointer);
void raise_win_cb(Widget, XtPointer, XtPointer);

client_t *head = NULL;
Widget dummy;

int main(int argc, char **argv)
{
    Widget toplevel, paned, cmds_box, menu_button, launch_menu;
    Widget clients_box;
    client_t *c;
    strut_t s = { 0, 0, 0, 0 };
    int i, height = 0, bw = 0, root_width, root_height;
    char *opt_config = NULL;
    int opt_bottom = 0;
    struct sigaction act;
    unsigned long read, left, desk = DESK_ALL;
    Window w;

    setlocale(LC_ALL, "");
    toplevel = XtInitialize(argv[0], "AePanel", NULL, 0, &argc, argv);

    for (i = 1; i < argc; i++) {
        if ARG("config", "rc", 1)  {
            opt_config = argv[++i];
        } else if ARG("bottom", "b", 0)  {
            opt_bottom = 1;
        } else {
            fprintf(stderr,
                "usage: aepanel [--bottom|-b] [--config|-rc <file>]\n");
            exit(2);
        }
    }

    act.sa_handler = sig_handler;
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, NULL);

    dpy = XtDisplay(toplevel);
    root = DefaultRootWindow(dpy);
    XSetErrorHandler(ignore_xerror);

    setup_switch_atoms();
    setup_panel_atoms();

    paned = XtVaCreateManagedWidget("paned",
        panedWidgetClass, toplevel,
        XtNorientation, XtEhorizontal,
        XtNshowGrip, XtEfalse, NULL);
    cmds_box = XtVaCreateManagedWidget("commands",
        boxWidgetClass, paned,
        XtNorientation, XtEhorizontal, NULL);
    menu_button = XtVaCreateManagedWidget("Launch",
        menuButtonWidgetClass, cmds_box, NULL);
    launch_menu = XtVaCreatePopupShell("menu",
        simpleMenuWidgetClass, menu_button, NULL);
    clients_box = XtVaCreateManagedWidget("clients",
        boxWidgetClass, paned,
        XtNorientation, XtEhorizontal, NULL);

    dummy = XtVaCreateWidget("dummy",
        coreWidgetClass, toplevel, XtNwidth, 1, XtNheight, 1, NULL);
    XtRegisterDrawable(dpy, root, dummy);

    XSelectInput(dpy, root, PropertyChangeMask);
    XtAddRawEventHandler(dummy, PropertyChangeMask,
        False, check_event, clients_box);

    make_launch_menu(opt_config, launch_menu, make_launchitem_cb);

    for (i = 0, left = 1; left; i += read) {
        read = get_atoms(root, net_client_list, XA_WINDOW, i, &w, 1, &left);
        if (read)
            make_clientitem(clients_box, w);
        else
            break;
    }

    for (c = head; c; c = c->next)
        cleanup_clientitem(c);

    XtRealizeWidget(toplevel);
    XtVaGetValues(toplevel, XtNheight, &height, XtNborderWidth, &bw, NULL);

    append_atoms(XtWindow(toplevel), net_wm_state, XA_ATOM,
        &net_wm_state_skipt, 1);
    append_atoms(XtWindow(toplevel), net_wm_state, XA_ATOM,
        &net_wm_state_skipp, 1);
    set_atoms(XtWindow(toplevel), net_wm_wintype, XA_ATOM,
        &net_wm_type_dock, 1);
    set_atoms(XtWindow(toplevel), net_wm_desk, XA_CARDINAL,
        &desk, 1);

    if (opt_bottom) s.bottom = height;
    else s.top = height;
    set_strut(XtWindow(toplevel), &s);

    root_width = DisplayWidth(dpy, DefaultScreen(dpy));
    root_height = DisplayHeight(dpy, DefaultScreen(dpy));

    XtMoveWidget(toplevel, 0, opt_bottom ? root_height - height : 0);
    XtResizeWidget(toplevel, root_width, height, bw);

    XtMainLoop();
    return 0;
}

void *make_launchitem_cb(void *menu, char *label, char *cmd)
{
    Widget item, sub_menu = NULL;

    item = XtVaCreateManagedWidget(label, smeBSBObjectClass, menu,
        XtNmenuName, label, NULL);

    if (cmd) {
        XtAddCallback(item, XtNcallback, fork_exec_cb, cmd);
    } else {
        sub_menu = XtVaCreatePopupShell(label,
            simpleMenuWidgetClass, menu, NULL);
        XtAddCallback(item, XtNcallback, popup_cb, (XtPointer)sub_menu);
    }

    return sub_menu;
}

void make_clientitem(Widget container, Window w)
{
    char buf[NAME_SIZE];
    client_t *c;

    if (container == NULL) return;

    for (c = head; c; c = c->next) {
        if (c->win == w) {
            if (is_on_cur_desk(w))
                XtManageChild((Widget)c->widget);
            else
                XtUnmanageChild((Widget)c->widget);
            c->save = 1;
            return;
        }
    }

    if (is_on_cur_desk(w) && !is_skip(w)) {
        c = malloc(sizeof *c);
        c->next = head;
        head = c;
        c->win = w;
        c->save = 1;

        snprint_wm_name(buf, sizeof buf, w);
        c->widget = XtVaCreateManagedWidget(buf,
            commandWidgetClass, container, NULL);
        XtAddCallback((Widget)c->widget,
            XtNcallback, raise_win_cb, (XtPointer)w);

        XtRegisterDrawable(dpy, c->win, dummy);
        XSelectInput(dpy, c->win, PropertyChangeMask);
    }
}

void cleanup_clientitem(client_t *c)
{
    client_t *p;

    if (c->save) {
        c->save = 0;
    } else {
        XtDestroyWidget((Widget)c->widget);
        if (head == c) {
            head = c->next;
        } else {
            for (p = head; p && p->next; p = p->next)
                if (p->next == c) p->next = c->next;
        }
        free(c);
    }
}

void check_event(Widget w, XtPointer data, XEvent *e, Boolean *dispatch)
{
    client_t *c;
    char buf[NAME_SIZE];

    if (e->type == PropertyNotify) {
        if (e->xproperty.window == root) {
            if (e->xproperty.atom == net_cur_desk ||
                    e->xproperty.atom == net_client_list)
                update_client_list(data);
        } else {
            if (e->xproperty.atom == net_wm_desk) {
                update_client_list(data);
            } else {
                /* don't really care which atom changed; just redo it */
                for (c = head; c; c = c->next) {
                    if (c->win == e->xproperty.window) {
                        snprint_wm_name(buf, sizeof buf, c->win);
                        XtVaSetValues((Widget)c->widget, XtNlabel, buf, NULL);
                    }
                }
            }
        }
    }
}

static void update_client_list(Widget container)
{
    client_t *c, *save_next;
    int i;
    unsigned long read, left;
    Window w;

    for (i = 0, left = 1; left; i += read) {
        read = get_atoms(root, net_client_list, XA_WINDOW, i, &w, 1, &left);
        if (read)
            make_clientitem(container, w);
        else
            break;
    }

    for (c = head; c; c = save_next) {
        save_next = c->next;
        cleanup_clientitem(c);
    }
}

/* icky icky icky */

int ignore_xerror(Display *dpy, XErrorEvent *e)
{
    return 0;
}

void popup_cb(Widget w, XtPointer data, XtPointer call)
{
    int x, y;

    get_pointer(&x, &y);
    XtVaSetValues((Widget)data, XtNx, x, XtNy, y, NULL);
    XtPopup((Widget)data, XtGrabExclusive);
}

void fork_exec_cb(Widget w, XtPointer data, XtPointer call)
{
    fork_exec(data);
}

void raise_win_cb(Widget w, XtPointer data, XtPointer call)
{
    raise_win((Window)data);
}
