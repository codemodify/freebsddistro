
// Qt headers
#include <QtGui/QAction>

// blocks
#include <FileSearch/FileSearch.h>
#include <FileSearchUi/FileSearchUi.h>
#include <IconProvider/ActionIconProvider/ActionIconProvider.h>

// local headers
#include "SearchResult.h"


SearchResult::SearchResult() :
	View()
{
	setTitleText( "Search Result" );

    QAction* close = new QAction( 0 );
             close->setIcon( ActionIconProvider().getIconForAction("close") );

    setTitleCloseButton( close, this, SLOT(close()) );

	_fileSearch = new FileSearch();

	_fileSearchUi = new FileSearchUi( _fileSearch );
	connect( _fileSearchUi, SIGNAL(fileDoubleClicked(QString)), this, SLOT(slot_EditFile(QString)) );

	setContentWidget( _fileSearchUi );
}

SearchResult::~SearchResult()
{
	delete _fileSearchUi;
	delete _fileSearch;
}

void SearchResult::slot_EditFile( QString filePath )
{
	emit editFile( filePath );
}

void SearchResult::setSearchFolder( QString folder )
{
	_fileSearchUi->setSearchFolder( folder );
}
