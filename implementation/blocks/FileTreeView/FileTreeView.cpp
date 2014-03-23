
// Qt headers
#include <QtGui/QMouseEvent>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QWheelEvent>

#include <QtCore/QUrl>
#include <QtCore/QMimeData>
#include <QtGui/QDrag>

// local headers
#include "FileTreeView.h"
#include "FileTreeModel/FileTreeModel.h"


FileTreeView::FileTreeView() :
    QTreeView(),
    _droppedItems( 0 ),
    _targetFolder( "" )
{
    // data model
    _fileTreeModel = new FileTreeModel();
    connect( _fileTreeModel, SIGNAL(itemsDropped(QStringList*,QString)), this, SLOT(modelReceivedItemsDrop(QStringList*,QString)) );


    // cosmetics
    this->setAcceptDrops            ( true                      );
    this->setAutoScroll             ( true                      );
    this->setAlternatingRowColors   ( true                      );
    this->setContextMenuPolicy      ( Qt::CustomContextMenu     );
    this->setDragEnabled            ( true                      );
    this->setDropIndicatorShown     ( true                      );
    this->setSelectionBehavior      ( QTreeView::SelectRows     );
    this->setSelectionMode          ( QTreeView::ExtendedSelection );
    this->setExpandsOnDoubleClick   ( true                      );
    this->setHeaderHidden           ( true                      );
    this->setEditTriggers           ( QTreeView::NoEditTriggers );
    this->setModel                  ( _fileTreeModel            );

    connect( this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuRequested(const QPoint&)) );
    connect( this, SIGNAL(collapsed(const QModelIndex&)), this, SLOT(adjustColumnWidth()) );
    connect( this, SIGNAL(expanded(const QModelIndex&)),  this, SLOT(adjustColumnWidth()) );
}

FileTreeView::~FileTreeView()
{
    delete _fileTreeModel;
}

void FileTreeView::adjustColumnWidth()
{
    resizeColumnToContents( 0 );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
// model wrapping
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
void FileTreeView::setCurrentFolder( QString currentFolder )
{
    QModelIndex index;
    QStringList fullPath = currentFolder.split( "/" );
    QStringList fullPathByStep;
    foreach( QString chunkOfPath, fullPath )
    {
        fullPathByStep << "/" << chunkOfPath;
        index = _fileTreeModel->index( fullPathByStep.join("") );
        setExpanded( index, true );
    }
    
    selectionModel()->clear();
    selectionModel()->setCurrentIndex( index, QItemSelectionModel::Select );
}
QString FileTreeView::getCurrentFolder()
{
    return getCurrentItemPath();
}

void FileTreeView::refreshContent()
{
    _fileTreeModel->refresh( QModelIndex() );
}

void FileTreeView::setRootFolder( QString rootFolder )
{
    setRootIndex( _fileTreeModel->index(rootFolder) );
}

void FileTreeView::getSelectedItems( QStringList& selectedItems )
{
    foreach( QModelIndex index, selectedIndexes() )
        selectedItems.append( _fileTreeModel->filePath(index) );
}

QString FileTreeView::getCurrentItemPath()
{
    return _fileTreeModel->filePath( currentIndex() );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// mouse handling
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
QPoint          FileTreeView::getMousePosition(){ return _mousePosition;    }
QStringList&    FileTreeView::getDroppedItems() { return *_droppedItems;    }
QString&        FileTreeView::getTargetFolder() { return _targetFolder;     }

void FileTreeView::mouseDoubleClickEvent( QMouseEvent* event )
{
    _mousePosition = this->mapToGlobal( event->pos() );

    emit mouseDoubleClicked();

    QTreeView::mouseDoubleClickEvent( event );
}

void FileTreeView::mousePressEvent( QMouseEvent* event )
{
    _time.start();

    QTreeView::mousePressEvent( event );
}

void FileTreeView::mouseMoveEvent( QMouseEvent* event )
{
    if( 1000 > _time.elapsed() )
        return;

    QStringList selectedItems;
    getSelectedItems( selectedItems );

    QList<QUrl> urls;
    foreach( QString item, selectedItems )
    {
        urls << QUrl::fromLocalFile( item );
    }

    QMimeData*  mimeData = new QMimeData();
                mimeData->setUrls( urls );

    QDrag*  drag = new QDrag( this );
            drag->setMimeData( mimeData );
            drag->exec();

    QTreeView::mouseMoveEvent( event );
}

void FileTreeView::contextMenuRequested( const QPoint& position )
{
    _mousePosition = this->mapToGlobal( position );

    emit mouseRightClicked();
}

void FileTreeView::modelReceivedItemsDrop( QStringList* sourceItems, QString targetFolder )
{
    // if there was something before then clean it up
    if( _droppedItems )
    {
        _droppedItems->clear();
        delete _droppedItems;
        _droppedItems = 0;
        _targetFolder = "";
    }

    _droppedItems = sourceItems;
    _targetFolder = targetFolder;

    emit mouseItemsDropped();
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// keyboard handling
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
void FileTreeView::keyPressEvent( QKeyEvent* keyEvent )
{
    // implementing our needs
    Qt::KeyboardModifiers   keyboardModifier    = keyEvent->modifiers();
    Qt::Key                 pressedKey          = static_cast<Qt::Key>( keyEvent->key() );

    if( Qt::NoModifier == keyboardModifier )
    {
        switch( pressedKey )
        {
            case Qt::Key_F1 :       emit f01keyPressed();   break;
            case Qt::Key_F2 :       emit f02keyPressed();   break;
            case Qt::Key_F3 :       emit f03keyPressed();   break;
            case Qt::Key_F4 :       emit f04keyPressed();   break;

            case Qt::Key_Delete:    emit deletePressed();   break;
            case Qt::Key_Return:    emit enterPressed();    break;
 
            default:                keyEvent->ignore();     break;
        }
    }

    // allowing the default behaviour
    QTreeView::keyPressEvent( keyEvent );
}

