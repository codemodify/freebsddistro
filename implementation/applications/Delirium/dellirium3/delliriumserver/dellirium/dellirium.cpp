
// Qt headers
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtGui/QStatusBar>
#include <QtGui/QLabel>

// local headers
#include "dellirium.h"
#include "mapeditor/mapeditor.h"
#include "2drenderer/2drenderer.h"

#define SERVER_PORT 8880

Dellirium::Dellirium():
	QMainWindow()
{
	setWindowTitle( "Dellirium - the killer game" );

	initMenu();
	
	initStatusBar();
	
	initNetworkLayer();
	
	initRenderer();
}

Dellirium::~Dellirium()
{
}

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

void Dellirium::initStatusBar()
{
	_statusBar = new QStatusBar();

	_peerOnPort = new QLabel();
	_weOnPort = new QLabel();
	
	_statusBar->addWidget( _peerOnPort );
	_statusBar->addWidget( _weOnPort );

	setStatusBar( _statusBar );
}

void Dellirium::initRenderer()
{
	_renderer = new TwoDRenderer();
	_renderer->setRuningMode( TwoDRenderer::Play );
	connect( _renderer, SIGNAL(userDidMoved(QByteArray)), this, SLOT(slot_DataAboutMovesMustBeSent(QByteArray)) );
	
	setCentralWidget( _renderer );
}

void Dellirium::slot_HelpMe()
{}

void Dellirium::initNetworkLayer()
{
	// creating a server
	_tcpServer = new QTcpServer();
	_tcpServer->listen( QHostAddress::Any, SERVER_PORT );
	connect( _tcpServer, SIGNAL(newConnection()), this, SLOT(slot_PeerConnected()) );

	// creating a client
	_tcpSendSocket = new QTcpSocket();
	connect( _tcpSendSocket, SIGNAL(connected()), this, SLOT(slot_ConectedToPeer()) );
}

void Dellirium::slot_PeerConnected()
{
	_tcpReadSocket = _tcpServer->nextPendingConnection();
	connect( _tcpReadSocket, SIGNAL(readyRead()), this, SLOT(slot_DataReceivedFromPeer()) );
	
	_receiverOnPort->setText( QString("%1:%2").arg(_tcpReadSocket->peerName()).arg(_tcpReadSocket->peerPort()) );
}

void Dellirium::slot_ConnectToServer()
{
	// asking user for the params and build the sockets to have the networkin' layer running
	QString address = QInputDialog::getText( this, "", "Enter the server to connect to:" );
	if( true == address.isEmpty() )
		return;

	_tcpSendSocket->connectToHost( QHostAddress(address), SERVER_PORT, QTcpSocket::WriteOnly );
}

void Dellirium::slot_PeerConnected()
{
	_tcpReadSocket = _tcpServer->nextPendingConnection();
	connect( _tcpReadSocket, SIGNAL(readyRead()), this, SLOT(slot_DataReceivedFromPeer()) );
	
	_receiverOnPort->setText( QString("%1:%2").arg(_tcpReadSocket->peerName()).arg(_tcpReadSocket->peerPort()) );
}

void Dellirium::slot_PeerConnected()
{
	_tcpSendSocket = _tcpServer->nextPendingConnection();
	
	_senderOnPort->setText( QString("%1:%2").arg(_tcpSendSocket->peerName()).arg(_tcpSendSocket->peerPort()) );
}

void Dellirium::slot_DataReceivedAboutMoves()
{
	//_tcpReadSocket->read();
	QByteArray moveContent;
	_renderer->addMove( moveContent );
}

void Dellirium::slot_DataAboutMovesMustBeSent( QByteArray moveContent )
{
	// here we are sending data over network
	_tcpSendSocket->write( moveContent );
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
