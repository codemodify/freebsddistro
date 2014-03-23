
// Qt headers
#include <QtCore/QProcess>
#include <QtCore/QFileInfo>
#include <QtGui/QSplitter>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QToolBar>
#include <QtGui/QInputDialog>
#include <QtGui/QDialog>
#include <QtGui/QKeyEvent>
#include <QtGui/QApplication>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>

// blocks
#include <FileSystemOperations/FileSystemOperations.h>
#include <FileSystemOperationsUi/FileSystemOperationsUi.h>
#include <FileListView/FileListView.h>
#include <FileTreeView/FileTreeView.h>
#include <FileEditor/FileEditor.h>
#include <Styling/Styling.h>
#include <Settings/Settings.h>
#include <IconProvider/ActionIconProvider/ActionIconProvider.h>
#include <FileLauncher/FileLauncher.h>
#include <Configurator/Configurator.h>
#include <ConfiguratorReader/ConfiguratorReader.h>

// local headers
#include "Commander.h"
#include "TabWidget/TabWidget.h"
#include "Dialogs/AddRemoveSelection/AddRemoveSelection.h"
#include "Dialogs/MultiRenameTool/MultiRenameTool.h"



Commander::Commander() :
    QMainWindow(),
    _activeTab( 0 )
{
    // self cosmetics
    setWindowTitle( "Commander" );
    setWindowIcon( QIcon(QApplication::applicationFilePath() + ".png") );


    Styling styling( *this );
            styling.loadOnScreenPosition();
            styling.loadFonts();

    // setup controls
    setupSystemTrayicon();
    setupMenu();
    setupToolbar();
    setupTabs();
    
    connect( QApplication::instance(), SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(focusChanged(QWidget*,QWidget*)) );

    // setup the file operations
    _fileSystemOperations = new FileSystemOperations();
    
    _fileSystemOperationsUi = new FileSystemOperationsUi( _fileSystemOperations );
    _fileSystemOperationsUi->setWindowTitle( "Commander File Operations" );
}

Commander::~Commander()
{
    closeAllTabs();

    delete _fileSystemOperations;
    delete _fileSystemOperationsUi;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Sys tray
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Commander::setupSystemTrayicon()
{
    QMenu*  sysTrayMenu = new QMenu();
            sysTrayMenu->addAction( ActionIconProvider().getIconForAction("exit"), "Exit", QApplication::instance(), SLOT(quit()) );

	QIcon icon( QApplication::applicationFilePath() + ".png" );

	Styling	styling( *this );
			styling.installSystrayIcon( &icon, sysTrayMenu );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Menu
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Commander::setupMenu()
{
    ActionIconProvider actionIconProvider;

    QMenu*  commanderMenu = menuBar()->addMenu( "Commander" );
            commanderMenu->addAction( actionIconProvider.getIconForAction("help"),  "Help Me!", this, SLOT(helpMe())    );
            commanderMenu->addAction( actionIconProvider.getIconForAction("exit"),  "Exit",     this, SLOT(close())     );

    QMenu*  tabsMenu = menuBar()->addMenu( "Tabs" );
            tabsMenu->addAction( actionIconProvider.getIconForAction("add"),        "Tab",                this,   SLOT(openNewTab())              );
            tabsMenu->addAction( actionIconProvider.getIconForAction("add"),        "Explorer",           this,   SLOT(openNewExplorerTab())      );
            tabsMenu->addSeparator();
            tabsMenu->addAction( actionIconProvider.getIconForAction("remove"),     "Current",            this,   SLOT(closeCurrentTab())         );
            tabsMenu->addAction( actionIconProvider.getIconForAction("remove"),     "All",                this,   SLOT(closeAllTabs())            );
            tabsMenu->addAction( actionIconProvider.getIconForAction("remove"),     "All Except Current", this,   SLOT(closeAllExceptCurrent())   );
}

void Commander::helpMe()
{
    QStringList configuratorParameter;
                configuratorParameter << Configurator::getAppConfigurationFile();
                configuratorParameter << "helpMe";
                configuratorParameter << "quickHelp";

	Configurator*	configurator = new Configurator( configuratorParameter );
	connect( configurator, SIGNAL(configurationChanged()), this, SLOT(readConfiguration()) );

	configurator->exec();
}

void Commander::closeEvent( QCloseEvent* event )
{
    Styling styling( *this );
            styling.saveOnScreenPosition();

    QMainWindow::closeEvent( event );
}

void Commander::openNewTab()
{
    // getting the tab to work with
    TabWidget*  tabWidget = _activeTab;
                tabWidget->addTab( "/" );

    FileListView* fileListView = tabWidget->getCurrentFileListView();

    // connect it to the matrix of mouse handling
    connect( fileListView, SIGNAL(mouseRightClicked     ()), this, SLOT(mouseRightClicked   ()) );
    connect( fileListView, SIGNAL(mouseDoubleClicked    ()), this, SLOT(mouseDoubleClicked  ()) );
    connect( fileListView, SIGNAL(mouseItemsDropped     ()), this, SLOT(mouseItemsDropped   ()) );

    // connect it to the matrix of keyboard handling
    connect( fileListView, SIGNAL(f01keyPressed         ()), this, SLOT(f01keyPressed       ()) );
    connect( fileListView, SIGNAL(f02keyPressed         ()), this, SLOT(f02keyPressed       ()) );
    connect( fileListView, SIGNAL(f03keyPressed         ()), this, SLOT(f03keyPressed       ()) );
    connect( fileListView, SIGNAL(f04keyPressed         ()), this, SLOT(f04keyPressed       ()) );
    connect( fileListView, SIGNAL(f05keyPressed         ()), this, SLOT(f05keyPressed       ()) );
    connect( fileListView, SIGNAL(f06keyPressed         ()), this, SLOT(f06keyPressed       ()) );
    connect( fileListView, SIGNAL(f07keyPressed         ()), this, SLOT(f07keyPressed       ()) );
    connect( fileListView, SIGNAL(f08keyPressed         ()), this, SLOT(f08keyPressed       ()) );

    connect( fileListView, SIGNAL(insertPressed         ()), this, SLOT(insertPressed       ()) );
    connect( fileListView, SIGNAL(deletePressed         ()), this, SLOT(f08keyPressed       ()) );

    connect( fileListView, SIGNAL(backspacePressed      ()), this, SLOT(backspacePressed    ()) );
    connect( fileListView, SIGNAL(enterPressed          ()), this, SLOT(mouseDoubleClicked  ()) );
    connect( fileListView, SIGNAL(spacePressed          ()), this, SLOT(spacePressed        ()) );

    connect( fileListView, SIGNAL(numpadMinusPressed    ()), this, SLOT(numpadMinusPressed  ()) );
    connect( fileListView, SIGNAL(numpadPlusPressed     ()), this, SLOT(numpadPlusPressed   ()) );
}

void Commander::openNewExplorerTab()
{
    // getting the tab to work with
    TabWidget*  tabWidget = _activeTab;
                tabWidget->addExplorerTab( "/" );

    FileTreeView* fileTreeView = tabWidget->getCurrentFileTreeView();

    // connect it to the matrix mouse handling
    connect( fileTreeView, SIGNAL(mouseRightClicked     ()), this, SLOT(mouseRightClicked   ()) );
    connect( fileTreeView, SIGNAL(mouseDoubleClicked    ()), this, SLOT(mouseDoubleClicked  ()) );
    connect( fileTreeView, SIGNAL(mouseItemsDropped     ()), this, SLOT(mouseItemsDropped   ()) );

    // connect it to the matrix keyboard handling
    connect( fileTreeView, SIGNAL(f01keyPressed         ()), this, SLOT(f01keyPressed       ()) );
    connect( fileTreeView, SIGNAL(f02keyPressed         ()), this, SLOT(f02keyPressed       ()) );
    connect( fileTreeView, SIGNAL(f03keyPressed         ()), this, SLOT(f03keyPressed       ()) );
    connect( fileTreeView, SIGNAL(f04keyPressed         ()), this, SLOT(f04keyPressed       ()) );

    connect( fileTreeView, SIGNAL(deletePressed         ()), this, SLOT(f08keyPressed       ()) );
    connect( fileTreeView, SIGNAL(enterPressed          ()), this, SLOT(mouseDoubleClicked  ()) );
}

void Commander::closeAllTabs()
{
    _leftTabWidget->closeAllTabs();
    _rightTabWidget->closeAllTabs();
}

void Commander::closeCurrentTab()
{
    _activeTab->closeCurrentTab();
}

void Commander::closeAllExceptCurrent()
{
    _leftTabWidget->closeAllTabsExceptCurrent();
    _rightTabWidget->closeAllTabsExceptCurrent();
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Toolbar
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Commander::setupToolbar()
{
    QToolBar* toolBar = new QToolBar();

    addToolBar( Qt::BottomToolBarArea, toolBar );

    toolBar->addAction( "F1 Properties",  this, SLOT(slot_properties())    );
    toolBar->addSeparator();

    toolBar->addAction( "F2 Rename",      this, SLOT(slot_rename())        );
    toolBar->addSeparator();

    toolBar->addAction( "F3 View",        this, SLOT(slot_view())          );
    toolBar->addSeparator();

    toolBar->addAction( "F4 Edit",        this, SLOT(slot_edit())          );
    toolBar->addSeparator();

    toolBar->addAction( "F5 Copy",        this, SLOT(slot_copy())          );
    toolBar->addSeparator();

    toolBar->addAction( "F6 Move",        this, SLOT(slot_move())          );
    toolBar->addSeparator();

    toolBar->addAction( "F7 Make Dir",    this, SLOT(slot_makedir())       );
    toolBar->addSeparator();

    toolBar->addAction( "F8 Remove",      this, SLOT(slot_remove())        );
    toolBar->addSeparator();

    toolBar->addAction( "File Operations",  this, SLOT(slot_showFileOperations())    );
}

void Commander::slot_properties()
{
    f01keyPressed();
}

void Commander::slot_rename()
{
    f02keyPressed();
}

void Commander::slot_view()
{
    f03keyPressed();
}

void Commander::slot_edit()
{
    f04keyPressed();
}

void Commander::slot_copy()
{
    f05keyPressed();
}

void Commander::slot_move()
{
    f06keyPressed();
}

void Commander::slot_makedir()
{
    f07keyPressed();
}

void Commander::slot_remove()
{
    f08keyPressed();
}

void Commander::slot_showFileOperations()
{
    _fileSystemOperationsUi->show();
}

void Commander::slot_duplicate()
{
    FileListView* fileListView = _activeTab->getCurrentFileListView();
    FileTreeView* fileTreeView = _activeTab->getCurrentFileTreeView();

    // get the source items
    QStringList itemsToOperateOn;
    if( fileListView )
        fileListView->getSelectedItems( itemsToOperateOn );
    else
        fileTreeView->getSelectedItems( itemsToOperateOn );
    
    if( itemsToOperateOn.isEmpty() )
        return;

    _fileSystemOperations->duplicateObjects( itemsToOperateOn );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// This takes care of the file operations
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Commander::mouseRightClicked()
{
    QObject* object = sender();
     
    FileListView* fileListView = qobject_cast<FileListView*>( object );
    FileTreeView* fileTreeView = qobject_cast<FileTreeView*>( object );
    
    ActionIconProvider actionIconProvider;
    
    QMenu*  contextMenu = new QMenu();
            contextMenu->addAction( actionIconProvider.getIconForAction("settings"),     "Properties",   this, SLOT(slot_properties())   );
            contextMenu->addAction( actionIconProvider.getIconForAction("rename"),       "Rename",       this, SLOT(slot_rename())       );
            contextMenu->addAction( actionIconProvider.getIconForAction("duplicate"),    "Duplicate",    this, SLOT(slot_duplicate())    );
            contextMenu->addAction( actionIconProvider.getIconForAction("newfolder"),    "Mk-Dir",       this, SLOT(slot_makedir())      );
            contextMenu->addSeparator();
            contextMenu->addAction( actionIconProvider.getIconForAction("view"),         "View",         this, SLOT(slot_view())         );
            contextMenu->addAction( actionIconProvider.getIconForAction("edit"),         "Edit",         this, SLOT(slot_edit())         );
            contextMenu->addSeparator();
            contextMenu->addAction( actionIconProvider.getIconForAction("remove"),       "Remove",       this, SLOT(slot_remove())       );
            contextMenu->addAction( actionIconProvider.getIconForAction("cut"),          "Cut",          this, SLOT(contextMenuCut())    );
            contextMenu->addAction( actionIconProvider.getIconForAction("copy"),         "Copy",         this, SLOT(contextMenuCopy())   );
            contextMenu->addAction( actionIconProvider.getIconForAction("paste"),        "Paste",        this, SLOT(contextMenuPaste())  );

    if( fileListView )
        contextMenu->popup( fileListView->getMousePosition() );
    else
        contextMenu->popup( fileTreeView->getMousePosition() );
}

void Commander::mouseDoubleClicked()
{
    // check that this is comming from a file list not tree list
    FileListView* fileListView = qobject_cast<FileListView*>( sender() );
    if( 0 == fileListView )
        return;

    QString path = _activeTab->getCurrentFileListView()->getCurrentItemPath();

    // if this is a folder then CD into it
    if( QFileInfo(path).isDir() )
        _activeTab->getCurrentFileListView()->setCurrentFolder( path );

    // if this is a file try to launch it
    else
    {
        FileLauncher    fileLauncher;
                        fileLauncher.launchFile( path );
    }
}

void Commander::mouseItemsDropped()
{
    FileListView* fileListView = qobject_cast<FileListView*>( sender() );
    FileTreeView* fileTreeView = qobject_cast<FileTreeView*>( sender() );

    QString&        targetFolder    = fileListView ? fileListView->getTargetFolder() : fileTreeView->getTargetFolder();
    QStringList&    droppedItems    = fileListView ? fileListView->getDroppedItems() : fileTreeView->getDroppedItems();

    // ask user to confirm the copy/move operation
    QLabel*     label = new QLabel();
                label->setText( QString("Items dropped into: \"%1\" \n\nApproval is needed to complete this.").arg(targetFolder) );

    QCheckBox*  checkBox = new QCheckBox();
                checkBox->setText( "remove source when done" );

    QPushButton* okButton        = new QPushButton( "Ok" );
    QPushButton* cancelButton    = new QPushButton( "Cancel" );
    QHBoxLayout* hBoxLayout     = new QHBoxLayout();
                 hBoxLayout->addStretch();
                 hBoxLayout->addWidget( okButton        );
                 hBoxLayout->addWidget( cancelButton    );

    QDialog*    dialog = new QDialog();
                dialog->setWindowTitle( "Commander - Items droped" );
                dialog->setMinimumWidth( 400 );

    QVBoxLayout* vBoxLayout = new QVBoxLayout( dialog );
                 vBoxLayout->addWidget( label         );
                 vBoxLayout->addWidget( checkBox      );
                 vBoxLayout->addLayout( hBoxLayout      );

    connect( okButton,        SIGNAL(pressed()), dialog, SLOT(accept()) );
    connect( cancelButton,    SIGNAL(pressed()), dialog, SLOT(reject()) );

    QDialog::DialogCode dialogCode = (QDialog::DialogCode)dialog->exec();
    if( QDialog::Rejected == dialogCode )
        return;

    // start the move
    if( checkBox->isChecked() )
        _fileSystemOperations->moveObjectsToFolder( droppedItems, targetFolder );        

    // start the copy
    else
        _fileSystemOperations->copyObjectsToFolder( droppedItems, targetFolder );
}

void Commander::f01keyPressed()
{
    qDebug( "FIXME: Commander::f01keyPressed()" );
}

void Commander::f02keyPressed()
{
    FileListView* fileListView = _activeTab->getCurrentFileListView();
    FileTreeView* fileTreeView = _activeTab->getCurrentFileTreeView();

    // get the source items
    QStringList itemsToOperateOn;
    if( fileListView )
        fileListView->getSelectedItems( itemsToOperateOn );
    else
        fileTreeView->getSelectedItems( itemsToOperateOn );
    
    if( itemsToOperateOn.isEmpty() )
        return;

    // if there are many items then we use the multi-rename tool
    if( itemsToOperateOn.count() > 1 )
    {
        MultiRenameTool*    multiRenameTool = new MultiRenameTool( itemsToOperateOn );
                            multiRenameTool->show();
    }
    else
    {
        bool okToRename     = false;
        QString currentName = itemsToOperateOn.at( 0 );

        QString itemName    = QFileInfo( currentName ).fileName();
        QString newName     = QInputDialog::getText
                                (
                                    this,
                                    "Enter The New Name",
                                    QString( "Current Name: %1" ).arg( itemName ),
                                    QLineEdit::Normal,
                                    itemName,
                                    &okToRename
                                );

        if( okToRename && !newName.isEmpty() )
        {
            if( QFileInfo(currentName).isDir() )
                _fileSystemOperations->renameFolder( currentName, newName );
            else
                _fileSystemOperations->renameFile( currentName, newName );
        }
    }
}

void Commander::f03keyPressed()
{
    f04keyPressed();
}

void Commander::f04keyPressed()
{    
    FileListView* fileListView = _activeTab->getCurrentFileListView();
    FileTreeView* fileTreeView = _activeTab->getCurrentFileTreeView();

    // get the source items
    QStringList itemsToOperateOn;
    if( fileListView )
        fileListView->getSelectedItems( itemsToOperateOn );
    else
        fileTreeView->getSelectedItems( itemsToOperateOn );
    
    if( itemsToOperateOn.isEmpty() )
        return;

    FileEditor* fileEditor = new FileEditor( itemsToOperateOn.at(0) );
                fileEditor->show();
}

void Commander::f05keyPressed()
{
    // checking that this is done from the FileList
    if( 0 == _activeTab->getCurrentFileListView() )
        return;

    // setting the source and destination
    FileListView* sourceFileListView        =   ( _activeTab == _leftTabWidget )
                                                ? _leftTabWidget->getCurrentFileListView() : _rightTabWidget->getCurrentFileListView();
    FileListView* destinationFileListView   =   ( _activeTab != _leftTabWidget )
                                                ? _leftTabWidget->getCurrentFileListView() : _rightTabWidget->getCurrentFileListView();

    // get the source files
    QStringList itemsToOperateOn;
    sourceFileListView->getSelectedItems( itemsToOperateOn );
    if( itemsToOperateOn.isEmpty() )
        return;

    QString sourceMessage = (itemsToOperateOn.count() > 1) ? 
						"\n" + itemsToOperateOn.value(0) +"\n"+ itemsToOperateOn.value(1) + "\n...\n" :
						itemsToOperateOn.value(0);

    QMessageBox::StandardButton userChoise = QMessageBox::question
                                                (
                                                    this,
                                                    "Copy Confirmation",
                                                    QString("Source: %1 \nTo: %2").arg(sourceMessage).arg(destinationFileListView->getCurrentFolder()),
                                                    QMessageBox::Yes | QMessageBox::No
                                                );
    // start the copy
    if( QMessageBox::Yes == userChoise )
        _fileSystemOperations->copyObjectsToFolder( itemsToOperateOn, destinationFileListView->getCurrentFolder() );
}

void Commander::f06keyPressed()
{
    // checking that this is done from the FileList
    if( 0 == _activeTab->getCurrentFileListView() )
        return;

    // setting the source and destination
    FileListView* sourceFileListView        =   ( _activeTab == _leftTabWidget )
                                                ? _leftTabWidget->getCurrentFileListView() : _rightTabWidget->getCurrentFileListView();
    FileListView* destinationFileListView   =   ( _activeTab != _leftTabWidget )
                                                ? _leftTabWidget->getCurrentFileListView() : _rightTabWidget->getCurrentFileListView();

    // get the source files
    QStringList itemsToOperateOn;
    sourceFileListView->getSelectedItems( itemsToOperateOn );
    if( itemsToOperateOn.isEmpty() )
        return;

    QString sourceMessage = (itemsToOperateOn.count() > 1) ? 
						"\n" + itemsToOperateOn.value(0) +"\n"+ itemsToOperateOn.value(1) + "\n...\n" :
						itemsToOperateOn.value(0);

    QMessageBox::StandardButton userChoise = QMessageBox::question
                                                (
                                                    this,
                                                    "Move Confirmation",
                                                    QString("Source: %1 \nTo: %2").arg(sourceMessage).arg(destinationFileListView->getCurrentFolder()),
                                                    QMessageBox::Yes | QMessageBox::No
                                                );
    // start the move
    if( QMessageBox::Yes == userChoise )
        _fileSystemOperations->moveObjectsToFolder( itemsToOperateOn, destinationFileListView->getCurrentFolder() );
}

void Commander::f07keyPressed()
{
    FileListView* fileListView = _activeTab->getCurrentFileListView();
    FileTreeView* fileTreeView = _activeTab->getCurrentFileTreeView();

    // get the user input
    bool okToCreate         = false;
    QString currentFolder   = fileListView ? fileListView->getCurrentFolder() : fileTreeView->getCurrentFolder();
    QString newFolderName   = QInputDialog::getText
                                (
                                    this,
                                    "Enter The Name For The New Folder",
                                    QString( "Current folder: %1" ).arg( currentFolder ),
                                    QLineEdit::Normal,
                                    "new-folder",
                                    &okToCreate
                                );

    if( okToCreate && !newFolderName.isEmpty() )
        _fileSystemOperations->createFolder( QString("%1/%2").arg(currentFolder).arg(newFolderName) );
}

void Commander::f08keyPressed()
{
    FileListView* fileListView = _activeTab->getCurrentFileListView();
    FileTreeView* fileTreeView = _activeTab->getCurrentFileTreeView();

    // get the source files
    QStringList itemsToOperateOn;
    if( fileListView )
        fileListView->getSelectedItems( itemsToOperateOn );
    else
        fileTreeView->getSelectedItems( itemsToOperateOn );
    
    if( itemsToOperateOn.isEmpty() )
        return;

    QString sourceMessage = (itemsToOperateOn.count() > 1) ? 
						"\n" + itemsToOperateOn.value(0) +"\n"+ itemsToOperateOn.value(1) + "\n...\n" :
						itemsToOperateOn.value(0);

    QMessageBox::StandardButton userChoise = QMessageBox::question
                                                (
                                                    this,
                                                    "Delete Confirmation",
                                                    QString( "Target: %1").arg(sourceMessage),
                                                    QMessageBox::Yes | QMessageBox::No
                                                );
    if( QMessageBox::Yes == userChoise )
        _fileSystemOperations->removeObjects( itemsToOperateOn );
}

void Commander::focusChanged( QWidget* /*oldWidget*/, QWidget* nowWidget )
{
    FileListView* fileListViewNew = qobject_cast<FileListView*>( nowWidget );
    FileTreeView* fileTreeViewNew = qobject_cast<FileTreeView*>( nowWidget );

    if( !fileListViewNew && !fileTreeViewNew )
        return;

    if(
        ((_leftTabWidget->getCurrentFileListView() == fileListViewNew) && fileListViewNew)   ||
        ((_leftTabWidget->getCurrentFileTreeView() == fileTreeViewNew) && fileTreeViewNew)
      )
	{
        _activeTab = _leftTabWidget;
	}
    else
	{
        _activeTab = _rightTabWidget;
	}
}

void Commander::readConfiguration()
{
    ConfiguratorReader configuratorReader;
    QStringList stringList = configuratorReader.getCheckedCheckBoxes( "fileOperations/delete/trashOptions" );

    _fileSystemOperations->setDeleteToTrash( stringList.contains("deleteToTrash") );
    _fileSystemOperations->setTrashPath( configuratorReader.getFileSystemPath("fileOperations/delete/trashFolder") );
}

void Commander::insertPressed()
{
    _activeTab->getCurrentFileListView()->addToSelection( _activeTab->getCurrentFileListView()->currentIndex() );
    _activeTab->getCurrentFileListView()->moveCurrentIndexToNext();
}

void Commander::backspacePressed()
{
    QString path = _activeTab->getCurrentFileListView()->getCurrentFolder();
    _activeTab->getCurrentFileListView()->setCurrentFolder( path + "/.." );
}

void Commander::spacePressed()
{
    _activeTab->getCurrentFileListView()->addToSelection( _activeTab->getCurrentFileListView()->currentIndex() );
}

void Commander::numpadMinusPressed()
{
    qDebug( "FIXME: Commander::numpadMinusPressed()" );

/*
    FileList* fileList = qobject_cast<FileList*>( sender() );
    if( 0 == fileList )
        return;

    QPointer<AddRemoveSelection> addRemoveSelection = new AddRemoveSelection( false );
    addRemoveSelection->exec();

    if( false == addRemoveSelection->haveToDoIt() )
        return;

    QString criteria = addRemoveSelection->getCriteria();
    if( "*" == criteria )
    {
        if( addRemoveSelection->extendedOtionsAvailable() )
        {
            if( addRemoveSelection->haveToApplyOnlyToFiles() )
                fileList->removeFilesFromSelection();
            else
                fileList->removeFoldersFromSelection();
        }
        else
            fileList->selectNone();
    }
    else
        fileList->removeFromSelection( criteria );

    fileList->reflectSelection();
*/
}

void Commander::numpadPlusPressed()
{
    qDebug( "FIXME: Commander::numpadPlusPressed()" );

/*
    FileList* fileList = qobject_cast<FileList*>( sender() );
    if( 0 == fileList )
        return;

    QPointer<AddRemoveSelection> addRemoveSelection = new AddRemoveSelection( true );
    addRemoveSelection->exec();

    if( false == addRemoveSelection->haveToDoIt() )
        return;

    QString criteria = addRemoveSelection->getCriteria();
    if( "*" == criteria )
    {
        if( addRemoveSelection->extendedOtionsAvailable() )
        {
            if( addRemoveSelection->haveToApplyOnlyToFiles() )
                fileList->addFilesToSelection();
            else
                fileList->addFoldersToSelection();
        }
        else
            fileList->selectAll();
    }
    else
        fileList->addToSelection( criteria );

    fileList->reflectSelection();
*/
}


void Commander::contextMenuCut()
{
    FileListView* fileListView = _activeTab->getCurrentFileListView();
    FileTreeView* fileTreeView = _activeTab->getCurrentFileTreeView();

    // get the source files
    QStringList itemsToOperateOn;
    if( fileListView )
        fileListView->getSelectedItems( itemsToOperateOn );
    else
        fileTreeView->getSelectedItems( itemsToOperateOn );
    
    if( itemsToOperateOn.isEmpty() )
        return;

    // mark the objects
    _fileSystemOperations->markObjectsForCut( itemsToOperateOn );
}

void Commander::contextMenuCopy()
{
    FileListView* fileListView = _activeTab->getCurrentFileListView();
    FileTreeView* fileTreeView = _activeTab->getCurrentFileTreeView();

    // get the source files
    QStringList itemsToOperateOn;
    if( fileListView )
        fileListView->getSelectedItems( itemsToOperateOn );
    else
        fileTreeView->getSelectedItems( itemsToOperateOn );
    
    if( itemsToOperateOn.isEmpty() )
        return;

    // mark the objects
    _fileSystemOperations->markObjectsForCopy( itemsToOperateOn );
}

void Commander::contextMenuPaste()
{
    FileListView* fileListView = _activeTab->getCurrentFileListView();
    FileTreeView* fileTreeView = _activeTab->getCurrentFileTreeView();

    QString destinationFolder = fileListView ? fileListView->getCurrentFolder() : fileTreeView->getCurrentFolder();

    _fileSystemOperations->pasteObjects( destinationFolder );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Tabs
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Commander::setupTabs()
{
    _leftTabWidget = new TabWidget();
    _rightTabWidget = new TabWidget();

    QSplitter*  splitter = new QSplitter();
                splitter->addWidget( _leftTabWidget );
                splitter->addWidget( _rightTabWidget );

    setCentralWidget( splitter );

    _activeTab = _leftTabWidget;
    openNewTab();

    _activeTab = _rightTabWidget;
    openNewTab();

    _activeTab = _leftTabWidget;
}

