
// X11 headers
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

// local heders
#include "ewmh.h"

const char* X11Agreements::EWMH::AtomName[] = 
{
	"AtomsBegin",

	"UTF8_STRING",

	"_NET_SUPPORTED",

	// window related
	"_NET_WM_NAME",
	"_NET_WM_ICON_NAME",

	"_NET_WM_WINDOW_TYPE",
	"_NET_WM_WINDOW_TYPE_DESKTOP",
	"_NET_WM_WINDOW_TYPE_DOCK",
	"_NET_WM_WINDOW_TYPE_TOOLBAR",
	"_NET_WM_WINDOW_TYPE_MENU",
	"_NET_WM_WINDOW_TYPE_UTILITY",
	"_NET_WM_WINDOW_TYPE_SPLASH",
	"_NET_WM_WINDOW_TYPE_DIALOG",
	"_NET_WM_WINDOW_TYPE_NORMAL",

        "_NET_WM_STATE",
	"_NET_WM_STATE_MODAL",
	"_NET_WM_STATE_STICKY",
	"_NET_WM_STATE_MAXIMIZED_VERT",
	"_NET_WM_STATE_MAXIMIZED_HORZ",
	"_NET_WM_STATE_SHADED",
	"_NET_WM_STATE_SKIP_TASKBAR",
	"_NET_WM_STATE_SKIP_PAGER",
	"_NET_WM_STATE_HIDDEN",
	"_NET_WM_STATE_FULLSCREEN",
	"_NET_WM_STATE_ABOVE",
	"_NET_WM_STATE_BELOW",
	"_NET_WM_STATE_DEMANDS_ATTENTION",

	"_NET_WM_ALLOWED_ACTIONS",
	"_NET_WM_ACTION_MOVE",
	"_NET_WM_ACTION_RESIZE",
	"_NET_WM_ACTION_MINIMIZE",
	"_NET_WM_ACTION_SHADE",
	"_NET_WM_ACTION_STICK",
	"_NET_WM_ACTION_MAXIMIZE_HORZ",
	"_NET_WM_ACTION_MAXIMIZE_VERT",
	"_NET_WM_ACTION_FULLSCREEN",
	"_NET_WM_ACTION_CHANGE_DESKTOP",
	"_NET_WM_ACTION_CLOSE",

	"_NET_WM_ICON",

	"_NET_WM_PID",

	// WindowManager related
	"_NET_WM_PING",

	"AtomsCount"
};


Atom X11Agreements::EWMH::atom[X11Agreements::EWMH::AtomsCount];


void X11Agreements::EWMH::getTextAtom( Display* display, Window window, Atom atomToGet, std::string& text )
{
	Atom realType;
	int realFormat;
	unsigned long itemsRead = 0;
	unsigned long bytesLeft = 0;
	unsigned char* data;

	XGetWindowProperty( display, window, atomToGet, 0, LONG_MAX, False, atom[UTF8_STRING], &realType, &realFormat, &itemsRead, &bytesLeft, &data );
	if( (8==realFormat) && (itemsRead>=1) )
	{
		text.clear();
		text.append( (char*)data );
	}
}

Atom X11Agreements::EWMH::getAtomForAtom( Display* display, Window window, Atom atom )
{
	Atom realType;
	int realFormat;
	unsigned long itemsRead = 0;
	unsigned long bytesLeft = 0;
	unsigned char* data;

	XGetWindowProperty( display, window, atom, 0, 1, False, XA_ATOM, &realType, &realFormat, &itemsRead, &bytesLeft, &data );
	if( (32==realFormat) && (0!=itemsRead) )
	{
		unsigned long* atomValue = (unsigned long*)data;
		XFree( data );

	return (Atom)*atomValue;
	}

	return 0;
}

void X11Agreements::EWMH::initAtoms( Display* display )
{
	for( int index = AtomsBegin; index < AtomsCount; ++index )
		atom[index] = XInternAtom( display, AtomName[index], False );
}               

void X11Agreements::EWMH::setWindowTitleText( Display* display, Window window, std::string& title )
{
	XChangeProperty( display, window, atom[_NET_WM_NAME], atom[UTF8_STRING], 8, 
			PropModeReplace, (unsigned char *)title.c_str(), title.length() );
}

void X11Agreements::EWMH::getWindowTitleText( Display* display, Window window, std::string& title )
{
	getTextAtom( display, window, atom[_NET_WM_NAME], title );
}

void X11Agreements::EWMH::getWindowTitleTextWhenIconified( Display* display, Window window, std::string& title )
{
	getTextAtom( display, window, atom[_NET_WM_ICON_NAME], title );
}

bool X11Agreements::EWMH::isWindowTypeDesktop( Display* display, Window window )
{
	return ( atom[_NET_WM_WINDOW_TYPE_DESKTOP] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
}

bool X11Agreements::EWMH::isWindowTypeDock( Display* display, Window window )
{
	return ( atom[_NET_WM_WINDOW_TYPE_DOCK] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
}

bool X11Agreements::EWMH::isWindowTypeToolbar( Display* display, Window window )
{
	return ( atom[_NET_WM_WINDOW_TYPE_TOOLBAR] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
}

bool X11Agreements::EWMH::isWindowTypeMenu( Display* display, Window window )
{
	return ( atom[_NET_WM_WINDOW_TYPE_MENU] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
}

bool X11Agreements::EWMH::isWindowTypeUtility( Display* display, Window window )
{
	return ( atom[_NET_WM_WINDOW_TYPE_UTILITY] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
}

bool X11Agreements::EWMH::isWindowTypeSplash( Display* display, Window window )
{
	return ( atom[_NET_WM_WINDOW_TYPE_SPLASH] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
}

bool X11Agreements::EWMH::isWindowTypeDialog( Display* display, Window window )
{
	return ( atom[_NET_WM_WINDOW_TYPE_DIALOG] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
}

bool X11Agreements::EWMH::isWindowTypeNormal( Display* display, Window window )
{
	return ( atom[_NET_WM_WINDOW_TYPE_NORMAL] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
}

bool X11Agreements::EWMH::isWindowSupposedTohaveDecorations( Display* display, Window window )
{
	return ( !isWindowTypeDesktop(display,window) || 
		 !isWindowTypeDock(display,window)    || 
		 !isWindowTypeToolbar(display,window) || 
		 !isWindowTypeMenu(display,window)    || 
		 !isWindowTypeSplash(display,window)  );
}

bool X11Agreements::EWMH::isWindowModal( Display* display, Window window )
{
	return ( atom[_NET_WM_STATE_MODAL] == getAtomForAtom(display,window,atom[_NET_WM_STATE]) );
}

bool X11Agreements::EWMH::isWindowFullscreen( Display* display, Window window )
{
	return ( atom[_NET_WM_STATE_FULLSCREEN] == getAtomForAtom(display,window,atom[_NET_WM_STATE]) );
}

bool X11Agreements::EWMH::isWindowDemandingAttention( Display* display, Window window )
{
	return ( atom[_NET_WM_STATE_DEMANDS_ATTENTION] == getAtomForAtom(display,window,atom[_NET_WM_STATE]) );
}

