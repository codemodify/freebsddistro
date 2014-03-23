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

wframe::wframe(QWidget *parent, const char *name) : QFrame(parent, name)
{
	setFrameStyle(QFrame::Panel|QFrame::Raised);
	setLineWidth(1);
	redraw_contents = TRUE;
	backpix = NULL;
}

void wframe::set_backpix(QPixmap *pix, bool center)
{
	backpix = pix;
	centerpix = center;
}

void wframe::set_redraw(bool sr)
{
	redraw_contents = sr;
}

void wframe::drawContents(QPainter *)
{
	if(redraw_contents && backpix != NULL && ! backpix->isNull())
	{
		if(centerpix)
		{
			int bmid = backpix->width()/2;
			int fmid = width()/2;
			
			if(backpix->width()+4 > width())
				bitBlt(this, 2, 2, backpix, bmid-fmid, 0, width()-4);
			else
				bitBlt(this, fmid-bmid+2, 2, backpix, 0, 0);
		}
		else bitBlt(this, 2, 2, backpix, 0, 0, width()-4);  // left align
	}
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
	CHECK_PTR(layout);
	leftframe = new wframe(this, "lleftframe");
	CHECK_PTR(leftframe);
	midframe = new wframe(this, "lmidframe");
	CHECK_PTR(midframe);
	rightframe = new wframe(this, "lrightframe");
	CHECK_PTR(rightframe);
	
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
	leftframe->set_redraw(FALSE);
	midframe->set_redraw(FALSE);
	rightframe->set_redraw(FALSE);
	setFixedHeight(defaults::lowerborderheight);
	leftframe->setFixedWidth(defaults::lowerborderwidth);
	rightframe->setFixedWidth(defaults::lowerborderwidth);
}

void uborder::set_normal(void)
{
	leftframe->set_redraw(TRUE);
	midframe->set_redraw(TRUE);
	rightframe->set_redraw(TRUE);
	setFixedHeight(defaults::windowbuttonsize);
	leftframe->setFixedWidth(defaults::windowbuttonsize);
	rightframe->setFixedWidth(defaults::windowbuttonsize);
}

uborder::uborder(bool showleft, QWidget *parent, const char *name) : QWidget(parent, name)
{
	layout = new QHBoxLayout(this);
	CHECK_PTR(layout);
	setFixedHeight(defaults::windowbuttonsize);

	if(showleft)
	{
		leftframe = new wframe(this, "uleftframe");
		CHECK_PTR(leftframe);
		leftframe->setFixedWidth(defaults::windowbuttonsize);
		leftframe->set_backpix(qapp::leftwinpix, FALSE);
			
		layout->add(leftframe);
	}
	
	midframe = new wframe(this, "umidframe");
	CHECK_PTR(midframe);
	rightframe = new wframe(this, "urightframe");
	CHECK_PTR(rightframe);
	rightframe->setFixedWidth(defaults::windowbuttonsize);
	rightframe->set_backpix(qapp::rightwinpix, FALSE);

	layout->add(midframe);
	layout->add(rightframe);
}
