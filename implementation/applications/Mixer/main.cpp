#include "mixer/mixer.h"

int main( int argc, char* argv[] )
{	
	// creating the qt-application object
	QApplication qapplication(argc, argv);
	qapplication.setQuitOnLastWindowClosed(true);
	
	// creating mixer object, and showing it
	Mixer mixer;
	if( false == mixer.initEnvironement() )
		return -1;
	mixer.show();
	
	// running the application
	return qapplication.exec();
}
