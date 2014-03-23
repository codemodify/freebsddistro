#ifndef __Filter_H__
#define __Filter_H__

// Qt headers
#include <QtGui/QWidget>

// bloks
#include <FileListView/FileListView.h>


// forwards
class QToolButton;
class QLineEdit;
class QHBoxLayout;


class Filter : public QWidget
{
    Q_OBJECT

    public:
        Filter( FileListView& fileListView );
        virtual ~Filter();


    signals:
        void filterClose();
        void filterAvailable( const QString& filter );


    // this needs to be overriden by the derived classes
    protected:
        virtual void addAdditionalWidgets( QHBoxLayout& hBoxLayout );

    // handle the data event
    protected slots:
        virtual void dataChanged( const QString& data );
    private:
        QLineEdit* _data;


    // handle the close event
    protected:
        void closeEvent( QCloseEvent* event );
    private:
        QToolButton* _close;


    protected:
        FileListView& _fileListView;

};

#endif
