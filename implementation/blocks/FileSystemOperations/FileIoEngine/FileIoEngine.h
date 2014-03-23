#ifndef __FileIoEngine_H__
#define __FileIoEngine_H__

// Qt headers
#include <QtCore/QObject>
#include <QtCore/QStringList>

// local headers
#include "FileSystemOperations/OperationsModel/Operation/Operation.h"


class FileIoEngine : public QObject
{
    Q_OBJECT

    public:
        FileIoEngine();

    // signals that this object will emit
    signals:
        void newOperation(      OperationId operationId, OperationId parentOperationId );
        void updateOperation(   OperationId operationId );

    // implements the code that is actually doing what it says
    public:
        void internalCreateFile( OperationId operationId, OperationId parentOperationId );
        void internalCopyFile  ( OperationId operationId, OperationId parentOperationId );
        void internalRemoveFile( OperationId operationId, OperationId parentOperationId );
        void internalRenameFile( OperationId operationId, OperationId parentOperationId );
        void internalMoveFile  ( OperationId operationId, OperationId parentOperationId );

        void internalCreateFolder( OperationId operationId, OperationId parentOperationId );
        void internalCopyFolder  ( OperationId operationId, OperationId parentOperationId );
        void internalRemoveFolder( OperationId operationId, OperationId parentOperationId );
        void internalRenameFolder( OperationId operationId, OperationId parentOperationId );
        void internalMoveFolder  ( OperationId operationId, OperationId parentOperationId );

        void internalCreateSymbolicLink( OperationId operationId, OperationId parentOperationId );

        // this will copy/move the files and folders from "sourceObjects", at the end it will make a "delete sourceObjects;"
        void internalCopyMoveObjects( OperationId operationId, OperationId parentOperationId, bool haveToMove );
        void internalRemoveObjects  ( OperationId operationId, OperationId parentOperationId );

    // some internal data to work with
    public:
        qint64 _bufferSize;
};

#endif

