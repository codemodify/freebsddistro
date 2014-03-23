#ifndef __GrabberThread_H__
#define __GrabberThread_H__

// Qt headers
#include <QtCore/QThread>
#include <QtCore/QList>

class QWidget;

class GrabberThread : public QThread
{
    public:
        GrabberThread( QString outputFolder, QWidget* widget );

        void run();
        void stop();
        
    private:
        bool            _stopRequested;
        QList<QPixmap>  _screenShots;
        QString         _outputFolder;
        QWidget*        _widget;
};

#endif
