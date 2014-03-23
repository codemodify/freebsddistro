#ifndef __FileTreeView_H__
#define __FileTreeView_H__

// Qt headers
#include <QtCore/QTime>
#include <QtGui/QTreeView>

// forwards
class IconProvider;
class FileTreeModel;

class FileTreeView : public QTreeView
{
    Q_OBJECT

    public:
        FileTreeView();
        ~FileTreeView();
        
    private slots:
        void adjustColumnWidth();



    // model wrapping
    signals:
        void currentFolderChanged   ( QString currentFolder );
    public:
        void    setCurrentFolder    ( QString currentFolder );
        QString getCurrentFolder    (                       );
        void    refreshContent      (                       );
    public:
        void    setRootFolder( QString currentFolder );
        void    getSelectedItems( QStringList& selectedItems );
        QString getCurrentItemPath();

    private:
        FileTreeModel* _fileTreeModel;


    // mouse handling
    signals:
        void mouseRightClicked();
        void mouseDoubleClicked();
        void mouseItemsDropped();

    public:
        QPoint          getMousePosition();
        QStringList&    getDroppedItems();
        QString&        getTargetFolder();

    protected:
        void mouseDoubleClickEvent  ( QMouseEvent* event );
        void mousePressEvent        ( QMouseEvent* event );
        void mouseMoveEvent         ( QMouseEvent* event );
        
    private slots:
        void contextMenuRequested( const QPoint& position );
        void modelReceivedItemsDrop( QStringList* sourceItems, QString targetFolder );

    private:
        QTime           _time;
        QPoint          _mousePosition;
        QStringList*    _droppedItems;
        QString         _targetFolder;


    // keyboard handling
    signals:
        void f01keyPressed  ();
        void f02keyPressed  ();
        void f03keyPressed  ();
        void f04keyPressed  ();

        void deletePressed  ();
        void enterPressed   ();

    protected:
        void keyPressEvent( QKeyEvent* event );

};

#endif

