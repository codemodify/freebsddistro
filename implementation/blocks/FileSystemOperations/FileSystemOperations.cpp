
// Qt headers
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

// local headers
#include "FileSystemOperations.h"
#include "FileIoEngine/FileIoEngine.h"
#include "ThreadedOperation/ThreadedOperation.h"
#include "OperationsModel/OperationsModel.h"



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- constructor area
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
FileSystemOperations::FileSystemOperations() :
    _threadLimit( 4 ),
    _haveToMove( false ),
    _destroyOnFinish( false ),
    _deleteToTrash( false )
{
    // init the file operation engine
    _fileIoEngine = new FileIoEngine();
    _fileIoEngine->_bufferSize = OneMegabyte;

    // init the model of the current operations
    _operationsModel = new OperationsModel();

    QObject::connect(   _fileIoEngine,      SIGNAL(newOperation(OperationId,OperationId)),
                        _operationsModel,   SLOT(newOperationHandler(OperationId,OperationId))      );

    QObject::connect(   _fileIoEngine,      SIGNAL(updateOperation(OperationId)),
                        _operationsModel,   SLOT(updateOperation(OperationId))                      );
}

FileSystemOperations::~FileSystemOperations()
{
    if( _fileIoEngine )
        delete _fileIoEngine;

    if( _operationsModel )
        delete _operationsModel;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- instructs to auto delete when done
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FileSystemOperations::setDeleteToTrash( bool deleteToTrash )
{
    _deleteToTrash = deleteToTrash;
}

void FileSystemOperations::setTrashPath( QString trashPath )
{
    _trashPath = trashPath;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- instructs to auto delete when done
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FileSystemOperations::setDestroyOnFinish( bool destroyOnFinish )
{
    _destroyOnFinish = destroyOnFinish;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- set the thread limit
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FileSystemOperations::setMaxThreadsLimit( int threadLimit )
{
    _threadLimit = threadLimit;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- implements a delegating mechanism to notify when the operation ends
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FileSystemOperations::makeSureThreadsGetTheirTurn()
{
    for( int index = 0; index < _threadLimit; ++index )
    {
        qDebug( QString("%1").arg(index).toAscii().data() );
    
        ThreadedOperation* threadedOperationLocal = _currentThreads.value( index );
        if( 0 == threadedOperationLocal )
        {
            if( _destroyOnFinish && (0 == _currentThreads.count()) )
                deleteLater();
            else
                continue;
        }

        else
        if( (false == threadedOperationLocal->isRunning()) && (false == threadedOperationLocal->isFinished())    )
            threadedOperationLocal->start();
    }
}

void FileSystemOperations::runThreadAndNotifyWhenEnds(  ThreadedOperation*          threadedOperation,
                                                        OperationId                 operationId,
                                                        OperatoinFinishedDelegate   operatoinFinishedDelegate )
{
    // save to the threads pool
    _currentThreads.append( threadedOperation );

    // save what delegates to call when operation is done
    _callBacksToCallWhenOperationsEnd.insert( operationId, operatoinFinishedDelegate );

    // subscribe for the thread-finish notification
    connect( threadedOperation, SIGNAL(finished()), this, SLOT(slot_threadFinished()) );

    makeSureThreadsGetTheirTurn();
}

void FileSystemOperations::slot_threadFinished()
{
	//QMutexLocker locker( &_mutex );

    // get the thread that just finished
    QObject* object = sender();
    if( 0 == object )
        return;

    ThreadedOperation* threadedOperation = qobject_cast<ThreadedOperation*>( object );
    if( 0 == threadedOperation )
        return;

    // run the delegate associated with its termination
    OperationId operationId = threadedOperation->getParameter1();
    OperatoinFinishedDelegate operatoinFinishedDelegate = _callBacksToCallWhenOperationsEnd[ operationId ];
    if( operatoinFinishedDelegate.first && operatoinFinishedDelegate.second )
        (operatoinFinishedDelegate.first->*operatoinFinishedDelegate.second)( operationId );


    // run the next thread in the queue
    _currentThreads.removeAt( _currentThreads.indexOf(threadedOperation) );

    delete threadedOperation;

    makeSureThreadsGetTheirTurn();
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- change at run time the buffer size
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FileSystemOperations::setIoBufferSize( qint64 bufferSize )
{
    _fileIoEngine->_bufferSize = bufferSize;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- gets the model of the current operations
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
OperationsModel* FileSystemOperations::getOperationsModel()
{
    return _operationsModel;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- queries the result for an operation
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
OperationStatus FileSystemOperations::getOperationStatus( OperationId operationId )
{
    return _operationsModel->getOperationStatus( operationId );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- File interfaces
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
OperationId FileSystemOperations::createFile( QString filePath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "CreateFile";
                operation->_source          = QFileInfo(filePath).fileName();
                operation->_sourcePath      = "";
                operation->_destination     = filePath;
                operation->_destinationPath = filePath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalCreateFile, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::copyFile( QString sourceFilePath, QString destinationFilePath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "CopyFile";
                operation->_source          = sourceFilePath;
                operation->_sourcePath      = sourceFilePath;
                operation->_destination     = destinationFilePath;
                operation->_destinationPath = destinationFilePath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalCopyFile, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::removeFile( QString filePath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    if( _deleteToTrash )
    {
        if( _trashPath.isEmpty() )
            return 0;

        return trashFile( filePath, _trashPath, operatoinFinishedDelegate );
    }

    Operation*  operation                   = new Operation();
                operation->_operationName   = "RemoveFile";
                operation->_source          = QFileInfo(filePath).fileName();
                operation->_sourcePath      = "";
                operation->_destination     = filePath;
                operation->_destinationPath = filePath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalRemoveFile, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::trashFile( QString sourceFilePath, QString destinationFilePath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "TrashFile";
                operation->_source          = sourceFilePath;
                operation->_sourcePath      = sourceFilePath;
                operation->_destination     = destinationFilePath;
                operation->_destinationPath = destinationFilePath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalMoveFile, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::renameFile( QString filePath, QString newName, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "RenameFile";
                operation->_source          = filePath;
                operation->_sourcePath      = filePath;
                operation->_destination     = newName;
                operation->_destinationPath = QString("%1/%2").arg(QFileInfo(filePath).absolutePath()).arg(newName);

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalRenameFile, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::moveFile( QString sourceFilePath, QString destinationFilePath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "MoveFile";
                operation->_source          = sourceFilePath;
                operation->_sourcePath      = sourceFilePath;
                operation->_destination     = destinationFilePath;
                operation->_destinationPath = destinationFilePath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalMoveFile, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- Folder interfaces
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
OperationId FileSystemOperations::createFolder( QString folderPath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "CreateFolder";
                operation->_source          = QFileInfo(folderPath).fileName();
                operation->_sourcePath      = "";
                operation->_destination     = folderPath;
                operation->_destinationPath = folderPath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalCreateFolder, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::copyFolder( QString sourceFolderPath, QString intoDestinationFolderPath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "CopyFolder";
                operation->_source          = sourceFolderPath;
                operation->_sourcePath      = sourceFolderPath;
                operation->_destination     = intoDestinationFolderPath;
                operation->_destinationPath = intoDestinationFolderPath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalCopyFolder, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::copyFolderContent( QString sourceFolderPath, QString intoDestinationFolderPath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    QStringList objects = QDir( sourceFolderPath ).entryList( QDir::AllDirs|QDir::Files|QDir::System|QDir::CaseSensitive|QDir::Hidden|QDir::NoDotAndDotDot );

    // adjust file path
    for( int index=0; index<objects.count(); index++ )
        objects[index] = sourceFolderPath + "/" + objects[index];

    copyObjectsToFolder( objects, intoDestinationFolderPath, OperatoinFinishedDelegate() );
}

OperationId FileSystemOperations::removeFolder( QString folderPath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    if( _deleteToTrash )
    {
        if( _trashPath.isEmpty() )
            return 0;

        return trashFolder( folderPath, _trashPath, operatoinFinishedDelegate );
    }

    Operation*  operation                   = new Operation();
                operation->_operationName   = "RemoveFolder";
                operation->_source          = QFileInfo(folderPath).fileName();
                operation->_sourcePath      = "";
                operation->_destination     = folderPath;
                operation->_destinationPath = folderPath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalRemoveFolder, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::trashFolder( QString sourceFolderPath, QString intoDestinationFolderPath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "MoveFolder";
                operation->_source          = sourceFolderPath;
                operation->_sourcePath      = sourceFolderPath;
                operation->_destination     = intoDestinationFolderPath;
                operation->_destinationPath = intoDestinationFolderPath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalMoveFolder, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::renameFolder( QString folderPath, QString newName, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "RenameFolder";
                operation->_source          = folderPath;
                operation->_sourcePath      = folderPath;
                operation->_destination     = newName;
                operation->_destinationPath = newName;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalRenameFolder, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::moveFolder( QString sourceFolderPath, QString intoDestinationFolderPath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "MoveFolder";
                operation->_source          = sourceFolderPath;
                operation->_sourcePath      = sourceFolderPath;
                operation->_destination     = intoDestinationFolderPath;
                operation->_destinationPath = intoDestinationFolderPath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalMoveFolder, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- Copy/Move objects to
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
OperationId FileSystemOperations::copyObjectsToFolder( QStringList& sourceObjects, QString destinationFolderPath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "CopyObjectsToFolder";
                operation->_sourceObjects   = new QStringList( sourceObjects );
                operation->_destination     = destinationFolderPath;
                operation->_destinationPath = destinationFolderPath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalCopyMoveObjects, operation, NoParentOperation, false );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::moveObjectsToFolder( QStringList& sourceObjects, QString destinationFolderPath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "MoveObjectsToFolder";
                operation->_sourceObjects   = new QStringList( sourceObjects );
                operation->_destination     = destinationFolderPath;
                operation->_destinationPath = destinationFolderPath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalCopyMoveObjects, operation, NoParentOperation, true );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::removeObjects( QStringList& sourceObjects, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    if( _deleteToTrash )
    {
        if( _trashPath.isEmpty() )
            return 0;

        return trashObjects( sourceObjects, _trashPath, operatoinFinishedDelegate );
    }

    Operation*  operation                   = new Operation();
                operation->_operationName   = "RemoveObjects";
                operation->_sourceObjects   = new QStringList( sourceObjects );

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalRemoveObjects, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::trashObjects( QStringList& sourceObjects, QString destinationFolderPath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "TrashObjects";
                operation->_sourceObjects   = new QStringList( sourceObjects );
                operation->_destination     = destinationFolderPath;
                operation->_destinationPath = destinationFolderPath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalCopyMoveObjects, operation, NoParentOperation, true );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}

OperationId FileSystemOperations::duplicateObjects( QStringList& sourceObjects, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    foreach( QString object, sourceObjects )
    {
        if( QFileInfo(object).isDir() )
        {
            createFolder( object+"_copy", OperatoinFinishedDelegate() );

            QStringList objects = QDir( object ).entryList( QDir::AllDirs|QDir::Files|QDir::System|QDir::CaseSensitive|QDir::Hidden|QDir::NoDotAndDotDot );

            // adjust file path
            for( int index=0; index<objects.count(); index++ )
                objects[index] = object + "/" + objects[index];

            copyObjectsToFolder( objects, object+"_copy", OperatoinFinishedDelegate() );
        }
        else
            copyFile( object, object+"_copy", OperatoinFinishedDelegate() );
    }

    return 0;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- Symbolic links
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
OperationId FileSystemOperations::createSymbolicLink( QString sourceObject, QString destinationObject, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "CreateSymbolicLink";
                operation->_source          = QFileInfo(sourceObject).fileName();
                operation->_sourcePath      = sourceObject;
                operation->_destination     = QFileInfo(destinationObject).fileName();;
                operation->_destinationPath = destinationObject;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalCreateSymbolicLink, operation, NoParentOperation );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- Mark for Copy/Paste
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FileSystemOperations::markObjectsForCopy( QStringList& sourceObjects )
{
    _sourceObjects.clear();
    _sourceObjects << sourceObjects;
    _haveToMove = false;
}

void FileSystemOperations::markObjectsForCut( QStringList& sourceObjects )
{
    markObjectsForCopy( sourceObjects );
    _haveToMove = true;
}

OperationId FileSystemOperations::pasteObjects( QString destinationFolderPath, OperatoinFinishedDelegate operatoinFinishedDelegate )
{
    Operation*  operation                   = new Operation();
                operation->_operationName   = "PasteObjects";
                operation->_sourceObjects   = new QStringList( _sourceObjects );
                operation->_destination     = QFileInfo(destinationFolderPath).fileName();;
                operation->_destinationPath = destinationFolderPath;

    _operationsModel->addNewOperation( operation, NoParentOperation );

    ThreadedOperation*  threadedOperation = new ThreadedOperation( _fileIoEngine );
                        threadedOperation->setThreadedCode( &FileIoEngine::internalCopyMoveObjects, operation, NoParentOperation, _haveToMove );

    runThreadAndNotifyWhenEnds( threadedOperation, operation, operatoinFinishedDelegate );

    return operation;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- Permissions
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
OperationId FileSystemOperations::getPermissionsForObject( QString /*object*/, Permissions& /*permissions*/, OperatoinFinishedDelegate /*operatoinFinishedDelegate*/ )
{
    // FIXME: add implementation here
    
    return 0;
}

OperationId FileSystemOperations::setPermissionsForObject( QString /*object*/, Permissions& /*permissions*/, bool /*recursive*/, OperatoinFinishedDelegate /*operatoinFinishedDelegate*/ )
{
    // FIXME: add implementation here
    
    return 0;
}

