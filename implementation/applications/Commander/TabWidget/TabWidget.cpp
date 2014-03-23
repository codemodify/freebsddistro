
#include <QtCore/QFileInfo>

// local headers
#include "TabWidget.h"
#include "TabContent/TabContent.h"


TabWidget::TabWidget() :
    QTabWidget()
{
    setUsesScrollButtons( true );

    // fix the focus
    tabBar()->setFocusPolicy( Qt::NoFocus );
}

TabWidget::~TabWidget()
{
    closeAllTabsForReal();
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//  interface
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void TabWidget::addTab( QString path )
{
    TabContent* tabContent = new TabContent( false );
                tabContent->getFileListView()->setCurrentFolder( path );

    connect( tabContent->getFileListView(), SIGNAL(currentFolderChanged(QString)),
             this,                          SLOT  (tabFolderChanged(QString)    )  );

    QTabWidget::addTab( tabContent, path );
    setCurrentIndex( count()-1 );
}

void TabWidget::addExplorerTab( QString path )
{
    TabContent* tabContent = new TabContent( true );

    QTabWidget::addTab( tabContent, path );
    setCurrentIndex( count()-1 );
}

void TabWidget::closeAllTabs()
{
    while( count()-1 )
    {
        setCurrentIndex( 0 );
        closeCurrentTab();
    }
}

void TabWidget::closeAllTabsForReal()
{
    while( count() )
    {
        setCurrentIndex( 0 );

        TabContent* tabContent = qobject_cast<TabContent*>( widget(0) );
        removeTab( 0 );
        delete tabContent;
    }
}

void TabWidget::closeCurrentTab()
{
    int index = currentIndex();
    if( -1 == index )
        return;
        
    if( count() == 1 )
        return;

    TabContent* tabContent = qobject_cast<TabContent*>( widget(index) );
    removeTab( index );
    delete tabContent;
}

void TabWidget::closeAllTabsExceptCurrent()
{
    int index = currentIndex();
    if( -1 == index )
        return;

    QString     label       = tabText( index );
    TabContent* tabContent  = qobject_cast<TabContent*>( widget(index) );
    removeTab( index );

    closeAllTabsForReal();

    QTabWidget::addTab( tabContent, label );
}

FileListView* TabWidget::getCurrentFileListView()
{
    int index = currentIndex();
    if( -1 == index )
        return QPointer<FileListView>( 0 );

    TabContent* tabContent = qobject_cast<TabContent*>( widget(index) );

    return tabContent->getFileListView();
}

FileTreeView* TabWidget::getCurrentFileTreeView()
{
    int index = currentIndex();
    if( -1 == index )
        return 0;

    TabContent* tabContent = qobject_cast<TabContent*>( widget(index) );

    return tabContent->getFileTreeView();
}

void TabWidget::tabFolderChanged( QString currentFolder )
{
    FileListView* fileListView = qobject_cast< FileListView* >( sender() );
    if( 0 == fileListView )
        return;

    for( int index = 0; index < count(); ++index )
    {
        TabContent* tabContent = qobject_cast<TabContent*>( widget(index) );
        if( fileListView == tabContent->getFileListView() )
        {
            setTabText( index, QFileInfo(currentFolder).fileName() );
            break;
        }
    }
}
