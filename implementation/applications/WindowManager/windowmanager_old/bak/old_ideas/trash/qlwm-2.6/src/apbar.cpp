/* 
*  File      : apbar.cpp 
*  Written by: alinden@gmx.de
*  Copyright : GPL
*
*  Manages the applets on the right side of the toolbar
*/

#include "defs.h"
#include "defaults.h"
#include "qapp.h"
#include "toolbar.h"
#include "apbar.h"

apbar::apbar(QWidget *parent, const char *name) : QWidget(parent, name)
{
	setFixedHeight(defaults::tc_height);
	setFixedWidth(1);
	ap.setAutoDelete(TRUE);

    	// overwrite Qt-defaults because we need SubstructureNotifyMask
	
	XSelectInput(qt_xdisplay(), winId(),
  		 PointerMotionMask |
		 EnterWindowMask | LeaveWindowMask |
  		 ExposureMask |
		 StructureNotifyMask |
		 SubstructureRedirectMask |
		 SubstructureNotifyMask);
}

void apbar::place_clients(void)
{
	WINDOW *cur;
	int twidth=2;  // total width

	for(cur = ap.first(); cur != NULL; cur = ap.next())
		twidth += cur->width+4;

	twidth -= 2;
	setUpdatesEnabled(FALSE);
	setFixedWidth(twidth);

	if(! twidth)
	{
		setUpdatesEnabled(TRUE);
		return;
	}	
		
	int cx = 2;
	for(cur = ap.first(); cur != NULL; cur = ap.next())
	{
		XMoveWindow(qt_xdisplay(), cur->w, cx, 0);
		
		XWindowAttributes attr;
		if(XGetWindowAttributes(qt_xdisplay(), cur->w, &attr))
		{
			XMapWindow(qt_xdisplay(), cur->w);
			XRaiseWindow(qt_xdisplay(), cur->w);
		}	
		cx += cur->width+4;
	}
	setUpdatesEnabled(TRUE);
}

void apbar::paletteChange(const QPalette &)  // property event will change client colors
{
	WINDOW *cur;
	XPropertyEvent xe;

	for(cur = ap.first(); cur != NULL; cur = ap.next())
	{
		xe.type = PropertyNotify;
		xe.window = qt_xrootwin();
		xe.state = PropertyNewValue;
		xe.atom = qapp::wm_resource_manager;
		XSendEvent(qt_xdisplay(), cur->w, True, PropertyChangeMask, (XEvent *)&xe);
	}
}

void apbar::release_all(void)
{
	WINDOW *cur;

	for(cur = ap.first(); cur != NULL; cur = ap.next())
		XReparentWindow(qt_xdisplay(), cur->w, qt_xrootwin(), 0, 0);
		
	ap.clear();
}		

bool apbar::add(Window w, int number, QString &name)
{
	WINDOW *cur;
	int i=2,pnum=1;

	for(cur = ap.first(); cur != NULL; cur = ap.next())
	{
		if(cur->name == name)
			return FALSE;   // already exists

		if(number > cur->number)
			pnum = i;

		i++;	
	}

	XWindowAttributes attr;
	if(! XGetWindowAttributes(qt_xdisplay(), w, &attr))
		return TRUE;

	int twidth=2;  // total width
	for(cur = ap.first(); cur != NULL; cur = ap.next())
		twidth += cur->width+4;

	if(twidth+attr.width+100 > tb_pb->width())
	{
		cerr << "WM: No space left on toolbar\n";
		return FALSE;
	}

	WINDOW *client = new WINDOW;

	client->w = w;
	client->name = name;
	client->width = attr.width;
	client->number = number;

	ap.insert(pnum-1, client);
	
	XSetWindowBorderWidth(qt_xdisplay(), w, 0);
	XResizeWindow(qt_xdisplay(), w, attr.width, defaults::tc_height);
	XReparentWindow(qt_xdisplay(), w, winId(), 0, 0);

	place_clients();

	return TRUE;
}

bool apbar::client_exists(Window w)
{
	WINDOW *cur;

	for(cur = ap.first(); cur != NULL; cur = ap.next())
	{
		if(cur->w == w)
			return TRUE;
	}
	return FALSE;
}

bool apbar::remove(Window w)
{
	WINDOW *cur;
	
	for(cur = ap.first(); cur != NULL; cur = ap.next())
	{
		if(cur->w == w)
		{
			ap.remove();
			place_clients();
			return TRUE;
		}
	}
	return FALSE;
}

void apbar::remove(void)
{
	WINDOW *cur;
	bool place = FALSE;

	for(cur = ap.first(); cur != NULL; cur = ap.next())
	{
		while(cur != NULL && qapp::apclients[cur->name] == 0)
		{
			ap.remove();
			XKillClient(qt_xdisplay(), cur->w);
			place = TRUE;
			cur = ap.current();
		}
	}
	if(place)
		place_clients();
}
