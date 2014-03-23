
// Qt headers
#include <QtGui/QTextCursor>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
#include <QtGui/QPlainTextEdit>

// local headers
#include "TextFinder.h"
#include "FileEditor.Content.h"


TextFinder::TextFinder( FileEditorContent* fileEditorContent ) :
	ToolWidget( fileEditorContent ),
	_textFileEditor( fileEditorContent->textEditor() )
{
	// adding the controls for user interaction
	_textToFind = new QLineEdit();

	_findBackward = new QCheckBox( "backward" );
	_findCaseSensitive = new QCheckBox( "case-sensitive" );
	_findWholeWords = new QCheckBox( "whole-words" );

	QPushButton* findButton = new QPushButton( "Find It" );
	connect( findButton, SIGNAL(pressed()), this, SLOT(slot_PushButtonPressed()) );

	QPushButton* closeButton = new QPushButton( "x" );
	connect( closeButton, SIGNAL(pressed()), this, SLOT(close()) );

	QHBoxLayout*	hBoxLayout = new QHBoxLayout( this );
				hBoxLayout->addWidget( _textToFind			);
				hBoxLayout->addWidget( findButton			);
				hBoxLayout->addWidget( closeButton			);

				hBoxLayout->addWidget( _findBackward 		);
				hBoxLayout->addWidget( _findCaseSensitive		);
				hBoxLayout->addWidget( _findWholeWords		);

	emit ready();
}

TextFinder::~TextFinder()
{}

void TextFinder::slot_PushButtonPressed()
{
	// getting the current cursor position in the editor
	int cursorPosition = _textFileEditor->textCursor().position();

	// setting the search criteria
	QTextDocument::FindFlags findFlags;

	if( _findBackward->isChecked() )
	{
		findFlags = QTextDocument::FindBackward;

		QTextCursor textCursor = _textFileEditor->textCursor();
		textCursor.movePosition( QTextCursor::WordLeft, QTextCursor::MoveAnchor );
		cursorPosition = textCursor.position();
	}
	if( _findCaseSensitive->isChecked() )
		findFlags |= QTextDocument::FindCaseSensitively;
	if( _findWholeWords->isChecked() )
		findFlags |= QTextDocument::FindWholeWords;

	// do the search
	QTextCursor textCursor = _textFileEditor->document()->find( _textToFind->text(), cursorPosition, findFlags );
	if( !textCursor.isNull() )
	{
		_textFileEditor->setTextCursor( textCursor );
		textCursor.clearSelection();
		textCursor.movePosition( QTextCursor::WordRight, QTextCursor::KeepAnchor );
	}
}
