
// local headers
#include "InternetServerPlatform.h"


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// related to the actions when a new connection occures
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
bool InternetServerPlatform::isPeerInBlockList( QTcpSocket* tcpSocket, BlockFrom& blockFrom )
{
	IpAddressAsString ipAsString = tcpSocket->peerAddress().toString();
	IpAddressAsNumber ipAsNumber = tcpSocket->peerAddress().toIPv4Address();

	// check if peer is in block-list
	if( blockFrom._blockedIpList.contains(ipAsString) )
	{
		dumpClient( tcpSocket, false );
		applyIspMood( tcpSocket );

		return true;
	}

	// check if peer is in block-list from the monitored file
	if( blockFrom._blockedIpListFromFile.contains(ipAsString) )
	{
		dumpClient( tcpSocket, false );
		applyIspMood( tcpSocket );

		return true;
	}

	// check if peer is in block-interval
	QMap< IpAddressAsNumber, IpAddressAsNumber >::const_iterator currentIp   = blockFrom._blockedIpIntervals.constBegin();
	QMap< IpAddressAsNumber, IpAddressAsNumber >::const_iterator endInterval = blockFrom._blockedIpIntervals.constEnd();
	while( currentIp != endInterval )
	{
		if( (ipAsNumber>=currentIp.key()) && (ipAsNumber<=currentIp.value()) )
		{
			if( false == blockFrom._exceptList.contains(ipAsNumber) )
			{
				// the peer was found in the blocked intervals
				dumpClient( tcpSocket, false );
				applyIspMood( tcpSocket );
				return true;
			}
			return false;
		}
		
		++currentIp;
 	}
	
	return false;
}

void InternetServerPlatform::applyIspMood( QTcpSocket* tcpSocket )
{
	switch( _ispSettings._mood._active )
	{
		case Mood::Business:
		case Mood::Hacker:
		{
			QFile file( _ispSettings._mood._business._explanatoryPage );
			if( file.open(QFile::ReadOnly) )
			{
				tcpSocket->write( file.readAll() );
				file.close();
			}
			tcpSocket->close();		
		}
		break;
		
		case Mood::Hole:
		case Mood::Brutal:
		{
			tcpSocket->abort();
		}
		break;
		
		default:
			tcpSocket->abort();			
	}
}

void InternetServerPlatform::slot_newConnection( int socketDescriptor )
{
	TcpServer* tcpServer = qobject_cast<TcpServer*>( sender() );
	if( 0 == tcpServer )
		return;
	
	QTcpSocket* tcpSocket = tcpServer->getSoketFromDescriptor( socketDescriptor );
	if( 0 == tcpSocket )
		return;

	// checking for block-list
	if( true == isPeerInBlockList(tcpSocket,_ispSettings._blockFrom) )
		return;

	IpAddressAsNumber ip	= tcpSocket->localAddress().toIPv4Address();
	Port				port	= tcpSocket->localPort();

	bool result = isPeerInBlockList( tcpSocket, _socketDaemonSettingsList.value(qMakePair(ip,port))._socketSettings->_blockFrom );
	if( true == result )
		return;

	Socket2 currentSocket = qMakePair( ip, port );
	ISocketDaemon* socketDaemon = _socketDaemonSettingsList.value( currentSocket )._socketDaemon;

	socketDaemon->connectionRequest( tcpSocket );

	// the access to services was allowed, delegate it to the correct one
	dumpClient( tcpSocket, true );

	connect( tcpSocket, SIGNAL(readyRead()   ), this, SLOT(slot_readClientRequest() ) );
	connect( tcpSocket, SIGNAL(disconnected()), this, SLOT(slot_clientDisconnected()) );
}

void InternetServerPlatform::slot_readClientRequest()
{
	// getting the TCP socket
	QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>( sender() );
	if( 0 == tcpSocket )
		return;

	// getting the request and handling it
	IpAddressAsString ipAsString = tcpSocket->localAddress().toString();
	IpAddressAsNumber ipAsNumber = tcpSocket->localAddress().toIPv4Address();
	Port				port	= tcpSocket->localPort();

	IpAddressAsString	peerIp	= tcpSocket->peerAddress().toString();
	Port				peerPort	= tcpSocket->peerPort();

	QByteArray request = tcpSocket->readAll();

	QByteArray response;
	
	Socket2 currentSocket = qMakePair( ipAsNumber, port );

	ISocketDaemon* socketDaemon = _socketDaemonSettingsList.value( currentSocket )._socketDaemon;

	bool isRequestvalid = false;
	if( socketDaemon )
		socketDaemon->dataRequest( request, response, isRequestvalid );
        
	if( false == isRequestvalid )
	{
		markPeerAsPotentialIntruder( tcpSocket );
		tcpSocket->abort();
		return;
	}
	
	// sending the respose
	tcpSocket->write( response );
}

void InternetServerPlatform::slot_clientDisconnected()
{
	QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>( sender() );
	if( 0 == tcpSocket )
		return;

	IpAddressAsNumber ip	= tcpSocket->localAddress().toIPv4Address();
	Port				port	= tcpSocket->localPort();

	Socket2 currentSocket = qMakePair( ip, port );
	ISocketDaemon* socketDaemon = _socketDaemonSettingsList.value( currentSocket )._socketDaemon;

	socketDaemon->disconnectionRequest( tcpSocket );

	tcpSocket->deleteLater();
}

void InternetServerPlatform::dumpClient( QTcpSocket* tcpSocket, bool accepted )
{
	QString	clientInfo( "[%1] local=[%2:%3] peer=[%4:%5]" );
			clientInfo = clientInfo.arg( accepted ? "ACCEPTING" : "BLOccKING" ); 
			clientInfo = clientInfo.arg( tcpSocket->localAddress().toString() );
			clientInfo = clientInfo.arg( tcpSocket->localPort()               );
			clientInfo = clientInfo.arg( tcpSocket->peerAddress().toString()  );
			clientInfo = clientInfo.arg( tcpSocket->peerPort()                );

	Logging::logInfo( this, clientInfo );
}

void InternetServerPlatform::markPeerAsPotentialIntruder( QTcpSocket* tcpSocket )
{
    dumpClient( tcpSocket, false );
    
    ISocketDaemon*  daemon = qobject_cast<ISocketDaemon*>( sender() );
    IPlugin*        plugin = dynamic_cast<IPlugin*>( daemon ); 
    if( 0 != plugin )
        Logging::logWarning( this, QString("daemon [%1] asked to mark peer [%2] as intruder").arg(plugin->getPluginInfo().name).arg(tcpSocket->peerAddress().toString()) );
    else
        Logging::logWarning( this, QString("peer [%1] made an invalid request, marking it as potential intruder").arg(tcpSocket->peerAddress().toString()) );

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // ... here actually must follow the logic to add the peer to the monitor list
}
