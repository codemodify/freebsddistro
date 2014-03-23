/*
*  File      : qapp.cpp
*  Written by: alinden@gmx.de
*  Copyright : GPL
*
*  Subclassing for QApplication, contains the X11 event
*  filter and some static functions that are common to all
*  classes
*/

#include "defs.h"
#include "conf.h"
#include "keyboard.h"
#include "defaults.h"
#include "toolbar.h"
#include "qapp.h"

QList <xwindow> clients;     // client list

Toolbar *qapp::tb;                          // Toolbar
QPixmap *qapp::leftwinpix;                  // window button pixmap
QPixmap *qapp::rightwinpix;                 // window button pixmap
QIntDict <xwindow> qapp::cwindows;          // client window lookup table
QIntDict <xwindow> qapp::pwindows;          // parent window lookup table
int qapp::next_x = 10;                      // autoplacement position counter
int qapp::next_y = defaults::tb_height+1;   // autoplacement position counter
int qapp::adesk;                            // current desktop
Atom qapp::wm_protocols;
Atom qapp::wm_delete;
Atom qapp::wm_change_state;
Atom qapp::wm_state;
Atom qapp::wm_take_focus;
Atom qapp::wm_resource_manager;
Atom qapp::wm_colormaps;
bool qapp::smode;
QPalette *qapp::ipal;                       // window inactive palette
QPalette *qapp::upal;                       // window urgent palette
bool qapp::mrb;                             // move/resize active
xwindow *qapp::focusclient;                 // currently active client
xwindow *qapp::tmaxclient;                  // maximized client in tiled mode
bool qapp::tdesks[10];                      // desks in tiled state
winfo *qapp::winf;
bool qapp::menu_open;                       // toolbar menu open
QMap <QString, int> qapp::cprops;           // client flags for Property command
QMap <QString, int> qapp::apclients;        // Toolbar clients, position number
QDateTime qapp::lmtime;                     // last defaults modification time 
bool qapp::sighup;                          // SIGHUP received
int qapp::servershapes;                     // server supports shapes

static int ShapeEventBase;                  // event base for shape extension
static bool rootptr = TRUE;                 // mouse pointer in root window

void sig_term(int);

qapp::qapp(int &argc, char **argv) : QApplication(argc, argv) 
{
	// get WM protocols required by ICCCM
	
	wm_protocols = XInternAtom(qt_xdisplay(), "WM_PROTOCOLS", FALSE); 
	wm_delete = XInternAtom(qt_xdisplay(), "WM_DELETE_WINDOW", FALSE);
	wm_change_state = XInternAtom(qt_xdisplay(), "WM_CHANGE_STATE", FALSE);
	wm_state = XInternAtom(qt_xdisplay(), "WM_STATE", FALSE);
	wm_take_focus = XInternAtom(qt_xdisplay(), "WM_TAKE_FOCUS", FALSE);
	wm_resource_manager = XInternAtom(qt_xdisplay(), "RESOURCE_MANAGER", FALSE);
	wm_colormaps = XInternAtom(qt_xdisplay(), "WM_COLORMAP_WINDOWS", FALSE);

	// save defaults modification time
	
	QString fname = get_cfile("defaults");

	if(! fname.isNull())
	{
		QFileInfo fi(fname);
		lmtime = fi.lastModified();
	}	

	// check if server supports nonrectangular windows
	
	int err;
	servershapes = XShapeQueryExtension(qt_xdisplay(), &ShapeEventBase, &err);
}

void qapp::setinactive(xwindow *client)  // set last active client to inactive
{
	if(client != focusclient)
	{
	
		if(focusclient != NULL && clients.find(focusclient) != -1)  // still there
			focusclient->setinactive();

		focusclient = client;
	}
}

void qapp::stopautofocus(void)
{
	if(focusclient != NULL && clients.find(focusclient) != -1)
		focusclient->stopautofocus();
}

void qapp::run_client(Window w)  // start new client
{
	xwindow *client;
	int apnumber;

	if((client = cwindows.find(w)) != NULL)
	{
		client->map();
	}
	else  // new client
	{
		XClassHint ch;
		if(XGetClassHint(qt_xdisplay(), w, &ch))
		{
			QString clname(ch.res_class);
			QString cclname(ch.res_name);
			cclname += ',';
			cclname += clname;
			
			if(clname == "unnamed")  // workaround for some Qt versions
				clname = ch.res_name;

			XFree(ch.res_name);
			XFree(ch.res_class);

			if(! clname.isEmpty() && (apnumber = apclients[clname])) 
			{
				if(tb_ap->add(w, apnumber, clname))  // add to toolbar
					return;
			}

			if(! cclname.isEmpty() && (apnumber = apclients[cclname])) 
			{
				if(tb_ap->add(w, apnumber, cclname))
					return;
			}
		}
		clients.prepend((client = new xwindow(w)));

		if(client->is_tileable() && tdesks[adesk] && ! client->is_unmapped())
		{
			tile_order(tmaxclient);

	    		client->show();
	    		XMapWindow(qt_xdisplay(), client->client_id());
	    		client->set_clientstate(NormalState);
	    		XSync(qt_xdisplay(), FALSE);
			client->raise();
		}	
	}
}

void qapp::install_colormap(Colormap cmap)
{
	static Colormap lastmap = None;

	if(cmap == lastmap)
		return;

	lastmap = cmap;
		
	if(cmap == None)
		cmap = DefaultColormap(qt_xdisplay(), DefaultScreen(qt_xdisplay()));
		
	XInstallColormap(qt_xdisplay(), cmap);
}

void qapp::wm_restart(void)
{
	xwindow *client;
	int i;

	QString fname = get_cfile("defaults");

	if(fname.isNull())
		return;
	
	QFileInfo fi(fname);

	if(fi.lastModified() == lmtime || fi.lastModified().addSecs(180) < QDateTime::currentDateTime())
		return;

	lmtime = fi.lastModified();	

	if(smode)
		keyboard::tscreen();

	for(i=0; i < defaults::vdesks; i++)
	{
		if(tdesks[i])
		{
			tb_pg->change_desk(i);
			toggle_tiled();
		}
	}

	winf->release_cancel();
	tb_pg->change_desk(0);
	tb_pb->remove_all();
	
	for(client = clients.first(); client != NULL; client = clients.next())
	{
		XRemoveFromSaveSet(qt_xdisplay(), client->client_id());
		XReparentWindow(qt_xdisplay(), client->client_id(), qt_xrootwin(), client->x(), client->y());
		XMapWindow(qt_xdisplay(), client->client_id());
	}
	clients.clear();
	tb_ap->release_all();
	delete tb;
	XSync(qt_xdisplay(), FALSE);
	
	char *argv[20];
	int rst=TRUE;

	for(i=0; i < defaults::argc && i < 18; i++)
	{
		argv[i] = defaults::argv[i];
			
		if(strcmp(argv[i], "-restart") == 0)
			rst = FALSE;
	}

	if(rst)
		argv[i++] = "-restart";
		
	argv[i] = NULL;

	execvp(argv[0], argv);
	perror(argv[0]);
	exit(1);
}

inline bool qapp::is_curdesk(xwindow *client)  // client on current desk
{
	if(client->x()+client->width()/2 > 0 && client->x()+client->width()/2 < QApplication::desktop()->width())
		return(TRUE);
	else
		return(FALSE);
}

void qapp::tile_order(xwindow *actclient)
{
	if(smode)
		return;

	xwindow *client,*tmcl=NULL;

	int cct=0,cheight,lh=0;
	QWidget *dt = QApplication::desktop();

	if(actclient != NULL && (clients.find(actclient) == -1 || ! actclient->is_tileable() || actclient->is_unmapped() || ! is_curdesk(actclient)))
		actclient = NULL;

	for(client = clients.first(); client != NULL; client = clients.next())
	{
		if(client != actclient && ! client->is_unmapped() && is_curdesk(client) && client->is_tileable())
			cct++;
	}

	if(actclient == NULL && cct > 0)
		cct--;

	tmaxclient = NULL;
		
	if(cct)
		cheight = (dt->height()-defaults::tb_height)/cct;

	int xpos,xcw,ypos,yp;
	ypos = yp = defaults::toolbar_top?defaults::tb_height+1:0;
	xpos = (int)(dt->width()*defaults::tleftspace);
	xcw = dt->width()-xpos-1;
	
	for(client = clients.first(); client != NULL; client = clients.next())
	{
		if(! client->is_tileable() || client->is_unmapped() || ! is_curdesk(client)) 
			continue;

		if(actclient == NULL || cct == 0)
			actclient = client;
			
		if(client == actclient)
		{
			tmcl = tmaxclient = client;
			continue;
		}
		
		if(lh < 0)
			lh = 0;
			
		client->minimize_frame(cct > defaults::wminframe?TRUE:FALSE);
			
		lh = client->set_tile(xpos+1, ypos-lh, xcw, cheight+lh);
		ypos += cheight;
	}

	if(tmcl != NULL)
	{
		tmcl->minimize_frame(FALSE);
		tmcl->set_tile(0, yp, xpos, dt->height()-defaults::tb_height);
	}
	
	if(actclient != NULL)
		clients.prepend(clients.take(clients.find(actclient)));
}

void qapp::tile_maximize(xwindow *client)
{
	int i;
	
	if(tmaxclient != NULL && (i = clients.find(tmaxclient)) != -1)
		clients.insert(clients.find(client),  clients.take(i));
		
	tile_order(client);
}

void qapp::toggle_tiled(void) // toggle overlapped/tiled desk 
{
	xwindow *client;
	
	if(smode)
		return;
	
	if(tdesks[adesk])
	{
		for(client = clients.first(); client != NULL; client = clients.next())
		{
			if(is_curdesk(client))
				client->unset_tile();
		}	
			
		tdesks[adesk] = FALSE;
		tmaxclient = NULL;

		if(focusclient != NULL && clients.find(client) != -1)
			focusclient->focus_mouse();
			
		tb_wl->set_pixmap();

		return;
	}
	tile_order(focusclient);
	tdesks[adesk] = TRUE;
	tb_wl->set_pixmap();
}

void qapp::read_cprops(void)  // read app defaults
{
	QString fname,cline,name,par;
	int flags;
	int apnum = 1;
	
	fname = get_cfile("appdefaults");
	
	if(fname.isNull())
		return;
	
	QFile istr(fname);
	
	if(! istr.open(IO_ReadOnly))
	{
		perror("cannot open appdefaults");
		return;
	}	
	cprops.clear();
	apclients.clear();
	
	while(! istr.atEnd())
	{
		istr.readLine(cline, 1024);
		QTextIStream si(&cline);
	
		si >> name;
		par = si.readLine();

		if(par.find("ToolBar") != -1)
		{
			apclients.insert(name, apnum++);
			continue;
		}
	
		flags = 0;	
		
		int i;
		int paf[] = { WindowListSkip,Sticky,SmallFrame,NoResize,NoTile,NoKey,NoScreen };
		char *pas[] = { "WindowListSkip","Sticky","SmallFrame","NoResize","NoTile","NoKey","NoScreen" };

		for(i=0; i < 7; i++)
		{
			if(par.find(pas[i]) != -1)
				flags |= paf[i];
		}
		
		if(flags)
			cprops.insert(name, flags);
	}
	istr.close();

	// check for clients to update
		
	xwindow *client;
		
	for(client = clients.first(); client != NULL; client = clients.next())
		client->set_pflags();
			
	tb_ap->remove();  // update clients on toolbar
}

QString qapp::get_cfile(char *name)  // get abs config file name
{
	QString fname;
	
	if(! defaults::cfdir.isNull())  // user config dir
	{
		fname = defaults::cfdir;
		fname += '/';
		fname += name;

		QFileInfo fi(fname);

		if(fi.isReadable())
			return(fname);
	}
	
	fname = CONFDIR;   // system config dir
	fname += name;
	
	QFileInfo fi(fname);

	if(fi.isReadable())
		return(fname);
		
	perror((const char *)fname);
	fname = QString();
	return(fname);
}

void qapp::send_configurenotify(xwindow *client) 
{
	XConfigureEvent ce;

	ce.type = ConfigureNotify;
	ce.event = client->client_id();
	ce.window = ce.event;
	ce.x = client->x();
	ce.y = client->get_clienty();
	ce.width = client->width();
	ce.height = client->getcheight();
	ce.above = None;
	ce.border_width = 0;
	ce.override_redirect = 0;
	XSendEvent(qt_xdisplay(), ce.window, False, StructureNotifyMask, (XEvent *) &ce);
}

// event filter returns FALSE to pass event to qt's event handler

bool qapp::x11EventFilter(XEvent *event)  
{
	xwindow *client;
	Window w;
	XEvent ev;
	XConfigureRequestEvent *cev;
	XClientMessageEvent *mev;
	XCrossingEvent *xev;
	XCirculateRequestEvent *rev;
	XPropertyEvent *pev;
			
#ifdef DEBUGMSG		
#include "eventnames.h"
	if(event->type < 36)
		cerr << "Received: " << event_names[event->type] << " (WId:" << event->xany.window << ")\n";
#endif
	while(waitpid(-1, NULL, WNOHANG) > 0);

	if(sighup)
	{
		wm_restart();
		tb_mn->readmenu();
		read_cprops();
		sighup = FALSE;
	}

	switch(event->type)
	{
		case DestroyNotify:
			w = event->xdestroywindow.window;

			if((client = cwindows.find(w)) != NULL)
			{
				clients.remove(client);
				
				if(smode && client->isstate())
					keyboard::tscreen();  // turn off screen mode

				tb_pg->draw_pager();
				
				return TRUE;
			}	
			if(tb_ap->remove(w))  // client on toolbar
				return TRUE;
				
			if(event->xdestroywindow.event != w)
				return TRUE;

			if(w == tb->winId() || w == tb_pg->winId() || w == tb_wl->winId() || w == tb_mn->winId() || w == tb_pb->winId())
				sig_term(SIGTERM);

			return FALSE;

		case MapNotify:
			if(event->xmap.event != event->xmap.window)
				return TRUE;
		
			if((client = pwindows.find(event->xmap.window)) != NULL)
				tb_pg->add(client);  // add to pager

			return FALSE;

		case UnmapNotify:
			if((client = cwindows.find(event->xunmap.window)) != NULL)
			{
				if(event->xunmap.send_event)
				{
					// client requested transitions 
					// normal -> withdrawn
					// iconic -> withdrawn
					
					client->withdraw();
				}
				else
					client->unmap();
				
				return TRUE;	
			}
			if(event->xunmap.event != event->xunmap.window)
				return TRUE;

			if(pwindows.find(event->xunmap.window) != NULL)
				tb_pg->draw_pager();
		
			return FALSE;

		case EnterNotify:
			xev = &event->xcrossing;
			
			if(event->xcrossing.window == qt_xrootwin())
			{
				stopautofocus();
				rootptr = TRUE;
			}
			else if(mrb == FALSE && menu_open == FALSE && (client = (xwindow *)widgetAt(xev->x_root, xev->y_root)) != NULL && 
			clients.find(client) != -1 && ((client = clients.current()) != focusclient || rootptr))
			{
				rootptr = FALSE;
				setinactive(client);  // old client to inactive, save new client
				
				if(xev->detail != NotifyInferior)
					client->startautofocus();

				client->setchildfocus(xev->time);
				client->setcmapfocus();
			}
			return FALSE;

		case ColormapNotify:
			if((client = cwindows.find(event->xcolormap.window)) != NULL)
			{
				client->setcmap(event->xcolormap.colormap);
				return TRUE;
			}	
			return FALSE;

		case PropertyNotify:
			pev = &event->xproperty;
			
			if((client = cwindows.find(pev->window)) != NULL)
			{
				if(pev->atom == XA_WM_NORMAL_HINTS)
				{
					client->get_wmnormalhints();
				}	
				else if(pev->atom == XA_WM_HINTS)
				{
					client->get_wmhints();
				}
				else if(pev->atom == XA_WM_NAME || pev->atom == XA_WM_ICON_NAME)
				{
					client->get_wmname();
				}
				else if(pev->atom == wm_colormaps)
				{
					client->get_colormaps();
					
					if(client == focusclient)
						client->setcmapfocus();
				}
				return TRUE;
			}
			return FALSE;

		case ConfigureNotify:
			if(event->xconfigure.event != event->xconfigure.window)
				return TRUE;
				
			if((client = pwindows.find(event->xconfigure.window)) != NULL)
			{
				tb_pg->draw_pager();
				while(XCheckTypedEvent(qt_xdisplay(), ConfigureNotify, &ev));
			}
			return TRUE;

		case ReparentNotify:
			if((client = cwindows.find(event->xreparent.window)) != NULL &&
			event->xreparent.parent != client->winId())
			{
				clients.remove(client);
				tb_pg->draw_pager();
			}	
			return TRUE;
		
		case ButtonPress:
			w = event->xbutton.window;
			
			if(w == qt_xrootwin())  // set focus to root window
				XSetInputFocus(qt_xdisplay(), w, RevertToPointerRoot, CurrentTime);

			if(w == tb->winId() || w == tb_pb->winId() || w == tb_ap->winId())
				XRaiseWindow(qt_xdisplay(), tb->winId());

			if(w == qt_xrootwin() || w == tb_pg->winId())
				install_colormap(None);
				
			return FALSE;

		case FocusOut:
			if(menu_open) // Qt 2.2.4 does not seem to like this if a menu is open
				return TRUE;

			return FALSE;

		case ClientMessage:
			mev = &event->xclient;
			
			if(mev->message_type == wm_change_state && mev->format == 32 && 
			mev->data.l[0] == IconicState && (client = cwindows.find(mev->window)) != NULL)
				client->iconify();

			return TRUE;	

		case CirculateRequest:
			rev = &event->xcirculaterequest;
			
			if(rev->place == PlaceOnTop)
				XRaiseWindow(qt_xdisplay(), rev->window);
			else
				XLowerWindow(qt_xdisplay(), rev->window);
				
			return TRUE;

		case ConfigureRequest:
			cev = &event->xconfigurerequest;
			XWindowChanges wc;
			
			if((client = cwindows.find(cev->window)) != NULL)
			{
#ifdef DEBUGMSG
	cerr << "configure request to client (WId:" << client->winId() << ")\n";
#endif	
				if(cev->value_mask & (CWWidth|CWHeight|CWX|CWY))
				{
					if(smode && client->isstate())
						keyboard::tscreen(); 
					
					if(! client->is_tiled() || client == tmaxclient)
					{	
						int cx,cy,cw,ch;

						if(cev->value_mask & CWWidth)
						cw = cev->width;
						else	
							cw = client->width();

						if(cev->value_mask & CWHeight)
							ch = cev->height;
						else
							ch = client->getcheight();

						if((cev->value_mask & CWX) && ! client->is_tiled())
							cx = cev->x;
						else
							cx = client->x();

						if((cev->value_mask & CWY) && ! client->is_tiled())
					    		cy = cev->y;
						else
							cy = client->y();
						
						client->resize_request(cx, cy, cw, ch);
					}
					cev->value_mask &= ~(CWWidth|CWHeight|CWX|CWY);
				}
				
				if(! cev->value_mask)
					return TRUE;

				wc.width = client->width();
				wc.height = client->height();
				wc.x = client->x();
				wc.y = client->y();
				wc.border_width = 0;
				wc.sibling = cev->above;
				wc.stack_mode = cev->detail;

				XConfigureWindow(qt_xdisplay(), client->winId(), cev->value_mask, &wc);
				send_configurenotify(client);
			}
			else  // never mapped window
			{
				if(cev->window == tb->winId() || tb_ap->client_exists(cev->window))  // deny requests on toolbar
					return TRUE;

#ifdef DEBUGMSG
	cerr << "configure request to unreparented window (WId:" << cev->window << ")\n";
#endif	

				wc.x = cev->x;
				wc.y = cev->y;
				wc.width = cev->width;
				wc.height = cev->height;
				cev->value_mask &= (CWX|CWY|CWWidth|CWHeight);
				
				XConfigureWindow(qt_xdisplay(), cev->window, cev->value_mask, &wc);	
			}
			return TRUE;
			
		case MapRequest:
			run_client(event->xmaprequest.window);
			return TRUE;
			
		case KeyPress:
			return(keyboard::keypress(&event->xkey));

		default:
			if(servershapes && event->type == (ShapeEventBase + ShapeNotify))
			{
				XShapeEvent *sev = (XShapeEvent *)event;
		
				if((client = cwindows.find(sev->window)) != NULL)
				{
					client->reshape();
					return TRUE;
				}	
			}
	}
	return FALSE;
}
