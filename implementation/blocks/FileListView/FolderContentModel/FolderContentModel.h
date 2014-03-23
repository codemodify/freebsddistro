#ifndef __FolderContentModel_H__
#define __FolderContentModel_H__

// Qt headers
#include <QtCore/QAbstractListModel>
#include <QtCore/QList>
#include <QtCore/QStringList>

// forwards
class FileSystemIconProvider;
class QDir;

class FolderContentModel : public QAbstractListModel
{
    Q_OBJECT

    public:
        FolderContentModel();
        ~FolderContentModel();

        typedef enum{ SelectedRole=(Qt::UserRole + 1) } FolderContentModelRole;



    // current folder
    public:
        void        setCurrentFolder( QString currentFolder );
        QString     getCurrentFolder(                       );
        QString     getCurrentFolder(                       ) const;
        void        refreshContent  (                       );
        void        setHiddenVisible( bool visible          );

        void        getContent( QStringList& content );
        void        setContent( QStringList& content );

    private:
        QStringList _folderContent;
        bool        _hiddenVisible;



    // selection
    public:
        void selectAll                  ();
        void selectNone                 ();

        void addFoldersToSelection      ();
        void removeFoldersFromSelection ();

        void addFilesToSelection        ();
        void removeFilesFromSelection   ();

        void addToSelection             ( QString criteria );
        void removeFromSelection        ( QString criteria );

        void addToSelection             ( const QModelIndex& mdoelIndex );

        void invertSelection();

        void getSelectedItems           ( QStringList& selectedItems );

        bool isItemSelected             ( const QModelIndex& modelIndex ) const;

    private:
        QList<bool> _selectedItems;



    // we need to implement this to allow views to extract data from this model
    public:
        // read-only access
        int             rowCount    ( const QModelIndex& parentModelIndex=QModelIndex()         ) const;
        QVariant        data        ( const QModelIndex& modelIndex, int role=Qt::DisplayRole   ) const;
        Qt::ItemFlags   flags       ( const QModelIndex& mdoelIndex                             ) const;

    // drag and drop
    public:
        Qt::DropActions supportedDragActions() const;
        Qt::DropActions supportedDropActions() const;

    // drag support, exporting data
    public:
        QMimeData*      mimeData    ( const QModelIndexList& mdoelIndexes                       ) const;

    // drop support, accepting data
    signals:
        void            itemsDropped( QStringList* sourceItems, QString targetFolder            );
    public:
        QStringList     mimeTypes   (                                                           ) const;
        bool            dropMimeData(   const QMimeData* mimeData,
                                        Qt::DropAction action,
                                        int row, int column,
                                        const QModelIndex &parentModelIndex                     );



    // some utility functions
    private:
        bool        isItemFolder        ( const QModelIndex& modelIndex ) const;
        QString     getItemPath         ( const QModelIndex& modelIndex );



    // data
    private:
        QDir*                   _dir;
        FileSystemIconProvider* _iconProvider;

};

#endif
