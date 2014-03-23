/*  
*   File      : dclock.cpp
*   Written by: alinden@gmx.de
*   Copyright : GPL
*
*   Shows a label with current time.
*/

#include "moc_dclock.cpp"

dclock::dclock(QWidget *parent, const char *name) : QLabel(parent, name)
{
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), SLOT(timeout()));
	setAutoResize(TRUE);
	QTime newtime = QTime::currentTime();
	settime(newtime);
	setFixedWidth(width());
	setAutoResize(FALSE);
	timer->start(5000);  // signal every 5 seconds
}

void dclock::timeout(void)
{
	QTime newtime = QTime::currentTime();
	if(newtime.minute() != time.minute())
		settime(time = newtime);
}

void dclock::settime(QTime &tm)
{
	setText(tm.toString().left(5));
}

void dclock::resizeEvent(QResizeEvent *)
{
	QFont dcfont(QApplication::font());
	
	if(dcfont.pixelSize() > height())
	{
		dcfont.setPixelSize(height());
		setFont(dcfont);
	}
	else
		setFont(QApplication::font());
}
