
// Qt headers
#include <QtGui/QCloseEvent>
#include <QtGui/QMenuBar>
#include <QtGui/QFileDialog>

// blocks
#include <Styling/Styling.h>
#include <IconProvider/ActionIconProvider/ActionIconProvider.h>

// local headers
#include "SchemaDrawer.h"
#include "DocumentDrawer/DocumentDrawer.h"


SchemaDrawer::SchemaDrawer():
	QMainWindow( 0 )
{
	ActionIconProvider actionIconProvider;

	// SchemaDrawer
	QMenu*	schemaDrawerMenu = menuBar()->addMenu( "SchemaDrawer" );
			schemaDrawerMenu->addAction( actionIconProvider.getIconForAction("help"), "Help Me!", this, SLOT(helpMe()) );
			schemaDrawerMenu->addSeparator();
			schemaDrawerMenu->addAction( actionIconProvider.getIconForAction("exit"), "Exit", this, SLOT(close()) );

	// Schema
	QMenu*	schemaMenu = menuBar()->addMenu( "Schema" );
    			schemaMenu->addAction( actionIconProvider.getIconForAction("add"),  "Schema", this, SLOT(newSchema())  );
    			schemaMenu->addAction( actionIconProvider.getIconForAction("save"), "Save",   this, SLOT(saveSchema()) );
			schemaMenu->addAction( actionIconProvider.getIconForAction("open"), "Open",   this, SLOT(openSchema()) );

	// preparing the workspace
	_workspace = new QWorkspace();
	setCentralWidget( _workspace );

	// cosmetics
	Styling	styling( *this );
			styling.loadOnScreenPosition();

	setWindowTitle( "Schema Drawer" );
}

SchemaDrawer::~SchemaDrawer()
{}

void SchemaDrawer::openSchemaByFilePath( QString filePath )
{
	newSchema();
	
	QWidget*			widget			= _workspace->activeWindow();
	DocumentDrawer*	documentDrawer	= qobject_cast<DocumentDrawer*>( widget );
	if( 0 == documentDrawer )
		return;

	documentDrawer->openDocument( filePath );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- inherited stuff
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void SchemaDrawer::closeEvent( QCloseEvent* event )
{
	_workspace->closeAllWindows();

	Styling	styling( *this );
			styling.saveOnScreenPosition();

	event->accept();
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- menu handlers
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void SchemaDrawer::helpMe()
{}

void SchemaDrawer::newSchema()
{
	DocumentDrawer* documentDrawer = new DocumentDrawer();

	_workspace->addWindow( documentDrawer );

	documentDrawer->showMaximized();
}

void SchemaDrawer::saveSchema()
{
	QWidget* widget = _workspace->activeWindow();

	DocumentDrawer* documentDrawer = qobject_cast<DocumentDrawer*>( widget );
	if( 0 == documentDrawer )
		return;

	documentDrawer->saveDocument();
}

void SchemaDrawer::openSchema()
{
 	QString fileName = QFileDialog::getOpenFileName( this, "Schema File", "/home" );
	if( fileName.isEmpty() )
		return;

	openSchemaByFilePath( fileName );
}
