
// Qt headers
#include <QtGui/QApplication>

// local headers
#include "Developer.h"

int main( int argc, char** argv )
{
    QApplication    qApplication( argc, argv );
                    qApplication.setQuitOnLastWindowClosed( true );

    Developer   developer;
                developer.show();

    return qApplication.exec();
}
