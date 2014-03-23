#include "mediaplayer/mediaplayer.h"

int main( int argc, char* argv[] )
{	
	// creating the qt-application object
	QApplication qapplication(argc, argv);
	qapplication.setQuitOnLastWindowClosed(true);
	
	// creating downloader object, and showing it
	Mediaplayer mediaplayer;
	mediaplayer.initEnvironement();
	mediaplayer.show();
	
	// running the application
	return qapplication.exec();
}
