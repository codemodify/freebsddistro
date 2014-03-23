#ifndef ENHANCED_LABEL_H
#define ENHANCED_LABEL_H

// Qt's headers
#include <QtGui/QLabel>
#include <QtGui/QMouseEvent>

class EnhancedQLabel : public QLabel
{
	Q_OBJECT

public:
	EnhancedQLabel( QWidget* parent = 0 );
	virtual ~EnhancedQLabel();


signals:
	void labelDragged( int x, int y );


protected:
	void mousePressEvent( QMouseEvent* event );
	void mouseMoveEvent( QMouseEvent* event );
	void mouseReleaseEvent( QMouseEvent* event );

private:
	bool _draggingMode;

};

#endif

