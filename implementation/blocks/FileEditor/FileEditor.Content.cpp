
// Qt headers
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QAction>
#include <QtGui/QFileDialog>
#include <QtGui/QCloseEvent>
#include <QtCore/QTimer>
#include <QtCore/QMetaObject>

// blocks
#include <IconProvider/ActionIconProvider/ActionIconProvider.h> 
#include <Styling/Styling.h>

// local headers
#include "FileEditor.Content.h"
#include "TextFileEditor/TextFileEditor.h"
#include "LineNumbers/LineNumbers.h"
#include "ToolWidget/TextFinder/TextFinder.h"


FileEditorContent::FileEditorContent( QString filePath ):
	QWidget()
{
	// init the file editor component
	_textFileEditor = new TextFileEditor( filePath );
	_textFileEditor->setReadOnly( true );

	connect( _textFileEditor->document(), SIGNAL(modificationChanged(bool)), this, SLOT(fileWasChanged(bool)) );

	// init the line numbers components
	LineNumbers*    linenumbers = new LineNumbers( _textFileEditor );
                    linenumbers->setFocusProxy( _textFileEditor );

	// init the tool widgets area
	_toolWidgets = new QVBoxLayout();
    _toolWidgets->setSpacing( 0 );
    _toolWidgets->setContentsMargins( 0, 0, 0, 0 );


	// assemble all the components
    QHBoxLayout*    hBoxLayout = new QHBoxLayout();
                    hBoxLayout->setSpacing( 0 );
                    hBoxLayout->setContentsMargins( 0, 0, 0, 0 );
                    hBoxLayout->addWidget( linenumbers );
                    hBoxLayout->addWidget( _textFileEditor );

    QVBoxLayout*    vBoxLayout = new QVBoxLayout( this );
                    vBoxLayout->setSpacing( 0 );
                    vBoxLayout->setContentsMargins( 0, 0, 0, 0 );
                    vBoxLayout->addLayout( hBoxLayout );
                    vBoxLayout->addLayout( _toolWidgets );

    Styling styling( *this );
            styling.loadOnScreenPosition();
}

QPlainTextEdit* FileEditorContent::textEditor()
{
    return _textFileEditor;
}

void FileEditorContent::fileWasChanged( bool modified )
{
	if( modified )
		emit fileModified();
	else
		emit fileSaved();
}

void FileEditorContent::find()
{
	new TextFinder( this );
}

void FileEditorContent::addToolWidget()
{
	QWidget* widgetTool = qobject_cast<QWidget*>( sender() );
	if( 0 == widgetTool )
		return;

	_toolWidgets->addWidget( widgetTool );
}

