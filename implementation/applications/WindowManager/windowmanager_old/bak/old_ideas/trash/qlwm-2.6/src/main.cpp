/* main.cpp */

#include "defs.h"
#include "conf.h"
#include "keyboard.h"
#include "winfo.h"
#include "qapp.h"
#include "defaults.h"

bool wm_init=TRUE;

void getrunprocs(void)  // get already running clients
{
	Window w,w1,w2,*wins;
	uint nwins,cwin;
	XWindowAttributes attr;

	if(XQueryTree(qt_xdisplay(), qt_xrootwin(), &w1, &w2, &wins, &nwins) == 0 || ! nwins)
		return;
	
	bool surgent = defaults::starturgent;
	defaults::starturgent = FALSE;

	for(cwin=0; cwin < nwins; cwin++)
	{
		w = wins[cwin];
		
		if(w == qapp::tb->winId())
			continue;

		XGetWindowAttributes(qt_xdisplay(), w, &attr);

		if(attr.map_state == IsViewable && ! attr.override_redirect)
			qapp::run_client(w);
	}
	XSync(qt_xdisplay(), FALSE);
	defaults::starturgent = surgent;
}

void sig_hup(int)
{
	qapp::sighup = TRUE;
}

void sig_term(int)  // terminate
{
	tb_pb->remove_all();
	clients.clear();
	XSync(qt_xdisplay(), FALSE);

	exit(0);
}

void startprg(void)  // start programs from init stack
{
	pid_t pid;
	QString prg;
	while(! defaults::initexec.isEmpty())
	{
		prg = *defaults::initexec.pop();
		
		if((pid = fork()) == 0)
		{
			execl("/bin/sh", "sh", "-c", (const char *)prg, NULL);
			perror("Exec");
			exit(1);
		}
		
		if(pid == -1)
			perror("fork");
	}
}

void make_bpixmaps(void)   // create scaled window button pixmaps
{
	int wh = defaults::windowbuttonsize-4;
	QImage limg(qapp::get_cfile("button_left.xpm"));
	QImage rimg(qapp::get_cfile("button_right.xpm"));
	
	qapp::leftwinpix = new QPixmap();
	qapp::rightwinpix = new QPixmap();
	
	if(! limg.isNull())
		qapp::leftwinpix->convertFromImage(limg.smoothScale(wh, wh));
	
	if(! rimg.isNull())
		qapp::rightwinpix->convertFromImage(rimg.smoothScale(wh, wh));
}

int xerrors(Display *d, XErrorEvent *event)
{
	char	msg[100];
	char	req[100];
	char	nmb[100];

	if(wm_init == TRUE && event->error_code == BadAccess)
	{
		cerr << "Another WM is already running\n";
		exit(1);
	}

#ifndef DEBUGMSG
	if(event->error_code == BadWindow || event->error_code == BadMatch)
		return 0;
#endif

	XGetErrorText(d, event->error_code, msg, sizeof(msg));
	sprintf(nmb, "%d", event->request_code);
	XGetErrorDatabaseText(d, "XRequest", nmb, nmb, req, sizeof(req));

	cerr << "WM: " << req << " on resource " << event->resourceid 
	<< " failed: " << msg << '\n';
		
	return 0;
}

int main(int argc, char **argv)
{
	for(int i=1; i < argc; i++)
	{
		if(strcmp(argv[i], "--version") == 0)
		{
			cerr << DVERSION;
			return(0);
		}
		
		if(strcmp(argv[i], "-restart") == 0)
			defaults::start_restart = TRUE;

		if(strcmp(argv[i], "-c") == 0 && argc > i+1)
			defaults::cfdir = argv[++i];
	}

	if(defaults::cfdir.isNull())  // get config dir
	{
		QString fname(getenv("HOME"));

		if(! fname.isNull())
			defaults::cfdir = fname + "/.qlwm";
	}
	defaults::argc = argc;
	defaults::argv = argv;
	
	qapp a(argc, argv);

	clients.setAutoDelete(TRUE);
	defaults::read_config();
	qapp::tb = new Toolbar(0, "Toolbar");
	qapp::read_cprops();
	make_bpixmaps();
	qapp::winf = new winfo(0, "winfo");
	a.setMainWidget(qapp::tb);
	
	XSetErrorHandler(xerrors);

	XIconSize *isize;
	if((isize = XAllocIconSize()) != NULL)
	{
		isize->min_width = isize->min_height = 2;
		isize->max_width = isize->max_height = 200;
		isize->width_inc = isize->height_inc = 1;

		XSetIconSizes(qt_xdisplay(), qt_xrootwin(), isize, 1);
		XFree(isize);
	}	

	if(defaults::root_pix.isNull()) 
		a.desktop()->setBackgroundColor(defaults::root_bg);
	else
		a.desktop()->setBackgroundPixmap(QPixmap(defaults::root_pix));

	qapp::upal = new QPalette(defaults::urgent_bg, defaults::urgent_bg);
	qapp::ipal = new QPalette(defaults::inactive_bg, defaults::inactive_bg);

	XSelectInput(qt_xdisplay(), qt_xrootwin(), SubstructureRedirectMask);
	XSync(qt_xdisplay(), FALSE);

	if(defaults::sttiled[0])
	{
		qapp::tdesks[0] = TRUE;
		defaults::sttiled[0] = FALSE;
	}
	tb_wl->set_pixmap();
	getrunprocs();
	defaults::start_restart = FALSE;

	XSelectInput(qt_xdisplay(), qt_xrootwin(),
	SubstructureNotifyMask|SubstructureRedirectMask| 
	ButtonPressMask|
	PropertyChangeMask|
	KeyPressMask|
	ColormapChangeMask|
	EnterWindowMask);

	XSync(qt_xdisplay(), FALSE);

	struct sigaction act;
	
	act.sa_handler = sig_hup;
	act.sa_flags = SA_RESTART;
	sigaction(SIGHUP, &act, NULL);

	act.sa_handler = sig_term;
	sigaction(SIGTERM, &act, NULL);

	keyboard::init();
	wm_init = FALSE;
	startprg();
	
	return(a.exec());
}
