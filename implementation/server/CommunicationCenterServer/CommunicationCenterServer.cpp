
// Qt headers
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtCore/QTimer>

// local headers
#include "CommunicationCenterServer.h"

Q_EXPORT_PLUGIN2( CommunicationCenterServer, CommunicationCenterServer )


CommunicationCenterServer::CommunicationCenterServer() :
	SocketDaemon()
{}

CommunicationCenterServer::~CommunicationCenterServer()
{}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//	implement inherited stuff
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
bool CommunicationCenterServer::initialize()
{
	Logging::logInfo( this, "CommunicationCenterServer::initialize()" );

	Bottles::registerForBottles( this, _idInsideBottlesWorld );	
	Bottles::createBottle( this, _idInsideBottlesWorld, "General", _rootBottleId );
    
	return true;
}

bool CommunicationCenterServer::shutdown()
{
	Logging::logInfo( this, "CommunicationCenterServer::shutdown()" );

	Bottles::unRegisterForBottles( this );

	return true;
}

QString CommunicationCenterServer::getDescription()
{
	return "Internet Server Platform - CommunicationCenterServer module";
}

QString CommunicationCenterServer::getName()
{
    return "CommunicationCenterServer";
}

void CommunicationCenterServer::connectionRequest( QTcpSocket* socket )
{
	Bottles::Peer	peer;
				peer._socket	= socket;

	Bottles::ID peerId;
	Bottles::addPeerToBottle( this, _rootBottleId, peer, peerId );

	// on peer connect send its ID and user list
	QByteArray	buffer;
				buffer.append( QByteArray::number(eSystemYourId) );
				buffer.append( '\0' );
				buffer.append( peerId.toString().toAscii() );
	Bottles::sendToPeer( this, _rootBottleId, peerId, buffer );

	// the timer here is a workaround, if to call the method directly it 
	// will not send data, there is a need for a delay
	QTimer::singleShot( 1000, this, SLOT(sendCurrentUserListToPeers()) );
}

bool peersCompareEngineBySocket( const Bottles::Peer& peer, void* searchData );
void CommunicationCenterServer::disconnectionRequest( QTcpSocket* socket )
{
	Bottles::PeerList peerList;

	Bottles::setPeersCompareEngine( peersCompareEngineBySocket );
	Bottles::searchPeer( this, _rootBottleId, (void*)socket, peerList );

	if( !peerList.isEmpty() )
	{
		Bottles::ID peerId = peerList.begin().key();
		Bottles::removePeerFromBottle( this, _rootBottleId, peerId );
	}

	sendCurrentUserListToPeers();
}

void CommunicationCenterServer::dataRequest( QByteArray& request, QByteArray& response, bool& requestIsValid )
{
	QList<QByteArray> dataAsList = request.split( '\0' );
	MessageType messageType = (MessageType) dataAsList.at(0).toInt();

	Logging::logInfo( this, "---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----" );
	Logging::logInfo( this, "---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----" );
	Logging::logInfo( this, QString("size : %1").arg(request.size()  ) );
	Logging::logInfo( this, QString("type : %1").arg(messageType     ) );

	switch( messageType )
	{
		case eText:
			Bottles::sendToAllPeers( this, _rootBottleId, request );
			break;

		default:
			break;
	}

	requestIsValid = true;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//	helpers
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void CommunicationCenterServer::sendCurrentUserListToPeers()
{
	QByteArray	buffer;
				buffer.append( QByteArray::number(eUserList) );
				buffer.append( '\0' );

	Bottles::PeerList peerList;
	Bottles::getPeers( this, _rootBottleId, peerList );

	Bottles::PeerList::const_iterator begin = peerList.begin();
	Bottles::PeerList::const_iterator end   = peerList.end();
	Bottles::PeerList::const_iterator it    = begin;

	for( ; it != end; ++it )
	{
		Bottles::ID peerId = it.key();

		QString	shortUserInfo("%1/%2");
				shortUserInfo = shortUserInfo.arg( peerId );
				shortUserInfo = shortUserInfo.arg( peerId );

		buffer.append( shortUserInfo );
		buffer.append( "#" );
	}

	Bottles::sendToAllPeers( this, _rootBottleId, buffer );
}

bool peersCompareEngineBySocket( const Bottles::Peer& peer, void* searchData )
{
	if( peer._socket == (QTcpSocket*)searchData )
		return true;

	return false;
}
