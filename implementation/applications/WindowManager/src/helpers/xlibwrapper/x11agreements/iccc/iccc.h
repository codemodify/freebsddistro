#ifndef ICCC_H
#define ICCC_H

// X11 headers
#include <X11/Xlib.h>

// STL headers
#include <string>
#include <list>


namespace X11Agreements
{
	namespace ICCC
	{

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
		extern const char* AtomName[];

		// the atoms' values
		extern Atom atom[AtomsCount];

 

		// Some ways to simplify the work with these Atoms

		// get the values from the X11-seerver for for the attoms that we support
		void initAtoms( Display* display );

		// sets a window's title text in ICCCM way
		void setWindowTitleText( Display* display, Window window, std::string& title );

		// gets a window name in a ICCCM way
		void getWindowTitleText( Display* display, Window window, std::string& title );

		// getting the text for the title, to display when window is iconified
		void getWindowTitleTextWhenIconified( Display* display, Window window, std::string& title );

		// getting the startup state that the window wish to be in
		int getWindowInitialState( Display* display, Window window );

		// getting the window that will serve as multi-colored icon for the application
		Window getWindowIcon( Display* display, Window window );

		// get the window's geometry
		void getWindowGeometry( Display* display, Window window, int& x, int& y, int& width, int& height );

		// get the window's geometry constraints
		void getWindowGeometryConstraints( Display* display, Window window, int& minWidth, int& minHeight, int& maxWidth, int& maxHeight );

		// getting the window class, this is used when we want to group windows of the same type
		std::string getWindowClass( Display* display, Window window );

		// checking if current window is transient for some other window
		bool isWindowTransient( Display* display, Window window, Window& windowTobeTransientFor );

		// getting the list of protocols the window is willing to participate
		typedef std::list<Atom> ProtocolList;
		bool getWindowProtocols( Display* display, Window windowi, ProtocolList& protocolList );

		// getting the state for a window
		int getWindowState( Display* display, Window window );

		// setting window's state to a different one
		void setWindowState( Display* display, Window window, int state );

		// checking if a window must have decorations
		bool isWindowSupposedTohaveDecorations( Display* display, Window window );
	};
};

#endif

