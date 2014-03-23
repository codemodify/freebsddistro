#ifndef ENHANCED_QWIDGET_H
#define ENHANCED_QWIDGET_H

// Qt headers
#include <QtGui/QWidget>



/*
	The only idea of this class compared to origianl QWidget, 
	is to emit some signals on mouse events
*/
class EnhancedQWidget : public QWidget
{

	Q_OBJECT

public:
	EnhancedQWidget( QWidget* parent=0 );
	virtual ~EnhancedQWidget();


signals:
	void mouseButtonPress( QMouseEvent* event );
	void mouseButtonRelease( QMouseEvent* event );
	void dragged( QPoint newPoint );



protected: // implementing the drag feature
	QPoint _startDragPoint;
	virtual void mouseMoveEvent( QMouseEvent* event );
	virtual void mousePressEvent( QMouseEvent* event );
	virtual void mouseReleaseEvent( QMouseEvent* event );

};

#endif

