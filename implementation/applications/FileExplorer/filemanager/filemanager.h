#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

// Qt's headers
#include <QtGui/QDirModel>
#include <QtGui/QSplitter>
#include <QtGui/QGridLayout>
#include <QtGui/QMenu>

// local headers
#include "ui_filemanager.h"
#include "../files/filesmodel/filesmodel.h"
#include "../files/filesview/filesview.h"


/*
 * 
 * This class is the main start point for all the "Filemanager application"
 * 
 */
 
class Filemanager : public QMainWindow, public  Ui::Filemanager
{
	// using Qt's signal/slot framework
	Q_OBJECT
	
public:
	Filemanager();
	~Filemanager();
	
	// prepares the startup environement for the object
	void initEnvironement();

public slots:

	// handling events from copy-manager
	void copyManagerFinished();
	void copyManagerHasErrors( QString error );

private slots:

	// handling the width-change of folders-tree when expanded/colapsed
	void foldersTreeChangedWidth();
	
	// handles the mouse click button, on a folder from the tree
	void folderClick( const QModelIndex & index );
	void folderRightClick( const QPoint & );
	
	// handles the 'mouse-clicks' / 'keyboard-presses' on files
	void filesRightClick( const QPoint & );
	void filesDrop( QDropEvent * event );
	void filesDoubleClicked( const QModelIndex& index );
	
	void filesKeyPress( QKeyEvent * event );

private slots:

	// file operations
	void filesDelete();
	void filesCut();
	void filesCopy();
	void filesPaste();
	void filesExecute();
	
private:
	
	// stores the current folder the model displays
	QString m_currentFolder;
	
	// the hierachycal model of the file system
	QDirModel* m_foldersModel;
	
	// the model for a list of files
	FilesModel* m_filesModel;
	
	// the splitter
	QSplitter* m_splitter;
	
	FilesView* m_files;
	QGridLayout* m_gridLayout;
	
	// setting-up the folders
	void setupFolders();
	
	// setting up the files
	void setupFiles();

	// setting up the store-panel
	void setupStorePanel();
	
	// setting up the trash
	void setupTrash();
	
	// changes the current folder to the specified one
	void changeDirectory( QString folderPath );
	
	// refreshes the content for current folder	
	void refreshCurrentFolder();
	
	// context menu for files
	QMenu m_filesRightClickMenu;
	
};

#endif
