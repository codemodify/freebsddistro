
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

// ISP headers
#include <isp/components/components.h>
using namespace IspComponents::Logging;

// local headers
#include "collaborativeschemaserver.h"

Q_EXPORT_PLUGIN2( collaborativeSchemaServer_h, CollaborativeSchemaServer )

CollaborativeSchemaServer::CollaborativeSchemaServer():Daemon(){}

CollaborativeSchemaServer::~CollaborativeSchemaServer(){}

bool CollaborativeSchemaServer::initialize()
{
    logInfo( this, QString("init module %1 ---- ---- ---- ---- ---- ----").arg(getDescription()), true );
    
//	QString settingsFilePath = _pathToModule + ".settings";
//	logInfo( this, QString("settings file to use: %1").arg(settingsFilePath), true );
	
	logInfo( this, "CollaborativeSchemaServer is up and ready for serving" );		
	return true;
}

bool CollaborativeSchemaServer::shutdown()
{
	logInfo( this, "CollaborativeSchemaServer is going down" );
    
    return true;
}

QString CollaborativeSchemaServer::getDescription()
{
	return "Internet Server Platform - CollaborativeSchemaServer module";
}

QString CollaborativeSchemaServer::getName()
{
    return "CollaborativeSchemaServer";
}

void CollaborativeSchemaServer::slot_newIncommingConnection( int socketDescriptor )
{
    QTcpSocket* tcpSocket = new QTcpSocket();

	bool result = tcpSocket->setSocketDescriptor( socketDescriptor );
    if( false == result )
    {
        logError( this, QString("failed to set socket descriptor: %1").arg(tcpSocket->errorString()) );
        delete tcpSocket;
        return;
    }
    
    logInfo( this, QString("new client [%1:%2]").arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort()), true );
    connect( tcpSocket, SIGNAL(readyRead()), this, SLOT(slot_IncommingData()) );
    connect( tcpSocket, SIGNAL(disconnected()), this, SLOT(slot_ClientDiconnected()) );
    
    _clients.append( tcpSocket );
}

void CollaborativeSchemaServer::slot_IncommingData()
{
    QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>( sender() );
    if( 0 == tcpSocket )
        return;
    
    QByteArray byteArray = tcpSocket->readAll();
    logInfo( this, QString("incomming data from [%1:%2]").arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort()), true );
    logInfo( this, QString("data: %1").arg(byteArray.data()), true );
    
    foreach( QTcpSocket* client, _clients )
        if( client != tcpSocket )
            client->write( byteArray );
}

void CollaborativeSchemaServer::slot_ClientDiconnected()
{
    QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>( sender() );
    if( 0 == tcpSocket )
        return;

    logInfo( this, QString("client diconnected [%1:%2]").arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort()), true );
    _clients.removeOne( tcpSocket );
    tcpSocket->deleteLater();    
}
