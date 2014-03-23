/* 
*  File      :  biff.cpp
*  Written by:  alinden@gmx.de
*  Copyright :  GPL
*
*  Pops up a small envelope if mail is present.
*  Changes color if new mail arrives.
*/

#ifndef MAILBOX_H
#define MAILBOX_H

#include <qwidget.h>
#include <qpixmap.h>

class biff : public QWidget
{
	Q_OBJECT

	QPixmap     hasmail;
	QPixmap     newmail;
	QString     mailfile;
	int         mailstate;

	enum state { Empty, Hasmail, Newmail };
	
protected:
	void	timerEvent(QTimerEvent *);
	void	paint_mailbox(void);
    
public:
	biff(QWidget *parent=0, const char *name=0);
};

class mailbox : public QWidget
{
    QPixmap	envelope;
    void 	paint_mailbox(void);

protected:
    void	paintEvent( QPaintEvent * );
    void	resizeEvent( QResizeEvent * );
    
public:
    mailbox(QWidget *parent=0, const char *name=0);
    void 	set_pixmap(QPixmap &);
};
#endif
