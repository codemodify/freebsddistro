
// Qt headers
#include <QtCore/QByteArray>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QtNetwork/QHostAddress>

// local headers
#include "CommunicationCenterServerClient.h"



CommunicationCenterServerClient::CommunicationCenterServerClient() :
	QObject(),
	_mainChannel( 0 ),
	_fileChannel( 0 )
{}

void CommunicationCenterServerClient::setIpPort( QString ip, quint16 port )
{
	_ip = ip;
	_port = port;

	emit netowrkStatusUpdate( eConnecting );

	if( 0 != _mainChannel )
	{
		delete _mainChannel;
		_mainChannel = 0;
	}

	if( 0 != _fileChannel )
	{
		delete _fileChannel;
		_fileChannel = 0;
	}

	_mainChannel = new QTcpSocket();

	connect( _mainChannel, SIGNAL(connected()                        ), this, SLOT(mainChannelConnected()                        ) );
	connect( _mainChannel, SIGNAL(disconnected()                     ), this, SLOT(mainChannelDisconnected()                     ) );
	connect( _mainChannel, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(mainChannelError(QAbstractSocket::SocketError)) );
	connect( _mainChannel, SIGNAL(readyRead()                        ), this, SLOT(mainChannelReadData()                         ) );

	_mainChannel->connectToHost( _ip, _port );
}

QString CommunicationCenterServerClient::getId()
{
	return _id;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- api
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void CommunicationCenterServerClient::sendTextMessage( QString& message )
{
	if( 0 == _mainChannel )
		return;

	int messageType = eText;

	QByteArray	buffer;
				buffer.append( QByteArray::number(messageType) );
				buffer.append( '\0' );
				buffer.append( _id.toAscii() );
				buffer.append( '\0' );
				buffer.append( message );

	_mainChannel->write( buffer );
}

void CommunicationCenterServerClient::sendFileMessage( QString& filePath )
{
/*
	qDebug( "CommunicationCenterServerClient::sendFileMessage()" );

	if( 0 == _mainChannel )
		return;

	int messageType = eFile;
	int length      = filePath.length();

	QByteArray	buffer;
				buffer.append( (const char*)&messageType , sizeof(int) );
				buffer.append( (const char*)&length      , sizeof(int) );
				buffer.append( filePath );

	qDebug( QString("buffer    : %1").arg(buffer.data()).toAscii().data() );
	qDebug( QString("bufferSize: %1").arg(buffer.size()).toAscii().data() );
	
	_mainChannel->write( buffer );
*/
}

void CommunicationCenterServerClient::getUserInfo( QString& userId )
{
	qDebug( "CommunicationCenterServerClient::getUserInfo()" );
	
	if( 0 == _mainChannel )
		return;

	int messageType = eUserInfo;
	int length      = userId.length();

	QByteArray	buffer;
				buffer.append( (const char*)&messageType , sizeof(int) );
				buffer.append( (const char*)&length      , sizeof(int) );
				buffer.append( userId );

	qDebug( QString("userId: %1")             .arg(userId         ).toAscii().data() );
	qDebug( QString("size: %1")               .arg(userId.length()).toAscii().data() );
	qDebug( QString("total message size : %1").arg(buffer.size()  ).toAscii().data() );

	_mainChannel->write( buffer );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- network delegates
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void CommunicationCenterServerClient::mainChannelConnected()
{
	emit netowrkStatusUpdate( eOn );
}

void CommunicationCenterServerClient::mainChannelDisconnected()
{
	setIpPort( _ip, _port );
}

void CommunicationCenterServerClient::mainChannelError( QAbstractSocket::SocketError )
{
	QTimer::singleShot( 2000, this, SLOT(mainChannelDisconnected()) );
}

void CommunicationCenterServerClient::mainChannelReadData()
{
	QByteArray buffer = _mainChannel->readAll();

	QList<QByteArray> dataAsList = buffer.split( '\0' );

	MessageType messageType = (MessageType) dataAsList.at(0).toInt();
	qDebug( QString("messageType: %1").arg(messageType).toAscii().data() );

	switch( messageType )
	{
		case eSystemYourId:
			_id = dataAsList.at( 1 );
			break;

		case eSystemText:
			emit newSystemMessage( "System", dataAsList.at(1) );
			break;

		case eText:
			emit newTextMessage( dataAsList.at(1), dataAsList.at(2) );
			break;

		case eMail:
		case eAudio:
		case eVideo:
			break;

		case eFile:
			break;

		case eUserList:
			{
				ShortUserInfoList*	 shortUserInfoList = new ShortUserInfoList();

				QString userList1 = dataAsList.at(1);

				QStringList userList2 = userList1.split( "#" );
				foreach( QString user, userList2 )
				{
					QStringList shortUserInfoAsString = user.split( "/" );

					ShortUserInfo	shortUserInfo;
								shortUserInfo._id   = shortUserInfoAsString. value( 0 );
								shortUserInfo._nick = shortUserInfoAsString. value( 1 );

					qDebug( shortUserInfo._id.toAscii().data() );
					qDebug( shortUserInfo._nick.toAscii().data() );

					shortUserInfoList->append( shortUserInfo );
				}

				emit userListAvailable( shortUserInfoList );
			}
			break;

		case eUserInfo:
			break;

		case eFilePort:
/*			{

			int fileIp   = buffer.left( length );

			length =	buffer.left( sizeof(int) ).toInt();
					buffer.remove( 0, sizeof(int) );

			int filePort = buffer.left( length ).toInt();
			
			_fileChannel = new QTcpSocket();

			connect( _fileChannel, SIGNAL(connected()                        ), this, SLOT(fileChannelConnected()                        ) );
			connect( _fileChannel, SIGNAL(disconnected()                     ), this, SLOT(fileChannelDisconnected()                     ) );
			connect( _fileChannel, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(fileChannelError(QAbstractSocket::SocketError)) );
			connect( _fileChannel, SIGNAL(readyRead()                        ), this, SLOT(fileChannelReadData()                         ) );

			_fileChannel->connectToHost( fileIp, filePort );
			}
*/
			break;

		default:
			qDebug( "Unknown Message" );
			break;
	}
}
