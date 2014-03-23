
// Qt's headers
#include <QtCore/QProcess>
#include <QtCore/QFileInfo>
#include <QtGui/QTabWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLayout>

// blocks
#include <FileSystemOperations/FileSystemOperations.h>
#include <Settings/Settings.h>
#include <Styling/Styling.h>
#include <IconProvider/ActionIconProvider/ActionIconProvider.h>
#include <Configurator/Configurator.h>

// local headers
#include "Developer.h"
#include "View/View.h"
#include "View/FileSystemBrowser/FileSystemBrowser.h"
#include "View/WorkspaceNavigator/WorkspaceNavigator.h"
#include "View/Editor/Editor.h"
#include "View/OutputConsole/OutputConsole.h"
#include "View/SearchResult/SearchResult.h"



Developer::Developer() :
    QMainWindow(),
    _statusBar( 0 ),
    _tabWidget( 0 ),
    _fileSystemBrowser( 0 ),
    _workspaceNavigator( 0 ),
    _outputConsole( 0 )
{
    initDeveloperMenu();
    initWindowsMenu();

    initViews();
    initTabs();
    initStatusBar();

    // open the settings file
    Styling styling( *this );
            styling.loadFonts();
            styling.loadOnScreenPosition();
}

Developer::~Developer()
{}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//    inherited overrides
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Developer::closeEvent( QCloseEvent* event )
{
    Styling styling( *this );
            styling.saveOnScreenPosition();

    QMainWindow::closeEvent( event );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//    here goes the "Developer" menu related stuff
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Developer::initDeveloperMenu()
{
    ActionIconProvider actionIconProvider;

    QMenu* developerMenu = menuBar()->addMenu( "Developer" );

    // help me    
    developerMenu->addAction( actionIconProvider.getIconForAction("help"), "Help Me!", this, SLOT(handleMenuItemHelpMe()) );
    developerMenu->addSeparator();
    
    // workspace
    QMenu* workspaceMenu = developerMenu->addMenu( actionIconProvider.getIconForAction("workspace"), "Workspace" );
    workspaceMenu->addAction( actionIconProvider.getIconForAction("workspace-switch"),       "Switch Workspace", this, SLOT(handleMenuItemSwitchWorkspace()) );
    workspaceMenu->addSeparator();

    QStringList recentWorspaces;
    Settings    settings;
                settings.setFile( Settings::getSettingsFileForApp() );
                settings.getValuesForKey( "recentWorkspaces", recentWorspaces );

    foreach( QString worspace, recentWorspaces )
        workspaceMenu->addAction( actionIconProvider.getIconForAction("workspace"), worspace, this, SLOT(handleMenuItemOpenRecentWorkspace()) );

    // exit
    developerMenu->addSeparator();    
    developerMenu->addAction( actionIconProvider.getIconForAction("exit"), "Exit", this, SLOT(handleMenuItemExit()) );
}

void Developer::handleMenuItemHelpMe()
{
    QStringList configuratorParameter;
                configuratorParameter << Configurator::getAppConfigurationFile();
                configuratorParameter << "helpMe";
                configuratorParameter << "quickHelp";

	Configurator*	configurator = new Configurator( configuratorParameter );
	//connect( configurator, SIGNAL(configurationChanged()), this, SLOT(readConfiguration()) );

	configurator->exec();
}

void Developer::handleMenuItemSwitchWorkspace()
{
    // getting a folder to be used as workspace
    QString workspaceFolder = QFileDialog::getExistingDirectory
    (
                this, 
                "Choose Workspace Folder",
                "/",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    
    if( true == workspaceFolder.isEmpty() )
        return;

    switchWorkspace( workspaceFolder );
}

void Developer::handleMenuItemOpenRecentWorkspace()
{
    QAction* action = qobject_cast<QAction*>( sender() );
    if( 0 == action )
        return;

    switchWorkspace( action->text() );
}

void Developer::handleMenuItemExit()
{
    Styling styling( *this );
            styling.saveOnScreenPosition();

    close();
}

void Developer::switchWorkspace( QString workspaceFolder )
{
    // if that is a new workspace then create
    if( false == WorkspaceNavigator::isWorkspaceValid(workspaceFolder) )
        WorkspaceNavigator::createWorkspace( workspaceFolder );

    // once this is a valid workspace we can add it to the recent workspace list
    QStringList recentWorkspaces;
    Settings    settings;
                settings.setFile( Settings::getSettingsFileForApp() );
                settings.getValuesForKey( "recentWorkspaces", recentWorkspaces );

    recentWorkspaces.removeAll( workspaceFolder );
    recentWorkspaces.append( workspaceFolder );

    settings.setValuesForKey( "recentWorkspaces", recentWorkspaces );
    settings.save();

    // destroy current workspace-navigator
    if( 0 != _workspaceNavigator )
    {
        _workspaceNavigator->hide();
        delete _workspaceNavigator;
    }

    _workspaceNavigator = new WorkspaceNavigator( QFileInfo(workspaceFolder).canonicalFilePath() );
    addDockWidget( Qt::RightDockWidgetArea, _workspaceNavigator, Qt::Vertical );
    connect( _workspaceNavigator, SIGNAL(editFile(QString)), this, SLOT(handleRequestToEditFile(QString)) );

    connect( _workspaceNavigator, SIGNAL(outputConsoleAdd       (QString)), _outputConsole, SLOT(outputConsoleAdd       (QString)    ) );
    connect( _workspaceNavigator, SIGNAL(outputConsoleAddWarning(QString)), _outputConsole, SLOT(outputConsoleAddWarning(QString)    ) );
    connect( _workspaceNavigator, SIGNAL(outputConsoleAddError  (QString)), _outputConsole, SLOT(outputConsoleAddError  (QString)    ) );
    connect( _workspaceNavigator, SIGNAL(outputConsoleClear     ()       ), _outputConsole, SLOT(outputConsoleClear     ()           ) );
    connect( _workspaceNavigator, SIGNAL(search                 (QString)), this          , SLOT(handleMenuItemSearchInFiles(QString)) );
    connect( _workspaceNavigator, SIGNAL(makeOutputConsoleVisible()      ), this          , SLOT(handleMenuItemOutputConsole()       ) );

    // mix filesystembrowser with workspace
    tabifyDockWidget( _workspaceNavigator, _fileSystemBrowser );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//    windows menu initialization
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Developer::initWindowsMenu()
{
    ActionIconProvider actionIconProvider;

    QMenu* windowsMenu = menuBar()->addMenu( "Windows" );
    
    windowsMenu->addAction( actionIconProvider.getIconForAction("workspace-navigator"), "Workspace Navigator",  this, SLOT(handleMenuItemWorkspaceNavigator())  );
    windowsMenu->addAction( actionIconProvider.getIconForAction("filesystembrowser"),   "File System Browser",  this, SLOT(handleMenuItemFileSystemBrowser())   );
    windowsMenu->addAction( actionIconProvider.getIconForAction("console"),             "Output Console",       this, SLOT(handleMenuItemOutputConsole())       );
    windowsMenu->addAction( actionIconProvider.getIconForAction("find"),                "Search In Files",      this, SLOT(handleMenuItemSearchInFiles())		);
}

void Developer::handleMenuItemWorkspaceNavigator()
{
    if( 0 != _workspaceNavigator )
        _workspaceNavigator->show();
}

void Developer::handleMenuItemFileSystemBrowser()
{
    _fileSystemBrowser->show();
}

void Developer::handleMenuItemOutputConsole()
{
    _outputConsole->show();
	_searchResult->hide();
}

void Developer::handleMenuItemSearchInFiles( QString folder )
{
	SearchResult* searchResult = (SearchResult*) _searchResult;
	if( 0 == searchResult )
		return;

	searchResult->setSearchFolder( folder );
	searchResult->show();
}

void Developer::handleMenuItemSearchInFiles()
{
	handleMenuItemSearchInFiles( "/" );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//    here goes the "views" related stuff
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Developer::initViews()
{
    // add the file system browser
    _fileSystemBrowser = new FileSystemBrowser();
    addDockWidget( Qt::RightDockWidgetArea, _fileSystemBrowser, Qt::Vertical );
    _fileSystemBrowser->hide();

    connect( _fileSystemBrowser, SIGNAL(editFile(QString)), this, SLOT(handleRequestToEditFile(QString)) );

    // add the console output view
    _outputConsole = new OutputConsole();
    addDockWidget( Qt::BottomDockWidgetArea, _outputConsole, Qt::Horizontal );
    _outputConsole->hide();

	// search result
	_searchResult = new SearchResult();
    connect( _searchResult, SIGNAL(editFile(QString)), this, SLOT(handleRequestToEditFile(QString)) );
	
	addDockWidget( Qt::BottomDockWidgetArea, _searchResult, Qt::Horizontal );
	tabifyDockWidget( _outputConsole, _searchResult );
	_searchResult->hide();
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//    other inits
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Developer::initTabs()
{
    _tabWidget = new QTabWidget();
    _tabWidget->setMovable( true );

    setCentralWidget( _tabWidget );
}

void Developer::initStatusBar()
{
    _statusBar = new QStatusBar();
//    setStatusBar( _statusBar );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//    this will handle the request to edit a file by its path
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Developer::handleRequestToEditFile( QString filePath )
{
    int index = 0;

    // seeking for an existing opened file
    for( ; index < _tabWidget->count(); ++index )
    {
        QWidget* widget = _tabWidget->widget( index );
        Editor* editor = qobject_cast<Editor*>( widget );
        if( 0 == editor )
            continue;

        if( editor->getFilePath() == filePath )
            break;
    }

    // if it was not opened then open it in a new editor
    if( index >= _tabWidget->count() )
    {
        Editor* editor = new Editor( filePath );
        connect( editor, SIGNAL(fileModified()), this, SLOT(fileWasModified()) );
        connect( editor, SIGNAL(fileSaved())   , this, SLOT(fileWasSaved   ()) );

        index = _tabWidget->addTab( editor, QFileInfo(filePath).fileName() );
    }

    // focusing the tab for the file beeing asked
    _tabWidget->setCurrentIndex( index );
}

void Developer::fileWasModified()
{
	QWidget* editor = qobject_cast<QWidget*>( sender() );
	if( 0 == editor )
		return;

	for( int index=0; index < _tabWidget->count(); index++ )
	{
		if( editor == _tabWidget->widget(index) )
		{
			QString currentTabText = _tabWidget->tabText( index );

			_tabWidget->setTabText( index, "* " + currentTabText );
		}
	}
}

void Developer::fileWasSaved()
{
	QWidget* editor = qobject_cast<QWidget*>( sender() );
	if( 0 == editor )
		return;

	for( int index=0; index < _tabWidget->count(); index++ )
	{
		if( editor == _tabWidget->widget(index) )
		{
			QString currentTabText = _tabWidget->tabText( index );

			_tabWidget->setTabText( index, currentTabText.remove("* ") );
		}
	}
}
