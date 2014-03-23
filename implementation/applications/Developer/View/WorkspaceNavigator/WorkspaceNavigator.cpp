
// Qt's headers
#include <QtCore/QFileInfo>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QToolButton>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>

// blocks
#include <Configurator/Configurator.h>
#include <ConfiguratorReader/ConfiguratorReader.h>
#include <Deployer/Deployer.h>

// local headers
#include "WorkspaceNavigator.h"
#include "WorkspaceFile.h"
#include "ProjectProperties/ProjectProperties.h"


WorkspaceNavigator::WorkspaceNavigator( QString workspacePath ) :
    FileSystemBrowser()
{
    setWindowTitle( "Workspace" );

    _workspacePath = workspacePath;

    initTitleBar();

    initMenu();
    
    initContent();
}

WorkspaceNavigator::~WorkspaceNavigator()
{
    delete _workspaceFile;
}

bool WorkspaceNavigator::isWorkspaceValid( QString workspaceFolder )
{
    return QFileInfo( QString("%1/%2").arg(workspaceFolder).arg(WorkspaceFile::FileName) ).exists();
}

void WorkspaceNavigator::createWorkspace( QString workspaceFolder )
{
    Settings    settings;
                settings.setFile( QString("%1/%2").arg(workspaceFolder).arg(WorkspaceFile::FileName) );
                settings.save();
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// inherited stuff
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void WorkspaceNavigator::initMenu()
{
    ActionIconProvider actionIconProvider;

    _fileBrowserMenu.addSeparator();
    addGroupMarkToMenu( "Project Management" );
    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("active"), "set as current", this, SLOT(workspaceNavigatorSetAsCurrentProject()));
    
    _buildMenu.setTitle( "build" );
    _buildMenu.setIcon( actionIconProvider.getIconForAction("compile") );

    _fileBrowserMenu.addMenu( &_buildMenu );
    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("add"),    "create project", this, SLOT(workspaceNavigatorCreateProject()));
    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("delete"), "delete project", this, SLOT(workspaceNavigatorDeleteProject()));
}

void WorkspaceNavigator::contextMenuAboutToPopup()
{
    // get the project
    QString project = getCurrentProject();
    if( project.isEmpty() )
        return;
    
    ActionIconProvider iconProvider;

    _buildMenu.clear();
    _buildMenu.addAction( iconProvider.getIconForAction("run"),       "Run", this, SLOT(workspaceNavigatorRunProject()));
    _buildMenu.addAction( iconProvider.getIconForAction("debug_run"), "Debug", this, SLOT(workspaceNavigatorDebugProject()));
    _buildMenu.addSeparator();
    
    int builtTargetsCount = _workspaceFile->getBuildTargetsCountForProject( project );
    for( int buildTargetIndex=0; buildTargetIndex < builtTargetsCount; buildTargetIndex++ )
    {
        QAction* action = _buildMenu.addAction
        (
            iconProvider.getIconForAction("compile"), _workspaceFile->getBuildTargetNameForProject(project,buildTargetIndex),
            this, SLOT(workspaceNavigatorExecuteBuildTarget())
        );
        
        // we'll this later to execute the right command
        action->setData( buildTargetIndex );
    }
    _buildMenu.addSeparator();
    
    _buildMenu.addAction( iconProvider.getIconForAction("edit"), "Edit Build Targets", this, SLOT(workspaceNavigatorEditBuildTargets()));
}

void WorkspaceNavigator::initTitleBar()
{
    ActionIconProvider actionIconProvider;

    // init 1    
    QAction*    compile = new QAction( 0 );
                compile->setIcon( actionIconProvider.getIconForAction("compile") );

    QAction*    run = new QAction( 0 );
                run->setIcon( actionIconProvider.getIconForAction("run") );

    QAction*    debug = new QAction( 0 );
                debug->setIcon( actionIconProvider.getIconForAction("debug_run") );
                    
    QAction*    fileFind = new QAction( 0 );
                fileFind->setIcon( actionIconProvider.getIconForAction("find2") );

    // init 2
    addTitleButton( compile , this,SLOT(workspaceNavigatorExecuteShortcutCompile())          );
    addTitleButton( run     , this,SLOT(workspaceNavigatorRunProject())                      );
    addTitleButton( debug   , this,SLOT(workspaceNavigatorDebugProject())                    );
    addTitleButton( fileFind, this,SLOT(workspaceNavigatorExecuteShortcutSearchInFiles())    );
}

void WorkspaceNavigator::initContent()
{
    _fileTreeView->setRootFolder( _workspacePath );
    
    _workspaceFile = new WorkspaceFile( _workspacePath );
    
    connect( &_process, SIGNAL(readyReadStandardError()),           this, SLOT(readBuildTargetStandardError())  );
    connect( &_process, SIGNAL(readyReadStandardOutput()),          this, SLOT(readBuildTargetStandardOutput()) );
    connect( &_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(readBuildTargetStandardOutput()) );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// project management
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void WorkspaceNavigator::workspaceNavigatorSetAsCurrentProject()
{
    _currentProject = "";
    setTitleText( getCurrentProject() );
}

void WorkspaceNavigator::workspaceNavigatorCreateProject()
{
	QString configFile = QApplication::applicationFilePath() + ".wizard";

	Configurator configurator( QStringList()<<configFile );
	if( QDialog::Accepted != configurator.exec() )
		return;

	ConfiguratorReader configuratorReader( configFile );

	// get project type
	QString projectType = configurator.getActiveSectionItem();

	// get project name
	QString projectName = configuratorReader.getLineInput( projectType + "/projectName" );

	// get app or lib
    QString appLibrary = configuratorReader.getCheckedRadioBox( projectType + "/appLibrary" );

	// get debug or release
    QString debugRelease = configuratorReader.getCheckedRadioBox( projectType + "/debugRelease" );

	Deployer	deployer;
			deployer.setSourceAndTarget( QApplication::applicationDirPath()+"/resources/file-templates/", _workspacePath +"/"+ projectName );
			deployer.deploy( QString("%1/%2/%3").arg(projectType).arg(appLibrary).arg(debugRelease) );

     _fileTreeView->refreshContent();
}

void WorkspaceNavigator::workspaceNavigatorDeleteProject()
{
    if( QMessageBox::Yes == QMessageBox::question(this,"","Confirm project removal ?",QMessageBox::Yes,QMessageBox::No) )
    {
        QString projectName = getCurrentProject();

        _fileSystemOperations.removeFolder( _workspacePath +"/"+ projectName );
        _workspaceFile->deleteProject( projectName );
        _fileTreeView->refreshContent();
    }
}

void WorkspaceNavigator::workspaceNavigatorRunProject()
{
    QString runAction = _workspaceFile->getRunActionForProject( getCurrentProject() );
    if( runAction.isEmpty() )
    {
        workspaceNavigatorEditBuildTargets();
        return;
    }
    
    runBuildTarget( runAction );
}

void WorkspaceNavigator::workspaceNavigatorDebugProject()
{
    QString debugAction = _workspaceFile->getDebugActionForProject( getCurrentProject() );
    if( debugAction.isEmpty() )
    {
        workspaceNavigatorEditBuildTargets();
        return;
    }
    
    runBuildTarget( debugAction );
}

void WorkspaceNavigator::workspaceNavigatorExecuteBuildTarget()
{
    QAction* action = qobject_cast<QAction*>( sender() );
    if( 0 == action )
        return;

    // get the build target index
    int buildTargetIndex = action->data().toInt();
    QString buildTargetAction = _workspaceFile->getBuildTargetActionForProject( getCurrentProject(), buildTargetIndex );

    runBuildTarget( buildTargetAction );
}

void WorkspaceNavigator::workspaceNavigatorEditBuildTargets()
{
    ProjectProperties*  projectProperties = new ProjectProperties( *_workspaceFile, getCurrentProject() );
                        projectProperties->exec();
}

void WorkspaceNavigator::workspaceNavigatorExecuteShortcutCompile()
{
    // get the build target index
    int buildTargetIndex = 0;
    QString buildTargetAction = _workspaceFile->getBuildTargetActionForProject( getCurrentProject(), buildTargetIndex );

    runBuildTarget( buildTargetAction );    
}

void WorkspaceNavigator::workspaceNavigatorExecuteShortcutSearchInFiles()
{
    QString currentFolder = _fileTreeView->getCurrentItemPath();

	if( !QFileInfo(currentFolder).isDir() )
		currentFolder = QFileInfo(currentFolder).path();

    emit search( currentFolder );
}

void WorkspaceNavigator::runBuildTarget( QString action )
{
	// inform that we need the output console to be visible
	emit makeOutputConsoleVisible();

	// run the build process
	QString workingDirectory = _workspacePath + "/" + getCurrentProject();
	emit outputConsoleClear();
	emit outputConsoleAdd( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----" );
	emit outputConsoleAdd( QString("// \"%1\"    --    \"%2\"").arg(workingDirectory).arg(action)                                   );
	emit outputConsoleAdd( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----" );

	_process.setWorkingDirectory( workingDirectory );
	_process.start( action );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// helpers
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
QString WorkspaceNavigator::getCurrentProject()
{
    if( false == _currentProject.isEmpty() )
        return _currentProject;

qDebug( "WorkspaceNavigator::getCurrentProject()" );
    QString project = _fileTreeView->getCurrentItemPath();
qDebug( project.toAscii().data() );
            project.remove( _workspacePath );
qDebug( project.toAscii().data() );
                
    QStringList paths = project.split( "/" );
                paths.removeAll( "" );

    project = paths.value( 0 );
qDebug( project.toAscii().data() );


    _currentProject = project;
    return project;
}

void WorkspaceNavigator::readBuildTargetStandardError()
{
	QByteArray byteArray = _process.readAllStandardError(); 
	if( false == byteArray.isEmpty() )
		emit outputConsoleAddError( byteArray );
}

void WorkspaceNavigator::readBuildTargetStandardOutput()
{
	QByteArray byteArray = _process.readAllStandardOutput();
	if( false == byteArray.isEmpty() )
		emit outputConsoleAdd( byteArray );
}

void WorkspaceNavigator::readBuildFinishStatus( int exitCode, QProcess::ExitStatus exitStatus )
{
	emit outputConsoleAdd( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----" );
	emit outputConsoleAdd( QString("// Done. exitCode=%1, exitStatus=%2").arg(exitCode).arg(exitStatus)                             );
	emit outputConsoleAdd( "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----" );
}

