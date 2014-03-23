#ifndef __ScreenRecorder_H__
#define __ScreenRecorder_H__

// Qt headers
#include <QtGui/QWidget>
#include <QtGui/QPushButton>

class GrabberThread;

class ScreenRecorder : public QWidget
{
    Q_OBJECT

    public:
        ScreenRecorder();
        ~ScreenRecorder();

    public slots:
        void setOutputFolder();
        void startRecord();
        void stopRecord();

    protected:
        void closeEvent( QCloseEvent* event );
        void contextMenuEvent( QContextMenuEvent* event );
        
    private:
        QString         _outputFolder;
        QPushButton*    _startRecord;
        QPushButton*    _stopRecord;
        GrabberThread*  _grabberThread;
};

#endif
