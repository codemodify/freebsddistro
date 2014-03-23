
#include <QtGui/QX11Info>
#include <QtGui/QDesktopWidget>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include "windowmanager.h"


//#include "keyboard/keyboard.h"
#include "defaults/defaults.h"
//#include "toolbar/toolbar.h"
//#include "pager/pager.h"
//#include "menu/menu.h"
//#include "procbar/procbar.h"
//#include "appbar/apbar.h"
#include "rubber/rubber.h"
//#include "xwindow/xwindow.h"

extern QList <xwindow*> clients;       // client list 

QPixmap *WindowManager::leftwinpix;                  // window button pixmap
QPixmap *WindowManager::rightwinpix;                 // window button pixmap
QHash <Window, xwindow*> WindowManager::cwindows;    // client window lookup table
QHash <Window, xwindow*> WindowManager::pwindows;    // parent window lookup table
int WindowManager::next_x = 10;                      // autoplacement position counter
int WindowManager::next_y = defaults::tb_height+1;   // autoplacement position counter
int WindowManager::adesk;                            // current desktop
Atom WindowManager::wm_protocols;
Atom WindowManager::wm_delete;
Atom WindowManager::wm_change_state;
Atom WindowManager::wm_state;
Atom WindowManager::wm_take_focus;
Atom WindowManager::wm_resource_manager;
Atom WindowManager::wm_colormaps;
Atom WindowManager::kde_net_wm_system_tray_window_for;
Atom WindowManager::net_wm_name;
Atom WindowManager::net_wm_icon_name;
bool WindowManager::smode;
QPalette *WindowManager::ipal;                       // window inactive palette
QPalette *WindowManager::upal;                       // window urgent palette
bool WindowManager::mrb;                             // move/resize active
xwindow *WindowManager::focusclient;                 // currently active client
xwindow *WindowManager::tmaxclient;                  // maximized client in tiled mode
bool WindowManager::tdesks[10];                      // desks in tiled state
bool WindowManager::menu_open;                       // toolbar menu open
QMap <QString, int> WindowManager::cprops;           // client flags for Property command
QMap <QString, int> WindowManager::apclients;        // Toolbar clients, position number
QDateTime WindowManager::lmtime;                     // last defaults modification time 
bool WindowManager::sighup;                          // SIGHUP received
int WindowManager::servershapes;                     // server supports shapes

static int ShapeEventBase;                  // event base for shape extension
static bool rootptr = TRUE;                 // mouse pointer in root window

void sig_term(int);

bool wm_init=TRUE;

void getrunprocs(void)  // get already running clients
{
	Window w,w1,w2,*wins;
	uint nwins,cwin;
	XWindowAttributes attr;

	if(XQueryTree(QX11Info::display(), QX11Info::appRootWindow(), &w1, &w2, &wins, &nwins) == 0 || ! nwins)
		return;

	bool surgent = defaults::starturgent;
	defaults::starturgent = FALSE;

	for(cwin=0; cwin < nwins; cwin++)
	{
		w = wins[cwin];

		//if(w == WindowManager::tb->winId())
			continue;

		XGetWindowAttributes(QX11Info::display(), w, &attr);

		if(attr.map_state == IsViewable && ! attr.override_redirect)
			WindowManager::run_client(w);
	}
	XSync(QX11Info::display(), FALSE);
	defaults::starturgent = surgent;
}

void sig_hup(int)
{
	WindowManager::sighup = TRUE;
}

void sig_term(int)  // terminate
{

	for(int i=0; i < clients.size(); i++)
	XReparentWindow(QX11Info::display(), clients.at(i)->winId(), QX11Info::appRootWindow(), 0, 0);

	XSync(QX11Info::display(), FALSE);

	exit(0);
}

void startprg(void)  // start programs from init stack
{
	pid_t pid;
	QByteArray *prg;
	while(! defaults::initexec.isEmpty())
	{
		prg = defaults::initexec.pop();

		if((pid = fork()) == 0)
		{
			execl("/bin/sh", "sh", "-c", prg->data(), NULL);
			perror("Exec");
			exit(1);
		}

		if(pid == -1)
			perror("fork");

		delete prg;
	}
}

void make_bpixmaps(void)   // create scaled window button pixmaps
{
	int wh = defaults::windowbuttonsize-4;
	QImage limg(WindowManager::get_cfile("button_left.xpm"));
	QImage rimg(WindowManager::get_cfile("button_right.xpm"));

	WindowManager::leftwinpix = new QPixmap();
	WindowManager::rightwinpix = new QPixmap();

	if(! limg.isNull())
		*WindowManager::leftwinpix = QPixmap::fromImage(limg.scaled(wh, wh, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

	if(! rimg.isNull())
		*WindowManager::rightwinpix = QPixmap::fromImage(rimg.scaled(wh, wh, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

}               


int xerrors(Display *d, XErrorEvent *event)
{
	char    msg[100];
	char    req[100];
	char    nmb[100];

	if(wm_init == TRUE && event->error_code == BadAccess)
	{
		logmsg << "Another WM is already running\n";
		exit(1);
	}

	#ifndef DEBUGMSG
	if(event->error_code == BadWindow || event->error_code == BadMatch)
	return 0;
	#endif          

	XGetErrorText(d, event->error_code, msg, sizeof(msg));
	sprintf(nmb, "%d", event->request_code); 
	XGetErrorDatabaseText(d, "XRequest", nmb, nmb, req, sizeof(req));

	logmsg << "WM: " << req << " on resource " << event->resourceid
	<< " failed: " << msg << '\n';

	return 0;
}

WindowManager::WindowManager(int &argc, char **argv) : QApplication(argc, argv) 
{
	// get WM protocols required by ICCCM
	
	wm_protocols = XInternAtom(QX11Info::display(), "WM_PROTOCOLS", FALSE); 
	wm_delete = XInternAtom(QX11Info::display(), "WM_DELETE_WINDOW", FALSE);
	wm_change_state = XInternAtom(QX11Info::display(), "WM_CHANGE_STATE", FALSE);
	wm_state = XInternAtom(QX11Info::display(), "WM_STATE", FALSE);
	wm_take_focus = XInternAtom(QX11Info::display(), "WM_TAKE_FOCUS", FALSE);
	wm_resource_manager = XInternAtom(QX11Info::display(), "RESOURCE_MANAGER", FALSE);
	wm_colormaps = XInternAtom(QX11Info::display(), "WM_COLORMAP_WINDOWS", FALSE);

	// Extensions 
	kde_net_wm_system_tray_window_for = XInternAtom(QX11Info::display(), "_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR", FALSE);
	net_wm_name = XInternAtom(QX11Info::display(), "_NET_WM_NAME", FALSE);
	net_wm_icon_name = XInternAtom(QX11Info::display(), "_NET_WM_ICON_NAME", FALSE);

	// save defaults modification time
	
	QString fname = get_cfile("defaults");

	if(! fname.isNull())
	{
		QFileInfo fi(fname);
		lmtime = fi.lastModified();
	}	

	// check if server supports nonrectangular windows
	
	int err;
	servershapes = XShapeQueryExtension(QX11Info::display(), &ShapeEventBase, &err);
}

bool WindowManager::initEnvironment()
{
	if(defaults::cfdir.isNull())  // get config dir
	{
		QString fname(getenv("HOME"));

		if(! fname.isNull())
		defaults::cfdir = fname + "/.qlwm";
	}
	defaults::argc = QApplication::argc();
	defaults::argv = QApplication::argv();

	defaults::read_config();
	WindowManager::read_cprops();
	make_bpixmaps();

	XSetErrorHandler(xerrors);

	XIconSize *isize;
	if((isize = XAllocIconSize()) != NULL)
	{
		isize->min_width = isize->min_height = 2;
		isize->max_width = isize->max_height = 200;
		isize->width_inc = isize->height_inc = 1;

		XSetIconSizes(QX11Info::display(), QX11Info::appRootWindow(), isize, 1);
		XFree(isize);
	}

	if(defaults::root_pix.isNull())
		desktop()->setPalette(QPalette(defaults::root_bg));
	else    
	{
		QPalette br;
		br.setBrush(QPalette::Window, QBrush(QPixmap(defaults::root_pix)));
		desktop()->setPalette(br);
	}
	XClearWindow(QX11Info::display(), QX11Info::appRootWindow());

	WindowManager::upal = new QPalette(defaults::urgent_bg);

	if(defaults::inactive_bg == NULL)
	{
		WindowManager::ipal = new QPalette;
		WindowManager::ipal->setBrush(QPalette::Window, WindowManager::ipal->brush(QPalette::Dark));
	}
	else
		WindowManager::ipal = new QPalette(*defaults::inactive_bg);

	XSelectInput(QX11Info::display(), QX11Info::appRootWindow(), SubstructureRedirectMask);
	XSync(QX11Info::display(), FALSE);

	if(defaults::sttiled[0])
	{
		WindowManager::tdesks[0] = TRUE;
		defaults::sttiled[0] = FALSE;
	}
	getrunprocs();
	defaults::start_restart = FALSE;

	XSelectInput(QX11Info::display(), QX11Info::appRootWindow(),
		SubstructureNotifyMask|SubstructureRedirectMask| 
		ButtonPressMask|
		PropertyChangeMask|
		KeyPressMask|
		ColormapChangeMask|
		EnterWindowMask);

	XSync(QX11Info::display(), FALSE);

	struct sigaction act;

	act.sa_handler = sig_hup;
	act.sa_flags = SA_RESTART;
	//sigaction(SIGHUP, &act, NULL);

	act.sa_handler = sig_term;
	//sigaction(SIGTERM, &act, NULL);

	wm_init = FALSE;
	startprg();

	return true;
}

void WindowManager::setinactive(xwindow *client)  // set last active client to inactive
{
	if(client != focusclient)
	{
	
		if(focusclient != NULL && clients.indexOf(focusclient) != -1)  // still there
			focusclient->setinactive();

		focusclient = client;
	}
}

void WindowManager::stopautofocus(void)
{
	if(focusclient != NULL && clients.indexOf(focusclient) != -1)
		focusclient->stopautofocus();
}

void WindowManager::run_client(Window w)  // start new client
{
	xwindow *client;
	int apnumber,format;
	Atom type;
	unsigned long nitems=0;
	unsigned long extra=0;
	unsigned char *data=NULL;

	stopautofocus();

	if((client = cwindows[w]) != NULL)
	{
		client->map();
	}
	else  // new client
	{
		XClassHint ch;
		if(XGetClassHint(QX11Info::display(), w, &ch))
		{
			QString clname(ch.res_class);
			QString cclname(ch.res_name);
			cclname += ',';
			cclname += clname;
			
			XFree(ch.res_name);
			XFree(ch.res_class);
			XGetWindowProperty(QX11Info::display(), w, kde_net_wm_system_tray_window_for, 0, 1L, FALSE, XA_WINDOW, &type, &format, &nitems, &extra, &data);

			if(data != NULL)
				XFree(data);

			if(! clname.isEmpty() && ((apnumber = apclients[clname]) || nitems))
			{
				;
			}

			if(! cclname.isEmpty() && ((apnumber = apclients[cclname]) || nitems))
			{
				;	
			}
		}
		clients.prepend((client = new xwindow(w)));

		if(client->is_tileable() && tdesks[adesk] && ! client->is_unmapped())
		{
			tile_order(tmaxclient);

	    		client->show();
	    		XMapWindow(QX11Info::display(), client->client_id());
	    		client->set_clientstate(NormalState);
	    		XSync(QX11Info::display(), FALSE);
			client->raise();
		}	
	}
}

void WindowManager::install_colormap(Colormap cmap)
{
	static Colormap lastmap = None;

	if(cmap == lastmap)
		return;

	lastmap = cmap;
		
	if(cmap == None)
		cmap = DefaultColormap(QX11Info::display(), DefaultScreen(QX11Info::display()));
		
	XInstallColormap(QX11Info::display(), cmap);
}

void WindowManager::wm_restart(void)
{
	xwindow *client;
	int i;

	for(i=0; i < defaults::vdesks; i++)
	{
		if(tdesks[i])
		{
			toggle_tiled();
		}
	}

	foreach(client, clients)
	{
		XRemoveFromSaveSet(QX11Info::display(), client->client_id());
		XReparentWindow(QX11Info::display(), client->client_id(), QX11Info::appRootWindow(), client->x(), client->y());
		XMapWindow(QX11Info::display(), client->client_id());
	}
	
	clients.clear();
	XSync(QX11Info::display(), FALSE);
	
	const char *argv[20];
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

	execvp(argv[0], (char **)argv);
	perror(argv[0]);
	exit(1);
}

bool WindowManager::is_curdesk(xwindow *client)  // client on current desk
{
	if(client->x()+client->width()/2 > 0 && client->x()+client->width()/2 < QApplication::desktop()->width())
		return(TRUE);
	else
		return(FALSE);
}

void WindowManager::tile_order(xwindow *actclient)
{
	if(smode)
		return;

	xwindow *client,*tmcl=NULL;

	int cct=0,i,cheight,lh=0;
	QWidget *dt = QApplication::desktop();

	if(actclient != NULL && (clients.indexOf(actclient) == -1 || ! actclient->is_tileable() || actclient->is_unmapped() || ! is_curdesk(actclient)))
		actclient = NULL;

	for(i=0; i < clients.size(); i++)
	{
		client = clients.at(i);
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
	
	foreach(client, clients)
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
		clients.prepend(clients.takeAt(clients.indexOf(actclient)));
}

void WindowManager::tile_maximize(xwindow *client)
{
	int i;
	
	if(tmaxclient != NULL && (i = clients.indexOf(tmaxclient)) != -1)
		clients.insert(clients.indexOf(client),  clients.takeAt(i));
		
	tile_order(client);
}

void WindowManager::toggle_tiled(void) // toggle overlapped/tiled desk 
{
	xwindow *client;
	
	if(smode)
		return;
	
	if(tdesks[adesk])
	{
		foreach(client, clients)
		{
			if(is_curdesk(client))
				client->unset_tile();
		}	
			
		tdesks[adesk] = FALSE;
		tmaxclient = NULL;

		if(focusclient != NULL && clients.indexOf(client) != -1)
			focusclient->focus_mouse();
			
		return;
	}
	tile_order(focusclient);
	tdesks[adesk] = TRUE;
}

void WindowManager::read_cprops(void)  // read app defaults
{
	QString fname,name,par;
	QByteArray cline;
	int flags;
	int apnum = 1;
	
	fname = get_cfile("appdefaults");
	
	if(fname.isNull())
		return;
	
	QFile istr(fname);
	
	if(! istr.open(QIODevice::ReadOnly))
	{
		perror("cannot open appdefaults");
		return;
	}	
	cprops.clear();
	apclients.clear();
	
	while(! istr.atEnd())
	{
		QTextStream si(istr.readLine(1024));
		si >> name;
		par = si.readLine();

		if(par.indexOf((QString("ToolBar")), Qt::CaseInsensitive) != -1)
		{
			apclients.insert(name, apnum++);
			continue;
		}
	
		flags = 0;	
		
		int i;
		int paf[] = { WindowListSkip,Sticky,SmallFrame,NoResize,NoTile,NoKey,NoScreen };
		const char *pas[] = { "WindowListSkip","Sticky","SmallFrame","NoResize","NoTile","NoKey","NoScreen" };

		for(i=0; i < 7; i++)
		{
			if(par.indexOf(pas[i]) != -1)
				flags |= paf[i];
		}
		
		if(flags)
			cprops.insert(name, flags);
	}
	istr.close();

	// check for clients to update
		
	for(int i=0; i < clients.size(); i++)
		clients.at(i)->set_pflags();
}

QString WindowManager::get_cfile(const char *name)  // get abs config file name
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
	
	fname = "/home/nicu/.qlwm/";   // system config dir
	fname += name;
	
	QFileInfo fi(fname);

	if(fi.isReadable())
		return(fname);
		
	perror((const char *)&fname);
	fname = QString();
	return(fname);
}

void WindowManager::send_configurenotify(xwindow *client) 
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
	XSendEvent(QX11Info::display(), ce.window, False, StructureNotifyMask, (XEvent *) &ce);
}

// event filter returns FALSE to pass event to qt's event handler

bool WindowManager::x11EventFilter(XEvent *event)  
{
	xwindow *client;
	Window w;
	XEvent ev;
	int i;
	XConfigureRequestEvent *cev;
	XClientMessageEvent *mev;
	XCrossingEvent *xev;
	XCirculateRequestEvent *rev;
	XExposeEvent *eev;
	XPropertyEvent *pev;

#ifdef DEBUGMSG		
#include "eventnames.h"
	if(event->type < 36 && event->type != 6)
		logmsg << "Received: " << event_names[event->type] << " (WId:" << event->xany.window << ")\n";
#endif
	if(sighup)
	{
		QString fname = get_cfile("defaults");

		if(! fname.isNull())
		{
			QFileInfo fi(fname);
			if(! (fi.lastModified() == lmtime || fi.lastModified().addSecs(180) < QDateTime::currentDateTime()))
				wm_restart();
				
			lmtime = fi.lastModified();	
		}
		read_cprops();
		sighup = FALSE;
	}

	switch(event->type)
	{
		case DestroyNotify:
			w = event->xdestroywindow.window;

			if((client = cwindows[w]) != NULL)
			{
				clients.removeAt(clients.indexOf(client));
				delete client;
				
				return TRUE;
			}	
				
			if(event->xdestroywindow.event != w)
				return TRUE;

			return FALSE;

		case MapNotify:
			if(event->xmap.event != event->xmap.window)
				return TRUE;
		
			return FALSE;

		case UnmapNotify:
			if((client = cwindows[event->xunmap.window]) != NULL)
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

			return FALSE;

		case EnterNotify:
			xev = &event->xcrossing;
			
			if(event->xcrossing.window == QX11Info::appRootWindow())
			{
				stopautofocus();
				rootptr = TRUE;
			}
			else if(mrb == FALSE && menu_open == FALSE && (client = pwindows[xev->window]) != NULL && 
			((i = clients.indexOf(client))) != -1 && ((client = clients.at(i)) != focusclient || rootptr))
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
			if((client = cwindows[event->xcolormap.window]) != NULL)
			{
				client->setcmap(event->xcolormap.colormap);
				return TRUE;
			}	
			return FALSE;

		case PropertyNotify:
			pev = &event->xproperty;
			
			if((client = cwindows[pev->window]) != NULL)
			{
				if(pev->atom == XA_WM_NORMAL_HINTS)
				{
					client->get_wmnormalhints();
				}	
				else if(pev->atom == XA_WM_HINTS)
				{
					client->get_wmhints();
				}
				else if(pev->atom == XA_WM_NAME || pev->atom == XA_WM_ICON_NAME || pev->atom == net_wm_name || pev->atom == net_wm_icon_name)
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
				
			if((client = pwindows[event->xconfigure.window]) != NULL)
			{
				while(XCheckTypedEvent(QX11Info::display(), ConfigureNotify, &ev));
			}
			return TRUE; 
			
		case ReparentNotify:
			if((client = cwindows[event->xreparent.window]) != NULL &&
			event->xreparent.parent != client->winId())
			{
				clients.removeAt(clients.indexOf(client));
			}	
			return TRUE;
		
		case ButtonPress:
			w = event->xbutton.window;
			
			if(w == QX11Info::appRootWindow())  // set focus to root window
				XSetInputFocus(QX11Info::display(), w, RevertToPointerRoot, CurrentTime);

			if(w == QX11Info::appRootWindow() )
				install_colormap(None);
				
			return FALSE;

		case ClientMessage:
			mev = &event->xclient;
			
			if(mev->message_type == wm_change_state && mev->format == 32 && 
			mev->data.l[0] == IconicState && (client = cwindows[mev->window]) != NULL)
				client->iconify();

			return TRUE;	

		case Expose:    // TB expose swallowed in Qt 4.1.3 ? 
			eev = &event->xexpose;

			return FALSE;

		case CirculateRequest:
			rev = &event->xcirculaterequest;
			
			if(rev->place == PlaceOnTop)
				XRaiseWindow(QX11Info::display(), rev->window);
			else
				XLowerWindow(QX11Info::display(), rev->window);
				
			return TRUE;

		case ConfigureRequest:
			cev = &event->xconfigurerequest;
			XWindowChanges wc;
			
			if((client = cwindows[cev->window]) != NULL)
			{
#ifdef DEBUGMSG
	logmsg << "configure request to client (WId:" << client->winId() << ")\n";
#endif	
				if(cev->value_mask & (CWWidth|CWHeight|CWX|CWY))
				{
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

				XConfigureWindow(QX11Info::display(), client->winId(), cev->value_mask, &wc);
				send_configurenotify(client);
			}
			else  // never mapped window
			{

#ifdef DEBUGMSG
	logmsg << "configure request to unreparented window (WId:" << cev->window << ")\n";

#endif	

				wc.x = cev->x;
				wc.y = cev->y;
				wc.width = cev->width;
				wc.height = cev->height;
				cev->value_mask &= (CWX|CWY|CWWidth|CWHeight);
				
				XConfigureWindow(QX11Info::display(), cev->window, cev->value_mask, &wc);	
			}
			return TRUE;
			
		case MapRequest:
			run_client(event->xmaprequest.window);
			return TRUE;
			
		default:
			if(servershapes && event->type == (ShapeEventBase + ShapeNotify))
			{
				XShapeEvent *sev = (XShapeEvent *)event;
		
				if((client = cwindows[sev->window]) != NULL)
				{
					client->reshape();
					return TRUE;
				}	
			}
	}
	return FALSE;
}
