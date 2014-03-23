/* 
*  File      : winlist.cpp
*  Written by: alinden@gmx.de
*  Copyright : GPL
*
*  Creates the winlist button
*/

#include "defs.h"
#include "defaults.h"
#include "qapp.h"
#include "toolbar.h"
#include "winlist.h"
#include "moc_winlist.cpp"

winlist::winlist(QWidget *parent, const char *name) : QPushButton(parent, name)
{
	setFixedSize(defaults::tc_height, defaults::tc_height);
	wmenu = new QPopupMenu(this);
	Q_CHECK_PTR(wmenu);
	connect(wmenu, SIGNAL(highlighted(int)), SLOT(highlight_pager(int)));

	int wh = defaults::tc_height-4;
	QImage oimg(qapp::get_cfile("winlist.xpm"));
	QImage timg(qapp::get_cfile("tiled.xpm"));

	if(! oimg.isNull())  // scale pixmap
		winlistpix.convertFromImage(oimg.smoothScale(wh, wh));

	if(! timg.isNull()) 
		tiledpix.convertFromImage(timg.smoothScale(wh, wh));
}

void winlist::set_pixmap(void)
{
	static int showtiled = 0;

	if(qapp::is_tileddesk())
	{
		if(showtiled == 1)
			return;

		setPixmap(tiledpix);
		showtiled = 1;
	}
	else
	{
		if(showtiled == 2)
			return;

		setPixmap(winlistpix);
		showtiled = 2;
	}	
}

// returns TRUE if client is fully obscured by other toplevel windows

bool winlist::isobscured(xwindow *client, Window *wins, uint nwins)
{
	xwindow *nclient;
	uint cwin;

	for(cwin=0; cwin < nwins; cwin++)  // find current window
		if(wins[cwin] == client->winId())
			break;

	if(cwin >= nwins)
		return FALSE;

	QRegion sr(client->frameGeometry());
	
	while(++cwin < nwins)
	{
		if((nclient = qapp::pwindows.find(wins[cwin])) == NULL || ! nclient->isVisible())
			continue;
		
		sr -= QRegion(nclient->frameGeometry());

		if(sr.isEmpty())
			return TRUE;
	}
	return FALSE;
}

// returns TRUE if client is below tiled windows

bool winlist::isbottom(xwindow *client, Window *wins, uint nwins)
{
	xwindow *nclient;
	uint cwin;

	for(cwin=0; cwin < nwins; cwin++) 
		if(wins[cwin] == client->winId())
			break;

	if(cwin >= nwins)
		return FALSE;

	while(++cwin < nwins)
	{
		if((nclient = qapp::pwindows.find(wins[cwin])) == NULL || ! nclient->isVisible() || ! qapp::is_curdesk(nclient))
			continue;

		if(nclient->is_tiled())
			return TRUE;
	}
	return FALSE;
}	

void winlist::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == QMouseEvent::RightButton)
		setDown(FALSE);
}

void winlist::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == QMouseEvent::RightButton)  // hidden to foreground
	{
		setDown(TRUE);
		hidden_win();
	}
	else if(event->button() == QMouseEvent::LeftButton)
		start_popup();
}

void winlist::hidden_win(void)
{
	Window w1,w2,*wins;
	uint nwins;

	if(XQueryTree(qt_xdisplay(), qt_xrootwin(), &w1, &w2, &wins, &nwins) == 0 || ! nwins)
		return;

	xwindow *win;
	int dwidth = QApplication::desktop()->width();

	for(win = clients.first(); win != NULL; win = clients.next())
	{
		if(win->isVisible() && win->x() <= dwidth && win->x() >= 0 && isobscured(win, wins, nwins))
		{
			win->raise();
			XFree(wins);
			return;
		}	
	}

	if(qapp::is_tileddesk())
	{
		for(win = clients.first(); win != NULL; win = clients.next())
		{
			if(win->isVisible() &&  win->x() <= dwidth && win->x() >= 0 && ! win->is_tiled() && isbottom(win, wins, nwins))
			{
				win->raise();
				XFree(wins);
				return;
			}
		}
	}
	XFree(wins);
}

void winlist::highlight_pager(int id)
{
	xwindow *win;

	if(qapp::smode)
		return;

	tb_pg->draw_pager();
	
	if((win = clients.at(id)) == NULL)
		return;
		
	tb_pg->add(win, TRUE);
}

void winlist::start_popup(void)
{
	if(qapp::menu_open)
		return;

	xwindow *focusclient = qapp::focusclient;
	qapp::menu_open = TRUE;
	
	setDown(TRUE);
	popup_list();

	tb_pg->draw_pager();
	qapp::menu_open = FALSE;

	XSetInputFocus(qt_xdisplay(), qt_xrootwin(), RevertToPointerRoot, CurrentTime);
	if(focusclient != NULL && clients.find(focusclient) != -1)
		focusclient->setchildfocus(CurrentTime);
}

void winlist::popup_list(void)
{
	xwindow *win;
	QString wname;
	Window rw,cw;
	int rx,ry,wx,wy;
	unsigned mr;

	for(win = clients.first(); win != NULL; win = clients.next())
	{
		if(win->iswithdrawn() || ((win->get_pflags() & qapp::WindowListSkip) && ! win->hidden_win()))
			continue;

		wname = "";
		QTextOStream wnm(&wname);
		
		if(! win->isVisible())
			wnm << '<';
			
		wnm << win->ccaption().left(100);

		if(! win->isVisible())
			wnm << '>';

	 	if(win->get_tnumber())
			wnm << '<' << win->get_tnumber() << '>';
		
		if(! win->getmachine().isNull())
			wnm << " (" << win->getmachine().left(20) << ')';
			
		wmenu->insertItem(wname, win, SLOT(focus_mouse_wlist()), 0, clients.at());
	}

	if(! defaults::show_winlist || qapp::smode)  // show at mouse position
	{
		XQueryPointer(qt_xdisplay(), qt_xrootwin(), &rw, &cw, &rx, &ry, &wx, &wy, &mr);
		wmenu->exec(QPoint(rx, ry));
	}
	else
	{
		if(! defaults::toolbar_top)   // menu above button
		{
			QPoint p = mapToGlobal(QPoint(0, 0));
			QSize s(wmenu->sizeHint());
			p.setY(p.y()-s.height());
			wmenu->exec(p);
		}
		else
			wmenu->exec(mapToGlobal(QPoint(0, height())));
	}		
		
	wmenu->clear();
	setDown(FALSE);
}
