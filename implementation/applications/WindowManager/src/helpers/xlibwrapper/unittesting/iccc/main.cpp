
// local headers
#include "x11agreements/iccc/iccc.h"

/******************************************************************************************
*
*	This program show that the ICCC namespace builds without errors.
*
*	To test functionality for the ICCC use the sample provided for 'XlibWrapper',
*	because there is a need for something more that is provided by that namespace.
*
*******************************************************************************************/

int main( int argc, char** argv )
{
	X11Agreements::ICCC::initAtoms(0);


	return 0;
}

