
// Qt headers
#include <QtCore/QModelIndex>


// local headers
#include "OperationsModel.h"
//#include "modeltest/modeltest.h"


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- constructor area
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
OperationsModel::OperationsModel() :
    QAbstractItemModel()
{
    _rootItem = new Operation();
    _rootItem->_modelIndex = QPersistentModelIndex( QModelIndex() );

    connect( this, SIGNAL(newOperationRequest(OperationId,OperationId)), this, SLOT(newOperationHandler(OperationId,OperationId)) );

    // new ModelTest( this, this );
}

OperationsModel::~OperationsModel()
{
    delete _rootItem;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- query for operation status
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
OperationStatus OperationsModel::getOperationStatus( OperationId operationId )
{
    return operationId->_status;
}





// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- the file-operation notifications handlers
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void OperationsModel::newOperationHandler( OperationId operationId, OperationId parentOperationId )
{
    // choosing the parent to work with
    Operation* theParent = isOperationValid(parentOperationId) ? parentOperationId : _rootItem;

    beginInsertRows( theParent->_modelIndex, theParent->childCount(), theParent->childCount()+1  );
    {
        theParent->appendChild( operationId );
        operationId->_parentItem = theParent; // doing the links between parent<-->child
        operationId->_modelIndex = QPersistentModelIndex( createIndex(operationId->row(),0,operationId) );
    }
    endInsertRows();
}

void OperationsModel::addNewOperation( OperationId operationId, OperationId parentOperationId )
{
    emit newOperationRequest(operationId,parentOperationId);
}

void OperationsModel::updateOperation( OperationId operationId )
{
    int row     = operationId->_modelIndex.row();
    int column  = operationId->_modelIndex.column();

    QModelIndex previous    = operationId->_modelIndex.sibling( row-1, column );
    QModelIndex current     = operationId->_modelIndex;
    QModelIndex next        = operationId->_modelIndex.sibling( row+1, column );

    emit dataChanged( previous, next );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- size
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
int OperationsModel::columnCount( const QModelIndex& parent ) const
{
     if( parent.isValid() )
         return static_cast<Operation*>(parent.internalPointer())->columnCount();
     else
         return _rootItem->columnCount();
}

int OperationsModel::rowCount( const QModelIndex& parent ) const
{
    Operation *parentItem;
     if( parent.column() > 0 )
         return 0;

     if( !parent.isValid() )
         parentItem = _rootItem;
     else
         parentItem = static_cast<Operation*>( parent.internalPointer() );

     return parentItem->childCount();
}

bool OperationsModel::hasChildren( const QModelIndex& parent ) const
{
    return rowCount( parent );
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- item mapping
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
QModelIndex OperationsModel::index( int row, int column, const QModelIndex& parent ) const
{
     if( !hasIndex(row,column,parent) )
         return QModelIndex();

     Operation *parentItem;

     if( !parent.isValid() )
         parentItem = _rootItem;
     else
         parentItem = static_cast<Operation*>( parent.internalPointer() );

     Operation *childItem = parentItem->child( row );
     if( childItem )
         return createIndex( row, column, childItem );
     else
         return QModelIndex();
}

QModelIndex OperationsModel::parent( const QModelIndex& index ) const
{
     if( !index.isValid() )
         return QModelIndex();

     Operation *childItem = static_cast<Operation*>( index.internalPointer() );
     Operation *parentItem = childItem->parent();

     if( parentItem == _rootItem )
         return QModelIndex();

     return createIndex( parentItem->row(), 0, parentItem );
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- data access
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
Qt::ItemFlags OperationsModel::flags( const QModelIndex& index ) const
{
    if( !index.isValid() )
        return Qt::NoItemFlags;

    return ( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
}

QVariant OperationsModel::data( const QModelIndex& index, int role ) const
{
     if( !index.isValid() )
         return QVariant();

     if( role != Qt::DisplayRole )
         return QVariant();

     Operation *item = static_cast<Operation*>( index.internalPointer() );

     return item->data( index.column() );
}

QVariant OperationsModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
     if( (orientation == Qt::Horizontal) && (role == Qt::DisplayRole) )
         return _rootItem->columnHeaderValue( section );

     return QVariant();
}
