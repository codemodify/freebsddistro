
// local headers
#include "TcpServer.h"


TcpServer::TcpServer() :
    QTcpServer()
{}

QTcpSocket* TcpServer::getSoketFromDescriptor( int socketDescriptor )
{
	QTcpSocket* tcpSocket = new QTcpSocket();
	
	// set the socket descriptor and return the socket
	if( tcpSocket->setSocketDescriptor(socketDescriptor) )
		return tcpSocket;

	// the socket descriptor was not valid
	delete tcpSocket;

	return 0;
}

/*
QSslSocket* TcpServer::getSecureSoketFromDescriptor( int socketDescriptor )
{
	QSslSocket* sslSocket = new QSslSocket();

	if( sslSocket->setSocketDescriptor(socketDescriptor) )
		return sslSocket;

	delete sslSocket;

	return 0;	
}
*/

void TcpServer::incomingConnection( int socketDescriptor )
{
    emit newIncommingConnection( socketDescriptor );
}
