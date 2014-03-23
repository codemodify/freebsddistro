
// Qt headers
#include <QtGui/QApplication>

// local headers
#include "CommunicationCenter.h"

int main( int argc, char** argv )
{
    QApplication	qApplication( argc, argv );
				qApplication.setQuitOnLastWindowClosed( false );

    CommunicationCenter	communicationCenter;
						communicationCenter.show();

    return qApplication.exec();
}
