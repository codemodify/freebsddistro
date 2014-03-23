#ifndef __FileSystemBrowser_H__
#define __FileSystemBrowser_H__

// Qt headers
#include <QtGui/QMenu>
#include <QtGui/QHBoxLayout>

// blocks
#include <FileSystemOperations/FileSystemOperations.h>
#include <IconProvider/ActionIconProvider/ActionIconProvider.h>
#include <FileTreeView/FileTreeView.h>

// local headers
#include "View/View.h"

// forwards
class FileTreeView;
class QPushButton;


class FileSystemBrowser : public View
{
    Q_OBJECT

    public:
        FileSystemBrowser();
        ~FileSystemBrowser();

    signals:
        void editFile( QString filePath );

    // file operations handlers
    private slots:
        void fileBrowserGoInto();
        void fileBrowserGoBack();
        void fileBrowserRefresh();
        void fileBrowserEdit();
        void fileBrowserOpenWithRegistered();
        void fileBrowserNewFile();
        void fileBrowserNewFolder();
        void fileBrowserRename();
        void fileBrowserDelete();
        void fileBrowserCut();
        void fileBrowserCopy();
        void fileBrowserPaste();
        void fileBrowserDuplicate();
        
        void fileBrowserDoubleClicked();
        void fileBrowserRightClick();

    // helpers
    protected:
                void addGroupMarkToMenu( QString text );
        virtual void initMenu();
        virtual void contextMenuAboutToPopup();
        QPushButton* createTitleBarButton( QString text="" );
                void addTitleBarWidget( QWidget* widget );
        virtual void initTitleBar();
        virtual void initContent();

    protected:
        QHBoxLayout*   _titlebarLayout;
        FileTreeView*  _fileTreeView;

        QString        _workspacePath;

        static FileSystemOperations _fileSystemOperations;
        QMenu                       _fileBrowserMenu;
};

#endif

