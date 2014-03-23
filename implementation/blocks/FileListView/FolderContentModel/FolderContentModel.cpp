
// Qt headers
#include <QtCore/QMutexLocker>
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QMimeData>

// blocks
#include <IconProvider/FileSystemIconProvider/FileSystemIconProvider.h>

// local headers
#include "FolderContentModel.h"



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- constructor area
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
FolderContentModel::FolderContentModel() :
    QAbstractListModel(),
    _iconProvider( 0 ),
    _hiddenVisible( true )
{
    _iconProvider = new FileSystemIconProvider();

    _dir = new QDir();
}

FolderContentModel::~FolderContentModel()
{
    delete _iconProvider;
    delete _dir;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- current folder
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FolderContentModel::setCurrentFolder( QString currentFolder )
{
    _dir->cd( currentFolder );

    QDir::Filters typeFilters = QDir::AllDirs | QDir::Files | QDir::System | QDir::CaseSensitive;// | QDir::NoDotAndDotDot;
    if( _hiddenVisible )
        typeFilters = typeFilters | QDir::Hidden;

    QDir::SortFlags sortFlags = QDir::Name | QDir::DirsFirst;

    _folderContent.clear();
    
    // list is empty if the directory is: unreadable, does not exist, or if nothing matches the specification
    _folderContent = _dir->entryList( typeFilters, sortFlags );
    if( _folderContent.isEmpty() )
    {
        _dir->cd( currentFolder + "/.." );
        _folderContent = _dir->entryList( typeFilters, sortFlags );
    }
    _folderContent.removeAll( "."  );
    _folderContent.removeAll( ".." );
    _folderContent.insert( 0, ".." );

    _selectedItems.clear();
    for( int index = 0; index <_folderContent.size(); ++index )
        _selectedItems.append( false );

    // inform the views to update the UI
    reset();
}

QString FolderContentModel::getCurrentFolder()
{
    return _dir->absolutePath();
}

QString FolderContentModel::getCurrentFolder() const
{
    return _dir->absolutePath();
}

void FolderContentModel::refreshContent()
{
    setCurrentFolder( getCurrentFolder() );
}

void FolderContentModel::setHiddenVisible( bool visible )
{
    _hiddenVisible = visible;
}

void FolderContentModel::getContent( QStringList& content )
{
    content = _folderContent;

    // remove the ".."
    if( !content.isEmpty() )
        content.removeFirst();
}

void FolderContentModel::setContent( QStringList& content )
{
    _folderContent.clear();

    _folderContent.append( ".." );
    _folderContent << content;

    _selectedItems.clear();
    for( int index = 0; index <_folderContent.size(); ++index )
        _selectedItems.append( false );

    reset();
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- selection
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FolderContentModel::selectAll()
{
    // this thing here will select everything
    for( int i = 1; i < _selectedItems.count(); ++i )
        _selectedItems[ i ] = true;

    // inform the view that the data changed
    QModelIndex first = index( 0 );
    QModelIndex last  = index( _selectedItems.count() );

    emit dataChanged( first, last );
}

void FolderContentModel::selectNone()
{
    // this thing here will un-select everything
    for( int i = 1; i < _selectedItems.count(); ++i )
        _selectedItems[ i ] = false;

    // inform the view that the data changed
    QModelIndex first = index( 0 );
    QModelIndex last  = index( _selectedItems.count() );

    emit dataChanged( first, last );
}

void FolderContentModel::addFoldersToSelection()
{
    // this thing here will select folders, everything else remains unchanged
    for( int i = 1; i < _folderContent.count(); ++i )
    {
        QString fullItemPath = QString( "%1/%2" ).arg( getCurrentFolder() ).arg( _folderContent.at(i) );
        if( QFileInfo(fullItemPath).isDir() )
            _selectedItems[ i ] = true;
    }

    // inform the view that the data changed
    QModelIndex first = index( 0 );
    QModelIndex last  = index( _selectedItems.count() );

    emit dataChanged( first, last );
}

void FolderContentModel::removeFoldersFromSelection()
{
    // this thing here will un-select folders, everything else remains unchanged
    for( int i = 1; i < _folderContent.count(); ++i )
    {
        QString fullItemPath = QString( "%1/%2" ).arg( getCurrentFolder() ).arg( _folderContent.at(i) );
        if( QFileInfo(fullItemPath).isDir() )
            _selectedItems[ i ] = false;
    }

    // inform the view that the data changed
    QModelIndex first = index( 0 );
    QModelIndex last  = index( _selectedItems.count() );

    emit dataChanged( first, last );
}

void FolderContentModel::addFilesToSelection()
{
    // this thing here will select files, everything else remains unchanged
    for( int i = 1; i < _folderContent.count(); ++i )
    {
        QString fullItemPath = QString( "%1/%2" ).arg( getCurrentFolder() ).arg( _folderContent.at(i) );
        if( !QFileInfo( fullItemPath ).isDir() )
            _selectedItems[ i ] = true;
    }

    // inform the view that the data changed
    QModelIndex first = index( 0 );
    QModelIndex last  = index( _selectedItems.count() );

    emit dataChanged( first, last );
}

void FolderContentModel::removeFilesFromSelection()
{
    // this thing here will select files, everything else remains unchanged
    for( int i = 1; i < _folderContent.count(); ++i )
    {
        QString fullItemPath = QString( "%1/%2" ).arg( getCurrentFolder() ).arg( _folderContent.at(i) );
        if( !QFileInfo( fullItemPath ).isDir() )
            _selectedItems[ i ] = false;
    }

    // inform the view that the data changed
    QModelIndex first = index( 0 );
    QModelIndex last  = index( _selectedItems.count() );

    emit dataChanged( first, last );
}

void FolderContentModel::addToSelection( QString criteria )
{
    for( int i = 1; i < _folderContent.count(); ++i )
    {
        if( _folderContent.at(i).contains( QRegExp(criteria) ) )
            _selectedItems[ i ] = true;
    }

    // inform the view that the data changed
    QModelIndex first = index( 0 );
    QModelIndex last  = index( _selectedItems.count() );

    emit dataChanged( first, last );
}

void FolderContentModel::removeFromSelection( QString criteria )
{
    for( int i = 1; i < _folderContent.count(); ++i )
    {
        if( _folderContent.at(i).contains( QRegExp(criteria) ) )
            _selectedItems[ i ] = false;
    }

    // inform the view that the data changed
    QModelIndex first = index( 0 );
    QModelIndex last  = index( _selectedItems.count() );

    emit dataChanged( first, last );
}

void FolderContentModel::addToSelection( const QModelIndex& mdoelIndex )
{
    if( !mdoelIndex.isValid() || !mdoelIndex.row() )
        return;

    _selectedItems[ mdoelIndex.row() ] = !_selectedItems[ mdoelIndex.row() ];

    // inform the view that the data changed
    QModelIndex first = index( 0 );
    QModelIndex last  = index( _selectedItems.count() );

    emit dataChanged( first, last );
}

void FolderContentModel::invertSelection()
{
    for( int i = 1; i < _selectedItems.count(); ++i )
        _selectedItems[ i ] = !_selectedItems.at( i );

    // inform the view that the data changed
    QModelIndex first = index( 0 );
    QModelIndex last  = index( _selectedItems.count() );

    emit dataChanged( first, last );
}

void FolderContentModel::getSelectedItems( QStringList& selectedItems )
{
    for( int i = 1; i < _folderContent.count(); ++i )
    {
        if( _selectedItems[i] )
            selectedItems.append( QString("%1/%2").arg(getCurrentFolder()).arg(_folderContent.at(i)) );
    }
}

bool FolderContentModel::isItemSelected( const QModelIndex& modelIndex ) const
{
    if( !modelIndex.isValid() )
        return false;

    return _selectedItems[ modelIndex.row() ];
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// we need to implement this to allow views to extract data from this model
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
int FolderContentModel::rowCount( const QModelIndex& parentModelIndex ) const
{
    if( parentModelIndex.isValid() )
        return 0;

    return _folderContent.count();
}

QVariant FolderContentModel::data( const QModelIndex& modelIndex, int role ) const
{
    if( !modelIndex.isValid() )
        return QVariant();

    int rowIndex = modelIndex.row();

    if( Qt::DecorationRole == role )
    {
        if( _iconProvider )
        {
            QString itemFullPath    = QString( "%1/%2" ).arg( getCurrentFolder() ).arg( _folderContent.at(rowIndex) );

            return _iconProvider->getIconForFile( itemFullPath );
        }
        else
            return QIcon();
    }
    else if( Qt::DisplayRole  == role )
        return _folderContent.at( rowIndex );

    else if( SelectedRole == role )
        return _selectedItems.at(rowIndex) ? QString("true"):QString("false");

    return QVariant();
}

Qt::ItemFlags FolderContentModel::flags( const QModelIndex& modelIndex ) const
{
    if( modelIndex.isValid() )
    {
        int index = modelIndex.row();

        // in case the drag is over a folder
        if( QFileInfo(_folderContent.at(index)).isDir() )
            return ( Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsDropEnabled );

        // in case the drag is over a file
        else
            return ( Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled );
    }

    // the drag is over the parent folder
    return Qt::ItemIsDropEnabled;
}

Qt::DropActions FolderContentModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions FolderContentModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QMimeData* FolderContentModel::mimeData( const QModelIndexList& modelIndexes ) const
{
    QList<QUrl> urls;

    foreach( QModelIndex modelIndex, modelIndexes )
    {
        if( !modelIndex.isValid() )
            continue;

        urls << QUrl::fromLocalFile( getCurrentFolder() + "/" + _folderContent.at(modelIndex.row()) );
    }

    QMimeData*  mimeData = new QMimeData();
                mimeData->setUrls( urls );

    return mimeData;
}

QStringList FolderContentModel::mimeTypes() const
{
    QStringList supportedMimeTypes;
                supportedMimeTypes << "text/uri-list";

    return supportedMimeTypes;
}

bool FolderContentModel::dropMimeData( const QMimeData* mimeData, Qt::DropAction /*action*/, int row, int /*column*/, const QModelIndex& /*parentModelIndex*/ )
{
    // here we don't care about the: 'parentModelIndex', 'column', 'action'

    if( !mimeData->hasUrls() )
        return false;

    QString targetFolder = getCurrentFolder();

    // checking if the drop action is wanted into a folder
    QString fullItemPath = QString( "%1/%2" ).arg( getCurrentFolder() ).arg( _folderContent.value(row) );
    if( (-1!=row) && QFileInfo(fullItemPath).isDir() )
        targetFolder = fullItemPath;

    QStringList* sourceItems = new QStringList();
    foreach( QUrl url, mimeData->urls() )
        sourceItems->append( url.toLocalFile() );

    emit itemsDropped( sourceItems, targetFolder );

    return true;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- utility functions
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
bool FolderContentModel::isItemFolder( const QModelIndex& modelIndex ) const
{
    if( !modelIndex.isValid() )
        return false;

    QString fullItemPath = QString( "%1/%2" ).arg( getCurrentFolder() ).arg( _folderContent.at(modelIndex.row()) );

    return QFileInfo( fullItemPath ).isDir();
}

QString FolderContentModel::getItemPath( const QModelIndex& modelIndex )
{
    if( !modelIndex.isValid() )
        return false;

    return QString( "%1/%2" ).arg( getCurrentFolder() ).arg( _folderContent.at(modelIndex.row()) );
}
