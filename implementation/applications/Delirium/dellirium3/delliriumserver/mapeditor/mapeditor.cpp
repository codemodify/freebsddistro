
// qt headers
#include <QtGui/QToolBar>
#include <QtGui/QPushButton>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QButtonGroup>
#include <QtGui/QFileDialog>


// local headers
#include "mapeditor.h"
#include "2drenderer/2drenderer.h"

MapEditor::MapEditor():
	QMainWindow()
{
	resize( 800, 500 );
	
	_renderer = new TwoDRenderer();
	_renderer->setRuningMode( TwoDRenderer::Edit );
	setCentralWidget( _renderer );
	
	initToolBar();
}

MapEditor::~MapEditor()
{
	delete _buttonGroup;
}	

void MapEditor::initToolBar()
{
	// save/load controls
	QPushButton* saveButton = new QPushButton( "Save" );
	connect( saveButton, SIGNAL(released()), this, SLOT(slot_SaveMap()) );
	
	QPushButton* loadButton = new QPushButton( "Load" );
	connect( loadButton, SIGNAL(released()), this, SLOT(slot_LoadMap()) );
	
	// neutral-grouped controls
	QRadioButton* wall = new QRadioButton( "Wall" ); connect( wall, SIGNAL(released()), _renderer, SLOT(objectTypeToAddChangedToWall()) );
	QRadioButton* bush = new QRadioButton( "Bush" ); connect( bush, SIGNAL(released()), _renderer, SLOT(objectTypeToAddChangedToBush()) );

	QGroupBox* neutralGroupBox = new QGroupBox( "Neutral smart objects" );
	QHBoxLayout* neutralLayout = new QHBoxLayout( neutralGroupBox );
	neutralLayout->addWidget( wall );
	neutralLayout->addWidget( bush );

	// me-grouped controls
	QRadioButton* meTarget = new QRadioButton( "Target" ); connect( meTarget, SIGNAL(released()), _renderer, SLOT(objectTypeToAddChangedToTarget()) );
	QRadioButton* meBall = new QRadioButton( "Ball" ); connect( meBall, SIGNAL(released()), _renderer, SLOT(objectTypeToAddChangedToBall()) );

	QGroupBox* meGroupBox = new QGroupBox( "Me against the world" );
	QHBoxLayout* meLayout = new QHBoxLayout( meGroupBox );
	meLayout->addWidget( meTarget );
	meLayout->addWidget( meBall );

	// my-enemy-grouped controls
	QRadioButton* enemyTarget = new QRadioButton( "Target" ); connect( enemyTarget, SIGNAL(released()), _renderer, SLOT(objectTypeToAddChangedToEnemyTarget()) );
	QRadioButton* enemyBall = new QRadioButton( "Ball"); connect( enemyBall, SIGNAL(released()), _renderer, SLOT(objectTypeToAddChangedToEnemyBall()) );

	QGroupBox* enemyGroupBox = new QGroupBox( "My enemy that has to die" );
	QHBoxLayout* enemyLayout = new QHBoxLayout( enemyGroupBox );
	enemyLayout->addWidget( enemyTarget );
	enemyLayout->addWidget( enemyBall );
	
	// set the radio buttons to act in exclusive mode
	_buttonGroup = new QButtonGroup();
	_buttonGroup->addButton( wall );
	_buttonGroup->addButton( bush );
	_buttonGroup->addButton( meTarget );
	_buttonGroup->addButton( meBall );
	_buttonGroup->addButton( enemyTarget );
	_buttonGroup->addButton( enemyBall );

	// the toolbar that will stay ready to be used
	QToolBar* toolBar = new QToolBar();
	toolBar->addWidget( saveButton );
	toolBar->addWidget( loadButton );
	toolBar->addSeparator();
	toolBar->addWidget( neutralGroupBox );
	toolBar->addWidget( meGroupBox );
	toolBar->addWidget( enemyGroupBox );
	addToolBar( toolBar );
}

void MapEditor::slot_SaveMap()
{
	 QString filePath = QFileDialog::getSaveFileName( this, "Save Map as...", "/", "Dellerium Maps (*.*)");
	 if( false == filePath.isEmpty() )
	 {
		_renderer->saveMap( filePath );
	 }
}

void MapEditor::slot_LoadMap()
{
	QString filePath = QFileDialog::getOpenFileName( this, "Open Map", "/", "Dellerium Maps (*.*)");
	if( false == filePath.isEmpty() )
	{
		_renderer->loadMap( filePath );
	}
}
