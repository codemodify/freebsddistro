
// Qt headers
#include <QtGui/QHBoxLayout>

// local headers
#include "xwindow/xwindow.h"
#include "taskbar.h"
#include "enhancedqpushbutton/enhancedqpushbutton.h"

TaskBar::TaskBar():
	QWidget()
{
	_layout = new QHBoxLayout(this);
}

void TaskBar::add( XWindow* window )
{
	EnhancedQPushButton* button = new EnhancedQPushButton();
	button->setText( window->getText() );
	button->_dataPointer = (void*)window;
	connect( button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()) );

	_layout->addWidget( button );
	_buttons.insert( window, button );
}

void TaskBar::remove( XWindow* window )
{
	if( false == _buttons.contains(window) )
		return;

	EnhancedQPushButton* button = _buttons[window];
	
	_layout->removeWidget( button );
	_buttons.remove( window );

	delete button;
}

void TaskBar::buttonClicked()
{
	QObject* object = sender();
	if( 0 == object )
		return;

	EnhancedQPushButton* button = qobject_cast<EnhancedQPushButton*>( object );
	if( 0 == button )
		return;

	XWindow* xWindow = (XWindow*)button->_dataPointer;
	if( 0 == xWindow )
		return;

	if( true == xWindow->isMinimized() )
		xWindow->unMinimizeWindow();
	else if( xWindow->isOnTop() )
		xWindow->minimizeWindow();
	else
		xWindow->raise();
}
