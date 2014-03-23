
#include "InternetServerPlatform.h"


InternetServerPlatform::InternetServerPlatform( int argc, char** argv ) :
	QCoreApplication( argc, argv )
{
    _pluginManager = new PluginManager();
    _pluginManager->setNoInitOnPluginLoad();
}

InternetServerPlatform::~InternetServerPlatform()
{
    delete _pluginManager;
}

int InternetServerPlatform::run()
{
	Logging::logInfo( this, "run()" );

	// load settings
	if( false == loadSettings() )
		return -1;

    QString applicationPath = QCoreApplication::applicationDirPath();
    QString errorString     = "";
    QString logMessage      = "";

	// apply settings
	foreach( SocketSettings* socketSettings, _socketSettingsList )
 	{
		logMessage = QString("[ %1 ]").arg( socketSettings->_daemon );
		
        // load the daemon plugin
        IPlugin* plugin = _pluginManager->loadPlugin( socketSettings->_daemon, errorString );
		if( 0 == plugin )
		{
		    logMessage += QString(" --> ERR on load(), hints: %1").arg( errorString );
			Logging::logError( this, logMessage );

			continue;
		}

		// init the loaded daemon
		QObject* object = dynamic_cast<QObject*>( plugin );
        Logging::registerForLogging( object, socketSettings->_logFile );

		plugin->setPluginPath( socketSettings->_daemon );
		if( false == plugin->doInit() )
		{
			_pluginManager->unLoadPlugin( socketSettings->_daemon );

    		logMessage += QString(" --> ERR on doInit(), hints in [ %1 ]" ).arg( socketSettings->_logFile );
			Logging::logError( this, logMessage );

			continue;
		}

		logMessage += QString(" --> OK, its activity in [ %1 ]" ).arg( socketSettings->_logFile );
		Logging::logInfo( this, logMessage );

		// build the list of IP:Port to bind to
		QList< Socket1 > socketList;
		foreach( QString ipPort, socketSettings->_on.split(",") )
		{
			if( 2 != ipPort.split(":").size() )
				continue;

			IpAddressAsString	ipAddressAsString	= ipPort.split( ":" ).at( 0 );
			Port				port				= ipPort.split( ":" ).at( 1 ).toUInt();

			if( "all" == ipAddressAsString )
			{
				QList<QHostAddress> interfaces = QNetworkInterface::allAddresses();
				foreach( QHostAddress interface, interfaces )
				{
					if( QAbstractSocket::IPv4Protocol == interface.protocol() )
						socketList.append( qMakePair(interface.toString(),port) );
				}
			}

			else
				socketList.append( qMakePair(ipAddressAsString,port) );
		}

		// bind it to the sockets
		foreach( Socket1 socket1, socketList )
		{
			TcpServer* tcpServer = new TcpServer();

			if( false == tcpServer->listen(QHostAddress(socket1.first),socket1.second) )
			{
				Logging::logError( this, tcpServer->errorString() );

				delete tcpServer;
				continue;
			}

			Logging::logInfo( this, QString("listen on: %1:%2").arg(socket1.first).arg(socket1.second) );

			Socket2 socket2 = qMakePair( QHostAddress(socket1.first).toIPv4Address(), socket1.second );

            ISocketDaemon*  daemon = dynamic_cast<ISocketDaemon*>( plugin );  

			SocketDaemonSettings    socketDaemonSettings;
                                    socketDaemonSettings._socketSettings= socketSettings;
                                    socketDaemonSettings._socketDaemon	= daemon;

			_socketDaemonSettingsList.insert( socket2, socketDaemonSettings );
			connect( tcpServer, SIGNAL(newIncommingConnection(int)), this, SLOT(slot_newConnection(int)) );
 		}
	}
	
	// empty some vars that have been used at startup
	_socketSettingsList.clear();

	return exec();
}
