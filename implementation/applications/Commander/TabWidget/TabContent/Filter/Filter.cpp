
// Qt headers
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QLineEdit>
#include <QtGui/QCloseEvent>

// local headers
#include "Filter.h"


Filter::Filter( FileListView& fileListView ) :
    QWidget(),
    _fileListView( fileListView )
{
    _data   = new QLineEdit();

    _close  = new QToolButton();
    _close  ->setText( "x" );

    QHBoxLayout*    hBoxLayout = new QHBoxLayout( this );
                    hBoxLayout->addWidget( _data        );
                    addAdditionalWidgets ( *hBoxLayout  );
                    hBoxLayout->addWidget( _close       );

    connect( _data,     SIGNAL(textChanged(const QString&)), this, SLOT(dataChanged(const QString&)) );
    connect( _close,    SIGNAL(pressed()                  ), this, SLOT(close()                    ) );
}

Filter::~Filter()
{
    delete _data;
    delete _close;

    _fileListView.refreshContent();
}

void Filter::addAdditionalWidgets( QHBoxLayout& hBoxLayout )
{}

void Filter::closeEvent( QCloseEvent* event )
{
    emit filterClose();

    event->ignore();
}

void Filter::dataChanged( const QString& data )
{}
