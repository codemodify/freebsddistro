
// Qt headers
#include <QtGui/QApplication>

// local headers
#include "gui/gui.h"

/******************************************************************************************
*
*	This program test the functionality of the "XLibWrapper" namespace.
*
*******************************************************************************************/

int main( int argc, char** argv )
{
	QApplication qApplication( argc, argv );

	Gui gui;
	gui.show();

	return qApplication.exec();
}

