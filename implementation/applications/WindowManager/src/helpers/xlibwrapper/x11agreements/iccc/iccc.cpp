
// X11 headers
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>


// local headers
#include "iccc.h"


const char* X11Agreements::ICCC::AtomName[] =
{
	"AtomsBegin",
 
	"WM_NAME",
	"WM_ICON_NAME",

	"WM_NORMAL_HINTS",
	"WM_HINTS",

	"WM_CLASS",
	"WM_TRANSIENT_FOR",
	"WM_PROTOCOLS",
	"WM_STATE",
	"WM_CHANGE_STATE",
	"WM_DELETE_WINDOW",

	"AtomsCount"
};

Atom X11Agreements::ICCC::atom[X11Agreements::ICCC::AtomsCount];


void X11Agreements::ICCC::initAtoms( Display* display )
{
	for( int index = AtomsBegin; index < AtomsCount; ++index )
		atom[index] = XInternAtom( display, AtomName[index], False );
}

void X11Agreements::ICCC::setWindowTitleText( Display* display, Window window, std::string& title )
{
	XTextProperty textProperty = { 0, 0, 0, 0 };

	char* strings[2];
	strings[0] = (char*)title.c_str();
	strings[1] = 0;

	XmbTextListToTextProperty( display, strings, 1, XStdICCTextStyle, &textProperty );

	XSetWMName( display, window, &textProperty );
}

void X11Agreements::ICCC::getWindowTitleText( Display* display, Window window, std::string& title )
{
	XTextProperty textProperty;
	Status status = XGetTextProperty( display, window, &textProperty, atom[WM_NAME] );

	// checking if this window can offer us what we require
	if( 0 == status )
		return;

	// making a std string and return it
	if( (XA_STRING==textProperty.encoding) && (8==textProperty.format) && (0!=textProperty.nitems) )
	{
		// *(textProperty.value+textProperty.nitems) = 0x0; // conforming to ICCCM at this address there i
		title.clear();
		title.replace( 0, 0, (const char*)textProperty.value );
	}
}

void X11Agreements::ICCC::getWindowTitleTextWhenIconified( Display* display, Window window, std::string& title )
{
 	XTextProperty textProperty;
 	Status status = XGetTextProperty( display, window, &textProperty, atom[WM_ICON_NAME] );

	// checking if this window can offer us what we require
	if( 0 == status )
		return;

	// making a std string and return it
	if( (XA_STRING==textProperty.encoding) && (8==textProperty.format) && (0!=textProperty.nitems) )
	{
		// *(textProperty.value+textProperty.nitems) = 0x0; // conforming to ICCCM at this address there i
		title.clear();
		title.replace( 0, 0, (const char*)textProperty.value );
	}
}

int X11Agreements::ICCC::getWindowInitialState( Display* display, Window window )
{
	int state = WithdrawnState;

	XWMHints* windowHints =  XGetWMHints( display, window );
	state = windowHints->initial_state;
	XFree( windowHints );

	return state;
}

Window X11Agreements::ICCC::getWindowIcon( Display* display, Window window )
{
	Window iconWindow = 0;

	XWMHints* windowHints =  XGetWMHints( display, window );
	iconWindow = windowHints->icon_window;
	XFree( windowHints );

	return iconWindow;
}

void X11Agreements::ICCC::getWindowGeometry( Display* display, Window window, int& x, int& y, int& width, int& height )
{
	XWindowAttributes windowAttributes;

	XGetWindowAttributes( display, window, &windowAttributes );

	x = windowAttributes.x;
	y = windowAttributes.y;
	width = windowAttributes.width;
	height = windowAttributes.height;
}

void X11Agreements::ICCC::getWindowGeometryConstraints( Display* display, Window window, int& minWidth, int& minHeight, int& maxWidth, int& maxHeight )
{
	// in any cases put there some default values
	minWidth = minHeight = 0;
	maxWidth = maxHeight = 10000;

	// now do some magic and get the values desired by window
	XSizeHints sizeHints;
	long unusedOutput;

	Status status = XGetWMNormalHints( display, window, &sizeHints, &unusedOutput );
	if( 0 == status )
		return; 

	minWidth = sizeHints.min_width;
	minHeight = sizeHints.min_height;
	maxWidth = sizeHints.max_width;
	maxHeight = sizeHints.max_height;
}

std::string X11Agreements::ICCC::getWindowClass( Display* display, Window window )
{
	XClassHint classHint;

	Status status = XGetClassHint( display, window, &classHint );
	if( 0 == status )
		return std::string();

	std::string windowClass( classHint.res_class );

	return windowClass;
}

bool X11Agreements::ICCC::isWindowTransient( Display* display, Window window, Window& windowTobeTransientFor )
{
	Status status = XGetTransientForHint( display, window, &windowTobeTransientFor );

	if( (0==status) || (0==windowTobeTransientFor) )
		return false;

	return true;
}

bool X11Agreements::ICCC::getWindowProtocols( Display* display, Window window, ProtocolList& protocolList )
{
	Atom* atoms;
	int listSize;

	Status status = XGetWMProtocols( display, window, &atoms, &listSize );
	if( 0 == status )
		return false;

	for( int index = 0; index < listSize; index++ )
		protocolList.push_back( atoms[index] );


	XFree( atoms );

	return true;
}

int X11Agreements::ICCC::getWindowState( Display* display, Window window )
{
	Atom realType;
	int realFormat = 0;
	unsigned long itemsRead = 0;
	unsigned long bytesLeft = 0;
	unsigned char *data;

	XGetWindowProperty( display, window, atom[WM_STATE], 0, 1, False, atom[WM_STATE], &realType, &realFormat, &itemsRead, &bytesLeft, &data );

	if( (32==realFormat) && (0!=itemsRead) )
	{
		unsigned long* windowState = (unsigned long*)data;
		XFree( data ); 

		return (int)(*windowState);
	}

	return NormalState;
}

void X11Agreements::ICCC::setWindowState( Display* display, Window window, int state )
{
	unsigned long data[2];
	data[0] = (unsigned long)state;
	data[1] = (unsigned long)None;

	XChangeProperty( display, window, atom[WM_STATE], atom[WM_STATE], 32, PropModeReplace, (unsigned char*)data, 2 );
}

bool X11Agreements::ICCC::isWindowSupposedTohaveDecorations( Display* display, Window window )
{
	XWindowAttributes windowAttributes;
	XGetWindowAttributes( display, window, &windowAttributes );

	return (False == windowAttributes.override_redirect);
}

