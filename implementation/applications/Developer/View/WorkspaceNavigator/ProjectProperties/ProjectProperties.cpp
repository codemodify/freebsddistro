
// Qt headers
#include <QtGui/QFileDialog>

// local headers
#include "ProjectProperties.h"


ProjectProperties::ProjectProperties( WorkspaceFile& workspaceFile, QString currentProject ) :
    QDialog(),
    _workspaceFile( workspaceFile ),
    _currentProject( currentProject )
{
	setupUi( this );

	setWindowModality( Qt::ApplicationModal );

	connect( _browseFile, 			SIGNAL(clicked()), this, SLOT(slot_BrowseFile())			);

	connect( _addBuildTarget,       SIGNAL(clicked()), this, SLOT(slot_AddBuildTarget())		);
	connect( _removeBuildTarget,    SIGNAL(clicked()), this, SLOT(slot_RemoveBuildTarget())	);

	connect( _runApp,               SIGNAL(textEdited(const QString&)), this, SLOT(runActionTextEdited(const QString&))        );
	connect( _debugApp,             SIGNAL(textEdited(const QString&)), this, SLOT(debugActionTextEdited(const QString&))      );
	connect( _buildTargetAction,    SIGNAL(textEdited(const QString&)), this, SLOT(buildTargetActionEdited(const QString&))    );
	
	connect( _buildTargets,     SIGNAL(itemChanged(QListWidgetItem*)),
	         this,              SLOT(buildTargetsCurrentItemTextChanged(QListWidgetItem*)) );
	
	connect( _buildTargets,     SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
	         this,              SLOT(buildTargetsCurrentItemChanged(QListWidgetItem*,QListWidgetItem*)) );
	         
	// load data
	_runApp->setText    ( _workspaceFile.getRunActionForProject(_currentProject)    );
	_debugApp->setText  ( _workspaceFile.getDebugActionForProject(_currentProject)  );
	
	int buildTargetsCount = _workspaceFile.getBuildTargetsCountForProject( _currentProject );
	for( int index = 0; index < buildTargetsCount; index++ )
	{
        QListWidgetItem*    item = new QListWidgetItem();
                            item->setFlags( item->flags() | Qt::ItemIsEditable );
                            item->setText( _workspaceFile.getBuildTargetNameForProject(_currentProject,index) );
        _buildTargets->addItem( item );
	}
	
	_buildTargetAction->setText( _workspaceFile.getBuildTargetActionForProject(_currentProject, 0) );
}

void ProjectProperties::slot_BrowseFile()
{
    // getting a folder to be used as workspace
    QString fileName = QFileDialog::getOpenFileName
    (
        this,
        "Select App To Run/Debug",
        "/"
    );

    if( true == fileName.isEmpty() )
        return;

    _runApp->setText( fileName );
    _debugApp->setText( fileName );

    runActionTextEdited( fileName );
    debugActionTextEdited( fileName );
}

void ProjectProperties::slot_AddBuildTarget()
{
    QListWidgetItem*    item = new QListWidgetItem();
                        item->setFlags( item->flags() | Qt::ItemIsEditable );
                        item->setText( "New Build Target" );
    _buildTargets->addItem( item );
}

void ProjectProperties::slot_RemoveBuildTarget()
{    
    int row = _buildTargets->currentRow();
    
    QListWidgetItem* item = _buildTargets->takeItem( row );
    delete item;
    
    _workspaceFile.deleteBuildTarget( _currentProject, row );
}

void ProjectProperties::runActionTextEdited( const QString& text )
{
    _workspaceFile.setRunActionForProject( _currentProject, text );
}

void ProjectProperties::debugActionTextEdited( const QString& text )
{
    _workspaceFile.setDebugActionForProject( _currentProject, text );
}

void ProjectProperties::buildTargetActionEdited( const QString& text )
{
    QListWidgetItem* currentItem = _buildTargets->currentItem();
    if( 0 == currentItem )
        return;

    _workspaceFile.setBuildTargetActionForProject( _currentProject, _buildTargets->row(currentItem), text );
}

void ProjectProperties::buildTargetsCurrentItemTextChanged( QListWidgetItem* item )
{
    _workspaceFile.setBuildTargetNameForProject( _currentProject, _buildTargets->row(item), item->text() );
}

void ProjectProperties::buildTargetsCurrentItemChanged( QListWidgetItem* current, QListWidgetItem* /*previous*/ )
{
    _buildTargetAction->setText( _workspaceFile.getBuildTargetActionForProject(_currentProject, _buildTargets->row(current)) );
}

void ProjectProperties::closeEvent( QCloseEvent* event )
{
    deleteLater();
    
    QDialog::closeEvent( event );
}

