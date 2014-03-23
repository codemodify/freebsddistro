#ifndef __OperationsModel_H__
#define __OperationsModel_H__

// Qt headers
#include <QtCore/QAbstractItemModel>


// local headers
#include "Operation/Operation.h"


/*************************************************************************************
*
*   This class implements a model that will represent the current
*   state for the file operations.
*
*************************************************************************************/
class OperationsModel : public QAbstractItemModel
{
    Q_OBJECT

    // constructor area
    public:
        OperationsModel();
        ~OperationsModel();




    // allows to query for operation status
    friend class FileSystemOperations;
    private:
        OperationStatus getOperationStatus   ( OperationId operationId );




    // the file-operation notifications handlers
    signals:
        void newOperationRequest( OperationId operationId, OperationId parentOperationId );
    private slots:
        void newOperationHandler( OperationId operationId, OperationId parentOperationId );
    public:
        void addNewOperation    ( OperationId operationId, OperationId parentOperationId );
        
    public slots:
        void updateOperation    ( OperationId operationId                                );




    // we need to implement these to actually implement a model
    public:

        // size
        int             columnCount ( const QModelIndex& parent=QModelIndex() ) const;
        int             rowCount    ( const QModelIndex& parent=QModelIndex() ) const;
        bool            hasChildren ( const QModelIndex& parent=QModelIndex() ) const;

        // item mapping
        QModelIndex     index       ( int row, int column, const QModelIndex& parent=QModelIndex()  ) const;
        QModelIndex     parent      ( const QModelIndex& index                                      ) const;

        // data access
        Qt::ItemFlags   flags       ( const QModelIndex& index                                              ) const;
        QVariant        data        ( const QModelIndex& index, int role=Qt::DisplayRole                    ) const;
        QVariant        headerData  ( int section, Qt::Orientation orientation, int role=Qt::DisplayRole    ) const;




    // model's internal data
    private:
        Operation* _rootItem;

};

#endif

