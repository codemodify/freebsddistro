
// Qt headers
#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>

// local headers
#include "PluginerUi.h"
#include "PluginDetails.h"


PluginerUi::PluginerUi( PluginManager& pluginManager ) :
    _pluginManager( pluginManager )
{
    _pluginDirectory = QApplication::applicationDirPath() + "/plugins";

    // prepare the list
    _listWidget = new QListWidget();
    connect( _listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(pluginDoubleClicked(QListWidgetItem*))    );
    connect( _listWidget, SIGNAL(itemChanged      (QListWidgetItem*)), this, SLOT(pluginCheckedUnchecked(QListWidgetItem*)) );

    // prepare the refrech button
    QPushButton* refreshButton = new QPushButton( "refresh" );
    connect( refreshButton, SIGNAL(released()), this, SLOT(refreshPluginList()) );      
    
    // assemble the UI controls
    QHBoxLayout*    hBoxLayout = new QHBoxLayout();
                    hBoxLayout->addWidget( refreshButton );
                    hBoxLayout->addSpacerItem( new QSpacerItem(0,0) );

    QVBoxLayout*    vBoxLayout = new QVBoxLayout( this );
                    vBoxLayout->addWidget( _listWidget );
                    vBoxLayout->addLayout( hBoxLayout );

    refreshPluginList();
}

void PluginerUi::pluginDoubleClicked( QListWidgetItem* item )
{
    // show info about the plugin only if it is loaded
    if( Qt::Unchecked == item->checkState() )
        return;

    IPlugin* plugin = 0;
    _pluginManager.getPluginByFileName( _pluginDirectory + "/" + item->text(), plugin );

    if( 0 == plugin )
        return;

    PluginDetails*  pluginDetails = new PluginDetails( *plugin );
                    pluginDetails->exec();
}

void PluginerUi::pluginCheckedUnchecked( QListWidgetItem* item )
{
    // check to see if this plugin failed to load
    if( Qt::NoItemFlags == item->flags() )
        return;

    // if it was checked then try to load it
    if( Qt::Checked == item->checkState() )
    {
        QString errorString;
    
        IPlugin* plugin = _pluginManager.loadPlugin( _pluginDirectory + "/"+ item->text(), errorString );
    
        if( 0 == plugin )
        {
            item->setFlags( Qt::NoItemFlags );
            item->setCheckState( Qt::Unchecked );
            item->setText ( "Load Failed: " + item->text() );
    
            const QString c_title = "Plugin Load Error";
            const QString c_text  = "Error details: " + errorString;
    
            QMessageBox::critical( this, c_title, c_text );
        }

        return;
    }

    // unload the plugin because it was unChecked
    _pluginManager.unLoadPlugin( _pluginDirectory + "/"+ item->text() );
}

void PluginerUi::refreshPluginList()
{
    // remove all previous items        
    while( _listWidget->count() )
    {
        QListWidgetItem* item = _listWidget->takeItem( 0 );
        delete item;
    }

    // refrech the list of plugins but "check-the-item" for those that are already loaded
    QStringList pluginFiles = QDir( _pluginDirectory ).entryList
    (
        QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot |QDir::CaseSensitive
    );

    foreach( QString pluginFile , pluginFiles )
    {
        IPlugin* plugin = 0;
        _pluginManager.getPluginByFileName( _pluginDirectory + "/" + pluginFile, plugin );

        QListWidgetItem*    listWidgetItem = new QListWidgetItem( pluginFile );
                            listWidgetItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
                            listWidgetItem->setCheckState( plugin ? Qt::Checked : Qt::Unchecked );

        _listWidget->addItem( listWidgetItem );
    }
}
