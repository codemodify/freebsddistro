
// Qt headers
#include <QtGui/QApplication>

// local headers
#include "Tester/Tester.h"

int main( int argc, char** argv )
{
    QApplication qApplication( argc, argv );

    Tester* tester = new Tester();
            tester->show();

    return qApplication.exec();
}
