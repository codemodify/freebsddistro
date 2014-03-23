#ifndef __FileSystemOperationsUi_H__
#define __FileSystemOperationsUi_H__

// Qt headers
#include <QtGui/QTreeView>

// common non-gui headers
class FileSystemOperations;

class FileSystemOperationsUi : public QTreeView
{
    public:
        FileSystemOperationsUi( FileSystemOperations* fileSystemOperations );
        ~FileSystemOperationsUi();

    private:
        FileSystemOperations* _fileSystemOperations;

};

#endif
