#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H

// Qt's headers
#include <QtGui/QWidget>
#include <QtCore/QPointer>
#include <QtGui/QHBoxLayout>

// local headers
#include "enhancedqlabel/enhancedqlabel.h"

#define BOTTOM_HEIGHT 10

class BottomBar : public QWidget
{

	Q_OBJECT

public:
	BottomBar();
	~BottomBar();


protected:
	QPointer<QHBoxLayout> _horizontaLayout;
	QPointer<EnhancedQLabel> _leftCorner;
	QPointer<EnhancedQLabel> _rightCorner;


signals:
	void resizeLeft( int globalX, int globalY );
	void resizeRight( int globalX, int globalY );

};

#endif
