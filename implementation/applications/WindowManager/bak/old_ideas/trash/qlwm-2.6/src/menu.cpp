/*
*  File      : menu.cpp
*  Written by: alinden@gmx.de
*  Copyright : GPL
*
*  Creates menu button and the XKill function
*/

#include "defs.h"
#include "defaults.h"
#include "qapp.h"
#include "toolbar.h"
#include "conf.h"
#include "moc_menu.cpp"

#define kcursor_width 32
#define kcursor_height 32
uchar menu::kcursor_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0x01,
   0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x08, 0x10, 0x01,
   0x80, 0x10, 0x08, 0x01, 0x80, 0x20, 0x04, 0x01, 0x80, 0x40, 0x02, 0x01,
   0x80, 0x80, 0x01, 0x01, 0x80, 0x80, 0x01, 0x01, 0x80, 0x40, 0x02, 0x01,
   0x80, 0x20, 0x04, 0x01, 0x80, 0x10, 0x08, 0x01, 0x80, 0x08, 0x10, 0x01,
   0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0xff, 0xff, 0x01,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uchar menu::kcursor_mask[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x03, 0xc0, 0xff, 0xff, 0x03,
   0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x1c, 0x38, 0x03,
   0xc0, 0x38, 0x1c, 0x03, 0xc0, 0x70, 0x0e, 0x03, 0xc0, 0xe0, 0x07, 0x03,
   0xc0, 0xc0, 0x03, 0x03, 0xc0, 0xc0, 0x03, 0x03, 0xc0, 0xe0, 0x07, 0x03,
   0xc0, 0x70, 0x0e, 0x03, 0xc0, 0x38, 0x1c, 0x03, 0xc0, 0x1c, 0x38, 0x03,
   0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0xff, 0xff, 0x03,
   0xc0, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


menu::menu(QWidget *parent, const char *name) : QPushButton(parent, name)
{
	setFixedSize(defaults::tc_height, defaults::tc_height);
	killop = FALSE;
	basemenu = new QPopupMenu(this, "basemenu");
	CHECK_PTR(basemenu);
	connect(basemenu, SIGNAL(activated(int)), this, SLOT(run_cmd(int)));
	
	mlist.setAutoDelete(TRUE);
	mdict.setAutoDelete(TRUE);
	QImage img(qapp::get_cfile("menu.xpm"));

	if(! img.isNull())  // make scaled pixmap
	{
		int wh = defaults::tc_height-4;
		QPixmap pix;
		pix.convertFromImage(img.smoothScale(wh, wh));
		setPixmap(pix);
	}
}

void menu::readmenu(void)
{
	QString fname = qapp::get_cfile("menuconfig");

	if(fname.isNull())
		return;
		
	QFileInfo fi(fname);

	if(fi.lastModified() == lmtime)
		return;

	lmtime == fi.lastModified();

	basemenu->clear();   
	mdict.clear();
	mlist.clear();
	
	QFile istr(fname);
	if(! istr.open(IO_ReadOnly))
	{
		perror("cannot open menu file");
		return;
	}	

	recurse(basemenu, istr);
	istr.close();
}

void menu::winkill(void)
{
	grabMouse(QCursor(QBitmap(kcursor_width, kcursor_height, kcursor_bits, TRUE),
	          QBitmap(kcursor_width, kcursor_height, kcursor_mask, TRUE)));
	
	killop = TRUE;
}

void menu::recurse(QPopupMenu *cm, QFile &istr)
{
	QString mline,p1,p2,p3;
	
	while(! istr.atEnd())
	{
		istr.readLine(mline, 1024);
		QTextIStream til(&mline);

		til >> p1;  // command 
		if(p1.isEmpty() || *(const char *)p1 == '#')
			continue;
		
		if(p1 == "End")
			return;

		if(p1 == "Separator")
		{
			cm->insertSeparator();
			continue;
		}	

		til >> p2;            // menu label
		
		if(p2.isEmpty())
			continue;
			
		p3 = til.readLine();  // command line

		if(p2[0] == '"')
		{
			if(p2[p2.length()-1] != '"')
			{
				int i = p3.find('"');
				if(i == -1)
					continue;
				
				p2 += p3.left(i);
				p2 = p2.mid(1, p2.length()-1);
				p3 = p3.right(p3.length()-i-1);
			}
			else p2 = p2.mid(1, p2.length()-2);
		}
		
		if(p1 == "Menu")
		{
			QPopupMenu *nm = new QPopupMenu(this, "menu_submenu");
			CHECK_PTR(nm);
			connect(nm, SIGNAL(activated(int)), this, SLOT(run_cmd(int)));
			mlist.append(nm);
			cm->insertItem(p2, nm);
			recurse(nm, istr);
			continue;
		}

		if(p1 == "Quit")
		{
			mdict.insert(basemenu->insertItem(p2), new QString("QUIT"));
			continue;
		}
		
		if(p1 == "Kill")
		{
			mdict.insert(basemenu->insertItem(p2), new QString("KILL WINDOW"));
			continue;
		}	
		
		if(p1 != "Entry")
			continue;

		p3.stripWhiteSpace();
		
		if(p3.isEmpty())
			continue;
			
		mdict.insert(cm->insertItem(p2), new QString(p3));
	}
}	

void menu::run_cmd(int id)  // execute menu item
{
	pid_t pid;

	QString cmd = *mdict.find(id);

	if(cmd == "KILL WINDOW")
			winkill();
	else if(cmd == "QUIT") {
			winkill_all();
			qApp->exit();
	} else
			goto LBL_UNMATCHED_ID;

	return;

LBL_UNMATCHED_ID:

	if((pid = fork()) == -1)
	{
		perror("fork()");
		return;
	}	
	if(pid)
		return;

	execl("/bin/sh", "sh", "-c", (const char*)cmd, NULL);
	perror("cannot run /bin/sh");
	exit(1);
}

void menu::start_popup(void)
{	
	Window rw,cw;
	int rx,ry,wx,wy;
	unsigned mr;

	if(qapp::menu_open)
		return;
	
	xwindow *focusclient = qapp::focusclient;
	qapp::menu_open = TRUE;
	
	setDown(TRUE);

	if(! defaults::show_menu || qapp::smode)  // show at mouse position
	{
		XQueryPointer(qt_xdisplay(), qt_xrootwin(), &rw, &cw, &rx, &ry, &wx, &wy, &mr);
		basemenu->exec(QPoint(rx, ry));
	}
	else
	{
		if(! defaults::toolbar_top)   // menu above button
		{
			QPoint p = mapToGlobal(QPoint(0, 0));
			QSize s(basemenu->sizeHint());
			p.setY(p.y()-s.height());
			basemenu->exec(p);
		}
		else
			basemenu->exec(mapToGlobal(QPoint(0, height())));
	}
	qapp::menu_open = FALSE;
	setDown(FALSE);
	
	XSetInputFocus(qt_xdisplay(), qt_xrootwin(), RevertToPointerRoot, CurrentTime);
	if(focusclient != NULL && clients.find(focusclient) != -1)
		focusclient->setchildfocus(CurrentTime);
}

void menu::mousePressEvent(QMouseEvent *event)
{
	if(killop)  //  window kill operation
	{
		xwindow *client;
		
		killop = FALSE;
		releaseMouse();
		
		if(event->button() != QMouseEvent::LeftButton)
			return;
		
		if((client = (xwindow *)QApplication::widgetAt(event->globalPos())) == NULL)
			return;
		
		XKillClient(qt_xdisplay(), client->client_id());
		XSync(qt_xdisplay(), FALSE);
		
		return;
	}
	start_popup();
}


void menu::winkill_all()
{
	xwindow *client;

	tb_pb->remove_all();
	for(client = clients.first(); client != NULL; client = clients.next()) {
		client->wdestroy();
	}
	clients.clear();
	XSync(qt_xdisplay(), FALSE);
}
