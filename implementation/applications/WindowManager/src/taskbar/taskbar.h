#ifndef TASKBAR_H
#define TASKBAR_H

// Qt headers
#include <QtGui/QWidget>
#include <QtCore/QMap>

class QHBoxLayout;
class XWindow;
class EnhancedQPushButton;

class TaskBar : public QWidget
{

	Q_OBJECT

public:
	TaskBar();


public:
	void add( XWindow* window );
	void remove( XWindow* window );


private:
	QHBoxLayout* _layout;
	QMap<XWindow*,EnhancedQPushButton*> _buttons;


private slots:
	void buttonClicked();

};

#endif
