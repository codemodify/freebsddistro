
// Qt headers
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QFileDialog>

// local headers
#include "Tester.h"


Tester::Tester() :
    QWidget()
{
    // init controls
    QPushButton* chooseConfigurationFile
        = new QPushButton( "Choose Configuration File" );

    // add controls to the visible area
    QVBoxLayout*    layout = new QVBoxLayout( this );
                    layout->addWidget( chooseConfigurationFile );

    // subscribe for the press events
    connect( chooseConfigurationFile, SIGNAL(pressed()), this, SLOT(slot_chooseConfigurationFile()) );
}

Tester::~Tester()
{}

void Tester::slot_chooseConfigurationFile()
{
    QString fileName = QFileDialog::getOpenFileName
    (
        this,
        "Choose The Configuration File To Test",
        "/home",
        "*.*"
    );

    if( fileName.isEmpty() )
        return;

    QStringList configuratorParameters;
                configuratorParameters << fileName;

	Configurator* configurator = new Configurator( configuratorParameters );
                  configurator->exec();
}
