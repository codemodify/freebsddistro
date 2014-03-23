
// local headers
#include "x11agreements/ewmh/ewmh.h"

/******************************************************************************************
*
*	This program show that the EWMH namespace builds without errors.
*
*	To test functionality for the EWMH use the sample provided for 'XlibWrapper',
*	because there is a need for something more that is provided by that namespace.
*
*******************************************************************************************/

int main( int argc, char** argv )
{
	X11Agreements::EWMH::initAtoms(0);

	return 0;
}

