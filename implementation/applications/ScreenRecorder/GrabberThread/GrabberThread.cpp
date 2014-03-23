
// Qt headers
#include <QtGui/QPixmap>

// local headers
#include "GrabberThread.h"

GrabberThread::GrabberThread( QString outputFolder, QWidget* widget ) :
    QThread(),
    _stopRequested( false ),
    _outputFolder( outputFolder ),
    _widget( widget )
{}

int a = 0;
void GrabberThread::run()
{
    //int winId = QApplication::desktop()->winId();
    
    QPixmap pixmap = QPixmap();
    while( !_stopRequested )
    {
        //_screenShots.append( QPixmap::grabWindow(QApplication::desktop()->winId()) );
        pixmap = QPixmap();
        pixmap = QPixmap::grabWidget( _widget );
        pixmap.save( QString("%1/%2.png").arg(_outputFolder).arg(a++) );
    }
    
    // save images as video
//    for( int index = 0; index < _screenShots.size(); index++ )
//    {
//        char fileName[100] = { 0 };
//        sprintf( fileName, "%3d", index );
//        qDebug( fileName );
//        _screenShots[index].save( QString("%1/%2.png").arg(_outputFolder).arg(index) );
//    }
}

void GrabberThread::stop()
{
    _stopRequested = true;
}
