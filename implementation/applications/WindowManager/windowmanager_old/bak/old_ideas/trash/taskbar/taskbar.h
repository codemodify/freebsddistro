#ifndef ENHANCEDQWIDGET_H_
#define ENHANCEDQWIDGET_H_

// Qt's headers
#include <QtGui/QWidget>
#include <QtCore/QPointer>


class QPixmap;
class QString;

class TaskBar: public QWidget
{

public:
	TaskBar( QWidget* parent);
	~TaskBar();


public: // appearence enhancements
	void setBackgroungImage( QPointer<QPixmap> backgroundImage );



public: // relating taskbar buttons to windows
	void addTaskBarButtonForWindow( Window window );
	void removeTaskBarButtonForWindow( Window window );



protected:
	void paintEvent( QPaintEvent* paintEvent );



protected:
	QPointer<QPixmap> _backgroundImage;


};

#endif
