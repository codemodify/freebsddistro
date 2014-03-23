
// Qt headers
#include <QtCore/QStringList>
#include <QtCore/QEvent>
#include <QtGui/QGraphicsView>
#include <QtGui/QFileDialog>
#include <QtGui/QPushButton>
#include <QtGui/QPainter>
#include <QtGui/QTabBar>
#include <QtGui/QFrame>
#include <QtGui/QInputDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollBar>

// blocks
#include <Settings/Settings.h>
#include <IconProvider/ActionIconProvider/ActionIconProvider.h>

// local headers
#include "DocumentDrawer.h"
#include "PageDrawer/PageDrawer.h"
#include "PageDrawer/Shapes.h"


DocumentDrawer::DocumentDrawer() :
	QGraphicsView(),
	_pageCount( 0 )
{
	setGeometry( 20, 20, 400, 700 );

	// setup the tab bar
	_tabBar = new QTabBar( this );
	_tabBar->setShape( QTabBar::TriangularEast );
	_tabBar->setAutoFillBackground( false );
	_tabBar->move( 0, 10 );
	_tabBar->setFixedHeight( height() );
	_tabBar->setExpanding( false );
	_tabBar->setContextMenuPolicy( Qt::CustomContextMenu );
	_tabBar->addTab( "+" );
	
	connect( _tabBar, SIGNAL(currentChanged(int)), this, SLOT(pageChanged(int)) );
	connect( _tabBar, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(tabBarContextMenu(const QPoint&)) );
	_tabBar->installEventFilter( this );

	// setup the menu
    ActionIconProvider actionIconProvider;

    _menu.addAction( actionIconProvider.getIconForAction("add")		, "Add Page"			, this, SLOT(addNewPage())			);
    _menu.addAction( actionIconProvider.getIconForAction("delete")	, "Remove Page"		, this, SLOT(removePage())			);
    _menu.addSeparator();
    _menu.addAction( actionIconProvider.getIconForAction("edit")	, "Set Page Text"		, this, SLOT(setPageText())			);
    _menu.addSeparator();
    _menu.addAction( actionIconProvider.getIconForAction("remove")	, "Remove Empty Pages"	, this, SLOT(removeEmptyPages())	);

	// setup the startup page
	addNewPage();

	// preparing the toolbar
	_buttonsFrame = new QFrame( this );
	_buttonsFrame->setAutoFillBackground( false );
	_buttonsFrame->setWindowFlags( Qt::FramelessWindowHint );
	_buttonsFrame->setFrameStyle( QFrame::NoFrame );

	QVBoxLayout* buttonsFrameLayout = new QVBoxLayout( _buttonsFrame );

	for( int shapeIndex = Shapes::eShapeBegin; shapeIndex < Shapes::eShapeCount; shapeIndex++ )
	{
		shapeButtons[shapeIndex] = new QToolButton();
		shapeButtons[shapeIndex]->setIcon( ActionIconProvider().getIconForAction(Shapes::shapeNames[shapeIndex]) );
		shapeButtons[shapeIndex]->setCheckable( true );
		shapeButtons[shapeIndex]->setAutoExclusive( true );

		buttonsFrameLayout->addWidget( shapeButtons[shapeIndex] );
	}

	shapeButtons[Shapes::eShapeBegin]->setChecked( true );
}

DocumentDrawer::~DocumentDrawer()
{
	removeAllPages();
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- for the public
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void DocumentDrawer::saveDocument()
{
	if(	_fileName.isEmpty() )
		_fileName = QFileDialog::getSaveFileName( this, "Save Schema" );

	if(	_fileName.isEmpty() )
		return;

	Settings	settings;
			settings.setFile( _fileName );

	// remove the content of the file to not have any stuff left from previous versions
	QStringList keys;
	settings.getChildrenNamesForKey( "", keys );

	foreach( QString key, keys )
		settings.removeKey( key );

	keys.clear();

	// add the objetcs to the file
	for( int index = 0; index < _pageDrawerList.count(); index++ )
	{
		QStringList objectsAsStringList;
		_pageDrawerList.at(index)->getObjectsAsStringList( objectsAsStringList );

		QString pageName = _tabBar->tabText( index ).replace( " ", "_abcdefghi_" );

		settings.setValuesForKey( pageName, objectsAsStringList );
	}

	settings.save();
}

void DocumentDrawer::openDocument( QString fileName )
{
	_fileName = fileName;

	Settings	settings;
			settings.setFile( _fileName );

	// clean what exists
	removeAllPages();

	// add pages from the file
	QStringList pages;
	settings.getChildrenNamesForKey( "", pages );

	QStringList objectsAsStringList;
	for( int index=0; index < pages.count(); index++ )
	{
		QString page = pages.at( index );

		objectsAsStringList.clear();
		settings.getValuesForKey( page, objectsAsStringList );

		PageDrawer*	pageDrawer = new PageDrawer();
					pageDrawer->setObjectsFromStringList( objectsAsStringList );

		_pageDrawerList.append( pageDrawer );
		_tabBar->insertTab( index, page.replace("_abcdefghi_", " ") );
	}

	connect( _tabBar, SIGNAL(currentChanged(int)), this, SLOT(pageChanged(int)) );
	connect( _tabBar, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(tabBarContextMenu(const QPoint&)) );

	_tabBar->setCurrentIndex( 0 );
	_pageCount = _pageDrawerList.count();
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- inherited stuff
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
bool DocumentDrawer::eventFilter( QObject* object, QEvent* event )
{
	if( QEvent::Wheel == event->type() )
		_wheelSwitch = true;

	else if( QEvent::MouseButtonRelease == event->type() )
		_wheelSwitch = false;

	return QGraphicsView::eventFilter( object, event );
}

void DocumentDrawer::resizeEvent( QResizeEvent* event )
{
	QGraphicsView::resizeEvent( event );

	_buttonsFrame->move( width() - _buttonsFrame->width() - (verticalScrollBar() ? verticalScrollBar()->width() : 0), 	50 );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- internal stuff
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void DocumentDrawer::pageChanged( int index )
{
	if( _tabBar->tabText(index) == "+" )
	{
		if( _wheelSwitch )
			_tabBar->setCurrentIndex( index-1 );
		else
			addNewPage();

		return;
	}
	
	setScene( _pageDrawerList.at(index) );
}

void DocumentDrawer::tabBarContextMenu( const QPoint& point )
{
	_menu.exec( mapToGlobal(point) );
}

void DocumentDrawer::addNewPage()
{
	int pageIndex = _tabBar->count() - 1;

	_pageDrawerList.append( new PageDrawer() );

	_tabBar->insertTab( pageIndex, QString("Page %1").arg(_pageCount) );
	_tabBar->setCurrentIndex( pageIndex );

	_pageCount++;
}

void DocumentDrawer::removePage()
{
	_tabBar->disconnect();

	int index = _tabBar->currentIndex();

	_tabBar->removeTab( index );

	delete _pageDrawerList[ index ];
	_pageDrawerList.removeAt( index );

	connect( _tabBar, SIGNAL(currentChanged(int)), this, SLOT(pageChanged(int)) );
	connect( _tabBar, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(tabBarContextMenu(const QPoint&)) );

	if( _pageDrawerList.isEmpty() )
		addNewPage();
	else
		_tabBar->setCurrentIndex( index-1 );
}

void DocumentDrawer::setPageText()
{
	int index = _tabBar->currentIndex();

	QString newText = QInputDialog::getText( this, "Page Text", "" );

	_tabBar->setTabText( index, newText );
}

void DocumentDrawer::removeEmptyPages()
{
	_tabBar->disconnect();
	
	for( int index = 0; index < _pageDrawerList.count(); )
	{
		PageDrawer* pageDrawer = _pageDrawerList.at( index );
		if( pageDrawer->items().count() == 1 )
		{
			delete pageDrawer;
			_pageDrawerList.removeAt( index );

			_tabBar->removeTab( index );
		}
		else
			index++;
	}

	connect( _tabBar, SIGNAL(currentChanged(int)), this, SLOT(pageChanged(int)) );
	connect( _tabBar, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(tabBarContextMenu(const QPoint&)) );

	if( _pageDrawerList.isEmpty() )
		addNewPage();
	else
		_tabBar->setCurrentIndex( 0 );
}

void DocumentDrawer::removeAllPages()
{
	_tabBar->disconnect();

	for( int i=0, j=0; i < _tabBar->count()-1; j++ )
	{
		_tabBar->removeTab( 0 );

		delete _pageDrawerList[j];
	}

	_pageDrawerList.clear();
}
