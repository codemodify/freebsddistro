
// local headers
#include "Architector.h"

int main( int argc, char** argv )
{
    QApplication qApplication( argc, argv );

    Architector architector;
                architector.show();

    return qApplication.exec();
}
