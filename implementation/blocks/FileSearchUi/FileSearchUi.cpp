
// Qt headers
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>

// blocks
#include <FileSearch/FileSearch.h>

// local headers
#include "FileSearchUi.h"


FileSearchUi::FileSearchUi( FileSearch* fileSearch ) :
    QWidget(),
	_fileSearch( fileSearch )
{
	// init these
	_where		= new QLineEdit();
	_nameFilter	= new QLineEdit();
	_contentFilter= new QLineEdit();

	// start search
	_search = new QPushButton( "Search" );
	connect( _search, SIGNAL(released()), this, SLOT(slot_Search()) );

	// result
	_result = new QListView();
	_result->setModel( fileSearch->getModel() );
	_result->setAlternatingRowColors( true );

	connect( _result, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(slot_DoubleClicked(const QModelIndex&)) );

	// assemble all
	QGridLayout*	gridLayout = new QGridLayout();
				gridLayout->addWidget( new QLabel("Where"),       0, 0	);
				gridLayout->addWidget( _where,                    1, 0 );

				gridLayout->addWidget( new QLabel("Name Filter"), 0, 1 );
				gridLayout->addWidget( _nameFilter,               1, 1 );

				gridLayout->addWidget( new QLabel("Content Filter"), 0, 2 );
				gridLayout->addWidget( _contentFilter,            1, 2 );

				gridLayout->addWidget( _search,                   1, 3 );

	QVBoxLayout*	hBoxLayout = new QVBoxLayout( this );
				hBoxLayout->addLayout( gridLayout );
				hBoxLayout->addWidget( _result    );
}

FileSearchUi::~FileSearchUi()
{}

void FileSearchUi::setSearchFolder( QString folder )
{
	_where->setText( folder );
}

void FileSearchUi::slot_DoubleClicked( const QModelIndex& modelIndex )
{
	emit fileDoubleClicked( (_result->model()->data(modelIndex)).toString() );
}

void FileSearchUi::slot_Search()
{
	_fileSearch->findItems( _where->text(), _nameFilter->text(), _contentFilter->text() );
}
