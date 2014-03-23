/*
*  File      : procbar.cpp
*  Written by: alinden@gmx.de
*  Copyright : GPL
*
*  Creates the buttons for iconified windows
*/

#include "defs.h"
#include "defaults.h"
#include "procbar.h"

// max. button width
#define max_bwidth 200

procbar::procbar(QWidget *parent, const char *name) : QWidget(parent, name)
{
	wlist.setAutoDelete(TRUE);
	setFont(defaults::toolbarfont);
}

QPixmap *procbar::make_pixmap(const QString &txt, QPixmap *pix)
{
	int txtstart = 4;
	int h=height()-4;
	QPixmap *newpix = new QPixmap(max_bwidth, height());
	CHECK_PTR(newpix);
	newpix->fill(foregroundColor());
	
	QBitmap mask(max_bwidth, height(), TRUE);  // all transparent
	QPainter painter(&mask);
	painter.setFont(font());

	if(! pix->isNull())   // draw scaled pixmap
	{
		bitBlt(newpix, 2, 2, pix, 0, 0);
		bitBlt(&mask, 2, 2, pix->mask(), 0, 0);
		txtstart = h+4;
	}
	painter.drawText(txtstart, 0, max_bwidth-txtstart, h, AlignLeft|AlignVCenter, txt);
	painter.end();
	newpix->setMask(mask);

	return(newpix);
}

void procbar::change_palette(const QPalette &bpal, xwindow *win)
{
	WINLIST *w;
	
	for(w=wlist.first(); w != NULL; w=wlist.next())
		if(w->win == win)
			break;
	
	if(w == NULL)
		return;
	
	w->button->setPalette(bpal);
}

void procbar::change_text(QString *icname, xwindow *win)
{	
	WINLIST *w;
	
	for(w=wlist.first(); w != NULL; w=wlist.next())
		if(w->win == win)
			break;
	
	if(w == NULL)
		return;
	
	delete w->pixmap;
	w->pixmap = make_pixmap(*icname, win->icon());
	draw_button(w);
}

bool procbar::remove_fg(void)   // return FALSE if nothing to delete
{
	WINLIST *w;
	
	for(w=wlist.first(); w != NULL; w=wlist.next())
	{
		if(w->win->isVisible())
		{
			delete w->button;
			delete w->pixmap;
			wlist.remove(w);
			
			return(TRUE);
		}
	}
	return(FALSE);
}

void procbar::draw_button(WINLIST *w)
{
	QPixmap pix(bwidth, height());
	bitBlt(&pix, 0, 0, w->pixmap, 0, 0);

	QBitmap mask(bwidth, height());
	bitBlt(&mask, 0, 0, w->pixmap->mask(), 0, 0);
	pix.setMask(mask);

	w->button->setPixmap(pix);
}

void procbar::draw_buttons(void)
{
	WINLIST *w;
	
	if(wlist.isEmpty())
		return;
	
	setUpdatesEnabled(FALSE);
		
	int h = height();
	int pos = 0;     // current position
	bwidth = (width()/wlist.count())-1;   // button width

	if(bwidth < h)         // scroll on overflow
	{
		bwidth = h;

		int tsize = wlist.count()*(h+1);
		int maxscr = ((tsize-width())/(h+1))+1;
		w=wlist.first();
		for(int i=0; i < maxscr; i++)
		{
			w->button->hide();
			w = wlist.next();
		}
	}
	else
	{
		w = wlist.first();
	
		if(bwidth > max_bwidth)
			bwidth = max_bwidth-1;
	}
	
 	while(w != NULL)
	{
		w->button->show();
		w->button->setGeometry(pos, 0, bwidth, h);
		draw_button(w);

		if(! w->button->isVisible())
			w->button->show();

		pos += bwidth+1;
		w = wlist.next();
	}
	setUpdatesEnabled(TRUE);
}

void procbar::add(xwindow *win)
{
	WINLIST *w;
	
	if(win == NULL)
		return;

	for(w = wlist.first(); w != NULL; w = wlist.next())
	{
		if(w->win == win)    // window already on bar
		{
			w->button->setOn(FALSE);
			return;
		}
	}
	int i = wlist.count(); 
	while(i--*max_bwidth > width() && remove_fg());
	
	w = new WINLIST;
	CHECK_PTR(w);
	w->button = new QPushButton(this, "procbar_button");
	CHECK_PTR(w->button);
	w->button->lower();  // last to redraw
	w->pixmap = make_pixmap(win->icaption(), win->icon());
	w->win = win;

	w->button->setToggleButton(TRUE);
	connect(w->button, SIGNAL(toggled(bool)), win, SLOT(state(bool)));

	wlist.append(w);
	draw_buttons();
}

void procbar::set_on(xwindow *win)
{
	WINLIST *w;
	
	for(w = wlist.first(); w != NULL; w = wlist.next())
	{
		if(w->win == win)    // window on bar
		{
			w->button->setOn(TRUE);
			break;
		}
	}
	return;
}	

void procbar::resizeEvent(QResizeEvent *)
{
	draw_buttons();
}

void procbar::paletteChange(const QPalette &)
{
	WINLIST *w;

	for(w=wlist.first(); w != NULL; w = wlist.next())
	{
		delete w->pixmap;
		w->pixmap = make_pixmap(w->win->icaption(), w->win->icon());
	}	
	
	draw_buttons();
}

void procbar::remove(xwindow *win)   // remove from bar
{
	WINLIST *w;

	for(w=wlist.first(); w != NULL && w->win != win; w = wlist.next());

	if(w == NULL)
		return;

	delete w->button;
	delete w->pixmap;
	wlist.remove();

	draw_buttons();
}

void procbar::remove_all(void)
{
	WINLIST *w;

	for(w=wlist.first(); w != NULL; w = wlist.next())
	{
		delete w->button;
		delete w->pixmap;
	}
	wlist.clear();
}

procbar::~procbar(void)
{
	WINLIST *w;
	
	for(w=wlist.first(); w != NULL; w = wlist.next())
		delete w->pixmap;
}
