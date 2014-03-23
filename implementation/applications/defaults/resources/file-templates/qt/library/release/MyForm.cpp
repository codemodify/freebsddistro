
// Qt headers
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>

// local headers
#include "MyForm.h"


MyForm::MyForm() :
    QWidget()
{
    QPushButton* pushButton = new QPushButton( "About Qt" );
    
    connect( pushButton, SIGNAL(released()), this, SLOT(showAbout()) );
    
    QHBoxLayout*    hBoxLayout = new QHBoxLayout( this );
                    hBoxLayout->addWidget( pushButton );
}

MyForm::~MyForm()
{}

void MyForm::showAbout()
{
    QMessageBox::aboutQt( this );
}

