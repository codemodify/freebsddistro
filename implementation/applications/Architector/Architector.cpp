
// Qt headers
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QFileDialog>

// blocks
#include <Styling/Styling.h>
#include <IconProvider/ActionIconProvider/ActionIconProvider.h>
#include <FileLauncher/FileLauncher.h>
#include <Configurator/Configurator.h>
#include <ConfiguratorReader/ConfiguratorReader.h>
#include <Pluginer/PluginManager.h>

// local headers
#include "Architector.h"
#include "Architecture.Renderer.IRendererFactory.h"
#include "Architecture.Shapes.h"
#include "Architecture.Ui.h"


Architector::Architector() :
    QMainWindow(),
    _pluginManager( 0 )
{
    // self cosmetics
    setWindowTitle( "Architector" );
    setWindowIcon( QIcon(QApplication::applicationFilePath() + ".png") );

    Styling styling( *this );
            styling.setTitle();
            styling.setIcon();
            styling.loadOnScreenPosition();
            styling.loadFonts();

    // setup the plugins
    setupPlugins();

    // setup the workspace
    setupMenu();
    
	_mdiArea = new QMdiArea();

	setCentralWidget( _mdiArea );
}

Architector::~Architector()
{}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Menu
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Architector::setupMenu()
{
    QMenu*  architectorMenu = menuBar()->addMenu( "Architector" );
            architectorMenu->addAction( ActionIconProvider().getIconForAction("help")       , "Help Me!"            , this, SLOT(helpMe())              );
            architectorMenu->addSeparator();
            architectorMenu->addAction( ActionIconProvider().getIconForAction("newfile")    , "New Architecture"    , this, SLOT(newArchitecture())     );
            architectorMenu->addAction( ActionIconProvider().getIconForAction("newfolder")  , "Open Architecture"   , this, SLOT(openArchitecture())    );
            architectorMenu->addSeparator();
            architectorMenu->addAction( ActionIconProvider().getIconForAction("exit")       , "Exit"                , this, SLOT(close())               );
}

void Architector::helpMe()
{
    QStringList configuratorParameter;
                configuratorParameter << Configurator::getAppConfigurationFile();
                configuratorParameter << "helpMe";
                configuratorParameter << "quickHelp";

	Configurator* configurator = new Configurator( configuratorParameter, _pluginManager );
	connect( configurator, SIGNAL(configurationChanged()), this, SLOT(readConfiguration()) );

	configurator->exec();
}

void Architector::newArchitecture()
{
    openArchitectureByFilePath( "" );
}

void Architector::openArchitecture()
{
 	QString fileName = QFileDialog::getOpenFileName( this, "Architecture File", "/home" );
	if( fileName.isEmpty() )
		return;

	openArchitectureByFilePath( fileName );    
}

void Architector::closeEvent( QCloseEvent* event )
{
    Styling styling( *this );
            styling.saveOnScreenPosition();

    QMainWindow::closeEvent( event );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//      this takes care of the plugins
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Architector::setupPlugins()
{
    if( 0 == _pluginManager )
    {
        _pluginManager = new PluginManager();

        ConfiguratorReader configuratorReader;
        
        QStringList plugins = configuratorReader.getPlugins( "plugins/plugins/plugins" );

        foreach( QString plugin, plugins )
        {
            QString errorString;

            _pluginManager->loadPlugin( plugin, errorString );
        }
    }
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Helpers
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Architector::openArchitectureByFilePath( QString filePath )
{
    QList< IRendererFactory* > rendederList;

    getLoadedPluginsFromPluginManager< IRendererFactory >( *_pluginManager, rendederList );

    if( rendederList.isEmpty() )
        return;

    Shapes*             model       = Shapes::loadFromFile( filePath );
    IRendererFactory*   view        = rendederList.value( 0 );
    Ui*                 controller  = new Ui( *model, *view );

	_mdiArea->addSubWindow( controller );
	controller->showMaximized();    
}

void Architector::readConfiguration()
{
    ConfiguratorReader configuratorReader;
    //QStringList stringList = configuratorReader.getCheckedCheckBoxes( "fileOperations/delete/trashOptions" );

    //_fileSystemOperations->setDeleteToTrash( stringList.contains("deleteToTrash") );
    //_fileSystemOperations->setTrashPath( configuratorReader.getFileSystemPath("fileOperations/delete/trashFolder") );
}

