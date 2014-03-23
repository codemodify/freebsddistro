
// Qt
#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtCore/QFileInfo>


// local
#include "FileTreeModel.h"

FileTreeModel::FileTreeModel() :
    QDirModel()
{
    setReadOnly( false );
    setSorting( QDir::Name | QDir::DirsFirst );
}

FileTreeModel::~FileTreeModel()
{}

int FileTreeModel::columnCount( const QModelIndex& parent ) const
{
    if( parent.column() > 0 )
        return 0;
        
    return 1;
}

bool FileTreeModel::dropMimeData(   const QMimeData* mimeData, Qt::DropAction action, 
                                    int row, int column, const QModelIndex& parent )
{
    if( !mimeData->hasUrls() )
        return false;

    QString targetFolder = filePath( parent );
    
    QStringList* items = new QStringList();
    foreach( QUrl url, mimeData->urls() )
        items->append( url.toLocalFile() );

    emit itemsDropped( items, targetFolder );
}

QIcon               FileTreeModel::fileIcon ( const QModelIndex& index ) const{ return QDirModel::fileIcon(index); }
QFileInfo           FileTreeModel::fileInfo ( const QModelIndex& index ) const{ return QDirModel::fileInfo(index); }
QString             FileTreeModel::fileName ( const QModelIndex& index ) const{ return QDirModel::fileName(index); }
QString             FileTreeModel::filePath ( const QModelIndex& index ) const{ return QDirModel::filePath(index); }

QModelIndex         FileTreeModel::mkdir    ( const QModelIndex& parent, const QString& name )  { return QDirModel::mkdir(parent,name); }
bool                FileTreeModel::remove   ( const QModelIndex& index )                        { return QDirModel::remove(index);      }
bool                FileTreeModel::rmdir    ( const QModelIndex& index )                        { return QDirModel::rmdir(index);       }

QFileIconProvider*  FileTreeModel::iconProvider () const        { return QDirModel::iconProvider();         }
bool                FileTreeModel::isReadOnly   () const        { return QDirModel::isReadOnly();           }
void                FileTreeModel::setReadOnly  ( bool enable ) {        QDirModel::setReadOnly(enable);    }

