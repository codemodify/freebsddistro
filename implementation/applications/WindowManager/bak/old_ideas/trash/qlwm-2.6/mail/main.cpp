/* main.cpp */

#include <qapplication.h>
#include "biff.h"


int main( int argc, char ** argv )
{
    QApplication a(argc, argv);
    biff b;
    a.setMainWidget(&b);
    return a.exec();
}
