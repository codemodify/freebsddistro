
// Qt headers
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QHostAddress>
#include <QtCore/QStringList>

// local headers
#include "dnsresolver.h"
#include "message/message.h"
#include "message/header/header.h"

DnsResolver::DnsResolver( bool isSocketTcpType ) :
	_isSocketTcpType(isSocketTcpType)
{}

DnsResolver::~DnsResolver()
{}

bool DnsResolver::requestARecord( QString& nameServer, QString& domainName, QStringList& ipAddresses, QString& errorString )
{
	// checking if the passed 'domainName' is not an IP already
	QHostAddress hostAddress;
	if( true == hostAddress.setAddress(domainName) )
	{
		ipAddresses.append( nameServer );
		return true;
	}
	
	// getting here means we have to resolve that thing
	QHostInfo hostInfo = QHostInfo::fromName( domainName );
	if( QHostInfo::NoError != hostInfo.error() )
	{
		errorString = hostInfo.errorString();
		return false;
	}
	
	QList<QHostAddress> hostAddresses = hostInfo.addresses();	
	foreach( QHostAddress hostAddress, hostAddresses )
	{
		ipAddresses.append( hostAddress.toString() );	
	}
	
	return true;
}

bool DnsResolver::requestMXRecord( QString& nameServer, QString& domainName, QStringList& mailServers, QString& errorString )
{
	bool result = false;
	
	// build a message that will contain a DNS request for the desired domain-name
	Message question;
	question.buildQueryMessagetForMXRecord( domainName.toAscii() );
	
	// send the message and receive the response 
	QByteArray questionAsByteArray = question.toByteArray();
	QByteArray responseAsByteArray;
	if( true == _isSocketTcpType )
		result = sendUsingTcp( nameServer, questionAsByteArray, responseAsByteArray, errorString );
	else
		result = sendUsingUdp( nameServer, questionAsByteArray, responseAsByteArray, errorString );
		
	if( false == result )
		return false;
		
	// once we get here then the server answered with some trash as response, now we need to figure out if we agree to buy that
	Message response;
	result = response.fromByteArray( responseAsByteArray );
	if( result == false )
	{
		errorString = "received response was not a DNS response";
		return false;
	}
	
	if( question._header->getID() != response._header->getID() )
	{
		errorString = "received response has a different ID";
		return false;
	}

	if( question._header->getOpcode() != response._header->getOpcode() )
	{
		errorString = "received response has a different Opcode";
		return false;
	}
	
	if( Header::QR::Response != response._header->getQR() )
	{
		errorString = "received data from socket is not a response message from DNS system";
		return false;
	}
		
	int responseCode = response._header->getRcode();
	if( Header::RCODE::NoError != responseCode )
	{
		switch( responseCode )
		{
			case Header::RCODE::FormatError: {
					// pretends to be dumb while tries to DDoS us
					errorString = "in response we have Header::RCODE::FormatError";
				} break;
			case Header::RCODE::ServerFailure: {
					// pretends to be dumb while tries to DDoS us
					errorString = "in response we have Header::RCODE::ServerFailure";
				} break;
			case Header::RCODE::NameError: {
					// pretends to be dumb while tries to DDoS us
					errorString = "in response we have Header::RCODE::NameError";
				} break;
			case Header::RCODE::NotImplemented: {
					// pretends to be dumb while tries to DDoS us
					errorString = "in response we have Header::RCODE::NotImplemented";
				} break;
			case Header::RCODE::Refused: {
					// server is in bad mood (read - admin pretends that his server is secure)
					errorString = "in response we have Header::RCODE::Refused";
					return false;
				} break;
			default:
				return false;
		}
	}

	if( Header::RA::RecursionAvailable != response._header->getRA() )
	{
		// the server does not support reqursive queries
		errorString += ";      also the server does not support reqursive queries";
	}
	
	if( Header::TC::Truncation != response._header->getTC() )
	{
		// response message is truncated, retrying by TCP
		errorString += ";      also response message is truncated,";
	}
	
	response.getServers( mailServers );
	
	return true;
}

bool DnsResolver::sendUsingTcp( QString& nameServer, QByteArray& messageToSend, QByteArray& responseMessage, QString& errorString )
{
	bool result = false;
//		// connect to the socket
//		tcpSocket->connectToHost( nameServer, 53 );
//		result = tcpSocket->waitForConnected();
//		if( false == result )
//			return false;
//
//		// sending the request for data to the DNS server	
//		tcpSocket->write( messageAsByteArray );
//		result = tcpSocket->waitForBytesWritten( -1 );
//		if( false == result )
//		{
//			const char* str = tcpSocket->errorString().toAscii().data();
//			return false;
//		}
//		
//		// getting the response from server
//		result = tcpSocket->waitForReadyRead( -1 );
//		if( false == result )
//		{
//			int e = tcpSocket->error();
//			const char* str = tcpSocket->errorString().toAscii().data();
//			return false;
//		}
//	
//		QByteArray byteArray = tcpSocket->readAll();
//		message.fromByteArray( byteArray );
//		
//		delete tcpSocket;
	
	return result;
}

bool DnsResolver::sendUsingUdp( QString& nameServer, QByteArray& messageToSend, QByteArray& responseMessage, QString& errorString )
{
	bool result = false;

	// connect to the DNS server
	QUdpSocket udpSocket;
	udpSocket.connectToHost( nameServer, 53 );
	result = udpSocket.waitForConnected();
	if( false == result )
	{
		errorString = "socket timedout";
		return false;
	}
	
	// send data
	qint64 writtenSize = udpSocket.write( messageToSend );
	if( -1 == writtenSize )
	{
		errorString = udpSocket.errorString();
		return false;
	}
		
	// wait for data
	result = udpSocket.waitForReadyRead( 30000 );
	if( false == result )
	{
		errorString = udpSocket.errorString();
		return false;
	}

	// read data
	while( udpSocket.hasPendingDatagrams() )
	{
		QByteArray datagram;
		datagram.resize( udpSocket.pendingDatagramSize() );
		udpSocket.readDatagram( datagram.data(), datagram.size() );
		
		responseMessage += datagram;
	}
	
	return true;
}
