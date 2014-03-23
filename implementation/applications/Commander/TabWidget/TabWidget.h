#ifndef __TabWidget_H__
#define __TabWidget_H__

// Qt headers
#include <QtCore/QPointer>
#include <QtGui/QTabWidget>

// forwards
class FileListView;
class FileTreeView;


class TabWidget : public QTabWidget
{
    Q_OBJECT

    public:
        TabWidget();
        ~TabWidget();


    // interface
    public:
        void addTab( QString path="" );
        void addExplorerTab( QString path="" );
        void closeAllTabs();
        void closeAllTabsForReal();
        void closeCurrentTab();
        void closeAllTabsExceptCurrent();

        FileListView* getCurrentFileListView();
        FileTreeView* getCurrentFileTreeView();

    //
    private slots:
        void tabFolderChanged( QString currentFolder );

};

#endif
