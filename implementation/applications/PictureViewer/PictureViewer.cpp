
// Qt headers
#include <QtGui/QGridLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QScrollBar>
#include <QtGui/QLabel>
#include <QtGui/QImage>
#include <QtGui/QPixmap>


// local headers
#include "PictureViewer.h"


PictureViewer::PictureViewer( QString imageFilePath ) :
    QWidget()
{
    QImage image( imageFilePath );

    QLabel* label = new QLabel();
            label->setBackgroundRole( QPalette::Base );
            label->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
            label->setScaledContents( true );
            label->setPixmap( QPixmap::fromImage(image) );

    QScrollArea*    scrollArea = new QScrollArea();
                    scrollArea->setBackgroundRole( QPalette::Dark );
                    scrollArea->setWidget( label );

    QGridLayout*    gridLayout = new QGridLayout( this );
                    gridLayout->addWidget( scrollArea, 0, 0 );
}

PictureViewer::~PictureViewer()
{}


// void PictureViewer::pictureReload()
// {
// 	QListWidgetItem* current = m_picturesList->currentItem();
// 	if( 0 != current )
// 	{
// 		QString fileToShow = current->data(Qt::DisplayRole).toString();
// 		m_image.load( fileToShow );
// 		m_picture->setPixmap( QPixmap::fromImage(m_image) );
// 		
// 		m_scaleFactor = 1.0;
// 		scaleImage(1.0);
// 	}
// }
// 
// void PictureViewer::itemChanged(QListWidgetItem* current, QListWidgetItem* previous)
// {
// 	pictureReload();
// }
// 
// void PictureViewer::scaleImage(double factor)
// {
// 	m_scaleFactor *= factor;
// 	m_picture->resize(m_scaleFactor * m_picture->pixmap()->size());
// 
// 	adjustScrollBar(m_scrollArea->horizontalScrollBar(), factor);
// 	adjustScrollBar(m_scrollArea->verticalScrollBar(), factor);
// }
// 
// void PictureViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)                                                         
// {                                                                                                                               
// 	scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));                                                       
// }
