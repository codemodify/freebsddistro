#ifndef __Developer_H__
#define __Developer_H__

// Qt headers
#include <QtGui/QMainWindow>


// forward declaration
class QTabWidget;
class QDockWidget;
class QStatusBar;


class Developer : public QMainWindow
{
    Q_OBJECT

    public:
        Developer();
        ~Developer();

    // inherited overrides
    protected:
        void closeEvent( QCloseEvent* event );

    // developer menu initialization
    private:
        void initDeveloperMenu();
    private slots:
        void handleMenuItemHelpMe();
        void handleMenuItemSwitchWorkspace();
        void handleMenuItemOpenRecentWorkspace();
        void handleMenuItemExit();
    private:
        void switchWorkspace( QString workspacePath );
        void addFolderToRecentWorkspaces( QString folder );

    // windows menu initialization
    private: 
        void initWindowsMenu();
    private slots:
        void handleMenuItemWorkspaceNavigator();
        void handleMenuItemFileSystemBrowser();
        void handleMenuItemOutputConsole();
        void handleMenuItemSearchInFiles( QString folder );
        void handleMenuItemSearchInFiles();		

    // this will take care of the views
    private:
        void initViews();
        QStatusBar*     _statusBar;
        QTabWidget*     _tabWidget;
        QDockWidget*    _fileSystemBrowser;
        QDockWidget*    _workspaceNavigator;
        QDockWidget*    _outputConsole;
        QDockWidget*    _searchResult;

    // other inits
    private:
        void initTabs();
        void initStatusBar();

    // this will handle the request to edit a file by its path
    private slots:
        void handleRequestToEditFile( QString filePath );
        void fileWasModified();
        void fileWasSaved();

};

#endif
