#ifndef __SkinWindow_H__
#define __SkinWindow_H__

// Qt headers
#include <QtCore/QObject>
#include <QtCore/QPoint>

// local headers
#include "SkinWidget.h"


class SkinWindow : public SkinWidget
{
	Q_OBJECT

	public:
		SkinWindow( QWidget* window );

	private slots:
		void mouseMoveEvent ( QEvent* event );
		void mousePressEvent( QEvent* event );

	private:	
		QPoint _initialMousePos;
		QPoint _initialWindowPos;
};

#endif
