
// Qt headers
#include <QtGui/QWidget>
#include <QtGui/QBitmap>
#include <QtGui/QImage>
#include <QtGui/QPainter>

// system headers
#include <iostream>
using namespace std;

// local headers
#include "EventFilter/PaintEventFilter.h"
#include "EventFilter/MouseEventFilter.h"
#include "EventFilter/EnabledChangeEventFilter.h"
#include "SkinWidget.h"


SkinWidget::SkinWidget(QWidget * widget) :
	QObject( widget ),
	_widget( widget ),
	_current( _regular )
{}

void SkinWidget::setPixmaps( QPixmap& regular, QPixmap& active )
{
	_regular = regular;
	_active  = active;

	// regular
	_widget->installEventFilter( new PaintEventFilter(this,SLOT(paintEvent())) );
	setWidgetShape( _regular );

	// active
	_widget->setAttribute( Qt::WA_Hover, true );

	_widget->installEventFilter( new MouseHoverLeaveEventFilter(this,SLOT(mouseHoverLeaveEvent())) );
	_widget->installEventFilter( new MouseHoverEnterEventFilter(this,SLOT(mouseHoverEnterEvent())) );
//		_widget->installEventFilter( new MouseReleaseEventFilter   (this SLOT(mouseReleaseEvent   ())) );
//		_widget->installEventFilter( new MousePressEventFilter     (this,SLOT(mousePressEvent     ())) );

	_widget->update();
}

void SkinWidget::setBackgroundPixmap( QPixmap& pixmap )
{
	QPalette	palette;
			palette.setBrush( _widget->backgroundRole(), QBrush(pixmap) );

	_widget->setPalette(palette);
}

void SkinWidget::setWidgetShape( QPixmap& pixmap )
{
	QBitmap maskImage = QBitmap::fromImage( pixmap.toImage().createAlphaMask() );

	_widget->setMask( maskImage );
}

void SkinWidget::mousePressEvent()
{
	if( _widget->isEnabled() )
		_current = _regular;
}

void SkinWidget::mouseReleaseEvent()
{
	if( _widget->isEnabled() )
		_current = _active;
}

void SkinWidget::mouseHoverEnterEvent()
{
	if( _widget->isEnabled() )
		_current = _active;
}

void SkinWidget::mouseHoverLeaveEvent()
{
	if( _widget->isEnabled() )
		_current = _regular;
}

void SkinWidget::paintEvent()
{
	QPixmap scaledPix = _current.scaled( _widget->width(), _widget->height() );

	QPainter	painter( _widget );
			painter.drawPixmap( 0, 0, _current );
}
