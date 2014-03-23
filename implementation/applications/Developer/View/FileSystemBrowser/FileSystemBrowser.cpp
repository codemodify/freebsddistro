
// Qt headers
#include <QtCore/QFileInfo>
#include <QtCore/QModelIndex>
#include <QtGui/QInputDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QApplication>
#include <QtGui/QPushButton>

// blocks
#include <FileLauncher/FileLauncher.h>

// local headers
#include "FileSystemBrowser.h"

// static stuff
FileSystemOperations FileSystemBrowser::_fileSystemOperations;


FileSystemBrowser::FileSystemBrowser():
    View(),
    _workspacePath( "/" )
{
    setWindowTitle( "File System" );
    
    initTitleBar();
    
    initMenu();
    
    initContent();
}

FileSystemBrowser::~FileSystemBrowser()
{}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// file operations handlers
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FileSystemBrowser::fileBrowserGoInto()
{
    _fileTreeView->setRootFolder( _fileTreeView->getCurrentItemPath() );
}

void FileSystemBrowser::fileBrowserGoBack()
{
    _fileTreeView->setRootFolder( _workspacePath );
}

void FileSystemBrowser::fileBrowserRefresh()
{
    _fileTreeView->refreshContent();
}

void FileSystemBrowser::fileBrowserEdit()
{
    QStringList selectedItems;
    _fileTreeView->getSelectedItems( selectedItems );
    
    foreach( QString file, selectedItems )
        emit editFile( file );
}

void FileSystemBrowser::fileBrowserOpenWithRegistered()
{
    FileLauncher fileLauncher;
                 fileLauncher.launchFile( _fileTreeView->getCurrentItemPath() );
}

void FileSystemBrowser::fileBrowserNewFile()
{
    QString fileName = QInputDialog::getText( this, "File Name", "" );
    if( false == fileName.isEmpty() )
    {
        _fileSystemOperations.createFile( _fileTreeView->getCurrentFolder() + "/" + fileName );
        _fileTreeView->refreshContent();
    }
}

void FileSystemBrowser::fileBrowserNewFolder()
{
    QString folderName = QInputDialog::getText( this, "Folder Name", "" );
    if( false == folderName.isEmpty() )
    {
        QString currentItem = _fileTreeView->getCurrentItemPath();
        if( false == QFileInfo(currentItem).isDir() )
            currentItem = QFileInfo(currentItem).absolutePath();

        _fileSystemOperations.createFolder( currentItem + "/" + folderName );
        _fileTreeView->refreshContent();
    }
}

void FileSystemBrowser::fileBrowserRename()
{
    QString item = _fileTreeView->getCurrentItemPath();   

    QString newName = QInputDialog::getText( this, 
                                             "Rename", QString("from: %1").arg(item),
                                             QLineEdit::Normal,
                                             QFileInfo(item).fileName() );
    if( false == newName.isEmpty() )
    {
        if( true == QFileInfo(item).isDir() )
            _fileSystemOperations.renameFolder( item, newName );
        else
            _fileSystemOperations.renameFile( item, newName );
            
        _fileTreeView->refreshContent();
    }
}

void FileSystemBrowser::fileBrowserDelete()
{
    QStringList selectedItems;
    _fileTreeView->getSelectedItems( selectedItems );

    _fileSystemOperations.removeObjects( selectedItems );
        
    _fileTreeView->refreshContent();
}

void FileSystemBrowser::fileBrowserCut()
{
    QStringList selectedItems;
    _fileTreeView->getSelectedItems( selectedItems );
    
    _fileSystemOperations.markObjectsForCut( selectedItems );
}

void FileSystemBrowser::fileBrowserCopy()
{
    QStringList selectedItems;
    _fileTreeView->getSelectedItems( selectedItems );
    
    _fileSystemOperations.markObjectsForCopy( selectedItems );
}

void FileSystemBrowser::fileBrowserPaste()
{
    QString targetFolder = _fileTreeView->getCurrentItemPath();
    if( false == QFileInfo(targetFolder).isDir() )
        targetFolder = QFileInfo(targetFolder).absolutePath();

    _fileSystemOperations.pasteObjects( targetFolder );
    _fileTreeView->refreshContent();
}

void FileSystemBrowser::fileBrowserDuplicate()
{
    QStringList selectedItems;

    _fileTreeView->getSelectedItems( selectedItems );

    _fileSystemOperations.duplicateObjects( selectedItems );
}

void FileSystemBrowser::fileBrowserDoubleClicked()
{
    QString item = _fileTreeView->getCurrentItemPath();
    
    if( false == QFileInfo(item).isDir() )
        emit editFile( item );
}

void FileSystemBrowser::fileBrowserRightClick()
{
    contextMenuAboutToPopup();
    _fileBrowserMenu.exec( _fileTreeView->getMousePosition() );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// helpers
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FileSystemBrowser::addGroupMarkToMenu( QString text )
{
    QFont   font;
            font.setBold( true );
            font.setStyle( QFont::StyleOblique );

    QAction* action = 0;

    action = _fileBrowserMenu.addAction( QString("~~~~~ %1 ~~~~~").arg(text) );
    action->setFont( font );
}

void FileSystemBrowser::initMenu()
{
    ActionIconProvider actionIconProvider;

    addGroupMarkToMenu( "File Management" );

    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("refresh"),   "refresh", this, SLOT(fileBrowserRefresh()) );
    _fileBrowserMenu.addSeparator();

    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("forward"),   "go into", this, SLOT(fileBrowserGoInto()));
    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("backward"),  "go back", this, SLOT(fileBrowserGoBack()));    
    _fileBrowserMenu.addSeparator();

    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("edit"),      "edit as text",                 this, SLOT(fileBrowserEdit()) );
    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("run"),       "open with registered program", this, SLOT(fileBrowserOpenWithRegistered()));
    _fileBrowserMenu.addSeparator();

    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("newfile"),   "new file",   this, SLOT(fileBrowserNewFile())   );
    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("newfolder"), "new folder", this, SLOT(fileBrowserNewFolder()) );
    _fileBrowserMenu.addSeparator();

    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("rename"),    "rename",    this, SLOT(fileBrowserRename())    );
    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("delete"),    "delete",    this, SLOT(fileBrowserDelete())    );
    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("cut"),       "cut",       this, SLOT(fileBrowserCut())       );
    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("copy"),      "copy",      this, SLOT(fileBrowserCopy())      );
    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("paste"),     "paste",     this, SLOT(fileBrowserPaste())     );
    _fileBrowserMenu.addAction( actionIconProvider.getIconForAction("duplicate"), "duplicate", this, SLOT(fileBrowserDuplicate()) );
}

void FileSystemBrowser::contextMenuAboutToPopup()
{}

void FileSystemBrowser::initTitleBar()
{
    QAction* close = new QAction( 0 );
             close->setIcon( ActionIconProvider().getIconForAction("close") );

    setTitleCloseButton( close, this, SLOT(close()) );
    setTitleLyoutButtonsAtLeft();
}

void FileSystemBrowser::initContent()
{
    _fileTreeView = new FileTreeView();
    _fileTreeView->setRootFolder( "/" );

    connect( _fileTreeView, SIGNAL(mouseDoubleClicked()),   this, SLOT(fileBrowserDoubleClicked())  );
    connect( _fileTreeView, SIGNAL(mouseRightClicked()),    this, SLOT(fileBrowserRightClick())     );

    setContentWidget( _fileTreeView );

    // setup the file operations
    _fileSystemOperations.setMaxThreadsLimit( 1 );

}

