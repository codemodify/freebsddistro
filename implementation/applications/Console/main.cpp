
// Qt headers
#include <QtGui/QApplication>

// local headers
#include "Console.h"

int main( int argc, char** argv )
{
    QApplication qapplication(argc, argv);
                 qapplication.setQuitOnLastWindowClosed( true );

    Console console;
            console.initEnvironement();
            console.show();

    return qapplication.exec();
}
