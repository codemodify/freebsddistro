/*
*  File      : wborder.cpp
*  Written by: alinden@gmx.de
*  Copyright : GPL
*
*  Creates the window borders
*/

#include "defs.h"
#include "defaults.h"
#include "qapp.h"
#include "moc_wborder.cpp"

wframe::wframe(QWidget *parent, const char *name) : QLabel(parent, name)
{
	setFrameStyle(QFrame::Panel|QFrame::Raised);
	setLineWidth(1);
	setMinimumWidth(1);
}

void wframe::set_text(QString btxt, bool center)
{
	setAlignment(Qt::SingleLine|(center?Qt::AlignCenter:Qt::AlignLeft|Qt::AlignVCenter));
	setText(btxt);
}

void wframe::resizeEvent(QResizeEvent *)
{
	repaint();
}

void wframe::mousePressEvent(QMouseEvent *event)
{
	emit press(event);
	emit press();

	if(event->button() == QMouseEvent::RightButton)
	{
		emit right_press();
	}	
	else if(event->button() == QMouseEvent::LeftButton)
	{
		emit left_press(event);
		emit left_press();
	}
	else if(event->button() == QMouseEvent::MidButton)
	{
		emit mid_press();
	}
}

void wframe::mouseReleaseEvent(QMouseEvent *event)
{
	emit release(event);

	if(event->button() == QMouseEvent::RightButton)
	{
		emit right_release();
	}
	else if(event->button() == QMouseEvent::LeftButton)
	{
		emit left_release(event);
	}
	else if(event->button() == QMouseEvent::MidButton)
	{
		emit mid_release();
	}
}

void wframe::mouseMoveEvent(QMouseEvent *event)
{
	emit mouse_move(event);
}

lborder::lborder(QWidget *parent, const char *name) : QWidget(parent, name)
{
	layout = new QHBoxLayout(this);
	Q_CHECK_PTR(layout);
	leftframe = new wframe(this, "lleftframe");
	Q_CHECK_PTR(leftframe);
	midframe = new wframe(this, "lmidframe");
	Q_CHECK_PTR(midframe);
	rightframe = new wframe(this, "lrightframe");
	Q_CHECK_PTR(rightframe);
	
	leftframe->setFixedWidth(defaults::lowerborderwidth);
	rightframe->setFixedWidth(defaults::lowerborderwidth);
	setFixedHeight(defaults::lowerborderheight);

	leftframe->setCursor(QCursor(Qt::sizeBDiagCursor));
	midframe->setCursor(QCursor(Qt::sizeVerCursor));
	rightframe->setCursor(QCursor(Qt::sizeFDiagCursor));

	layout->add(leftframe);
	layout->add(midframe);
	layout->add(rightframe);
}

void uborder::set_small(void)
{
	setFixedHeight(defaults::lowerborderheight);
	leftframe->setFixedWidth(defaults::lowerborderwidth);
	rightframe->setFixedWidth(defaults::lowerborderwidth);
	leftframe->setPixmap(QPixmap());
	rightframe->setPixmap(QPixmap());
	midframe->setText("");
}

void uborder::set_normal(void)
{
	setFixedHeight(defaults::windowbuttonsize);
	leftframe->setFixedWidth(defaults::windowbuttonsize);
	rightframe->setFixedWidth(defaults::windowbuttonsize);
	leftframe->setPixmap(*qapp::leftwinpix);
	rightframe->setPixmap(*qapp::rightwinpix);
}

uborder::uborder(bool showleft, QWidget *parent, const char *name) : QWidget(parent, name)
{
	layout = new QHBoxLayout(this);
	Q_CHECK_PTR(layout);
	setFixedHeight(defaults::windowbuttonsize);

	if(showleft)
	{
		leftframe = new wframe(this, "uleftframe");
		Q_CHECK_PTR(leftframe);
		leftframe->setFixedWidth(defaults::windowbuttonsize);
		leftframe->setAlignment(Qt::AlignCenter);
		leftframe->setPixmap(*qapp::leftwinpix);
			
		layout->add(leftframe);
	}
	
	midframe = new wframe(this, "umidframe");
	Q_CHECK_PTR(midframe);
	rightframe = new wframe(this, "urightframe");
	Q_CHECK_PTR(rightframe);
	rightframe->setFixedWidth(defaults::windowbuttonsize);
	rightframe->setAlignment(Qt::AlignCenter);
	rightframe->setPixmap(*qapp::rightwinpix);

	layout->add(midframe);
	layout->add(rightframe);
}
