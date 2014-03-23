
// Qt headers
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
//#include <QtGui/QPlainTextEdit>
#include <QtGui/QGridLayout>

// blocks headers
#include <Pluginer/IPlugin.h>

// local headers
#include "PluginDetails.h"


PluginDetails::PluginDetails( IPlugin& plugin ) :
    QDialog()
{
    const PluginInfo& pluginInfo = plugin.getPluginInfo();

    QLabel* nameProperty            = new QLabel( "name:"           );
    QLabel* descriptionProperty     = new QLabel( "description:"    );
    QLabel* versionProperty         = new QLabel( "version:"        );
    QLabel* compatVersionProperty   = new QLabel( "compatVersion:"  );
    QLabel* copyrightProperty       = new QLabel( "copyright:"      );
    QLabel* licenseProperty         = new QLabel( "license:"        );
    QLabel* urlProperty             = new QLabel( "url:"            );

    QLabel* nameValue            = new QLabel( pluginInfo.name           );
    QLabel* descriptionValue     = new QLabel( pluginInfo.description    );
    QLabel* versionValue         = new QLabel( pluginInfo.version        );
    QLabel* compatVersionValue   = new QLabel( pluginInfo.compatVersion  );
    QLabel* copyrightValue       = new QLabel( pluginInfo.copyright      );
    QLabel* licenseValue         = new QLabel( pluginInfo.license        );
    QLabel* urlValue             = new QLabel( pluginInfo.url            );

    QPushButton* okButton = new QPushButton( "Ok" );
    connect( okButton, SIGNAL(released()), this, SLOT(close()) );

    QGridLayout*    gridLayout = new QGridLayout( this );
                    gridLayout->addWidget( nameProperty,            0,0 );
                    gridLayout->addWidget( descriptionProperty,     1,0 );
                    gridLayout->addWidget( versionProperty,         2,0 );
                    gridLayout->addWidget( compatVersionProperty,   3,0 );
                    gridLayout->addWidget( copyrightProperty,       4,0 );
                    gridLayout->addWidget( licenseProperty,         5,0 );
                    gridLayout->addWidget( urlProperty,             6,0 );

                    gridLayout->addWidget( nameValue,               0,1 );
                    gridLayout->addWidget( descriptionValue,        1,1 );
                    gridLayout->addWidget( versionValue,            2,1 );
                    gridLayout->addWidget( compatVersionValue,      3,1 );
                    gridLayout->addWidget( copyrightValue,          4,1 );
                    gridLayout->addWidget( licenseValue,            5,1 );
                    gridLayout->addWidget( urlValue,                6,1 );
                    
                    gridLayout->addWidget( okButton,                7,1 );
}

void PluginDetails::closeEvent( QCloseEvent* event )
{
    QDialog::closeEvent( event );

    deleteLater();
}
