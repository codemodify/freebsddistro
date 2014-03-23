
// Qt headers
#include <QtGui/QWidget>
#include <QtGui/QPalette>
#include <QtGui/QPixmap>
#include <QtGui/QBrush>
#include <QtGui/QMouseEvent>

// system headers
#include <iostream>
using namespace std;

// local headers
#include "EventFilter/MouseEventFilter.h"
#include "SkinWindow.h"


SkinWindow::SkinWindow( QWidget* window ) :
	SkinWidget( window )
{
	_widget->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowSystemMenuHint );
	_widget->setAttribute  ( Qt::WA_NoSystemBackground );
	_widget->setAttribute  ( Qt::WA_PaintOnScreen      );

	_widget->installEventFilter( new MouseMoveEventFilter (this,SLOT(mouseMoveEvent(QEvent*)) ) );
	_widget->installEventFilter( new MousePressEventFilter(this,SLOT(mousePressEvent(QEvent*))) );
}

void SkinWindow::mouseMoveEvent( QEvent* event )
{
	QMouseEvent* e = (QMouseEvent*) event;

	QPoint newMousePos = e->globalPos();

	// alculates the window top left corner position as the
	// difference between the new and initial mouse cursor positions
	QPoint upLeft = _initialWindowPos + newMousePos - _initialMousePos;

	_widget->move( upLeft );
}

void SkinWindow::mousePressEvent(QEvent * event)
{
	QMouseEvent* e = (QMouseEvent*) event;

	_initialMousePos = e->globalPos();

	_initialWindowPos = _widget->pos();
}
