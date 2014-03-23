
// Qt hedaers
#include <QtGui/QApplication>
#include <QtGui/QAction>
#include <QtGui/QClipboard>
#include <QtGui/QLayout>
#include <QtGui/QBoxLayout>
#include <QtGui/QToolBar>

// blocks
#include <IconProvider/ActionIconProvider/ActionIconProvider.h> 
#include <Styling/Styling.h>

// local headers
#include "FileEditor.h"
#include "FileEditor.Content.h"


FileEditor::FileEditor( QString filePath, QWidget* parent ):
	CustomDockWidget( parent ),
    _filePath( filePath )
{
    // preapre the content
    _editorWidget = new FileEditorContent( filePath );
    connect( _editorWidget, SIGNAL(fileModified()), this, SIGNAL(fileModified()) );
    connect( _editorWidget, SIGNAL(fileSaved())   , this, SIGNAL(fileSaved()) );

    setTitleText( filePath );
    setContentWidget( _editorWidget );

    // preapre the titlebar
    setTitleStyleSheet
    (
        "QWidget {                                                                                              "\
        "   background:     url(:/images/titlebar-background.png);                                              "\
        "   max-height:     25px;                                                                               "\
        "                                                                                                       "\
        "   border-style:   none;                                                                               "\
        "   border-width:   0px;                                                                                "\
        "                                                                                                       "\
        "   margin-top:     0px;                                                                                "\
        "   margin-bottom:  0px;                                                                                "\
        "   margin-left:    0px;                                                                                "\
        "   margin-right:   0px;                                                                                "\
        "                                                                                                       "\
        "   spacing:        0;                                                                                  "\
        "}                                                                                                      "\
        "QToolButton {                                                                                          "\
        "   border-style:   none;                                                                               "\
        "   border-width:   0px;                                                                                "\
        "   border-radius:  0px;                                                                                "\
        "}                                                                                                      "\
        "QToolButton:hover {                                                                                    "\
        "   background:     url(:/images/titlebar-button-hover-background.png);                                 "\
        "}                                                                                                      "\
        "QToolButton:checked {                                                                                  "\
        "   background:     url(:/images/titlebar-button-checked-background.png);                               "\
        "}                                                                                                      "\
        "QToolButton:pressed {                                                                                  "\
        "   background:     url(:/images/titlebar-button-pressed-background.png);                               "\
        "}                                                                                                      "\
        "QLabel {                                                                                               "\
        "   font-family:    Verdana;                                                                            "\
        "   color:          rgb( 255, 255, 255 );                                                               "\
        "   font-size:      12px;                                                                               "\
        "}"
    );

    _actionReadonly = new QAction( 0 );
    _actionReadonly ->setIcon( QIcon(":/images/lock.png") );
    _actionReadonly ->setCheckable( true );
    _actionReadonly ->setChecked( true );

    _actionSave     = new QAction( 0 );
    _actionSave     ->setIcon( QIcon(":/images/save.png") );
    _actionSave     ->setEnabled( false );

    _actionUndo     = new QAction( 0 );
    _actionUndo     ->setIcon( QIcon(":/images/undo.png") );
    _actionUndo     ->setEnabled( false );

    _actionRedo     = new QAction( 0 );
    _actionRedo     ->setIcon( QIcon(":/images/redo.png") );
    _actionRedo     ->setEnabled( false );

    _actionSearch   = new QAction( 0 );
    _actionSearch   ->setIcon( QIcon(":/images/search.png") );

    QAction* closeButton = new QAction( 0 );
             closeButton->setIcon( QIcon(":/images/close.png") );

    addTitleButton( _actionReadonly, this                       , SLOT(setDocumentReadOnly()) );
    addTitleButton( _actionSave    , _editorWidget->textEditor(), SLOT(save()               ) );
    addTitleButton( _actionUndo    , _editorWidget->textEditor(), SLOT(undo()               ) );
    addTitleButton( _actionRedo    , _editorWidget->textEditor(), SLOT(redo()               ) );
    addTitleButton( _actionSearch  , _editorWidget,               SLOT(find()               ) );             
    setTitleCloseButton( closeButton, this, SLOT(close()) );

    connect( _editorWidget->textEditor()->document() , SIGNAL(modificationChanged(bool)) , _actionSave   , SLOT(setEnabled(bool))   );
    connect( _editorWidget->textEditor()->document() , SIGNAL(undoAvailable(bool))       , _actionUndo   , SLOT(setEnabled(bool))   );
	connect( _editorWidget->textEditor()->document() , SIGNAL(redoAvailable(bool))       , _actionRedo   , SLOT(setEnabled(bool))   );
}

QString FileEditor::getFilePath()
{
    return _filePath;
}

void FileEditor::setDocumentReadOnly()
{
    bool isReadOnly = _editorWidget->textEditor()->isReadOnly();

    _actionReadonly->setChecked( !isReadOnly );
    _actionReadonly->setIcon( !isReadOnly ? QIcon(":/images/lock.png") : QIcon(":/images/unlock.png") );
    
    _editorWidget->textEditor()->setReadOnly( !isReadOnly );
}

void FileEditor::closeEvent( QCloseEvent* event )
{
    Styling styling( *this );
            styling.saveOnScreenPosition();

    CustomDockWidget::closeEvent( event );

    deleteLater();
}

