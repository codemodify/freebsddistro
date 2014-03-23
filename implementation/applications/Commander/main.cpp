
// Qt headers
#include <QtGui/QApplication>

// local headers
#include "Commander.h"

int main( int argc, char** argv )
{
    QApplication	qApplication( argc, argv );
				qApplication.setQuitOnLastWindowClosed( false );

    Commander	commander;
    			commander.show();

    return qApplication.exec();
}
