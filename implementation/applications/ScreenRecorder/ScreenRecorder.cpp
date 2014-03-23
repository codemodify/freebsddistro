
// Qt headers
#include <QtGui/QFileDialog>
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>

// blocks
#include <Styling/Styling.h>

// local headers
#include "ScreenRecorder.h"
#include "GrabberThread/GrabberThread.h"


ScreenRecorder::ScreenRecorder() :
    QWidget()
{
    setWindowTitle( "Screen Recorder" );
    
    _startRecord = new QPushButton( "Start Record" );
    connect( _startRecord, SIGNAL(released()), this, SLOT(startRecord()) );
    
    _stopRecord = new QPushButton( "Stop Record" );
    connect( _stopRecord, SIGNAL(released()), this, SLOT(stopRecord()) );
    
    QVBoxLayout*    vBoxLayout = new QVBoxLayout( this );
                    vBoxLayout->addWidget( _startRecord );
                    vBoxLayout->addWidget( _stopRecord );

    // load settings
    Styling styling( *this );
            styling.loadOnScreenPosition();
            styling.loadFonts();
}

ScreenRecorder::~ScreenRecorder()
{}

void ScreenRecorder::closeEvent( QCloseEvent* event )
{
    Styling styling( *this );
            styling.saveOnScreenPosition();

    stopRecord();

    QWidget::closeEvent( event );
}

void ScreenRecorder::contextMenuEvent( QContextMenuEvent* event )
{
    QMenu*  menu = new QMenu();
            menu->addAction( "Set Output Folder", this, SLOT(setOutputFolder()) );
            menu->exec();
}

void ScreenRecorder::setOutputFolder()
{
    QString dir = QFileDialog::getExistingDirectory( this, "Output Folder", "/home", 
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

    if( false == dir.isEmpty() )
    {
        _outputFolder = dir;
    }
}

void ScreenRecorder::startRecord()
{
    if( _outputFolder.isEmpty() )
         QMessageBox::information( this, "", "No Output Folder" );
         
    _startRecord->setEnabled( false );
    _stopRecord->setEnabled( true );
    
//    static QDesktopWidget desktopWidget;
//    static QWidget* widget = desktopWidget.screen();
    int id = QApplication::desktop()->winId();

QPixmap pixmap = QPixmap();
int a = 0;
    while( 1)
    {
        //_screenShots.append( QPixmap::grabWindow(QApplication::desktop()->winId()) );
        pixmap = QPixmap();
        pixmap = QPixmap::grabWindow( id );
        pixmap.save( QString("%1/%2.png").arg(_outputFolder).arg(a++) );
    }

//    _grabberThread = new GrabberThread( _outputFolder, this );
//    _grabberThread->start();
}

void ScreenRecorder::stopRecord()
{
    qDebug( "ScreenRecorder::stopRecord()" );
    
    _startRecord->setEnabled( true );
    _stopRecord->setEnabled( false );
    
    _grabberThread->stop();
    delete _grabberThread;
}

