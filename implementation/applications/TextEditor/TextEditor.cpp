
// Qt headers
#include <QtCore/QFileInfo>
#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtGui/QKeyEvent>
#include <QtGui/QColor>
#include <QtGui/QTabBar>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QMoveEvent>

// blocks
#include <IconProvider/ActionIconProvider/ActionIconProvider.h>
#include <Styling/Styling.h>
#include <FileEditor/FileEditor.h>
#include <FileTreeView/FileTreeView.h>

// local headers
#include "TextEditor.h"


TextEditor::TextEditor() :
    QTabWidget(),
    _fileTreeView( 0 )
{
    setWindowTitle( "TextEditor" );
    setContextMenuPolicy( Qt::CustomContextMenu );
    setAcceptDrops( true );

    // init the context menu
    ActionIconProvider actionIconProvider;

    _contextMenu.addAction( actionIconProvider.getIconForAction("add")                 , "tab"         , this, SLOT(newEmptyTab())     );
    _contextMenu.addAction( actionIconProvider.getIconForAction("remove")              , "tab"         , this, SLOT(closeTab())        );
    _contextMenu.addAction( actionIconProvider.getIconForAction("forward")             , "next"        , this, SLOT(nextTab())         );
    _contextMenu.addAction( actionIconProvider.getIconForAction("backward")            , "prev"        , this, SLOT(previousTab())     );
    _contextMenu.addAction( actionIconProvider.getIconForAction("fullscreen")          , "fullscreen"  , this, SLOT(fullScreen())      );
    _contextMenu.addAction( actionIconProvider.getIconForAction("filesystembrowser")   , "explorer"    , this, SLOT(explorer())        );
    _contextMenu.addAction( actionIconProvider.getIconForAction("ontop")               , "on Top"      , this, SLOT(onTop())           );

    connect(    this, SIGNAL(customContextMenuRequested(const QPoint&)),
                this, SLOT(rightClick(const QPoint&))                   );

    // load settings
    Styling styling( *this );
            styling.loadOnScreenPosition();
            styling.loadFonts();
}

TextEditor::~TextEditor()
{
    if( 0 != _fileTreeView )
        delete _fileTreeView;
}

void TextEditor::newEmptyTab()
{
    newTab( "" );
}

void TextEditor::newTab( QString filePath )
{
    FileEditor* fileEditor = new FileEditor( filePath );

    insertTab( 0, fileEditor, QIcon(), QFileInfo(filePath).fileName() );

//    int index = addTab( fileEditor, QFileInfo(filePath).fileName() );
//    tabBar()->setTabTextColor( index, QColor::fromHsv(rand() % 256,255,190) );

    setCurrentIndex( 0 );

    // handling the close actions
    connect( fileEditor, SIGNAL(destroyed(QObject*)), this, SLOT(closeTab()) );
}

void TextEditor::rightClick( const QPoint& point )
{
    _contextMenu.exec( mapToGlobal(point) );
}

void TextEditor::closeTab()
{
    removeTab( currentIndex() );
}

void TextEditor::fullScreen()
{
    if( isFullScreen() )
        showNormal();
    else
        showFullScreen();
}

void TextEditor::nextTab()
{
    int index = currentIndex();

//    if( ++index >= count() )
//        index = 0;

    setCurrentIndex( index );
}

void TextEditor::previousTab()
{
    int index = currentIndex();

//    if( --index < 0 )
//        index = count() - 1;

    setCurrentIndex( index );
}

void TextEditor::explorer()
{
    if( 0 == _fileTreeView )
    {
        _fileTreeView = new FileTreeView();
        _fileTreeView->setParent( this );
        _fileTreeView->setWindowTitle( "File Explorer" );
        //_fileTreeView->setCurrentFolder( "/" );
        _fileTreeView->setWindowFlags( Qt::Tool | Qt::FramelessWindowHint );

        connect( _fileTreeView, SIGNAL(mouseDoubleClicked()), this, SLOT(explorerMouseDoubleClick()) );
        connect( _fileTreeView, SIGNAL(enterPressed      ()), this, SLOT(explorerMouseDoubleClick()) );
    }

    move( pos().x()+1, pos().y()+1 );
    move( pos().x()-1, pos().y()-1 );

    _fileTreeView->setVisible( !_fileTreeView->isVisible() );
}

void TextEditor::onTop()
{
//    Qt::WindowFlags flags = 0;
//                    flags|= Qt::X11BypassWindowManagerHint;
//                    flags|= 

    setWindowFlags( Qt::WindowStaysOnTopHint );
}

void TextEditor::explorerMouseDoubleClick()
{
    QString path = _fileTreeView->getCurrentItemPath();

    if( QFileInfo(path).isDir() )
        ;
    else
        newTab( path );
}

void TextEditor::closeEvent( QCloseEvent* event )
{
    Styling styling( *this );
            styling.saveOnScreenPosition();

    if( _fileTreeView )
        _fileTreeView->close();

    QTabWidget::closeEvent( event );
}

void TextEditor::dragEnterEvent( QDragEnterEvent* event )
{
    event->accept();
}

void TextEditor::dropEvent( QDropEvent* event )
{
    const QMimeData* c_mimeData = event->mimeData();

    if( !c_mimeData->hasUrls() )
        return;

    foreach( QUrl url, c_mimeData->urls() )
        newTab( url.toLocalFile() );

    QTabWidget::dropEvent( event );
}

void TextEditor::resizeEvent( QResizeEvent* event )
{
    QTabWidget::resizeEvent( event );
    
    move( pos().x()+1, pos().y()+1 );
    move( pos().x()-1, pos().y()-1 );
}

void TextEditor::moveEvent( QMoveEvent* event )
{
    QTabWidget::moveEvent( event );

    if( 0 != _fileTreeView )
        _fileTreeView->setGeometry( geometry().x() + geometry().width(), geometry().y()+40, 260, geometry().height()-90 );
 }

