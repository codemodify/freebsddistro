#ifndef __FileTreeModel_H__
#define __FileTreeModel_H__

// Qt
#include <QtGui/QDirModel>

// blocks
// FIXME:
// #include <IconProvider/FileSystemIconProvider/FileSystemIconProvider.h>

class FileTreeModel : public QDirModel
{
    Q_OBJECT

    public:
        FileTreeModel();
        ~FileTreeModel();

    signals:
        void itemsDropped( QStringList* items, QString targetFolder );

    public:
        int     columnCount ( const QModelIndex& parent=QModelIndex() ) const;
        bool    dropMimeData( const QMimeData* data, Qt::DropAction action, 
                              int row, int column, const QModelIndex& parent   );

    public:
        QString     filePath        ( const QModelIndex& index  ) const;

        
    private:
        QIcon       fileIcon( const QModelIndex& index                          ) const;
        QFileInfo   fileInfo( const QModelIndex& index                          ) const;
        QString     fileName( const QModelIndex& index                          ) const;

        QModelIndex mkdir   ( const QModelIndex& parent, const QString& name    );
        bool        remove  ( const QModelIndex& index                          );
        bool        rmdir   ( const QModelIndex& index                          );

        QFileIconProvider* iconProvider() const;

        bool isReadOnly     (               ) const;
        void setReadOnly    ( bool enable   );

};

#endif

