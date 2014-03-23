/* winfo.cpp */

#include "defs.h"
#include "qapp.h"
#include "moc_winfo.cpp"

static int qcf[] = { qapp::Sticky,qapp::WindowListSkip,qapp::SmallFrame,qapp::NoResize,
qapp::NoTile,qapp::NoKey,qapp::NoScreen,qapp::ClassOnly };

winfo::winfo(QWidget *parent, const char *name) : QWidget(parent, name)
{
	client = NULL;
	XSetWindowBorderWidth(qt_xdisplay(), winId(), 1);

	left = new QLabel(this);
	CHECK_PTR(left);
	right = new QLabel(this);
	CHECK_PTR(right);
	right->setAutoResize(TRUE);

	int i;
	QVBoxLayout *vl = new QVBoxLayout(this);
	vl->setMargin(2);
	
	static char *qcs[] = { "Sticky","Skip windowlist","Small frame","No resize","No tiling",
	"Skip keyboard access","Skip in screenmode","Apply to all in class" };

	for(i=0; i < 8; i++)
	{
		qcbox[i] = new QCheckBox(qcs[i], this);
		vl->add(qcbox[i]);
	}

	QString txtl;
	QTextOStream tsl(&txtl);
	
	tsl << " Title"      << '\n'
	    << " Name"       << '\n' 
	    << " Class"      << '\n'
	    << " Location"   << '\n'
	    << " Invocation" << '\n'
	    << " Geometry";
	    
	left->setText(txtl);

	QHBoxLayout *hl1 = new QHBoxLayout(vl);
	hl1->add(left);
	hl1->add(right);
	hl1->addStretch();
	vl->addSpacing(4);
	QHBoxLayout *hl2 = new QHBoxLayout(vl);
	qb1 = new QPushButton("Permanent", this);
	qb2 = new QPushButton("Temporary", this);
	qb3 = new QPushButton(" Cancel  ", this);
	hl2->add(qb1);
	hl2->addStretch();
	hl2->add(qb2);
	hl2->addStretch();
	hl2->add(qb3);
	connect(qb1, SIGNAL(pressed()), this, SLOT(release_permanent()));
	connect(qb2, SIGNAL(pressed()), this, SLOT(release_temporary()));
	connect(qb3, SIGNAL(pressed()), this, SLOT(release_cancel()));
}

void winfo::show_info(xwindow *cl, QString &wmname, QString &clientfqdn, QString &command,
QString &rn, QString &rc, int inc_w, int inc_h, int base_w, int base_h)
{
	QWidget *dt = QApplication::desktop();
	int i;
	
	if(client != NULL)
	{
		release_cancel();
		return;
	}	
		
	qapp::menu_open = TRUE;
	client = cl;
	res_name = rn;
	res_class = rc;
	
	int cx,cw,ch;
	cx = client->x()+(qapp::adesk*dt->width());
	cw = (inc_w > 1)?((client->width()-base_w)/inc_w):(client->width());
	ch = (inc_h > 1)?((client->height()-base_h)/inc_h):(client->height());

	QString gm;
	gm.sprintf("%dx%d+%d+%d", cw, ch, cx, client->y());

	QString txtr;
	QTextOStream tsr(&txtr);

	tsr << ": " << wmname.left(100)     << " \n"
	    << ": " << res_name.left(100)   << " \n"
	    << ": " << res_class.left(100)  << " \n"
	    << ": " << clientfqdn.left(100) << " \n"
	    << ": " << command.left(100)    << " \n"
	    << ": " << gm << ' ';

	right->setText(txtr);

	for(i=0; i < 8; i++)
		qcbox[i]->setChecked((client->get_pflags() & qcf[i])?TRUE:FALSE);

	setFixedWidth(((qb1->width()*3 > left->width()+right->width())?qb1->width()*3:left->width()+right->width())+4);
	move((dt->width()/2)-(width()/2), (dt->height()/2)-(height()/2));
	XRaiseWindow(qt_xdisplay(), winId());
	show();
}

void winfo::release_permanent(void)
{
	if(client == NULL)
		return;
		
	QString fname,cclname;

	if(qcbox[7]->isChecked())  // class only 
	{
		cclname = client->get_clientname();
	}
	else
	{
		cclname = res_name;
		cclname += ',';
		cclname += res_class;
	}
	
	if(defaults::cfdir.isNull())
	{
		release_temporary();
		return;
	}
	
	fname = defaults::cfdir;
	fname += "/appdefaults";

	QFile apfile(fname);
	QString cln;
	QList<QString> lns;
	lns.setAutoDelete(TRUE);
	
	if(apfile.open(IO_ReadOnly))
	{
		while(! apfile.atEnd())
		{
			apfile.readLine(cln, 1024);

			if(cln.find(cclname) != 0)
				lns.append(new QString(cln));
		}
		apfile.close();
	}

	if(! apfile.open(IO_WriteOnly))
	{
		perror((const char *)fname);
		release_temporary();
		return;
	}
	
	QString *cur;
	
	for(cur = lns.first(); cur != NULL; cur = lns.next())
			apfile.writeBlock((const char *)*cur, cur->length());
	
	int i;
	bool ref=FALSE;
	static char *qcs[] = { "Sticky","WindowListSkip","SmallFrame","NoResize","NoTile","NoKey","NoScreen" };
	QString ostr;
	
	for(i=0; i < 7; i++)
	{
		if(qcbox[i]->isChecked())
		{
			if(! ref)
			{
				ostr = cclname;
				ostr += ' ';
				ref = TRUE;
			}
			else
				ostr = ',';

			ostr += qcs[i];
			apfile.writeBlock((const char *)ostr, ostr.length());
		}
	}
	if(ref)
		apfile.writeBlock("\n", 1);

	apfile.close();	

	int tl = (client->get_pflags() & (qapp::NoTile|qapp::Sticky));
	qapp::read_cprops();

	if(qapp::is_tileddesk())
	{
		if(tl && ! (client->get_pflags() & (qapp::NoTile|qapp::Sticky)))
		{
			qapp::tile_order(client);
		}
		else if(! tl && (client->get_pflags() & (qapp::NoTile|qapp::Sticky)))
		{
			qapp::tile_order(qapp::tmaxclient);
			client->raise();
		}
	}	
	release_cancel();
}

void winfo::release_temporary(void)
{
	if(client == NULL)
		return;
		
	int flags=0,i;
	
	for(i=0; i < 8; i++)
	{
		if(qcbox[i]->isChecked())
			flags |= qcf[i];
	}

	int tl = (client->get_pflags() & (qapp::NoTile|qapp::Sticky));
	client->set_pflags(flags);

	if(qapp::is_tileddesk())
	{
		if(tl && ! (client->get_pflags() & (qapp::NoTile|qapp::Sticky)))
		{
			qapp::tile_order(client);
		}
		else if(! tl && (client->get_pflags() & (qapp::NoTile|qapp::Sticky)))
		{
			qapp::tile_order(qapp::tmaxclient);
			client->raise();
		}
	}	
	release_cancel();
}

void winfo::release_cancel(void)
{
	if(client == NULL)
		return;

	hide();
	XSync(qt_xdisplay(), FALSE);
	qb1->setDown(FALSE);
	qb2->setDown(FALSE);
	qb3->setDown(FALSE);
	client = NULL;
	qapp::menu_open = FALSE;
}
