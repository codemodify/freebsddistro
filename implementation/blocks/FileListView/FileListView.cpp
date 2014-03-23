
// Qt headers
#include <QtGui/QMouseEvent>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QWheelEvent>

#include <QtCore/QUrl>
#include <QtCore/QMimeData>
#include <QtGui/QDrag>

// local headers
#include "FileListView.h"
#include "FolderContentModel/FolderContentModel.h"
#include "ItemDelegate/ItemDelegate.h"


FileListView::FileListView() :
    QListView(),
    _droppedItems( 0 ),
    _targetFolder( "" )
{
    // data model
    _folderContentModel = new FolderContentModel();

    connect (
                _folderContentModel,    SIGNAL(modelReset()   ),
                this,                   SLOT  (modelWasReset())
            );

    connect (
                _folderContentModel,    SIGNAL(itemsDropped(QStringList*,QString)            ),
                this,                   SLOT  (modelReceivedItemsDrop(QStringList*,QString)  )
            );

    // cosmetics
    this->setAcceptDrops            ( true                      );
    this->setAutoScroll             ( true                      );
    this->setAlternatingRowColors   ( true                      );
    this->setContextMenuPolicy      ( Qt::CustomContextMenu     );
    this->setDragEnabled            ( true                      );
    this->setDropIndicatorShown     ( true                      );
    this->setSelectionBehavior      ( QListView::SelectRows     );
    this->setSelectionMode          ( QListView::NoSelection    );

    connect( this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuRequested(const QPoint&)) );

    // data binding
    this->setModel( _folderContentModel );

    // custom paints on the item
    this->setItemDelegate( new ItemDelegate(*this) );
}

FileListView::~FileListView()
{
    delete _folderContentModel;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
// helpers
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
void FileListView::moveCurrentIndexToNext()
{
    QModelIndex modelIndex = currentIndex();

    QModelIndex nextModelIndex = modelIndex.sibling( modelIndex.row()+1, 0 );
    if( nextModelIndex.isValid() )
        setCurrentIndex( nextModelIndex );
}

void FileListView::moveCurrentIndexToPrevious()
{
    QModelIndex modelIndex = currentIndex();

    QModelIndex previousModelIndex = modelIndex.sibling( modelIndex.row()-1, 0 );
    if( previousModelIndex.isValid() )
        setCurrentIndex( previousModelIndex );
}

QString FileListView::getCurrentItemPath()
{
//     if( !currentIndex().row() )
//         return QString();

    return QString("%1/%2") .arg( _folderContentModel->getCurrentFolder()               )
                            .arg( _folderContentModel->data(currentIndex()).toString()  );
}

void FileListView::modelWasReset()
{
    setCurrentIndex( _folderContentModel->index(0) );

    emit currentFolderChanged( _folderContentModel->getCurrentFolder() );
}

void FileListView::modelReceivedItemsDrop( QStringList* sourceItems, QString targetFolder )
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
// model wrapping
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
void    FileListView::setCurrentFolder              ( QString currentFolder         )       {        _folderContentModel->setCurrentFolder          ( currentFolder );  }
QString FileListView::getCurrentFolder              (                               )       { return _folderContentModel->getCurrentFolder          (               );  }
void    FileListView::refreshContent                (                               )       {        _folderContentModel->refreshContent            (               );  }
void    FileListView::setHiddenVisible              ( bool visible                  )       {        _folderContentModel->setHiddenVisible          ( visible       );  }
void    FileListView::getContent                    ( QStringList& content          )       {        _folderContentModel->getContent                ( content       );  }
void    FileListView::setContent                    ( QStringList& content          )       {        _folderContentModel->setContent                ( content       );  }
void    FileListView::selectAll                     (                               )       {        _folderContentModel->selectAll                 (               );  }
void    FileListView::selectNone                    (                               )       {        _folderContentModel->selectNone                (               );  }
void    FileListView::addFoldersToSelection         (                               )       {        _folderContentModel->addFoldersToSelection     (               );  }
void    FileListView::removeFoldersFromSelection    (                               )       {        _folderContentModel->removeFoldersFromSelection(               );  }
void    FileListView::addFilesToSelection           (                               )       {        _folderContentModel->addFilesToSelection       (               );  }
void    FileListView::removeFilesFromSelection      (                               )       {        _folderContentModel->removeFilesFromSelection  (               );  }
void    FileListView::addToSelection                ( QString criteria              )       {        _folderContentModel->addToSelection            ( criteria      );  }
void    FileListView::removeFromSelection           ( QString criteria              )       {        _folderContentModel->removeFromSelection       ( criteria      );  }
void    FileListView::addToSelection                ( const QModelIndex& mdoelIndex )       {        _folderContentModel->addToSelection            ( mdoelIndex    );  }
void    FileListView::invertSelection               (                               )       {        _folderContentModel->invertSelection           (               );  }
void    FileListView::getSelectedItems              ( QStringList& selectedItems    )
{
    _folderContentModel->getSelectedItems( selectedItems );

    if( selectedItems.isEmpty() && !getCurrentItemPath().isEmpty() )
        selectedItems.append( getCurrentItemPath() );
}
bool    FileListView::isItemSelected                ( const QModelIndex& modelIndex ) const { return _folderContentModel->isItemSelected            ( modelIndex    );  }



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// mouse handling
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
QPoint          FileListView::getMousePosition(){ return _mousePosition;    }
QStringList&    FileListView::getDroppedItems() { return *_droppedItems;    }
QString&        FileListView::getTargetFolder() { return _targetFolder;     }

void FileListView::mouseDoubleClickEvent( QMouseEvent* event )
{
    _mousePosition = this->mapToGlobal( event->pos() );

    emit mouseDoubleClicked();

    QListView::mouseDoubleClickEvent( event );
}

void FileListView::mousePressEvent( QMouseEvent* event )
{
    _time.start();

    QListView::mousePressEvent( event );
}

void FileListView::mouseMoveEvent( QMouseEvent* event )
{
    if( 1000 > _time.elapsed() )
        return;

    QStringList selectedItems;
    getSelectedItems( selectedItems );

    QList<QUrl> urls;
    foreach( QString item, selectedItems )
        urls << QUrl::fromLocalFile( item );

    QMimeData*  mimeData = new QMimeData();
                mimeData->setUrls( urls );

    QDrag*  drag = new QDrag( this );
            drag->setMimeData( mimeData );
            drag->exec();

    QListView::mouseMoveEvent( event );
}

void FileListView::contextMenuRequested( const QPoint& position )
{
    _mousePosition = this->mapToGlobal( position );

    emit mouseRightClicked();
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// keyboard handling
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
void FileListView::keyPressEvent( QKeyEvent* keyEvent )
{
    // implementing our needs
    Qt::KeyboardModifiers   keyboardModifier    = keyEvent->modifiers();
    Qt::Key                 pressedKey          = static_cast<Qt::Key>( keyEvent->key() );

    if( Qt::NoModifier == keyboardModifier )
    {
        switch( pressedKey )
        {
            case Qt::Key_Escape:    emit escPressed();      break;

            case Qt::Key_Period:    emit consolePressed();  break;

            case Qt::Key_F1 :       emit f01keyPressed();   break;
            case Qt::Key_F2 :       emit f02keyPressed();   break;
            case Qt::Key_F3 :       emit f03keyPressed();   break;
            case Qt::Key_F4 :       emit f04keyPressed();   break;
            case Qt::Key_F5 :       emit f05keyPressed();   break;
            case Qt::Key_F6 :       emit f06keyPressed();   break;
            case Qt::Key_F7 :       emit f07keyPressed();   break;
            case Qt::Key_F8 :       emit f08keyPressed();   break;
            case Qt::Key_F9 :       emit f09keyPressed();   break;
            case Qt::Key_F10:       emit f10keyPressed();   break;
            case Qt::Key_F11:       emit f11keyPressed();   break;
            case Qt::Key_F12:       emit f12keyPressed();   break;

            case Qt::Key_Print:     emit printPressed();    break;
            case Qt::Key_Pause:     emit pausePressed();    break;

            case Qt::Key_Tab:       emit tabPressed();      break;
            case Qt::Key_CapsLock:  emit capsLockPressed(); break;
            case Qt::Key_Shift:     emit leftShiftPressed();break;
            case Qt::Key_Control:   emit leftCtrlPressed(); break;

            case Qt::Key_Insert:    emit insertPressed();   break;
            case Qt::Key_Delete:    emit deletePressed();   break;
            case Qt::Key_Home:      emit homePressed();     break;
            case Qt::Key_End:       emit endPressed();      break;
            case Qt::Key_PageUp:    emit pageUpPressed();   break;
            case Qt::Key_PageDown:  emit pageDownPressed(); break;

            case Qt::Key_Backspace: emit backspacePressed();break;
            case Qt::Key_Return:    emit enterPressed();    break;
            case Qt::Key_Space:     emit spacePressed();    break;

            default:                keyEvent->ignore();     break;
        }
    }

    else if( Qt::AltModifier == keyboardModifier )
    {
        switch( pressedKey )
        {
            case Qt::Key_0: emit alt0Pressed(); break;
            case Qt::Key_1: emit alt1Pressed(); break;
            case Qt::Key_2: emit alt2Pressed(); break;
            case Qt::Key_3: emit alt3Pressed(); break;
            case Qt::Key_4: emit alt4Pressed(); break;
            case Qt::Key_5: emit alt5Pressed(); break;
            case Qt::Key_6: emit alt6Pressed(); break;
            case Qt::Key_7: emit alt7Pressed(); break;
            case Qt::Key_8: emit alt8Pressed(); break;
            case Qt::Key_9: emit alt9Pressed(); break;
            default:        keyEvent->ignore(); break;
        }
    }

    else if( Qt::ControlModifier == keyboardModifier )
    {
        switch( pressedKey )
        {
            case Qt::Key_0: emit ctrl0Pressed(); break;
            case Qt::Key_1: emit ctrl1Pressed(); break;
            case Qt::Key_2: emit ctrl2Pressed(); break;
            case Qt::Key_3: emit ctrl3Pressed(); break;
            case Qt::Key_4: emit ctrl4Pressed(); break;
            case Qt::Key_5: emit ctrl5Pressed(); break;
            case Qt::Key_6: emit ctrl6Pressed(); break;
            case Qt::Key_7: emit ctrl7Pressed(); break;
            case Qt::Key_8: emit ctrl8Pressed(); break;
            case Qt::Key_9: emit ctrl9Pressed(); break;
            default:        keyEvent->ignore();  break;
        }
    }

    else if( Qt::ShiftModifier == keyboardModifier )
    {
        switch( pressedKey )
        {
            case Qt::Key_0: emit shift0Pressed(); break;
            case Qt::Key_1: emit shift1Pressed(); break;
            case Qt::Key_2: emit shift2Pressed(); break;
            case Qt::Key_3: emit shift3Pressed(); break;
            case Qt::Key_4: emit shift4Pressed(); break;
            case Qt::Key_5: emit shift5Pressed(); break;
            case Qt::Key_6: emit shift6Pressed(); break;
            case Qt::Key_7: emit shift7Pressed(); break;
            case Qt::Key_8: emit shift8Pressed(); break;
            case Qt::Key_9: emit shift9Pressed(); break;
            default:        keyEvent->ignore();   break;
        }
    }

    else if( Qt::KeypadModifier == keyboardModifier )
    {
        switch( pressedKey )
        {
            case Qt::Key_Minus:     emit numpadMinusPressed();      break;
            case Qt::Key_Plus:      emit numpadPlusPressed();       break;
            case Qt::Key_Slash:     emit numpadSlashPressed();      break;
            case Qt::Key_Asterisk:  emit numpadAsteriskPressed();   break;
            default:                keyEvent->ignore();             break;
        }
    }

    // allowing the default behaviour
    QListView::keyPressEvent( keyEvent );
}
