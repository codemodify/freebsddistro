#ifndef __Operation_H__
#define __Operation_H__

// Qt headers
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QModelIndex>
#include <QtCore/QPersistentModelIndex>

#define NoParentOperation 0
#define NoOperation 0
#define isOperationValid(operation) (NoOperation!=operation)


typedef enum
{
    StatusStart = 0,

    Success=0, Error, Running, Stopped, Prepared,

    ErrorExistFileSource,
    ErrorExistFileDestination,      ErrorWriteFileDestination,

    ErrorExistFolderSource,
    ErrorExistFolderDestination,

    ErrorCreateFileSource,          ErrorOpenFileSource,        ErrorDeleteFileSource,
    ErrorCreateFileDestination,     ErrorOpenFileDestination,   ErrorDeleteFileDestination,

    ErrorCreateFolderSource,        ErrorOpenFolderSource,      ErrorDeleteFolderSource,
    ErrorCreateFolderDestination,   ErrorOpenFolderDestination, ErrorDeleteFolderDestination,

    StatusEnd

} OperationStatus;


class Operation
{
    public:
        Operation();
        ~Operation();


    // some utility functions
    public:
        void        appendChild ( Operation* child  );
        Operation*  child       ( int row           );
        int         childCount  (                   ) const;
        int         columnCount (                   ) const;
        QVariant    data        ( int column        ) const;
        int         row         (                   ) const;
        Operation*  parent      (                   );
        QVariant    columnHeaderValue( int columnHeaderIndex );


    // ramifications
    public:
        QPersistentModelIndex   _modelIndex;
        Operation*              _parentItem;
        QList<Operation*>       _childItems;


    // internal data
    public:
        OperationStatus _status;            // column 0
        QString         _operationName;     // column 1
        qint64          _totalSize;         // column 2
        int             _percentProgress;   // column 3
        QString         _source;            // column 4
        QString         _destination;       // column 5

        QString         _sourcePath;
        QString         _destinationPath;

        QStringList*    _sourceObjects;

        void initAllToZero();
};

typedef Operation* OperationId;


#endif

