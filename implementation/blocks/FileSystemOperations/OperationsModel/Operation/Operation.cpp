
// local headers
#include "Operation.h"

static const char* OperationStatusName[StatusEnd] = 
{
    "Success", "Error", "Running", "Stopped", "Prepared",

    "ErrorExistFileSource",
    "ErrorExistFileDestination",    "ErrorWriteFileDestination",
    
    "ErrorExistFolderSource",
    "ErrorExistFolderDestination",

    "ErrorCreateFileSource",        "ErrorOpenFileSource",          "ErrorDeleteFileSource",
    "ErrorCreateFileDestination",   "ErrorOpenFileDestination",     "ErrorDeleteFileDestination",
    
    "ErrorCreateFolderSource",      "ErrorOpenFolderSource",        "ErrorDeleteFolderSource",
    "ErrorCreateFolderDestination", "ErrorOpenFolderDestination",   "ErrorDeleteFolderDestination"    
};

Operation::Operation()
{
    // mark the "OperationId" as valid to be used in signal/slot as parameter
    qRegisterMetaType<OperationId>( "OperationId" );

    initAllToZero();
}

Operation::~Operation()
{
    foreach( Operation* operation, _childItems )
        delete operation;

    _childItems.clear();
}

void Operation::initAllToZero()
{
    _parentItem = 0;

    // data
    _status             = Prepared;
    _operationName      = "";
    _totalSize          = 0;
    _percentProgress    = 0;
    _source             = "";
    _destination        = "";

    _sourcePath         = "";
    _destinationPath    = "";

    _sourceObjects      = 0;
}

void        Operation::appendChild  ( Operation* child  )       {           _childItems.append( child );    }
Operation*  Operation::child        ( int row           )       { return    _childItems.value( row );       }
int         Operation::childCount   (                   )const  { return    _childItems.count();            }
int         Operation::columnCount  (                   )const  { return    6;                              }
Operation*  Operation::parent       (                   )       { return    _parentItem;                    }
QVariant    Operation::data         ( int column        )const
{
    switch( column )
    {
        case 0: return OperationStatusName[ _status ];
        case 1: return _operationName;
        case 2:
        {
            qint64  bytes   = _totalSize;
            qint64 kBytes   = _totalSize / ( 1024                      );
            qint64 mBytes   = _totalSize / ( 1024 * 1024               );
            qint64 gBytes   = _totalSize / ( 1024 * 1024 * 1024        );
            qint64 tBytes   = _totalSize / ( 1024 * 1024 * 1024 * 1024 );

            return  tBytes ? QString( "%1 T" ).arg( tBytes ) :
                    (
                        gBytes ? QString( "%1 G" ).arg( gBytes ) :
                        (
                            mBytes ? QString( "%1 M" ).arg( mBytes ) :
                            (
                                kBytes ? QString( "%1 K" ).arg( kBytes ) :
                                (
                                    QString( "%1 B" ).arg( bytes )
                                )
                            )
                        )
                    );
        }
        case 3: return ( _percentProgress ? QString("%1 %").arg(_percentProgress) : QString() );
        case 4: return _source;
        case 5: return _destination;
    }

    return QVariant();
}

int Operation::row() const
{
    return _parentItem ? _parentItem->_childItems.indexOf( const_cast<Operation*>(this) ) : 0;
}

QVariant Operation::columnHeaderValue( int columnHeaderIndex )
{
    if( 0 == columnHeaderIndex )
        return "status";

    else if( 1 == columnHeaderIndex )
        return "operation";

    else if( 2 == columnHeaderIndex )
        return "total size";

    else if( 3 == columnHeaderIndex )
            return "progress";

    else if( 4 == columnHeaderIndex )
            return "source";

    else if( 5 == columnHeaderIndex )
            return "destination";

    return QVariant();
}
