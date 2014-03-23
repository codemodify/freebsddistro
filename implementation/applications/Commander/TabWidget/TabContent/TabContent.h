#ifndef __TabContent_H__
#define __TabContent_H__

// Qt headers
#include <QtGui/QWidget>

// bloks
#include <FileListView/FileListView.h>
#include <FileTreeView/FileTreeView.h>


// forward declarations
class QToolButton;
class QLineEdit;
class QMenu;
class QVBoxLayout;

class TabContent : public QWidget
{
    Q_OBJECT

    public:
        TabContent( bool tree );
        ~TabContent();


    // interface
    public:
        FileListView* getFileListView();
        FileTreeView* getFileTreeView();
        
    
   // helpers
   private:
        void buildTreeViewContent();
        void buildListViewContent();


    // some handy operations
    private slots:
		void cdToPathPressed();
        void cdToRootPressed();
        void cdToParentPressed();
        void refreshPressed();
        void searchPressed();
        void bookmarksPressed();
        void filterPressed();
        void currentFolderChanged( QString currentFolder );

    private slots:
        void bookmarksAddBook();
        void bookmarksBookChoosed();
        void filterNameFilter();
        void filterContentFilter();
        void filterClose();
        void fileSearchDoubleClicked( QString filePath );


    // list of widgets to stock
    private:
        QLineEdit*      _currentPath;
        QToolButton*    _cdToRoot;
        QToolButton*    _cdToParent;
        QToolButton*    _refresh;
        QToolButton*    _search;
        QToolButton*    _bookMarks;
        QToolButton*    _filters;

        QVBoxLayout*    _filtersArea;

        FileListView*   _fileListView;
        FileTreeView*   _fileTreeView;

};

#endif
