#ifndef __FileListView_H__
#define __FileListView_H__

// Qt headers
#include <QtCore/QTime>
#include <QtGui/QListView>

// forwards
class IconProvider;
class FolderContentModel;

class FileListView : public QListView
{
    Q_OBJECT

    public:
        FileListView();
        ~FileListView();



    // helpers
    public:
        void moveCurrentIndexToNext();
        void moveCurrentIndexToPrevious();
        QString getCurrentItemPath();
    private slots:
        void modelWasReset();
        void modelReceivedItemsDrop( QStringList* sourceItems, QString targetFolder );




    // model wrapping
    signals:
        void currentFolderChanged   ( QString currentFolder );
    public:
        void    setCurrentFolder    ( QString currentFolder );
        QString getCurrentFolder    (                       );
        void    refreshContent      (                       );

        void    setHiddenVisible    ( bool visible );

        void    getContent          ( QStringList& content );
        void    setContent          ( QStringList& content );

        void selectAll                  ();
        void selectNone                 ();

        void addFoldersToSelection      ();
        void removeFoldersFromSelection ();

        void addFilesToSelection        ();
        void removeFilesFromSelection   ();

        void addToSelection             ( QString criteria );
        void removeFromSelection        ( QString criteria );

        void addToSelection             ( const QModelIndex& mdoelIndex );

        void invertSelection();

        void getSelectedItems           ( QStringList& selectedItems );

        bool isItemSelected             ( const QModelIndex& modelIndex ) const;
    private:
        FolderContentModel* _folderContentModel;




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

    private:
        QTime           _time;
        QPoint          _mousePosition;
        QStringList*    _droppedItems;
        QString         _targetFolder;




    // keyboard handling
    signals:
        void escPressed         ();

        void consolePressed     (); // ~

        void f01keyPressed      ();
        void f02keyPressed      ();
        void f03keyPressed      ();
        void f04keyPressed      ();
        void f05keyPressed      ();
        void f06keyPressed      ();
        void f07keyPressed      ();
        void f08keyPressed      ();
        void f09keyPressed      ();
        void f10keyPressed      ();
        void f11keyPressed      ();
        void f12keyPressed      ();

        void printPressed       ();
        void pausePressed       ();

        void tabPressed         ();
        void capsLockPressed    ();
        void leftShiftPressed   ();
        void leftCtrlPressed    ();

        void alt0Pressed        ();
        void alt1Pressed        ();
        void alt2Pressed        ();
        void alt3Pressed        ();
        void alt4Pressed        ();
        void alt5Pressed        ();
        void alt6Pressed        ();
        void alt7Pressed        ();
        void alt8Pressed        ();
        void alt9Pressed        ();

        void ctrl0Pressed       ();
        void ctrl1Pressed       ();
        void ctrl2Pressed       ();
        void ctrl3Pressed       ();
        void ctrl4Pressed       ();
        void ctrl5Pressed       ();
        void ctrl6Pressed       ();
        void ctrl7Pressed       ();
        void ctrl8Pressed       ();
        void ctrl9Pressed       ();

        void shift0Pressed      ();
        void shift1Pressed      ();
        void shift2Pressed      ();
        void shift3Pressed      ();
        void shift4Pressed      ();
        void shift5Pressed      ();
        void shift6Pressed      ();
        void shift7Pressed      ();
        void shift8Pressed      ();
        void shift9Pressed      ();

        void insertPressed      ();
        void deletePressed      ();
        void homePressed        ();
        void endPressed         ();
        void pageUpPressed      ();
        void pageDownPressed    ();

        void backspacePressed   ();
        void enterPressed       ();
        void spacePressed       ();

        void numpadMinusPressed     ();
        void numpadPlusPressed      ();
        void numpadSlashPressed     ();
        void numpadAsteriskPressed  ();


    protected:
        void keyPressEvent( QKeyEvent* event );

};

#endif
