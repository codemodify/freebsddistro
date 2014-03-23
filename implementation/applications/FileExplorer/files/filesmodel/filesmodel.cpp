
// Qt's headers
#include <QtGui/QIcon>
#include <QtCore/QDir>
#include <QtCore/QMimeData>
#include <QtCore/QList>
#include <QtCore/QUrl>

// common's headers
#include <Logger>

// local headers
#include "filesmodel.h"

FilesModel::FilesModel( QString folder )
{
	setCurrentFolder( folder );
	setReadOnly();
}

FilesModel::~FilesModel()
{
}

int FilesModel::rowCount( const QModelIndex & parent ) const
{
	return stringList().count();
}

Qt::DropActions FilesModel::supportedDropActions () const
{
	return Qt::CopyAction | Qt::MoveAction;
}

QVariant FilesModel::data( const QModelIndex & index, int role ) const
{
    static QIcon folderIcon(QPixmap("res/folder.png"));
    static QIcon fileIcon(QPixmap("res/file.png"));

    // returning the icon to show
    if( role == Qt::DecorationRole )
        return m_fileInfoList[ index.row() ].isDir() ? qVariantFromValue( folderIcon ) : qVariantFromValue( fileIcon );
    
    // in case of edit/display_text of the item returning the correspoding text 
    if( role == Qt::DisplayRole || role == Qt::EditRole )
		return stringList()[ index.row() ];

    // in case of copy/status_bar_tip of the item returning the full path of the file/folder
    if( role == Qt::StatusTipRole )
		return m_currentFolder + "/" + stringList()[ index.row() ];
        
    return QVariant();
}

void FilesModel::setCurrentFolder( QString folder )
{
	// doing some checks 
	if( true  == folder.isEmpty() )
		return;
	
	QDir directory;
	directory.setPath( folder );
	
	QStringList folderContent = directory.entryList();
	
	// removing the "."
	folderContent.removeAt(0);
	
	setStringList( folderContent );
	m_fileInfoList = directory.entryInfoList();
	
	// removing info for "."
	m_fileInfoList.removeAt(0);
	

	// storing the folder beeing displaied
	m_currentFolder = directory.absolutePath();

}

Qt::ItemFlags FilesModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags defaultFlags = QStringListModel::flags(index);
	
	if (index.isValid())
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
	else
		return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList FilesModel::mimeTypes() const
{
	return QStringList(QLatin1String(MIMETYPE));
}

QMimeData* FilesModel::mimeData ( const QModelIndexList & indexes ) const
{
	// the list of selected items
	QList<QUrl> urls;
	
	foreach( QModelIndex index, indexes )
	{
		if( index.isValid() )
		{
			QString filePath = data(index, Qt::StatusTipRole).toString();
			urls << QUrl::fromLocalFile( filePath );
		}
	}
	
	QMimeData* mimeData = new QMimeData();
	mimeData->setUrls( urls );
	
	return mimeData;	
}

QString FilesModel::getCurrentFolder()
{
	return m_currentFolder;
}

void FilesModel::setReadOnly( bool readOnly )
{
	m_readOnly = readOnly;
}

QIcon FilesModel::fileIcon( const QModelIndex& index ) const
{
	return QIcon();
}

QFileInfo FilesModel::fileInfo( const QModelIndex& index ) const
{
	return QFileInfo();
}

QString FilesModel::fileName( const QModelIndex& index ) const
{
	return QString();
}

QString FilesModel::filePath( const QModelIndex& index ) const
{
	return QString();
}

bool	 FilesModel::isFolder( const QModelIndex& index ) const
{
	return true;
}

QModelIndex	FilesModel::createFolder( const QString& folderName )
{
	return QModelIndex();
}

QModelIndex	FilesModel::createFile( const QString& fileName )
{
	return QModelIndex();
}

bool	 FilesModel::removeFolderOrFile( const QModelIndex& index )
{
	return true;
}
