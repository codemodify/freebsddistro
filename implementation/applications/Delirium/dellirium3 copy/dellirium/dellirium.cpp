
// qt headers
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>

// local headers
#include "dellirium.h"
#include "mapeditor/mapeditor.h"
#include "2drenderer/2drenderer.h"

#define SERVER_PORT_TO_SEND_DATA 8880
#define SERVER_PORT_TO_READ_DATA 8881

Dellirium::Dellirium():
	QMainWindow()
{
	setWindowTitle( "Dellerium - the killer game" );

	initMenu();
	
	_renderer = new TwoDRenderer();
	_renderer->setRuningMode( TwoDRenderer::Play );
	setCentralWidget( _renderer );
}

Dellirium::~Dellirium()
{}

void Dellirium::initMenu()
{
	// build here the menu-bar sections
	QMenu* deleriumMenu = new QMenu( "Delerium" );
	deleriumMenu->addAction( "Help Me!", this, SLOT(slot_HelpMe()) );
	deleriumMenu->addSeparator();
	deleriumMenu->addAction( "Play client mode", this, SLOT(slot_PlayClient()) );
	deleriumMenu->addSeparator();
	deleriumMenu->addAction( "Play server mode", this, SLOT(slot_PlayServer()) );
	deleriumMenu->addAction( "Restart the Game", this, SLOT(slot_RestartGame()) );
	deleriumMenu->addSeparator();
	deleriumMenu->addAction( "Quit", this, SLOT(slot_Quit()) );
	
	QMenu* mapEditorMenu = new QMenu( "Map" );
	mapEditorMenu->addAction( "Create/Open Map", this, SLOT(slot_CreateOpenMap()) );
	mapEditorMenu->addAction( "Send Map to Client", this, SLOT(slot_SendMapToClient()) );
	mapEditorMenu->addAction( "Choose Map to Play", this, SLOT(slot_ChooseMapToPlay()) );
	
	// add the menu bar sections to the main window
	QMenuBar* menuBar = new QMenuBar();
	menuBar->addMenu( deleriumMenu );
	menuBar->addMenu( mapEditorMenu );
	setMenuBar( menuBar );
}

void Dellirium::slot_HelpMe()
{}

void Dellirium::destroyNetworkingLayer()
{
	// delete servers if any
	if( false == _tcpReadServer.isNull() )
		delete _tcpReadServer;

	if( false == _tcpSendServer.isNull() )
		delete _tcpSendServer;

	// delete clients if any
	if( false == _tcpSendSocket.isNull() )
		delete _tcpSendSocket;

	if( false == _tcpReadSocket.isNull() )
		delete _tcpReadSocket;
}

void Dellirium::slot_PlayClient()
{
	// destroy the current networking objects if any
	destroyNetworkingLayer();

	// asking user for the params and build the sockets to have networkin'
	QString address = QInputDialog::getText( this, "", "Enter the server to connect to:" );
	if( true == address.isEmpty() )
		return;

	_tcpSendSocket = new QTcpSocket();
	_tcpSendSocket.setPeerName( address );
	_tcpSendSocket.setPeerPort( SERVER_PORT_TO_SEND_DATA );
	_tcpSendSocket.open( QTcpSocket::WriteOnly );

	_tcpReadSocket = new QTcpSocket();
	_tcpReadSocket.setPeerName( address );
	_tcpReadSocket.setPeerPort( SERVER_PORT_TO_READ_DATA );
	_tcpReadSocket.open( QTcpSocket::ReadOnly );
	
	connect( _tcpReadSocket, SIGNAL(), this, SLOT(slot_DataReceivedAboutMoves()) );

}

void Dellirium::slot_PlayServer()
{
	// destroy the current networking objects if any
	destroyNetworkingLayer();

	_tcpReadServer = new QTcpServer();
	connect( _tcpReadServer, SIGNAL(newConnection()), this, SLOT(slot_SenderConnected()) );
	_tcpReadServer.listen( QHostAddress::Any, SERVER_PORT_TO_SEND_DATA );
	
	_tcpSendServer = new QTcpServer();
	connect( _tcpSendServer, SIGNAL(newConnection()), this, SLOT(slot_ReaderConnected()) );
	_tcpSendServer.listen( QHostAddress::Any, SERVER_PORT_TO_READ_DATA );	
}

void Dellirium::slot_SenderConnected()
{
	_tcpReadSocket = _tcpReadServer.nextPendingConnection();
	connect( _tcpReadSocket, SIGNAL(), this, SLOT(slot_DataReceivedAboutMoves()) );
}

void Dellirium::slot_ReaderConnected()
{
	_tcpSendSocket = _tcpSendServer.nextPendingConnection();
}

void Dellirium::slot_DataReceivedAboutMoves()
{
	QByteArray moveContent;
	_renderer->addMove( moveContent );
}

void Dellirium::slot_RestartGame()
{}

void Dellirium::slot_Quit()
{
	close();
}

void Dellirium::slot_CreateOpenMap()
{
	MapEditor* mapEditor = new MapEditor();
	mapEditor->show();
}

void Dellirium::slot_SendMapToClient()
{}

void Dellirium::slot_ChooseMapToPlay()
{
	QString filePath = QFileDialog::getOpenFileName( this, "Open Map", "/", "Dellerium Maps (*.*)");
	if( false == filePath.isEmpty() )
	{
		_renderer->loadMap( filePath );
	}
}
