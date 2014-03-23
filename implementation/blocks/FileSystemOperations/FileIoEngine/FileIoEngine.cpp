
// Qt headers
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QDir>

// local headers
#include "FileIoEngine.h"

FileIoEngine::FileIoEngine() :
    QObject()
{}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FileIoEngine::internalCreateFile( OperationId operationId, OperationId /*parentOperationId*/ )
{
    // the "File" object to work with
    QFile file( operationId->_destinationPath );

    // there are no things here requiring much time, so set progree to finished
    operationId->_percentProgress = 100;

    // check if detination file exists
    if( file.exists() )
    {
        operationId->_status = ErrorExistFileDestination;
        emit updateOperation( operationId );
        return;
    }

    // create the file
    if( !file.open(QIODevice::WriteOnly) )
    {
        operationId->_status = ErrorCreateFileDestination;
        emit updateOperation( operationId );
        return;
    }

    file.close();
    operationId->_status = Success;
    emit updateOperation( operationId );
}

void FileIoEngine::internalCopyFile( OperationId operationId, OperationId /*parentOperationId*/ )
{
    // check if destination file exists
    if( QFile::exists(operationId->_destinationPath)  )
    {
        operationId->_status = ErrorExistFileDestination;
        operationId->_percentProgress = 100;
        emit updateOperation( operationId );
        return;
    }

    // check if we can read the source
    QFile sourceFile( operationId->_sourcePath );
    if( !sourceFile.open(QIODevice::ReadOnly) )
    {
        operationId->_status            = ErrorOpenFileSource;
        operationId->_percentProgress   = 100;
        emit updateOperation( operationId );
        return;
    }

    // check if we can write destination
    QFile destinationFile( operationId->_destinationPath );
    if( !destinationFile.open(QIODevice::WriteOnly) )
    {
        operationId->_status            = ErrorCreateFileDestination;
        operationId->_percentProgress   = 100;
        emit updateOperation( operationId );
        sourceFile.close();
        return;
    }

    // so far seems ok, try to do the copying, a write error may occur if no space left
    operationId->_totalSize = sourceFile.size();
    operationId->_status    = Running;
    emit updateOperation( operationId );

    QByteArray buffer;
    qint64 readBytes = 0;
    while( buffer=sourceFile.read(_bufferSize), !buffer.isEmpty() )
    {
        qint64 bytesWritten = destinationFile.write( buffer );
        if( -1 == bytesWritten )
        {
            operationId->_totalSize = sourceFile.size();
            operationId->_status    = ErrorWriteFileDestination;
            emit updateOperation( operationId );
            sourceFile.close();
            destinationFile.close();
            return;
        }

        readBytes += buffer.size();

        operationId->_percentProgress = ( readBytes * 100 ) / sourceFile.size();
        emit updateOperation( operationId );
    }

    // clean up
    sourceFile.close();
    destinationFile.close();

    operationId->_status            = Success;
    operationId->_percentProgress   = 100;
    emit updateOperation( operationId );
}

void FileIoEngine::internalRemoveFile( OperationId operationId, OperationId /*parentOperationId*/ )
{
    // the "File" object to work with
    QFile file( operationId->_destinationPath );

    operationId->_percentProgress = 100;

    if( !file.remove() )
    {
        if( !file.exists() )
            operationId->_status = Success;
        else
            operationId->_status = ErrorDeleteFileDestination;

        emit updateOperation( operationId );
        return;
    }

    operationId->_status = Success;
    emit updateOperation( operationId );
}

void FileIoEngine::internalRenameFile( OperationId operationId, OperationId /*parentOperationId*/ )
{
    QFile file( operationId->_sourcePath );

    operationId->_percentProgress = 100;

    if( !file.rename(operationId->_sourcePath,operationId->_destinationPath) )
    {
        if( !file.exists() )
            operationId->_status = ErrorOpenFileSource;
        else
            operationId->_status = ErrorCreateFileDestination;

        emit updateOperation( operationId );
        return;
    }

    operationId->_status = Success;
    emit updateOperation( operationId );
}

void FileIoEngine::internalMoveFile( OperationId operationId, OperationId parentOperationId )
{
    internalCopyFile( operationId, parentOperationId );

    // we'll need to adjust the operationId's fields here to be valid for the next operation
    operationId->_destinationPath = operationId->_sourcePath;

    internalRemoveFile( operationId, parentOperationId );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void getFullContentForFolder( QString sourceFolder, QStringList& content )
{
    // make sure there is no trash
    content.clear();

    // add this so we can use the algo below
    content.append( sourceFolder );

    for( int index=0; index<content.size(); ++index )
    {
        // getting the item
        QString itemPath = content.at( index );

        // item is a folder then scanning for its content
        if( !QFileInfo(itemPath).isDir() )
            continue;

        QStringList itemContent = 
            QDir(itemPath).entryList( QDir::Hidden | QDir::System | QDir::CaseSensitive | QDir::NoDotAndDotDot | QDir::AllEntries );

        // here we fix the paths to be full paths
        for( int i=0; i!=itemContent.size(); ++i )
            itemContent[i] = QString("%1/%2").arg(itemPath).arg(itemContent[i]);

        // append to the main list 
        content += itemContent; 
    }
}

void FileIoEngine::internalCreateFolder( OperationId operationId, OperationId /*parentOperationId*/ )
{
    QDir dir;

    if( dir.mkpath(operationId->_destinationPath) )
        operationId->_status = Success;
    else
        operationId->_status = ErrorCreateFolderDestination;

    operationId->_percentProgress = 100;
    emit updateOperation( operationId );
}

void FileIoEngine::internalCopyFolder( OperationId operationId, OperationId parentOperationId )
{
    operationId->_status = Running;
    emit updateOperation( operationId );

    // building the source list of files and folders
    QStringList fullFoderContentSource;
    getFullContentForFolder( operationId->_sourcePath, fullFoderContentSource );

    // building the destination list of files and folders
    QStringList fullFoderContentDestination;
    foreach( QString sorceItem, fullFoderContentSource )
    {
        if( !QFileInfo(sorceItem).isDir() )
        {
            operationId->_totalSize += QFileInfo(sorceItem).size();
            emit updateOperation( operationId );
        }

        sorceItem.remove( operationId->_sourcePath );
        sorceItem.insert( 0, QDir(operationId->_sourcePath).dirName() );

        QString destination = QString("%1/%2").arg(operationId->_destinationPath).arg(sorceItem);

        fullFoderContentDestination.append( destination );
    }

    // do the copy
    bool errorOccured = false;
    for( int index=0; index<fullFoderContentSource.size(); ++index )
    {
        QString source = fullFoderContentSource.at( index );
        QString destination = fullFoderContentDestination.at( index );

        Operation* operation        = new Operation();
        operation->_source          = source;
        operation->_sourcePath      = source;
        operation->_destination     = destination;
        operation->_destinationPath = destination;

        if( QFileInfo(source).isDir() )
        {
            operation->_operationName = "CreateFolder";
            emit newOperation( operation, operationId );

            internalCreateFolder( operation, operationId );
        }
        else
        {
            operation->_operationName = "CopyFile";
            emit newOperation( operation, operationId );

            internalCopyFile( operation, operationId );
        }

        errorOccured = errorOccured ? true : (operation->_status!=Success);
    }

    if( errorOccured )
        operationId->_status = Error;
    else
        operationId->_status = Success;

    emit updateOperation( operationId );
}

void FileIoEngine::internalRemoveFolder( OperationId operationId, OperationId /*parentOperationId*/ )
{
    QStringList fullFoderContent;
    getFullContentForFolder( operationId->_destinationPath, fullFoderContent );

    bool errorOccured = false;

    // make the first pass and remove all files
    for( int index=0; index<fullFoderContent.size(); ++index )
    {
        QString item = fullFoderContent.at(index);
        if( QFileInfo(item).isFile() )
        {
            Operation* operation        = new Operation();
            operation->_operationName   = "RemoveFile";
            operation->_source          = QFileInfo(item).fileName();
            operation->_sourcePath      = "";
            operation->_destination     = item;
            operation->_destinationPath = item;
            emit newOperation( operation, operationId );

            internalRemoveFile( operation, operationId );

            operationId->_percentProgress = ( index * 100 ) / fullFoderContent.size();
            emit updateOperation( operationId );

            errorOccured = errorOccured ? true : (operation->_status!=Success);
        }
    }

    // make second pass and remove all folders
    for( int index=fullFoderContent.size()-1; index>=0; --index )
    {
        QString item = fullFoderContent.at(index);
        if( QFileInfo(item).isDir() )
        {
            Operation* operation        = new Operation();
            operation->_operationName   = "RemoveFolder";
            operation->_source          = QFileInfo(item).fileName();
            operation->_sourcePath      = "";
            operation->_destination     = item;
            operation->_destinationPath = item;
            emit newOperation( operation, operationId );

            if( !QDir().rmdir(operation->_destinationPath) )
                operation->_status = ErrorDeleteFolderDestination;
            else
                operation->_status = Success;

            emit updateOperation( operation );

            errorOccured = errorOccured ? true : (operation->_status!=Success);
        }
    }

    operationId->_status = errorOccured ? Error : Success;
    operationId->_percentProgress = 100;
    emit updateOperation( operationId );
}

void FileIoEngine::internalRenameFolder( OperationId operationId, OperationId /*parentOperationId*/ )
{
    operationId->_percentProgress = 100;

    // FIX: "QDir().rename()" requires a full path as part of the new name
    QString validNewName = QFileInfo(operationId->_sourcePath).absolutePath();
            validNewName = validNewName + "/" + operationId->_destinationPath;

    bool operationResult = QDir().rename( operationId->_sourcePath, validNewName );
    if( false == operationResult )
        operationId->_status = ErrorCreateFolderDestination;
    else
        operationId->_status = Success;

    emit updateOperation( operationId );
}

void FileIoEngine::internalMoveFolder( OperationId operationId, OperationId parentOperationId )
{
    internalCopyFolder( operationId, parentOperationId );

    operationId->_destination       = operationId->_source;
    operationId->_destinationPath   = operationId->_sourcePath;
    internalRemoveFolder( operationId, parentOperationId );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FileIoEngine::internalCreateSymbolicLink( OperationId operationId, OperationId /*parentOperationId*/ )
{
    operationId->_percentProgress   = 100;

    if( !QFile::link(operationId->_sourcePath,operationId->_destinationPath) )
    {
        operationId->_status = ErrorCreateFileDestination;
        emit updateOperation( operationId );
        return;
    }

    operationId->_status = Success;
    emit updateOperation( operationId );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void FileIoEngine::internalCopyMoveObjects( OperationId operationId, OperationId /*parentOperationId*/, bool haveToMove )
{
    // make some checks to see if the basics are ok to proceed
    if( 0 == operationId->_sourceObjects )
        return;

    if( operationId->_sourceObjects->isEmpty() )
        return;

    if( false == QFileInfo(operationId->_destinationPath).isDir() )
        return;

    operationId->_status = Running;
    emit updateOperation( operationId );

    // now do the actual copy or move operation
    bool errorOccured = false;
    foreach( QString object, *operationId->_sourceObjects )
    {
        Operation* operation        = new Operation();
        operation->_source          = object;
        operation->_sourcePath      = object;
        operation->_destination     = operationId->_destinationPath;
        operation->_destinationPath = operationId->_destinationPath;
        emit newOperation( operation, operationId );

        if( true == QFileInfo(object).isDir() )
        {
            operation->_operationName = "CopyFolder";
            internalCopyFolder( operation, operationId );
        }
        else
        {
            operation->_operationName = "CopyFile";
            operation->_destinationPath = 
                QString("%1/%2").arg(operationId->_destinationPath).arg(QFileInfo(object).fileName());

            internalCopyFile( operation, operationId );
        }

        errorOccured = errorOccured ? true : (operation->_status!=Success);
    }

    if( errorOccured )
    {
        operationId->_status = Error;
        emit updateOperation( operationId );
        return;
    }

    // remove the source if it is a 'move' operation
    if( true == haveToMove )
    {
        foreach( QString object, *operationId->_sourceObjects )
        {
            Operation* operation        = new Operation();
            operation->_source          = QFileInfo(object).fileName();
            operation->_sourcePath      = "";
            operation->_destination     = object;
            operation->_destinationPath = object;
            emit newOperation( operation, operationId );

            if( true == QFileInfo(object).isDir() )
            {
                operation->_operationName = "RemoveFolder";
                internalRemoveFolder( operation, operationId );
            }
            else
            {
                operation->_operationName = "RemoveFile";
                internalRemoveFile( operation, operationId );
            }

            errorOccured = errorOccured ? true : (operation->_status!=Success);
        }
    }

    operationId->_sourceObjects->clear();
    delete operationId->_sourceObjects;

    if( errorOccured )
        operationId->_status = Error;
    else
        operationId->_status = Success;

    emit updateOperation( operationId );
}

void FileIoEngine::internalRemoveObjects( OperationId operationId, OperationId /*parentOperationId*/ )
{
    // make some checks to see if the basics are ok to proceed
    if( 0 == operationId->_sourceObjects )
        return;

    if( operationId->_sourceObjects->isEmpty() )
        return;

    operationId->_status = Running;
    emit updateOperation( operationId );

    // do the actual remove
    bool errorOccured = false;
    foreach( QString object, *operationId->_sourceObjects )
    {
        Operation* operation        = new Operation();
        operation->_source          = QFileInfo(object).fileName();
        operation->_sourcePath      = "";
        operation->_destination     = object;
        operation->_destinationPath = object;
        emit newOperation( operation, operationId );

        if( true == QFileInfo(object).isDir() )
        {
            operation->_operationName = "RemoveFolder";
            internalRemoveFolder( operation, operationId );
        }
        else
        {
            operation->_operationName = "RemoveFile";
            internalRemoveFile( operation, operationId );
        }

        errorOccured = errorOccured ? true : (operation->_status!=Success);
    }

    operationId->_sourceObjects->clear();
    delete operationId->_sourceObjects;

    if( errorOccured )
        operationId->_status = Error;
    else
        operationId->_status = Success;

    emit updateOperation( operationId );
}

