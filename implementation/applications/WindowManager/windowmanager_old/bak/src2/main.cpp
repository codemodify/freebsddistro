
#include "windowmanager/windowmanager.h"

int main( int argc, char** argv )
{
	WindowManager windowmanager( argc, argv );
	windowmanager.initEnvironment();

	return windowmanager.exec();
}
