
#include <QtGui/QApplication>
#include "bar/bar.h"

int main( int argc, char* argv[] )
{
	// creating the qt-application object
	QApplication qapplication(argc, argv);
	qapplication.setQuitOnLastWindowClosed(true);
	
	// creating downloader object, and showing it
	Bar bar;
	bar.initEnvironement();
	
	// running the application
	return qapplication.exec();
}
