
// Qt headers
#include <QtCore/QFile>
#include <QtCore/QByteArray>
#include <QtCore/QTextStream>
#include <QtCore/QMimeData>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QKeyEvent>
#include <QtGui/QTextCursor>

// blocks
#include <Styling/Styling.h>

// local headers
#include "TextFileEditor.h"
#include "TabSettings.h"
#include "SyntaxHighlighter/SyntaxHighlighter.h"

#define CurrentHighlighedLineColor "#778899"


TextFileEditor::TextFileEditor( QString filePath ) :
	QPlainTextEdit(),
    _filePath( filePath )
{
    setContentsMargins( 0, 0, 0, 0 );

    // preparing the editor
    setTabStopWidth( fontMetrics().width("o") * 4 );
    setLineWrapMode( QPlainTextEdit::NoWrap );

    _keyHandlers.insert( Qt::Key_Escape                             , &TextFileEditor::handleEscapeKey          );
    _keyHandlers.insert( Qt::Key_Tab                                , &TextFileEditor::handleTabKey             );
    _keyHandlers.insert( Qt::Key_Backtab                            , &TextFileEditor::handleShiftTabKey        );
    _keyHandlers.insert( Qt::Key_Backspace                          , &TextFileEditor::handleBackspaceKey       );
    _keyHandlers.insert( Qt::ControlModifier + Qt::Key_Slash        , &TextFileEditor::handleControlSlash       );
    _keyHandlers.insert( Qt::ControlModifier + Qt::Key_Backslash    , &TextFileEditor::handleControlBackSlash   );
    _keyHandlers.insert( Qt::ControlModifier + Qt::Key_S            , &TextFileEditor::handleControlSKey        );
    _keyHandlers.insert( Qt::ControlModifier + Qt::Key_X            , &TextFileEditor::handleControlXKey        );
    _keyHandlers.insert( Qt::ControlModifier + Qt::Key_C            , &TextFileEditor::handleControlCKey        );
    _keyHandlers.insert( Qt::ControlModifier + Qt::Key_V            , &TextFileEditor::handleControlVKey        );

    // preparing the syntax-highlighter
    _syntaxHighlighter = new SyntaxHighlighter( this, _filePath );
    
    // load the content into memory
    loadFile();
    
    // this thing here is needed to have the current line highlighted
    connect( this, SIGNAL(cursorPositionChanged()), viewport(), SLOT(update()) );

    // some makeup
    Styling styling( *this );
            styling.loadFonts();
}

TextFileEditor::~TextFileEditor()
{
	delete _syntaxHighlighter;
}

void TextFileEditor::save()
{
	if( _filePath.isEmpty() )
		return saveAs();
	
	QFile file( _filePath );
	if( false == file.open(QFile::WriteOnly) )
	{
		QMessageBox::critical( this, "File Saving Error", 
							QString("File: '%1' was not saved").arg(_filePath) );
		return;
	}

	QTextStream	textStream( &file );
				textStream << document()->toPlainText();
	file.close();
	
	document()->setModified( false );
}

void TextFileEditor::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName( this, "Save as...", QString() );
	if( fileName.isEmpty() )
		return;

	_filePath = fileName;
	save();
}

void TextFileEditor::loadFile()
{
	if( false == QFile::exists(_filePath) )
		return;

	QFile file( _filePath );
	if( false == file.open(QFile::ReadOnly) )
	{
		QMessageBox::critical( this, "Load File Error",
							QString("File: '%1' was not loaded").arg(_filePath) );
		return;
	}

	setPlainText( file.readAll() );
	file.close();

	document()->setModified( false );    
}

void TextFileEditor::paintEvent( QPaintEvent* paintEvent )
{
	QRect rect = cursorRect();

	int x		= 0;
	int y		= rect.y() - 2;
	int width		= viewport()->width() - 2;
	int height	= rect.height() + 3;

	QPainter	painter( viewport() );
			painter.setPen( QColor(CurrentHighlighedLineColor) );

			painter.drawLine(	QPoint(x,y),			QPoint(width,y) 		);
			painter.drawLine(	QPoint(x,y+height), 	QPoint(width,y+height) );

			painter.end();

	QPlainTextEdit::paintEvent( paintEvent );
}

void TextFileEditor::insertFromMimeData( const QMimeData* source )
{
	insertPlainText( source->text() );
}

void TextFileEditor::keyPressEvent( QKeyEvent* event )
{
    // get the info abut the event
    Qt::Key               key       = static_cast< Qt::Key >( event->key() );
    Qt::KeyboardModifiers modifiers = event->modifiers();

    // check to see if there are handlers for a speciffic sequence or key
    KeyHandler keyHandler = 0;

    if( _keyHandlers.contains(key) )
        keyHandler = _keyHandlers[ key ];
        
    else if( _keyHandlers.contains(modifiers+key) )
        keyHandler = _keyHandlers[ modifiers+key ];

    if( keyHandler )
    {
        (this->*keyHandler)();

        event->accept();
        return;
    }

    // once getting here means normal treatment of the event
    QPlainTextEdit::keyPressEvent( event );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//      keyboard handlers
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void TextFileEditor::handleEscapeKey()
{
    QTextCursor cursor = textCursor();
                cursor.clearSelection();

    setTextCursor( cursor );
}

void TextFileEditor::handleTabKey()
{
    doIndent( true );
}

void TextFileEditor::handleShiftTabKey()
{
    doIndent( false );
}

void TextFileEditor::handleBackspaceKey()
{
    QTextCursor cursor = textCursor();

    TabSettings tabSettings;
				tabSettings.m_spacesForTabs = true;
				tabSettings.m_autoIndent = true;
				tabSettings.m_smartBackspace = true;
				tabSettings.m_tabSize = 4;
				tabSettings.m_indentSize = 4;

    QTextBlock currentBlock = cursor.block();
    int positionInBlock = cursor.position() - currentBlock.position();
    const QString blockText = currentBlock.text();
    if (cursor.atBlockStart() || tabSettings.firstNonSpace(blockText) < positionInBlock) {
        cursor.deletePreviousChar();
        return;
    }

    int previousIndent = 0;
    const int indent = tabSettings.columnAt(blockText, positionInBlock);

    for (QTextBlock previousNonEmptyBlock = currentBlock.previous();
         previousNonEmptyBlock.isValid();
         previousNonEmptyBlock = previousNonEmptyBlock.previous()) {
        QString previousNonEmptyBlockText = previousNonEmptyBlock.text();
        if (previousNonEmptyBlockText.trimmed().isEmpty())
            continue;
        previousIndent = tabSettings.columnAt(previousNonEmptyBlockText,
                                              tabSettings.firstNonSpace(previousNonEmptyBlockText));
        if (previousIndent < indent)
            break;
    }

    if (previousIndent >= indent)
        previousIndent = 0;

    cursor.beginEditBlock();
    cursor.setPosition(currentBlock.position(), QTextCursor::KeepAnchor);
    cursor.insertText(tabSettings.indentationString(0, previousIndent));
    cursor.endEditBlock();
}

void TextFileEditor::handleControlSlash()
{
    QTextDocument* doc = document();

    QTextCursor cursor = textCursor();

    cursor.beginEditBlock();
    {
        if( false == cursor.hasSelection() )
        {
            cursor.setPosition( cursor.block().position() );
            cursor.insertText ( "//" );
        }

        else
        {
            QTextBlock startBlock = doc->findBlock( cursor.selectionStart() );
            QTextBlock endBlock   = doc->findBlock( cursor.selectionEnd()   );

            cursor.clearSelection();
            for( QTextBlock block = startBlock; block != endBlock; block = block.next() )
            {
                cursor.setPosition( block.position() );
                cursor.insertText ( "//" );
            }
        }
    }
    cursor.endEditBlock();
}

void TextFileEditor::handleControlBackSlash()
{
    QTextDocument* doc = document();

    QTextCursor cursor = textCursor();

    cursor.beginEditBlock();
    {
        if( false == cursor.hasSelection() )
        {
            QString text = cursor.block().text();

            if( text.startsWith("//") )
            {
                cursor.setPosition( cursor.block().position() );
                cursor.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );
                cursor.removeSelectedText();
                cursor.insertText ( QString("%1").arg(text.remove(0,2)) );
            }
        }

        else
        {
            QTextBlock startBlock = doc->findBlock( cursor.selectionStart() );
            QTextBlock endBlock   = doc->findBlock( cursor.selectionEnd()   );

            cursor.clearSelection();
            for( QTextBlock block = startBlock; block != endBlock; block = block.next() )
            {
                QString text = block.text();

                if( text.startsWith("//") )
                {
                    cursor.setPosition( block.position() );
                    cursor.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );
                    cursor.removeSelectedText();
                    cursor.insertText ( QString("%1").arg(text.remove(0,2)) );
                }
            }
        }
    }
    cursor.endEditBlock();
}

void TextFileEditor::handleControlSKey()
{
    save();
}

void TextFileEditor::handleControlXKey()
{
    cut();
}

void TextFileEditor::handleControlCKey()
{
    copy();
}

void TextFileEditor::handleControlVKey()
{
    paste();
}

void TextFileEditor::doIndent( bool doIndent )
{
    QTextCursor cursor = textCursor();
                cursor.beginEditBlock();

    int pos = cursor.position();
	TabSettings 	tabSettings;
				tabSettings.m_spacesForTabs = true;
				tabSettings.m_autoIndent = true;
				tabSettings.m_smartBackspace = true;
				tabSettings.m_tabSize = 4;
				tabSettings.m_indentSize = 4;

    QTextDocument* doc = document();
    if
	(
		!cursor.hasSelection()
		||
		(doc->findBlock(cursor.selectionStart()) == doc->findBlock(cursor.selectionEnd()) )
	)
	{
        cursor.removeSelectedText();
        QTextBlock block = cursor.block();
        QString text = block.text();
        int indentPosition = (cursor.position() - block.position());;
        int spaces = tabSettings.spacesLeftFromPosition(text, indentPosition);
        int startColumn = tabSettings.columnAt(text, indentPosition - spaces);
        int targetColumn = tabSettings.indentedColumn(tabSettings.columnAt(text, indentPosition), doIndent);

        cursor.setPosition(block.position() + indentPosition);
        cursor.setPosition(block.position() + indentPosition - spaces, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        cursor.insertText(tabSettings.indentationString(startColumn, targetColumn));
	}

	else
	{
        int anchor = cursor.anchor();
        int start = qMin(anchor, pos);
        int end = qMax(anchor, pos);

        QTextBlock startBlock = doc->findBlock(start);
        QTextBlock endBlock = doc->findBlock(end-1).next();

        for (QTextBlock block = startBlock; block != endBlock; block = block.next()) {
            QString text = block.text();
            int indentPosition = tabSettings.lineIndentPosition(text);
            if (!doIndent && !indentPosition)
                indentPosition = tabSettings.firstNonSpace(text);
            int targetColumn = tabSettings.indentedColumn(tabSettings.columnAt(text, indentPosition), doIndent);
            cursor.setPosition(block.position() + indentPosition);
            cursor.insertText(tabSettings.indentationString(0, targetColumn));
            cursor.setPosition(block.position());
            cursor.setPosition(block.position() + indentPosition, QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
        }
    }

    cursor.endEditBlock();
}
