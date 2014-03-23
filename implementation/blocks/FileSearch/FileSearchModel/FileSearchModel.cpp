
// Qt headers
#include <QtCore/QModelIndex>

// blocks
#include <IconProvider/FileSystemIconProvider/FileSystemIconProvider.h>

// local headers
#include "FileSearchModel.h"



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- constructor area
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
FileSearchModel::FileSearchModel() :
    QAbstractListModel()
{
    _iconProvider = new FileSystemIconProvider();
}

FileSearchModel::~FileSearchModel()
{
    delete _iconProvider;
}

void FileSearchModel::clear()
{
	_items.clear();
	reset();
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- notification handlers
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FileSearchModel::itemAvailable( QString item )
{
	beginInsertRows( QModelIndex(), _items.size(), _items.size()+1 );
	{
		_items.append( item );
	}
	endInsertRows();
}

// void FileSearchModel::itemStatusChange( QString item, FileSearchModel::SearchStatus searchStatus )
// {
//     int row     = _items.indexOf( item );
//     int column  = 0;
// 
//     switch( searchStatus )
//     {
//         case eGrepping      : _status[row] = "GREPPING";      break;
//         case eDataFound     : _status[row] = "DATAFOUND";     break;
//         case eNoDataFound   : _status[row] = "NODATAFOUND";   break;
//         default             : _status[row] = "";              break;
//     }
// 
//     QModelIndex previous    = QModelIndex().child( row-1, column );
//     QModelIndex current     = QModelIndex().child( row  , column );
//     QModelIndex next        = QModelIndex().child( row+1, column );
// 
//     emit dataChanged( previous, next );
// }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- we need to implement these to actually implement a model
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
int FileSearchModel::rowCount( const QModelIndex& parent ) const
{
    if( parent.isValid() )
        return 0;
    else
        return _items.size();
}

QVariant FileSearchModel::data( const QModelIndex& index, int role ) const
{
    if( !index.isValid() )
        return QVariant();

    if( role == Qt::DecorationRole )
        return _iconProvider->getIconForFile( _items.value(index.row()) );

    else
    if( role == Qt::DisplayRole )
        return _items.value( index.row() );

    return QVariant();
}

// QVariant FileSearchModel::headerData( int section, Qt::Orientation orientation, int role ) const
// {
//     if( (orientation == Qt::Horizontal) && (role == Qt::DisplayRole) )
//     {
//         switch( section )
//         {
//             case 0: return "Status";
//             case 1: return "";
//             case 2: return "Path";
//         }
//     }
// 
//     return QVariant();
// }
