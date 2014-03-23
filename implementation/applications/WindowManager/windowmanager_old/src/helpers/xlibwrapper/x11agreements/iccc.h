#ifndef ICCC_H
#define ICCC_H

// X11 headers
#include <X11/X.h>

// STL headers
#include <string>
#include <list>


namespace X11Agreements
{
	namespace ICCC
	{

	//private-supposed-to-be:
		// the atoms as described in ICCCM
		enum AtomID
		{
			AtomsBegin,

			WM_NAME, 
			WM_ICON_NAME,

			WM_NORMAL_HINTS,
			WM_HINTS,

			WM_CLASS,
			WM_TRANSIENT_FOR,
			WM_PROTOCOLS,
			WM_STATE,
			WM_CHANGE_STATE,
			WM_DELETE_WINDOW,
			
			AtomsCount
		};

		// the atoms as are recognized by the X11-server
		static const char* AtomName[] = 
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

		// the atoms' values
		static Atom atom[AtomsCount];



	//public-supposed-to-be: // some ways to simplify the work with these Atoms

		// get the values from the X11-seerver for for the attoms that we support
		static void initAtoms( Display* display )
		{
			for( int index = AtomsBegin; index < AtomsCount; ++index )
				atom[index] = XInternAtom( display, AtomName[index], False );
		}

		// gets a window name in a ICCCM way
		static std::string getWindowTitleText( Display* display, Window window )
		{
			XTextProperty textProperty;
			Status status = XGetTextProperty( display, window, &textProperty, atom[WM_NAME] );

			// checking if this window can offer us what we require
			if( 0 == status )
				return std::string();

			// making a std string and return it
			if( (XA_STRING==textProperty.encoding) && (8==textProperty.format) && (0!=textProperty.nitems) )
			{
				*(textProperty.value+textProperty.nitems) = 0x0; // conforming to ICCCM at this address there is a NULL value, to enable correct string copy we put there zero
				std::string windowName;
				windowName.replace( 0, 0, (const char*)textProperty.value );
				return windowName;
			}
			else
				return std::string();
		}

		// getting the text for the title, to display when window is iconified
		static std::string getWindowTitleTextWhenIconified( Display* display, Window window )
		{
			
			XTextProperty textProperty;
			Status status = XGetTextProperty( display, window, &textProperty, atom[WM_ICON_NAME] );

			// checking if this window can offer us what we require
			if( 0 == status )
				return std::string();

			// making a std string and return it
			if( (XA_STRING==textProperty.encoding) && (8==textProperty.format) && (0!=textProperty.nitems) )
			{
				*(textProperty.value+textProperty.nitems) = 0x0; // conforming to ICCCM at this address there is a NULL value, to enable correct string copy we put there zero
				std::string windowName;
				windowName.replace( 0, 0, (const char*)textProperty.value );
				return windowName;
			}
			else
				return std::string();
		}

		// getting the startup state that the window wish to be in
		static int getWindowInitialState( Display* display, Window window )
		{
			int state = WithdrawnState;

			XWMHints* windowHints =  XGetWMHints( display, window );
			state = windowHints->initial_state;
			XFree( windowHints );

			return state;
		}

		// getting the window that will serve as multi-colored icon for the application
		static Window getWindowIcon( Display* display, Window window )
		{
			Window iconWindow = 0;

			XWMHints* windowHints =  XGetWMHints( display, window );
			iconWindow = windowHints->icon_window;
			XFree( windowHints );

			return iconWindow;
		}

		// get the window's geometry
		static void getWindowGeometry( Display* display, Window window, int& x, int& y, int& width, int& height )
		{
			XWindowAttributes windowAttributes;

			XGetWindowAttributes( display, window, &windowAttributes );

			x = windowAttributes.x;
			y = windowAttributes.y;
			width = windowAttributes.width;
			height = windowAttributes.height;
		}

		// get the window's geometry constraints
		static void getWindowGeometryConstraints( Display* display, Window window, int& minWidth, int& minHeight, int& maxWidth, int& maxHeight )
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

		// getting the window class, this is used when we want to group windows of the same type
		static std::string getWindowClass( Display* display, Window window )
		{
			XClassHint classHint;

			Status status = XGetClassHint( display, window, &classHint );
			if( 0 == status )
				return std::string();

			std::string windowClass( classHint.res_class );

			return windowClass;
		}

		// checking if current window is transient for some other window
		static bool isWindowTransient( Display* display, Window window, Window& windowTobeTransientFor )
		{
			Status status = XGetTransientForHint( display, window, &windowTobeTransientFor );

			if( (0==status) || (0==windowTobeTransientFor) )
				return false;

			return true;
		}

		// getting the list of protocols the window is willing to participate
		static std::list<Atom> getWindowProtocols( Display* display, Window window )
		{
			Atom* atoms;
			int listSize;

			Status status = XGetWMProtocols( display, window, &atoms, &listSize );
			if( 0 == status )
				return std::list<Atom>();

			std::list<Atom> protocolList;
			for( int index = 0; index < listSize; index++ )
				protocolList.push_back( atoms[index] );


			XFree( atoms );

			return protocolList;
		}

		// getting the state for a window
		static int getWindowState( Display* display, Window window )
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

		// setting window's state to a different one
		static void setWindowState( Display* display, Window window, int state )
		{
			XChangeProperty( display, window, atom[WM_STATE], atom[WM_STATE], 32, PropModeReplace, (unsigned char*)state, 1 );

		}

		// checking if a window must have decorations
		static bool isWindowSupposedTohaveDecorations( Display* display, Window window )
		{
			XWindowAttributes windowAttributes;
			XGetWindowAttributes( display, window, &windowAttributes );

			return (False == windowAttributes.override_redirect);
		}
	};
};

#endif

