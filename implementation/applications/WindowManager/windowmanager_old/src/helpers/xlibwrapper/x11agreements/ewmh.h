#ifndef EWMH_H
#define EWMH_H

// X11 headers
#include <X11/X.h>

// STL headers
#include <string>
#include <list>


namespace X11Agreements
{
	namespace EWMH
	{
	//private-supposed-to-be:
		// the atoms as described in EWMH
		enum AtomID
		{
			AtomsBegin,

			UTF8_STRING,

			_NET_SUPPORTED, // indicates that this WM supports EWMH

			// window related
			_NET_WM_NAME, // UTF8_STRING
			_NET_WM_ICON_NAME, // UTF8_STRING
			
			_NET_WM_WINDOW_TYPE, // ATOM[]/32
			_NET_WM_WINDOW_TYPE_DESKTOP,
			_NET_WM_WINDOW_TYPE_DOCK,
			_NET_WM_WINDOW_TYPE_TOOLBAR,
			_NET_WM_WINDOW_TYPE_MENU,
			_NET_WM_WINDOW_TYPE_UTILITY,
			_NET_WM_WINDOW_TYPE_SPLASH,
			_NET_WM_WINDOW_TYPE_DIALOG,
			_NET_WM_WINDOW_TYPE_NORMAL,

			_NET_WM_STATE, // ATOM[]
			_NET_WM_STATE_MODAL,
			_NET_WM_STATE_STICKY,
			_NET_WM_STATE_MAXIMIZED_VERT,
			_NET_WM_STATE_MAXIMIZED_HORZ,
			_NET_WM_STATE_SHADED,
			_NET_WM_STATE_SKIP_TASKBAR,
			_NET_WM_STATE_SKIP_PAGER,
			_NET_WM_STATE_HIDDEN,
			_NET_WM_STATE_FULLSCREEN,
			_NET_WM_STATE_ABOVE,
			_NET_WM_STATE_BELOW,
			_NET_WM_STATE_DEMANDS_ATTENTION,

			_NET_WM_ALLOWED_ACTIONS, // ATOM[]
			_NET_WM_ACTION_MOVE,
			_NET_WM_ACTION_RESIZE,
			_NET_WM_ACTION_MINIMIZE,
			_NET_WM_ACTION_SHADE,
			_NET_WM_ACTION_STICK,
			_NET_WM_ACTION_MAXIMIZE_HORZ,
			_NET_WM_ACTION_MAXIMIZE_VERT,
			_NET_WM_ACTION_FULLSCREEN,
			_NET_WM_ACTION_CHANGE_DESKTOP,
			_NET_WM_ACTION_CLOSE,

			_NET_WM_ICON, // CARDINAL[][2+n]/32

			_NET_WM_PID, // CARDINAL

			// WindowManager related
			_NET_WM_PING,
			

			AtomsCount
		};

		// the atoms as are recognized by the X11-server
		static const char* AtomName[] = 
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

		// the atoms' values
		static Atom atom[AtomsCount];


	//private-supposed-to-be:
		static std::string getTextAtom( Display* display, Window window, Atom atomToGet )
		{
			Atom realType;
			int realFormat;
			unsigned long itemsRead = 0;
			unsigned long bytesLeft = 0;
			unsigned char* data;

			XGetWindowProperty( display, window, atomToGet, 0, LONG_MAX, False, atom[UTF8_STRING], &realType, &realFormat, &itemsRead, &bytesLeft, &data );
			if( (8==realFormat) && (itemsRead>=1) )
			{
				std::string windowTitle( (char*)data );
				return windowTitle;
			}

			return std::string();
		}

		static Atom getAtomForAtom( Display* display, Window window, Atom atom )
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


	//public-supposed-to-be: // some ways to simplify the work with these Atoms

		// get the values from the X11-seerver for for the attoms that we support
		static void initAtoms( Display* display )
		{
			for( int index = AtomsBegin; index < AtomsCount; ++index )
				atom[index] = XInternAtom( display, AtomName[index], False );
		}



		//######################################################################

		// gets a window name in a EWMH way
		static std::string getWindowTitleText( Display* display, Window window )
		{
			return getTextAtom( display, window, atom[_NET_WM_NAME] );
		}

		// getting the text for the title, to display when window is iconified
		static std::string getWindowTitleTextWhenIconified( Display* display, Window window )
		{
			return getTextAtom( display, window, atom[_NET_WM_ICON_NAME] );
		}



		//######################################################################

		// checking if window is of Desktop type
		static bool isWindowTypeDesktop( Display* display, Window window )
		{
			return ( atom[_NET_WM_WINDOW_TYPE_DESKTOP] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
		}
		
		// checking if window is of Dock type
		static bool isWindowTypeDock( Display* display, Window window )
		{
			return ( atom[_NET_WM_WINDOW_TYPE_DOCK] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
		}

		// checking if window is of Toolbar type
		static bool isWindowTypeToolbar( Display* display, Window window )
		{
			return ( atom[_NET_WM_WINDOW_TYPE_TOOLBAR] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
		}

		// checking if window is of Menu type
		static bool isWindowTypeMenu( Display* display, Window window )
		{
			return ( atom[_NET_WM_WINDOW_TYPE_MENU] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
		}

		// checking if window is of Utility type
		static bool isWindowTypeUtility( Display* display, Window window )
		{
			return ( atom[_NET_WM_WINDOW_TYPE_UTILITY] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
		}

		// checking if window is of Splash type
		static bool isWindowTypeSplash( Display* display, Window window )
		{
			return ( atom[_NET_WM_WINDOW_TYPE_SPLASH] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
		}

		// checking if window is of Dialog type
		static bool isWindowTypeDialog( Display* display, Window window )
		{
			return ( atom[_NET_WM_WINDOW_TYPE_DIALOG] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
		}

		// checking if window is of Normal type
		static bool isWindowTypeNormal( Display* display, Window window )
		{
			return ( atom[_NET_WM_WINDOW_TYPE_NORMAL] == getAtomForAtom(display,window,atom[_NET_WM_WINDOW_TYPE]) );
		}

		// checking if a window must have decorations
		static bool isWindowSupposedTohaveDecorations( Display* display, Window window )
		{
			return (isWindowTypeDesktop(display,window) || isWindowTypeDock(display,window) || isWindowTypeToolbar(display,window) || isWindowTypeMenu(display,window) || isWindowTypeSplash(display,window) );
		}



		//########################################################################

		// checking if window is Modal
		static bool isWindowModal( Display* display, Window window )
		{
			return ( atom[_NET_WM_STATE_MODAL] == getAtomForAtom(display,window,atom[_NET_WM_STATE]) );
		}

		// checking if window is Fullscreen
		static bool isWindowFullscreen( Display* display, Window window )
		{
			return ( atom[_NET_WM_STATE_FULLSCREEN] == getAtomForAtom(display,window,atom[_NET_WM_STATE]) );
		}

		// checking if window DemandsAttention
		static bool isWindowDemandingAttention( Display* display, Window window )
		{
			return ( atom[_NET_WM_STATE_DEMANDS_ATTENTION] == getAtomForAtom(display,window,atom[_NET_WM_STATE]) );
		}
	};
};

#endif

