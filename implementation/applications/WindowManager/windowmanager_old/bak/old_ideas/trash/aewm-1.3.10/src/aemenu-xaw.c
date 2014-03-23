/* aewm - Copyright 1998-2006 Decklin Foster <decklin@red-bean.com>.
 * This program is free software; please see LICENSE for details. */

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xatom.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include "common.h"
#include "atom.h"
#include "menu.h"

void *make_launchitem_cb(void *, char *, char *); 
void make_clientitem(Widget, Window);
void popup_cb(Widget, XtPointer, XtPointer);
void raise_win_cb(Widget, XtPointer, XtPointer);
void fork_exec_cb(Widget, XtPointer, XtPointer);

int main(int argc, char **argv)
{
    Widget toplevel, main_menu;
    int i, mode = LAUNCH;
    char *opt_config = NULL;
    unsigned long read, left;
    Window w;

    setlocale(LC_ALL, "");
    toplevel = XtInitialize(argv[0], "AeMenu", NULL, 0, &argc, argv);

    for (i = 1; i < argc; i++) {
        if ARG("config", "rc", 1)  {
            opt_config = argv[++i];
        } else if ARG("launch", "l", 0)   {
            mode = LAUNCH;
        } else if ARG("switch", "s", 0)   {
            mode = SWITCH;
        } else {
            fprintf(stderr,
                "usage: aemenu [--switch|-s] [--config|-rc <file>]\n");
            exit(2);
        }
    }

    dpy = XtDisplay(toplevel);
    root = DefaultRootWindow(dpy);
    main_menu = XtVaCreatePopupShell("menu",
        simpleMenuWidgetClass, toplevel, NULL);

    if (mode == LAUNCH) {
        make_launch_menu(opt_config, main_menu, make_launchitem_cb);
    } else /* mode == SWITCH */ {
        setup_switch_atoms();
        for (i = 0, left = 1; left; i += read) {
            read = get_atoms(root, net_client_list, XA_WINDOW, i,
                &w, 1, &left);
            if (read)
                make_clientitem(main_menu, w);
            else
                break;
        }
    }

    popup_cb(NULL, main_menu, NULL);
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

void make_clientitem(Widget menu, Window w)
{
    char buf[BUF_SIZE];
    Widget menu_item;

    if (is_on_cur_desk(w) && !is_skip(w)) {
        snprint_wm_name(buf, sizeof buf, w);
        menu_item = XtVaCreateManagedWidget(buf,
            smeBSBObjectClass, menu, NULL);
        XtAddCallback(menu_item,
            XtNcallback, raise_win_cb, (XtPointer)w);
    }
}

void popup_cb(Widget w, XtPointer data, XtPointer call)
{
    int x, y;

    get_pointer(&x, &y);
    XtVaSetValues((Widget)data, XtNx, x, XtNy, y, NULL);
    XtPopup((Widget)data, XtGrabExclusive);
}

void raise_win_cb(Widget w, XtPointer data, XtPointer call)
{
    raise_win((Window)data);
    exit(0);
}

void fork_exec_cb(Widget w, XtPointer data, XtPointer call)
{
    fork_exec(data);
    exit(0);
}
