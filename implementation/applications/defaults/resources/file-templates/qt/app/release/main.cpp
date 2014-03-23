
// Qt headers
#include <QtGui/QApplication>

// local headers
#include "MyForm.h"

int main( int argc, char** argv )
{
    QApplication qApplication( argc, argv );

    MyForm myForm;
    myForm.show();

    return qApplication.exec();
}
