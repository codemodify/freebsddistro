#ifndef EWMH_H
#define EWMH_H

// X11 headers
#include <X11/Xlib.h>

// STL headers
#include <string>
#include <list>


namespace X11Agreements
{
	namespace EWMH
	{

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
		extern const char* AtomName[];

		// the atoms' values
		extern Atom atom[AtomsCount];


		//private-supposed-to-be: some utility functions designed to be used only by the namesace's functions
		void getTextAtom( Display* display, Window window, Atom atomToGet, std::string& text );
		std::string getTextAtom( Display* display, Window window, Atom atomToGet );

		Atom getAtomForAtom( Display* display, Window window, Atom atom );


		// Some ways to simplify the work with these Atoms

		// get the values from the X11-seerver for for the attoms that we support
		void initAtoms( Display* display );



		//######################################################################

		// sets a window title text in EWMH way
		void setWindowTitleText( Display* display, Window window, std::string& title );

		// gets a window name in a EWMH way
		void getWindowTitleText( Display* display, Window window, std::string& title );

		// getting the text for the title, to display when window is iconified
		void getWindowTitleTextWhenIconified( Display* display, Window window, std::string& title );



		//######################################################################

		// checking if window is of Desktop type
		bool isWindowTypeDesktop( Display* display, Window window );

		// checking if window is of Dock type
		bool isWindowTypeDock( Display* display, Window window );

		// checking if window is of Toolbar type
		bool isWindowTypeToolbar( Display* display, Window window );

		// checking if window is of Menu type
		bool isWindowTypeMenu( Display* display, Window window );

		// checking if window is of Utility type
		bool isWindowTypeUtility( Display* display, Window window );

		// checking if window is of Splash type
		bool isWindowTypeSplash( Display* display, Window window );

		// checking if window is of Dialog type
		bool isWindowTypeDialog( Display* display, Window window );

		// checking if window is of Normal type
		bool isWindowTypeNormal( Display* display, Window window );

		// checking if a window must have decorations
		bool isWindowSupposedTohaveDecorations( Display* display, Window window );



		//########################################################################

		// checking if window is Modal
		bool isWindowModal( Display* display, Window window );

		// checking if window is Fullscreen
		bool isWindowFullscreen( Display* display, Window window );

		// checking if window DemandsAttention
		bool isWindowDemandingAttention( Display* display, Window window );
	};
};

#endif

