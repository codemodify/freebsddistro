
#include <QtGui/QMdiArea>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QInputDialog>
#include <QtGui/QGraphicsView>
#include <QtNetwork/QTcpSocket>

#include "collaborativeschema.h"
#include "graphicscene/graphicscene.h"

CollaborativeSchema::CollaborativeSchema() :
	QMainWindow()
{
	// status bar
	setStatusBar( new QStatusBar() );
	connect( this, SIGNAL(status(QString)), this, SLOT(slot_StatusChanged(QString)) );
	
	// white board
	GraphicsScene* graphicsScene = new GraphicsScene();
	connect( graphicsScene, SIGNAL(sendObject(QByteArray)), this, SLOT(slot_SendSchemaToPeers(QByteArray)) );
	connect( this, SIGNAL(objectAvailable(QByteArray)), graphicsScene, SLOT(addObject(QByteArray)) );

	QGraphicsView* graphicsView = new QGraphicsView();
	graphicsView->setScene( graphicsScene );
	
	setCentralWidget( graphicsView );
	
	// build the menu
	setMenuBar( new QMenuBar() );
	menuBar()->addAction( "connect", this, SLOT(slot_Connect()) );
	menuBar()->addAction("|")->setEnabled( false );
	menuBar()->addAction( "clear white-board", graphicsScene, SLOT(slot_Clear()) );
	menuBar()->addAction("|")->setEnabled( false );
	menuBar()->addAction( "send my white-board-snapshot", graphicsScene, SLOT(slot_SendSnapshot()) );
}

CollaborativeSchema::~CollaborativeSchema()
{}

void CollaborativeSchema::slot_StatusChanged( QString statusString )
{
	statusBar()->showMessage( statusString );
}

void CollaborativeSchema::slot_Connect()
{
	_connection = new QTcpSocket();
	
	connect( _connection, SIGNAL(connected()), this, SLOT(slot_ConnectionConnected()) );
	connect( _connection, SIGNAL(disconnected()), this, SLOT(slot_ConnectionDisconnected()) );
	connect( _connection, SIGNAL(readyRead()), this, SLOT(slot_ConnectionReadyRead()) );

	QString ipAsString = QInputDialog::getText( this, "", "enter IP address for the server collaborative-schema server" );
	if( ipAsString.isEmpty() )
		return;
		
	_connection->connectToHost( ipAsString, 8099 );	
}

void CollaborativeSchema::slot_ConnectionConnected()
{
	emit status( "connected" );
}

void CollaborativeSchema::slot_ConnectionDisconnected()
{
	emit status( "disconnected" );
}

void CollaborativeSchema::slot_ConnectionReadyRead()
{
	QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>( sender() );
	if( 0 == tcpSocket )
		return;
	
	emit status( "recieving data" );
	
	QByteArray byteArray = tcpSocket->readAll();
	emit objectAvailable( byteArray );
}

void CollaborativeSchema::slot_SendSchemaToPeers( QByteArray objectToSend )
{
	if( _connection.isNull() )	
		return;
		
	if( !_connection->isOpen() )
		return;
				
	_connection->write( objectToSend );
}
