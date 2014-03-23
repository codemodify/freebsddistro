#ifndef __FileSystemOperations_H__
#define __FileSystemOperations_H__


// Qt headers
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QStringList>

// local headers
#include <FileSystemOperations/OperationsModel/OperationsModel.h>


// defines a delegate to be called when an operations ends
typedef void (QObject::*OperatoinDelegate)( OperationId operationId );
typedef QPair< QObject*, OperatoinDelegate > OperatoinFinishedDelegate;


// forwards
class FileIoEngine;
class OperationsModel;
class ThreadedOperation;


/*********************************************************************************************
*
*   This class defines an interface to operations on files and folders.
*
*
*   Usage:
*       FileSystemOperations fileSystemOperations;
*
*
*       // create one file
*       fileSystemOperations.createFile( "/home/user/my-file.text" );
*
*
*       // copy in parallel  (A),(B),(C)  then  (D),(E)
*       fileSystemOperations.setThreadingModel( FileSystemOperations::OneThreadPerOperation );
*       fileSystemOperations.setMaxThreadsLimit( 3 );
*       fileSystemOperations.copyFolder( "/home/user/my-folder", "/home/user/my-folder-duplicate1" ); // (A)
*       fileSystemOperations.copyFolder( "/home/user/my-folder", "/home/user/my-folder-duplicate2" ); // (B)
*       fileSystemOperations.copyFolder( "/home/user/my-folder", "/home/user/my-folder-duplicate3" ); // (C)
*       fileSystemOperations.copyFolder( "/home/user/my-folder", "/home/user/my-folder-duplicate4" ); // (D)
*       fileSystemOperations.copyFolder( "/home/user/my-folder", "/home/user/my-folder-duplicate5" ); // (E)
*
*
*       // get notified when an operation ends, regardless of its status
*       FileSystemOperations::OperationId operationId = fileSystemOperations.copyFolder
*                                                   (
*                                                       "/home/user/my-folder",
*                                                       "/home/user/my-folder-duplicate",
*                                                       QPair( this, &MyQObjectDerivedClass::methodToCall )
*                                                   );
*
*       // check for the operation status
*       FileSystemOperations::OperationId       operationId     = fileSystemOperations.createFile( "/home/user/my-file.text" );
*       FileSystemOperations::OperationStatus   operationStatus = fileSystemOperations.getOperationStatus( operationId );
*
*
*********************************************************************************************/
class FileSystemOperations : public QObject
{
    Q_OBJECT

    // constructor area
    public:
        FileSystemOperations();
        ~FileSystemOperations();



    // allows to move files to trash and not to destroy them
    public:
        void setDeleteToTrash( bool deleteToTrash );
        void setTrashPath( QString trashPath );
    private:
        bool    _deleteToTrash;
        QString _trashPath;



    // instructs to auto delete when done
    public:
        void setDestroyOnFinish( bool destroyOnFinish );
    private:
        bool _destroyOnFinish;



    // allows to set the thread limit
    public:
        void setMaxThreadsLimit( int threadLimit );
    private:
        QList< ThreadedOperation* > _currentThreads;
        int                         _threadLimit;



    // implements a delegating mechanism to notify when the operation ends
    private:
        void makeSureThreadsGetTheirTurn();
        void runThreadAndNotifyWhenEnds( ThreadedOperation* threadedOperation, OperationId operationId, OperatoinFinishedDelegate operatoinFinishedDelegate );
    private:
        QMap< OperationId, OperatoinFinishedDelegate > _callBacksToCallWhenOperationsEnd;
    private slots:
        void slot_threadFinished();



    // queries the result for an operation
    public:
        OperationStatus getOperationStatus( OperationId operationId );



    // allows to change at run time the buffer size for file IO operations
    public:
        typedef enum{ OneKilobyte=1024, OneMegabyte=(1024*1024) } BufferSize;

        void setIoBufferSize( qint64 bufferSize );



    // returns the model of the current operations, useful if to attach a UI to the model
    public:
        OperationsModel* getOperationsModel();
    private:
        OperationsModel* _operationsModel;



    // operations allowed
    public:
        OperationId createFile          ( QString filePath,                                                 OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId copyFile            ( QString sourceFilePath,       QString destinationFilePath,        OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId removeFile          ( QString filePath,                                                 OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId trashFile           ( QString sourceFilePath,       QString destinationFilePath,        OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId renameFile          ( QString filePath,             QString newName,                    OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId moveFile            ( QString sourceFilePath,       QString destinationFilePath,        OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );

        OperationId createFolder        ( QString folderPath,                                               OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId copyFolder          ( QString sourceFolderPath,     QString intoDestinationFolderPath,  OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId copyFolderContent   ( QString sourceFolderPath,     QString intoDestinationFolderPath,  OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId removeFolder        ( QString folderPath,                                               OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId trashFolder         ( QString sourceFolderPath,     QString intoDestinationFolderPath,  OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId renameFolder        ( QString folderPath,           QString newName,                    OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId moveFolder          ( QString sourceFolderPath,     QString intoDestinationFolderPath,  OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );

        OperationId copyObjectsToFolder ( QStringList& sourceObjects,   QString destinationFolderPath,      OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId moveObjectsToFolder ( QStringList& sourceObjects,   QString destinationFolderPath,      OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId removeObjects       ( QStringList& sourceObjects,                                       OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId trashObjects        ( QStringList& sourceObjects,   QString destinationFolderPath,      OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
        OperationId duplicateObjects    ( QStringList& sourceObjects,                                       OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );

        OperationId createSymbolicLink  ( QString sourceObject,         QString destinationObject,          OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );

    public:// remembers a copy/cut operation, pastes the last copy/cut
        void        markObjectsForCopy  ( QStringList& sourceObjects );
        void        markObjectsForCut   ( QStringList& sourceObjects );
        OperationId pasteObjects        ( QString destinationFolderPath,                                    OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );
    private:
        QStringList _sourceObjects;
        bool        _haveToMove;

    public:// operations allowed on permissions
        typedef struct{ bool _read; bool _write; bool _execute; } PermissionsFlags;
        typedef struct
        {
            PermissionsFlags _owner;
            PermissionsFlags _user;
            PermissionsFlags _group;
            PermissionsFlags _everybody;
        } Permissions;

        OperationId getPermissionsForObject( QString object, Permissions& permissions,                      OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate());
        OperationId setPermissionsForObject( QString object, Permissions& permissions, bool recursive=false,OperatoinFinishedDelegate operatoinFinishedDelegate=OperatoinFinishedDelegate() );




    // internal stuff
    private:
        FileIoEngine* _fileIoEngine;

};

#endif
