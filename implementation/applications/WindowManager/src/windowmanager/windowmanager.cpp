
// Qt's headers
#include <QtGui/QX11Info>

// local headers
#include "windowmanager.h"
#include "xwindow/xwindow.h"
#include "taskbar/taskbar.h"


using namespace XLibWrapper;

WindowManager::WindowManager( int argc, char* argv[] ):
	QApplication(argc, argv)
{
}

WindowManager::~WindowManager()
{
	_rootWindows.clear();
}

bool WindowManager::initEnvironment()
{
	// getting the X11 display (our channel to talk to X server)
	Display* x11Display = QX11Info::display();

	// init the XLib wrapper
	XLibWrapper::setDisplay( x11Display );

	// init the task bar
	_taskBar = new TaskBar();
	int x = 0;
	int y = XLibWrapper::getScreenHeight( XLibWrapper::getScreenForWindow(_taskBar->winId()) ) - 40;
	int width = XLibWrapper::getScreenWidth( XLibWrapper::getScreenForWindow(_taskBar->winId()) );
	int height = 40;
	_taskBar->setGeometry( x, y, width, height );
	_taskBar->show();

	// getting the number of screens available and subscribe for notifications from X11-server
	// one-screen-one-root-window
	int screensCount = XScreenCount( x11Display );
	for( int screenIndex = 0; screenIndex < screensCount; ++screenIndex )
	{
		Window rootWindow = XRootWindow( x11Display, screenIndex );

		_rootWindows.append( rootWindow );

		if( BadWindow == XSelectInput(x11Display,rootWindow,EventMaskForRootWindow) )
			return false;

		XSync( x11Display, FALSE );
	}

	return true;
}

bool WindowManager::x11EventFilter( XEvent* event )
{
	// processing events we need, to be able to make happy the plancenta-riuses that uses this application
	switch( event->type )
	{
		// it would be wise to satisfy the requests that are supposed to be finished by a WindowManager
		// if we don't do this then the only useful side of this WindowManager would be to uninstall it.
		case CirculateRequest:
			handleCirculateRequest( &event->xcirculaterequest );
			break;

		case ConfigureRequest:
			handleConfigureRequest( &event->xconfigurerequest );
			break;

		case MapRequest:
			handleMapRequest( &event->xmaprequest );
			break;


		// ok, done with direct dutty, now we pretend to enhance the behaviour of the WindowManager
		// by intercepting some aditional notification sent after a window was done with it
		case DestroyNotify:
			handleDestroyNotification( &event->xdestroywindow );
			break;

		case ClientMessage:
			handleClientMessage( &event->xclient );
			break;

		case PropertyNotify:
			handlePropertyChangedNotification( &event->xproperty );
			break;

		case EnterNotify:
			handleEnterNotification( &event->xcrossing );
			break;

		case KeyRelease:
			handleKeyRelease( &event->xkey );

		case ButtonRelease:
			handleButtonRelease( &event->xbutton );

		default:
			break;
	};
	
	// passing event to qt's event handler
	return false;
}



/*****************************************************************************************************************************
*	Next follows the implementation of the methods that will nicely take care of the WM's  functionality
******************************************************************************************************************************/

void WindowManager::handleCirculateRequest( XCirculateRequestEvent* event )
{
	// it makes sence to "bring on top"/"send to back" windows only if the parent is one of the root windows
	if( false == _rootWindows.contains(event->parent) )
	{
		// Yeah, right.  who cares. The circulation is allowed only to Country-Riders and their offspings - for ex: Oleg Voroning, AKA Voron Voronovichi Voronov, 
		//	AKA Vornonin II, AKA The Capitalist Face of the Comunism in RM - who tells people to shut the fuck up if they don't want troubles - after he is cutting amounts of forests counted as national treasure.
		//
		// No circulation allowed. Stay in country(cage), we've made nice barriers for that.
		// The only choise left for you is to pay taxes because we(PCRM,PPCD, and the rest of fat assholes) have to thrive on something.
		// The source for you to pay taxes is totally your problem - usllay you can ask your parents to send money from Itally, Grece, Spain, Russia - we don't mind.
		// Btw - this is the only way for us to show how our economy is growing and pretend that we did that by our genial strategy.
		// If we fail we will find a person who blowed our genial strategy, and we'll teach him a lesson - this is the way we do in USSR, Russia, RM.
		// When we see a trouble or already got into it then we try to focus public on things like: changing the name for the christmass tree, 
		// changing the history, bulshiting people about UE, voroninShiting people about the "getting back the Transnistria" - while actually this is a nice
		// plan to drag RM back into USSR-style environment. During all this time while we drive the country we are
		// Putin-Lick-Assers - a new word missing from the dictionaries. Putin - an asshole fucking Russian's nation.
		// Putin-ANobodyFromTiraspol-Voronin - AKA local bermudian multi-angle.

		return;
	}

	Window window = event->window;
	if( PlaceOnTop == event->place )
		XLibWrapper::bringWindowOnTop( window );
	else
		XLibWrapper::sendWindowToBack( window );
}

void WindowManager::handleConfigureRequest( XConfigureRequestEvent* event )
{
	Window window = event->window;

	// if we don't have a such a mafia-family-member, we just pretend that we didn't received any request - just as our justice do with us.
	// Or better be kinder and help it out as usually non RM governments do, probably this will make him stay and not leave the country in search for a better life
	if( false == _theKnowlegeBase.contains(window) )
	{
		XLibWrapper::configureWindow( event );
		return;
	}

	// damn, we have a such citizen (what the heck he wants - we're soooo busy), 
	// have to stop for not more than a second from my family business and do actually for what I've been elected
	XWindow* xWindow = _theKnowlegeBase[window];
	xWindow->configureWindow( event );
}

void WindowManager::handleMapRequest( XMapRequestEvent* event )
{
	Window window = event->window;

	// if we have this window in our collection then we just show it and exit
	if( true == _theKnowlegeBase.contains(window) )
	{
		XWindow* xWindow = _theKnowlegeBase[window];
		xWindow->show();

		return;
	}

	if( true == XLibWrapper::isWindowSupposedTohaveDecorations(window) )
	{
		// once we got here it means we have to do some magic on this window to make it usable for homo-erectus descendants

		// so here we start: abracadabra, jmih, jmah jjjahh...
		XWindow* xWindow = new XWindow( window );
		if( 0 != xWindow )
		{
			_theKnowlegeBase.insert( window, xWindow );
			xWindow->show();
			_taskBar->add( xWindow );
			XSync( QX11Info::display(), FALSE );
		}
		// hmm...  seems that was it
	}
	else
		// in all other cases it must be a shaped or popup window - we don't care about that, we just show it naked, let people see how ugly it is
		XLibWrapper::showWindow( window );
}

void WindowManager::handleDestroyNotification( XDestroyWindowEvent* event )
{
	Window window = event->window;

	// it seems some body died, this is a good sign from the sky for the "Popes". They will have a new money source for the next 3 days, after that at 7 days after death,
	// 40 days, 1 year, etc... kindda forever money paying engine, probably this is why they fight who will take care of the dead - A better car than the pope
	// from the next church is better than a worser car than the pope from the next church. Poor aged people, they are fucked by two mfias at once - 
	// the country riders and the church.


	// The only what we have to do for that is say Amin - it's like "it kindda means something, just shhish and pretend it has some power over you".
	if( false == _theKnowlegeBase.contains(window) )
		return;

	XWindow* xWindow = _theKnowlegeBase[window]; 

	// god is with you, actually it will be only after I will say "Amin" and after you pay for that, if you don't pay it will not be with you, 
	// the devil will be with you - better pay if you want A god, a personal GOD, a VIP god - everything depends on how much you pay. 
	// And don't forget to kiss my hand and call me father - because I've learned for 
	// that and my marks from the speciffic schools allow me to be called "father" - not messenger or helper but 'father' because I'm Fat - starts with the same letter.
	_taskBar->remove( xWindow );
	delete xWindow;
	_theKnowlegeBase.remove( window );
}

void WindowManager::handleClientMessage( XClientMessageEvent* event )
{
	Window window = event->window;

	// here we try to conform to many "wise and helpful" standards, these standards have the aim to help you to sleep better and make your life longer
	// usually this is called workaround for the "features" from X11 anarchy
}

void WindowManager::handlePropertyChangedNotification( XPropertyEvent* event )
{
	Window window = event->window;

	Atom atom = event->atom;

	// we're interested in this only if this is a window that is under our responsability to make it nice looking
	if( false  == _theKnowlegeBase.contains(window) )
		return;

	XWindow* xWindow = _theKnowlegeBase[window];
	xWindow->propertyChanged( event );
}

void WindowManager::handleEnterNotification( XCrossingEvent* event )
{
	Window window = event->window;

	XLibWrapper::focusWindow( window );

	for( int index=0; index < _rootWindows.size(); ++index )
		XSelectInput( QX11Info::display(), _rootWindows.at(index), EventMaskForRootWindow );
}

void WindowManager::handleKeyRelease( XKeyReleasedEvent* event )
{
}

void WindowManager::handleButtonRelease( XButtonReleasedEvent* event )
{
}
















































































































































































































































































































































































































































//	Things to not understand.











































































































































































































































































































// Some aditional useful methods, only enlightened persons will not understand them
void handleUnmapEvent( XUnmapEvent* unMapEvent )
{
	Window window = unMapEvent->window;

	// so what if somebody is not visible in country and left, the better for us, this event means that the plan to destroy this nation works as planned,
	// no big deal, just don't make too much noise about that.
}

