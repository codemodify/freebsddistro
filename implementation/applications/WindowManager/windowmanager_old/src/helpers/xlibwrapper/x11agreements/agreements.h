#ifndef X11_AGREEMENTS_H
#define X11_AGREEMENTS_H

// X11 masks for brown-liquid based humanoids
#define Keyboard			KeyPressMask | KeyReleaseMask
#define MouseButtons			ButtonPressMask | ButtonReleaseMask
#define MouseEntersWindow		EnterWindowMask
#define WindowCreationAndManipulation	SubstructureRedirectMask
#define WindowManipulatesItself		StructureNotifyMask | PropertyChangeMask
#define SubwindowManipulatesItself	SubstructureNotifyMask | PropertyChangeMask
#define EVENT_MASK_FOR_ROOT_WINDOW ( Keyboard | MouseButtons | WindowCreationAndManipulation )
#define EVENT_MASK_FOR_NORMAL_WINDOW ( Keyboard | MouseEntersWindow | WindowManipulatesItself )
#define EVENT_MASK_FOR_PARENT_FOR_NORMAL_WINDOW ( Keyboard | SubwindowManipulatesItself )


// local heders
#include "iccc.h"
#include "ewmh.h"

#endif
