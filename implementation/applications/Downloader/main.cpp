#include "downloader/downloader.h"

int main( int argc, char* argv[] )
{
	// creating the qt-application object
	QApplication qApplication( argc, argv );
	qApplication.setQuitOnLastWindowClosed( true );
	
	// creating downloader object, and showing it
	Downloader downloader;
	downloader.initEnvironement();
	downloader.show();
	
	// running the application
	return qApplication.exec();
}
