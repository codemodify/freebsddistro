/*  
*   File      : dclock.h
*   Written by: alinden@gmx.de
*   Copyright : GPL
*
*   Shows a label with current time.
*/

#include <qwidget.h>
#include <qdatetime.h>
#include <qtimer.h>
#include <qfont.h>
#include <qlabel.h>

class dclock : public QLabel
{
	Q_OBJECT
	
	QTime time;
	void settime(QTime &);
	
public slots:
	void timeout(void);

protected:
	virtual void resizeEvent(QResizeEvent *);
	
public:
	dclock(QWidget *parent=0, const char *name=0);
};
