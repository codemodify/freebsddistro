/*
*  File     : toolbar.cpp
*  Writen by: alinden@gmx.de
*  Copyright: GPL
*
*  Draw the toolbar and place items on it
*/

#include "defs.h"
#include "qapp.h"
#include "toolbar.h"

pager   *tb_pg;     // pager
winlist *tb_wl;     // winlist
menu    *tb_mn;     // menu
procbar *tb_pb;     // procbar
apbar   *tb_ap;     // apbox

Toolbar::Toolbar(QWidget *parent, const char *name) : QFrame(parent, name)
{
	layout = new QHBoxLayout(this);
	layout->setSpacing(1);
	
	setFrameStyle(QFrame::Panel|QFrame::Raised);
	setLineWidth(1);

	setGeometry(0, (defaults::toolbar_top)?(0):(QApplication::desktop()->height()-defaults::tb_height),
	QApplication::desktop()->width(), defaults::tb_height);
	layout->addSpacing(4);

	// pager
	tb_pg = new pager(this, "pager");
	layout->add(tb_pg);
	layout->addSpacing(3);

	// winlist
	if(defaults::show_winlist)
	{
		tb_wl = new winlist(this, "winlist");
		layout->add(tb_wl);
	}
	else
		tb_wl = new winlist(0, "winlist");

	// menu
	if(defaults::show_menu)
	{
		tb_mn = new menu(this, "menu");
		layout->add(tb_mn);
	}	
	else
		tb_mn = new menu(0, "menu");
		
	tb_mn->readmenu();

	if(defaults::show_menu || defaults::show_winlist)
		addsep();

	// procbar
	tb_pb = new procbar(this, "procbar");
	tb_pb->setFixedHeight(defaults::tc_height);
	layout->add(tb_pb);

	tb_ap = new apbar(this, "apbar");
	layout->add(tb_ap);

	layout->addSpacing(2);
	show();
}

void Toolbar::paletteChange(const QPalette &)
{
	xwindow *client;

	for(client=clients.first(); client != NULL; client = clients.next())
		client->set_title();
}

void Toolbar::addsep(void)
{
	layout->addSpacing(3);
	QFrame *frame = new QFrame(this);
	frame->setLineWidth(1);
	frame->setMidLineWidth(0);
	frame->setFrameStyle(QFrame::VLine|QFrame::Sunken);
	frame->setFixedHeight(height()-4);
	layout->addWidget(frame);
	layout->addSpacing(3);
}
