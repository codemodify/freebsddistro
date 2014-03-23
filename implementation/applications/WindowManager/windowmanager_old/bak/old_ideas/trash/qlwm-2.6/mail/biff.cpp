/* 
*  File      :  biff.cpp
*  Written by:  alinden@gmx.de
*  Copyright :  GPL
*
*  Pops up a small envelope if mail is present.
*  Changes color if new mail arrives.
*/

#include <qfileinfo.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "conf.h"
#include "mail.xpm"
#include "newmail.xpm"
#include "moc_biff.cpp"

biff::biff(QWidget *parent, const char *name) : QWidget(parent, name)
{
	QString s(getenv("MAIL"));
	QFileInfo fi = s;
	if(! fi.exists()) 
	{
		s = MAILDIR;
		s += getlogin();
		fi.setFile(s);
	}

	if(fi.exists())
	{
		mailfile = fi.absFilePath();
		startTimer(3000);
    	}
	else
	{
		perror((const char *)s);
		exit(1);
	}

	hasmail = QPixmap((const char **)mail_xpm);
	newmail = QPixmap((const char **)newmail_xpm);
	
	mailstate = Empty;
}

void biff::timerEvent(QTimerEvent *)
{
	static mailbox *mbox = NULL;
	
	QFileInfo fi(mailfile);
	
	if(mailstate != Empty && fi.size() == 0)
	{
		if(mbox != NULL)
		{
			delete mbox;
			mbox = NULL;
		}
		mailstate = Empty;
	}
	else if(mailstate != Newmail && fi.lastModified() > fi.lastRead())
	{
		if(mbox == NULL)
			mbox = new mailbox();

		mbox->set_pixmap(newmail);
		mailstate = Newmail;
	}
	else if(mailstate != Hasmail && fi.size() && fi.lastRead() > fi.lastModified())
	{
		if(mbox == NULL)
			mbox = new mailbox();

		mbox->set_pixmap(hasmail);
		mailstate = Hasmail;
	}
}

mailbox::mailbox(QWidget *parent, const char *name) : QWidget(parent, name)
{
	resize(16, 16);
	show();
}

void mailbox::set_pixmap(QPixmap &pix)
{
	envelope = pix;
	repaint(FALSE);
}

void mailbox::paintEvent(QPaintEvent *)
{
	paint_mailbox();
}

void mailbox::resizeEvent(QResizeEvent *)
{
	paint_mailbox();
}

void mailbox::paint_mailbox(void)
{
	bitBlt(this, 0, (height()/2)-(envelope.height()/2), &envelope);
}
