#ifndef __Commander_H__
#define __Commander_H__

// Qt headers
#include <QtGui/QMainWindow>
#include <QtGui/QSystemTrayIcon>

// local headers
#include "TabWidget/TabWidget.h"

// forwards
class FileSystemOperationsUi;
class FileSystemOperations;


class Commander : public QMainWindow
{
    Q_OBJECT

    // default stuff
    public:
        Commander();
        ~Commander();


    // sys tray icon
    private:
        void setupSystemTrayicon();


    // this takes care of the menu
    private:
        void setupMenu();
    private slots:
        void helpMe();
        void openNewTab();
        void openNewExplorerTab();
        void closeAllTabs();
        void closeCurrentTab();
        void closeAllExceptCurrent();
    protected:
        void closeEvent( QCloseEvent* event );


    // this takes care of the toolbar
    private:
        void setupToolbar();
    private slots:
        void slot_properties();
        void slot_rename();
        void slot_duplicate();
        void slot_view();
        void slot_edit();
        void slot_copy();
        void slot_move();
        void slot_makedir();
        void slot_remove();
        void slot_showFileOperations();


    // this takes care of the tabs
    private:
        void setupTabs();
    private:
        TabWidget* _activeTab;
        TabWidget* _leftTabWidget;
        TabWidget* _rightTabWidget;


    // this takes care of the file operations
    private slots:
        // mouse
        void mouseRightClicked  ();
        void mouseDoubleClicked ();
        void mouseItemsDropped  ();

        // keyboard
        void f01keyPressed      ();
        void f02keyPressed      ();
        void f03keyPressed      ();
        void f04keyPressed      ();
        void f05keyPressed      ();
        void f06keyPressed      ();
        void f07keyPressed      ();
        void f08keyPressed      ();
        void insertPressed      ();
        void backspacePressed   ();
        void spacePressed       ();
        void numpadMinusPressed ();
        void numpadPlusPressed  ();

        // context menu
        void contextMenuCut     ();
        void contextMenuCopy    ();
        void contextMenuPaste   ();


    private slots:
        void focusChanged( QWidget* oldWidget, QWidget* nowWidget );
        void readConfiguration();


    private:
        FileSystemOperations*   _fileSystemOperations;
        FileSystemOperationsUi* _fileSystemOperationsUi;
};

#endif
