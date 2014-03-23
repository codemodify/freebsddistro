
// Qt headers
#include <QtCore/QFileInfo>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QLineEdit>
#include <QtGui/QMenu>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QDialog>

// blocks
#include <Settings/Settings.h>
#include <FileSearch/FileSearch.h>
#include <FileSearchUi/FileSearchUi.h>
#include <IconProvider/ActionIconProvider/ActionIconProvider.h>
#include <IconProvider/FileSystemIconProvider/FileSystemIconProvider.h>
#include <ConfiguratorReader/ConfiguratorReader.h>

// local headers
#include "TabContent.h"
#include "Filter/Filter.h"
#include "Filter/NameFilter/NameFilter.h"
#include "Filter/ContentFilter/ContentFilter.h"


TabContent::TabContent( bool tree ) :
    QWidget(),
    _fileListView( 0 ),
    _fileTreeView( 0 )
{
    if( tree )
        buildTreeViewContent();
    else
        buildListViewContent();
}

TabContent::~TabContent()
{}

FileListView* TabContent::getFileListView()
{
    return _fileListView;
}

FileTreeView* TabContent::getFileTreeView()
{
    return _fileTreeView;
}

void TabContent::buildTreeViewContent()
{
    ActionIconProvider actionIconProvider;

    // init controls
    _currentPath    = new QLineEdit();
    _refresh        = new QToolButton();    _refresh      ->setIcon( actionIconProvider.getIconForAction("refresh")     );
    _search         = new QToolButton();    _search       ->setIcon( actionIconProvider.getIconForAction("find")        );
    _bookMarks      = new QToolButton();    _bookMarks    ->setIcon( actionIconProvider.getIconForAction("bookmark")    );

    _fileTreeView = new FileTreeView();

    // assign handlers
    connect( _currentPath,  SIGNAL(returnPressed()),    this, SLOT(cdToPathPressed())   );
    connect( _refresh,      SIGNAL(pressed()),          this, SLOT(refreshPressed())    );
    connect( _search,       SIGNAL(pressed()),          this, SLOT(searchPressed())     );
    connect( _bookMarks,    SIGNAL(pressed()),          this, SLOT(bookmarksPressed())  );
    
    connect( _fileTreeView, SIGNAL(currentFolderChanged(QString)),
             this,          SLOT  (currentFolderChanged(QString)) );


    // bind all the above together
    QHBoxLayout* hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget( _currentPath );
    hBoxLayout->addWidget( _refresh     );
    hBoxLayout->addWidget( _search      );
    hBoxLayout->addWidget( _bookMarks   );

    QVBoxLayout* vBoxLayout = new QVBoxLayout( this );
    vBoxLayout->addLayout( hBoxLayout       );
    vBoxLayout->addWidget( _fileTreeView );

    // fix the focus
    _currentPath->  setFocusPolicy( Qt::ClickFocus );
    _refresh->      setFocusPolicy( Qt::NoFocus );
    _search->       setFocusPolicy( Qt::NoFocus );
    _bookMarks->    setFocusPolicy( Qt::NoFocus );
}

void TabContent::buildListViewContent()
{
    ActionIconProvider actionIconProvider;

    // init controls
    _currentPath    = new QLineEdit();
    _cdToRoot       = new QToolButton();    _cdToRoot     ->setText( "/"  );
    _cdToParent     = new QToolButton();    _cdToParent   ->setIcon( actionIconProvider.getIconForAction("up")          );
    _refresh        = new QToolButton();    _refresh      ->setIcon( actionIconProvider.getIconForAction("refresh")     );
    _search         = new QToolButton();    _search       ->setIcon( actionIconProvider.getIconForAction("find")        );
    _bookMarks      = new QToolButton();    _bookMarks    ->setIcon( actionIconProvider.getIconForAction("bookmark")    );
    _filters        = new QToolButton();    _filters      ->setIcon( actionIconProvider.getIconForAction("filter")      );

    _filtersArea    = new QVBoxLayout();

    _fileListView = new FileListView();
    _fileListView->setHiddenVisible( ConfiguratorReader().getCheckedCheckBoxes("fileOperations/hiddenFiles/hiddenFiles").contains("showHideHiddenFiles") );

    // assign handlers
    connect( _currentPath,  SIGNAL(returnPressed()),    this, SLOT(cdToPathPressed())   );
    connect( _cdToRoot,     SIGNAL(pressed()),          this, SLOT(cdToRootPressed())   );
    connect( _cdToParent,   SIGNAL(pressed()),          this, SLOT(cdToParentPressed()) );
    connect( _refresh,      SIGNAL(pressed()),          this, SLOT(refreshPressed())    );
    connect( _search,       SIGNAL(pressed()),          this, SLOT(searchPressed())     );
    connect( _bookMarks,    SIGNAL(pressed()),          this, SLOT(bookmarksPressed())  );
    connect( _filters,       SIGNAL(pressed()),         this, SLOT(filterPressed())     );

    connect( _fileListView, SIGNAL(currentFolderChanged(QString)),
             this,          SLOT  (currentFolderChanged(QString)) );

    // bind all the above together
    QHBoxLayout* hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget( _currentPath );
    hBoxLayout->addWidget( _cdToRoot    );
    hBoxLayout->addWidget( _cdToParent  );
    hBoxLayout->addWidget( _refresh     );
    hBoxLayout->addWidget( _search      );
    hBoxLayout->addWidget( _bookMarks   );
    hBoxLayout->addWidget( _filters     );

    QVBoxLayout* vBoxLayout = new QVBoxLayout( this );
    vBoxLayout->addLayout( hBoxLayout       );
    vBoxLayout->addLayout( _filtersArea     );
    vBoxLayout->addWidget( _fileListView    );

    // fix the focus
    _currentPath->  setFocusPolicy( Qt::ClickFocus );
    _cdToRoot->     setFocusPolicy( Qt::NoFocus );
    _cdToParent->   setFocusPolicy( Qt::NoFocus );
    _refresh->      setFocusPolicy( Qt::NoFocus );
    _search->       setFocusPolicy( Qt::NoFocus );
    _bookMarks->    setFocusPolicy( Qt::NoFocus );
    _filters->      setFocusPolicy( Qt::NoFocus );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//  some handy operations
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void TabContent::cdToPathPressed()
{
    if( _fileListView )
        _fileListView->setCurrentFolder( _currentPath->text() );
    else
        _fileTreeView->setCurrentFolder( _currentPath->text() );
}

void TabContent::cdToRootPressed()
{
    _fileListView->setCurrentFolder( "/" );
}

void TabContent::cdToParentPressed()
{
    _fileListView->setCurrentFolder( _fileListView->getCurrentFolder() + "/.." );
}

void TabContent::refreshPressed()
{
    if( _fileListView )
        _fileListView->setCurrentFolder( _fileListView->getCurrentFolder() );
    else
        _fileTreeView->refreshContent();
}

void TabContent::filterPressed()
{
    QMenu*  filtersMenu = new QMenu();
            filtersMenu->addAction( "filter-By-name",       this, SLOT(filterNameFilter())      );
            filtersMenu->addAction( "filter-By-content",    this, SLOT(filterContentFilter())   );
            filtersMenu->popup( mapToGlobal( QPoint(_filters->x(),_filters->y()+_filters->height()) ) );
}

void TabContent::searchPressed()
{
    FileSearch* fileSearch = new FileSearch();

    FileSearchUi*   fileSearchUi = new FileSearchUi( fileSearch );
                    fileSearchUi->setSearchFolder( _currentPath->text() );

    connect( fileSearchUi, SIGNAL(fileDoubleClicked(QString)),
             this,         SLOT(fileSearchDoubleClicked(QString)) );

    fileSearchUi->show();
}

void TabContent::bookmarksPressed()
{
    QStringList bookmarkList;
    Settings    settings;
                settings.setFile( Settings::getSettingsFileForApp() );
                settings.getValuesForKey( "bookmarks", bookmarkList );

    ActionIconProvider actionIconProvider;
    FileSystemIconProvider fileSystemIconProvider;

    QMenu*  bookmarksMenu = new QMenu();
            bookmarksMenu->addAction(   actionIconProvider.getIconForAction("add"),
                                        "current folder",
                                        this,
                                        SLOT(bookmarksAddBook()) );

            foreach( QString bookmark, bookmarkList )
                bookmarksMenu->addAction(   fileSystemIconProvider.getIconForFile(bookmark), 
                                            bookmark, 
                                            this,
                                            SLOT(bookmarksBookChoosed()) );

            bookmarksMenu->popup( mapToGlobal( QPoint(_bookMarks->x(),_bookMarks->y()+_bookMarks->height()) ) );
}

void TabContent::bookmarksAddBook()
{
    QStringList bookmarkList;

    Settings    settings;
                settings.setFile( Settings::getSettingsFileForApp() );
                settings.getValuesForKey( "bookmarks", bookmarkList );

    if( _fileListView )
        bookmarkList.append( _fileListView->getCurrentFolder() );
    else
        bookmarkList.append( _fileTreeView->getCurrentFolder() );

    settings.setValuesForKey( "bookmarks", bookmarkList );
    settings.save();
}

void TabContent::bookmarksBookChoosed()
{
    QAction* action = qobject_cast<QAction*>( sender() );
    if( 0 == action )
        return;

    if( _fileListView )
        _fileListView->setCurrentFolder( action->text() );
    else
        _fileTreeView->setCurrentFolder( action->text() );
}

void TabContent::currentFolderChanged( QString currentFolder )
{
    _currentPath->setText( currentFolder );
}

void TabContent::filterNameFilter()
{
    Filter* filter = new NameFilter( *_fileListView );

    connect( filter, SIGNAL(filterClose()), this, SLOT(filterClose()) );

    _filtersArea->addWidget( filter );
}

void TabContent::filterContentFilter()
{
    Filter* filter = new ContentFilter( *_fileListView );

    connect( filter, SIGNAL(filterClose()), this, SLOT(filterClose()) );

    _filtersArea->addWidget( filter );
}

void TabContent::filterClose()
{
    Filter* filter = qobject_cast<Filter*>( sender() );
    if( 0 == filter )
        return;

    _filtersArea->removeWidget( filter );
    delete filter;
}

void TabContent::fileSearchDoubleClicked( QString filePath )
{
	_fileListView->setCurrentFolder( QFileInfo(filePath).absolutePath() );
}
