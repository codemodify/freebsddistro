
// Qt's headers
#include <QtGui/QX11Info>
#include <QtGui/QDesktopWidget>
#include <QtGui/QAction>

// common headers
#include <General>
#include <Logger>

// local headers
#include "windowmanager.h"
#include "xlibwrapper/xlibwrapper.h"

using namespace XLibWrapper;

WindowManager::WindowManager( int argc, char* argv[] ):
	QApplication(argc, argv)
{
}

WindowManager::~WindowManager()
{
}

bool WindowManager::initEnvironment()
{
	// getting the X11 display (our channel to talk to X server)
	_x11Display = QX11Info::display();
	XLibWrapper::setDisplay( _x11Display );


	#define EVENT_MASK_FOR_ROOT_WINDOW (SubstructureRedirectMask|SubstructureNotifyMask | ButtonPressMask|ButtonReleaseMask) 


	// getting the number of screens available and subscribe for notifications from X11-server
	// one-screen-one-root-window
	int screensCount = XScreenCount( _x11Display );
	for( int screenIndex = 0; screenIndex < screensCount; ++screenIndex )
	{
		Window rootWindow = XRootWindow( _x11Display, screenIndex );

		if( BadWindow == XSelectInput(_x11Display,rootWindow,SubstructureRedirectMask) )
			return false;

		XSync( _x11Display, FALSE );
	}


	// init the task bar
	//#define TASKBAR_HEIGHT 40
	//int resX = QApplication::desktop()->width();
	//int resY = QApplication::desktop()->height();
	//_taskBar = new QToolBar();
	//_taskBar->setGeometry( 0, resY-TASKBAR_HEIGHT, resX, TASKBAR_HEIGHT );
	//_taskBar->show();


	return true;
}

bool WindowManager::x11EventFilter( XEvent* event )
{
	// processing events we need
	switch( event->type )
	{
		case ButtonPress:
			handleButtonPressEvent( &event->xbutton );
			break;

		case ButtonRelease:
			handleButtonReleaseEvent( &event->xbutton );
			break;







		case ConfigureRequest:
			handleConfigureRequestEvent( &event->xconfigurerequest );
			break;

		case CirculateRequest:
			handleCirculateRequestEvent( &event->xcirculaterequest );
			break;

		case MapRequest:
			handleMapRequestEvent( &event->xmaprequest );
			break;






		case UnmapNotify:
			handleUnmapEvent( &event->xunmap );
			break;

		case DestroyNotify:
			handleDestroyEvent( &event->xdestroywindow );
			break;

		case ClientMessage:
			handleClientMessageEvent( &event->xclient );
			break;

		case ColormapNotify:
			handleColorMapChangeEvent( &event->xcolormap );
			break;

		case PropertyNotify:
			handlePropertyChangeEvent( &event->xproperty );
			break;

		case EnterNotify:
			handleEnterEvent( &event->xcrossing );
			break;

		case Expose:
			handleExposeEvent( &event->xexpose );
			break;

		default:
			break;
	};
	
	// passing event to qt's event handler
	return false;
}

void WindowManager::handleButtonPressEvent( XButtonPressedEvent* buttonPressedEevent )
{}

void WindowManager::handleButtonReleaseEvent( XButtonReleasedEvent* buttonReleasedEvent )
{
}

void WindowManager::handleMapRequestEvent( XMapRequestEvent* mapRequestEvent )
{
	XWindow* xwindow = new XWindow(mapRequestEvent->window);
XMapWindow( QX11Info::display(), xwindow->winId() );
XMapWindow( QX11Info::display(), mapRequestEvent->window );

}

void WindowManager::handleUnmapEvent( XUnmapEvent* unMapEvent )
{}

void WindowManager::handleConfigureRequestEvent( XConfigureRequestEvent* configureRequestEvent )
{}

void WindowManager::handleCirculateRequestEvent( XCirculateRequestEvent* circulateRequestEvent )
{}

void WindowManager::handleDestroyEvent( XDestroyWindowEvent* destroyEvent )
{}

void WindowManager::handleClientMessageEvent( XClientMessageEvent* clientMessageEvent )
{}

void WindowManager::handlePropertyChangeEvent( XPropertyEvent* propertyEvent )
{}

void WindowManager::handleEnterEvent( XCrossingEvent* crossingEvent )
{}

void WindowManager::handleColorMapChangeEvent( XColormapEvent* colorMapEvent )
{}

void WindowManager::handleExposeEvent( XExposeEvent* exposeEvent )
{}

