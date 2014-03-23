
// Qt's headers
#include <QtGui/QX11EmbedContainer>
#include <QtGui/QCloseEvent>

// local headers
#include "windowmanagerwindow.h"

WindowManagerWindow::WindowManagerWindow( Window window ):
	QDockWidget(),
	_x11Window( window )
{
	setAllowedAreas( Qt::AllDockWidgetAreas );
	setFeatures( QDockWidget::AllDockWidgetFeatures );

	// init the titlebar
	std::string windowName = XLibWrapper::getWindowName( _x11Window );
	setWindowTitle( windowName.c_str() );
	//XLibWrapper::showWindow( _x11Window );

	//QX11EmbedContainer* embedContainer = new QX11EmbedContainer(this);
	//setWidget( embedContainer );
	//embedContainer->embedClient( _x11Window );

	XLibWrapper::showWindow( winId() );

	setGeometry( 100, 100, 400, 300 );
}

WindowManagerWindow::~WindowManagerWindow()
{
	XLibWrapper::destroyWindow( _x11Window );
}

void WindowManagerWindow::closeEvent( QCloseEvent* event )
{
	deleteLater();

	event->accept();
}

