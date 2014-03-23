
// local headers
#include "taskbar.h"


TaskBar::TaskBar( QWidget* parent ):
	QWidget( parent )
{
}

TaskBar::~TaskBar()
{}

void TaskBar::paintEvent( QPaintEvent* paintEvent )
{
	QWidget::paintEvent(paintEvent);
// 25         QPainter painter(this);
// 26 
// 27         if( m_backgroundImage )
// 28         {
// 29                 int _height = height();
// 30                 int _width = width();
// 31 
// 32                 int _imgHeight = m_backgroundImage->height();
// 33                 int _imgWidth = m_backgroundImage->width();
// 34 
// 35                 painter.drawTiledPixmap( 0,0,10,height(), *m_backgroundImage, 0,0 );
// 36 
// 37                 QImage image = m_backgroundImage->toImage().copy(10, 0, _imgWidth-20, _imgHeight );
// 38 
// 39                 QPixmap pixmap = QPixmap::fromImage( image );
// 40 
// 41                 painter.drawTiledPixmap( 10,0,width()-10,height(), pixmap, 10, 0 );
// 42 
// 43                 painter.drawTiledPixmap( width()-10,0,width(),height(), *m_backgroundImage, m_backgroundImage->wi
// 44         }
}

void TaskBar::setBackgroungImage( QPointer<QPixmap> backgroundImage )
{
	_backgroundImage = backgroundImage;
}

