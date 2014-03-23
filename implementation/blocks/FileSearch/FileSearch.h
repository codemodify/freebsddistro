#ifndef __FileSearch_H__
#define __FileSearch_H__

// Qt headers
#include <QtCore/QThread>
#include <QtCore/QStringList>

// local headers
#include "FileSearchModel/FileSearchModel.h"


/*********************************************************************************************
*
*   This class defines an interface to find folders / files and dig into their content.
*
*
*   Usage:
*       FileSearch  fileSearch;
*                   fileSearch.findItems( nameFilter, contentFilter );
*
*                   QStringList items;
*                   fileSearch.getItems( items );
*
*********************************************************************************************/
class FileSearch : public QThread
{
    Q_OBJECT

    public:
        FileSearch();
        ~FileSearch();


    // interface
    public:
        void findItems( QString targetFolder,
                        QString nameFilter,
                        QString contentFilter,
                        bool folders=false );

        void getItems( QStringList& items );


    signals:
        void itemAvailable      ( QString item );


    private:
        void getFullContentForFolder( QStringList& content );


    // returns the model
    public:
        QAbstractItemModel* getModel();
    private:
        FileSearchModel* _fileSearchModel;


    private:
        QString _targetFolder;
        QString _nameFilter;
        QString _contentFilter;
        bool    _folders;


    protected:
        void run();

};

#endif
