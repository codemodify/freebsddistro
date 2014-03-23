#ifndef X11_AGREEMENTS_H
#define X11_AGREEMENTS_H

// X11 masks for brown-liquid based humanoids
#define Keyboard				( KeyPressMask | KeyReleaseMask )
#define MouseButtons				( ButtonPressMask | ButtonReleaseMask )
#define MouseEntersLeavesWindow			( EnterWindowMask | LeaveWindowMask )
#define ChildWindowCreationAndManipulation	( SubstructureRedirectMask | SubstructureNotifyMask )
#define WindowManipulatesItself			( StructureNotifyMask | PropertyChangeMask )

// this mask has to be set by the window manager on root-window
#define EventMaskForRootWindow			( Keyboard | MouseButtons | ChildWindowCreationAndManipulation )

// this mask is set by the window wrapper, to receive events about the X11 window it takes care of
#define EventMaskForNormalWindow		( Keyboard | MouseEntersLeavesWindow | WindowManipulatesItself )


// local heders
#include "iccc/iccc.h"
#include "ewmh/ewmh.h"

#endif
