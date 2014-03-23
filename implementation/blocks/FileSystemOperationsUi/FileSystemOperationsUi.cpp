
// common-nongui headers
#include <FileSystemOperations/FileSystemOperations.h>

// local headers
#include "FileSystemOperationsUi.h"

FileSystemOperationsUi::FileSystemOperationsUi( FileSystemOperations* fileSystemOperations ) :
    QTreeView()
{
    _fileSystemOperations = fileSystemOperations;
    
    setModel( _fileSystemOperations->getOperationsModel() );
    setAlternatingRowColors( true );
    setAnimated( false );
}

FileSystemOperationsUi::~FileSystemOperationsUi()
{}
