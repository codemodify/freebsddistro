
#include <QtGui/QTabWidget>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QToolBar>

#include <QtWebKit/QWebView>

#include "WebBrowser.h"


WebBrowser::WebBrowser() :
	QMainWindow()
{
	initGui();
}

WebBrowser::~WebBrowser()
{}

void WebBrowser::initGui()
{
	// setup the tool bar
	_back		= new QPushButton( "|<< Back" );
	_stop		= new QPushButton( "stop" );
	_reload		= new QPushButton( "reload" );
	_forward	= new QPushButton( "Forward >>|" );
	_newTab		= new QPushButton( "nTab" );
	_closeTab	= new QPushButton( "cTab" );
	_address	= new QLineEdit();
	_go		= new QPushButton( "Go" );
	
	QToolBar* toolBar = new QToolBar();
	toolBar->addWidget( _back	);
	toolBar->addWidget( _stop	);
	toolBar->addWidget( _reload	);
	toolBar->addWidget( _forward	);
	toolBar->addSeparator();
	toolBar->addWidget( _newTab	);
	toolBar->addWidget( _closeTab	);
	toolBar->addWidget( _address	);
	toolBar->addWidget( _go		);

	connect( _newTab, SIGNAL(released()), this, SLOT(newTab()) );
	connect( _closeTab, SIGNAL(released()), this, SLOT(closeTab()) );
	connect( _go, SIGNAL(released()), this, SLOT(goToAddress()) );
	
	this->addToolBar( toolBar );

	// setup the tab area
	_tabWidget = new QTabWidget();
	connect( _tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabActivated(int)) );

	setCentralWidget( _tabWidget );
}

void WebBrowser::newTab()
{
	QWebView* webView = new QWebView();
	connect( webView, SIGNAL(titleChanged(const QString&)), this, SLOT(titleChanged(const QString&)) );
	connect( webView, SIGNAL(urlChanged(const QUrl&)), this, SLOT(urlChanged(const QUrl&)) );

	_tabWidget->addTab( webView, "" );
	_tabWidget->setCurrentWidget( webView );

	webView->setUrl( QUrl("http://www.google.com") );
}

void WebBrowser::closeTab()
{
	QWidget* widget = _tabWidget->currentWidget();
	QWebView* webView = qobject_cast<QWebView*>( widget );
	if( 0 == webView )
		return;

	_tabWidget->removeTab( _tabWidget->currentIndex() );
	delete webView;
}

void WebBrowser::goToAddress()
{
	QWidget* widget = _tabWidget->currentWidget();
	QWebView* webView = qobject_cast<QWebView*>( widget );
	if( 0 == webView )
		return;

	webView->setUrl( QUrl(_address->text()) );
}

void WebBrowser::tabActivated( int tabIndex )
{
	QWidget* widget = _tabWidget->currentWidget();
	QWebView* webView = qobject_cast<QWebView*>( widget );
	if( 0 == webView )
		return;

	_back->disconnect();
	_stop->disconnect();
	_reload->disconnect();
	_forward->disconnect();

	_address->setText( webView->url().toString() );
	connect( _back, SIGNAL(released()), webView, SLOT(back()) );
	connect( _stop, SIGNAL(released()), webView, SLOT(stop()) );
	connect( _reload, SIGNAL(released()), webView, SLOT(reload()) );
	connect( _forward, SIGNAL(released()), webView, SLOT(forward()) );
}

void WebBrowser::titleChanged( const QString& newTitle )
{
	_tabWidget->setTabText( _tabWidget->indexOf(qobject_cast<QWidget*>(sender())), newTitle );
}

void WebBrowser::urlChanged( const QUrl& url )
{
	if( _tabWidget->currentWidget() == sender() )
		_address->setText( url.toString() ) ;
}

