#ifndef _FILESMODEL_H_
#define _FILESMODEL_H_

#include <QtGui/QStringListModel>
#include <QtCore/QFileInfoList>
#include <QtCore/QVariant>

#define MIMETYPE "text/uri-list"

/*
 * The "FilesModel" class is designed to be an analog for the "QDirModel" class from the Qt4 toolkit,
 * with the focus on one directory, not the whole file system as "QDirModel" does.
 * The difference: this model can be used as a model for the ListViews.
 * 
 * The main purpose for the class is to provide copy/delete/rename operations on the directory that it is reflecting.
 * 
 * It is based on the "QStringListModel", an amount of methods are reimplemented from the base class,
 * the other ones are implementing the file-operations as was mentioned.
 *  
 */

class FilesModel : public QStringListModel
{

public:

	// the Constructor / Destructor
	FilesModel( QString folder = "" );
	~FilesModel();


public:
	//
	// The public interface of this class in adition to the "QStringListModel"
	//

	// returns the current folder the content of which the model is reflecting currently
	QString getCurrentFolder();

	// sets the model's data, after this call the model is filled with new data
	void setCurrentFolder( QString folder );

	// if set to read-only, the model is not making any changes in file-system
	void setReadOnly( bool readOnly = true );

	// file operations
	QIcon		fileIcon( const QModelIndex& index ) const;
	QFileInfo	fileInfo( const QModelIndex& index ) const;
	QString		fileName( const QModelIndex& index ) const;
	QString		filePath( const QModelIndex& index ) const;

	bool			isFolder( const QModelIndex& index ) const;

	QModelIndex	createFolder( const QString& folderName );
	QModelIndex	createFile( const QString& fileName );
	bool			removeFolderOrFile( const QModelIndex& index );

public:
	//
	// The reimplemnented methods from the base class
	//

	// returns the number of items in the list
	int rowCount( const QModelIndex& parent = QModelIndex() ) const;

	// returns the data stored under the given role for the item referred to by the index
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

	// enabling the drag and dropping for items
	Qt::ItemFlags flags( const QModelIndex& index ) const;

	// returning the supported drop acctions
	Qt::DropActions supportedDropActions () const;

	// exporting the dragged items from 'us'
	QStringList mimeTypes() const;
	QMimeData* mimeData ( const QModelIndexList & indexes ) const;

	// importing the dragged items


private:
	//
	// The internal stuff used only by this class
	//

	// keeping the information about files/folder beeing displaied
	QFileInfoList m_fileInfoList;

	// stores the path on file system for the current folder we're working with
	QString m_currentFolder;

	// stores the state for the model, by allowing or not the File-System changes
	bool m_readOnly;
};

#endif
