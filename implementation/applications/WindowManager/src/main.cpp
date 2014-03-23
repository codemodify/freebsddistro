
#include "windowmanager/windowmanager.h"

int main( int argc, char* argv[] )
{
	WindowManager windowManager( argc, argv );
	if( false == windowManager.initEnvironment() )
		return -1;

	return windowManager.exec();
}
