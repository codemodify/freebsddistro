#ifndef __FileSearchModel_H__
#define __FileSearchModel_H__

// Qt headers
#include <QtCore/QAbstractListModel>
#include <QtCore/QStringList>


class FileSystemIconProvider;

class FileSearchModel : public QAbstractListModel
{
    Q_OBJECT

    // constructor area
    public:
        FileSearchModel();
        ~FileSearchModel();


    public:
        QStringList& getItems(){ return _items; }
        void clear();


    // notification handlers
    public slots:
        void itemAvailable( QString item );


    // we need to implement these to actually implement a model
    public:
        int         rowCount( const QModelIndex& parent=QModelIndex()               ) const;
        QVariant    data    ( const QModelIndex& index, int role=Qt::DisplayRole    ) const;


    // model's internal data
    private:
        QStringList             _items;
        FileSystemIconProvider* _iconProvider;

};

#endif
