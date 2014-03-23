
// local headers
#include "InternetServerPlatform.h"



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// related to settings reading
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
bool InternetServerPlatform::loadSettings()
{
	// get the settings file path
	QString settingsFilePath = QCoreApplication::applicationFilePath() + ".config";
    Logging::logInfo( this, QString("loadSettingsFile() -> %1").arg(settingsFilePath) );

	// get all key nodes
	QDomDocument settingsFileAsXml; 
	QDomNode internetServerPlatform	= IspComponents::Dom::loadXmlFileAndGetRootNode( settingsFilePath, settingsFileAsXml  );
	QDomNode variables				= IspComponents::Dom::getChildNodeByName( internetServerPlatform, "variables"         );
	QDomNode ispSettings			= IspComponents::Dom::getChildNodeByName( internetServerPlatform, "ispSettings"       );
	QDomNode sockets				= IspComponents::Dom::getChildNodeByName( internetServerPlatform, "sockets"           );

	readVariables  	( variables	    );
	readIspSettings	( ispSettings	);
	readSockets		( sockets		);

	return true;
}

void InternetServerPlatform::readVariables( QDomNode node )
{
	Logging::logInfo( this, "readVariables()" );

	if( node.isNull() )
		return;

	QDomNode variable = node.firstChild();
	while( false == variable.isNull() )
	{
		if( QDomNode::CommentNode != variable.nodeType() )
		{
			QString variableName = variable.nodeName();

			if( _variableList.contains(variableName) )
				continue;

			QString variableValue = variable.attributes().namedItem("value").nodeValue();

			replaceVariables( variableValue );

			_variableList.insert( variableName, variableValue );

			Logging::logInfo( this, QString("%1 = %2").arg(variableName).arg(variableValue) );

			variable = variable.nextSibling();
		}	
	}	
}

bool InternetServerPlatform::readIspSettings( QDomNode node )
{
	Logging::logInfo( this, "readIspSettings()" );

	if( node.isNull() )
		return false;

	// read the log file
	QDomNode logFile = IspComponents::Dom::getChildNodeByName( node, "logFile" );
	if( logFile.isNull() )
		return false;

	_ispSettings._logFile = logFile.attributes().namedItem("value").nodeValue();

	// read the block-from
	QDomNode blockFrom = IspComponents::Dom::getChildNodeByName( node, "blockFrom" );
	readBlockFrom( blockFrom, _ispSettings._blockFrom );

	// read the mood
	QDomNode mood = IspComponents::Dom::getChildNodeByName( node, "mood" );
	readMood( mood, _ispSettings._mood );

	// init the logs for ISP once we have the log file
	IspComponents::Logging::registerForLogging( this, _ispSettings._logFile );

	return true;
}

bool InternetServerPlatform::readSockets( QDomNode node )
{
	Logging::logInfo( this, "readSockets()" );

	if( node.isNull() )
		return false;

    QString applicationPath = QCoreApplication::applicationDirPath();
	
	QDomNode socket = IspComponents::Dom::getChildNodeByName( node, "socket" );
	while( false == socket.isNull() )
	{
		if( QDomNode::CommentNode != socket.nodeType() )
		{
			SocketSettings* socketSettings = new SocketSettings();

            // read the logfile read on
			socketSettings->_on = socket.attributes().namedItem( "on" ).nodeValue();

            // read the daemon
			socketSettings->_daemon	= socket.attributes().namedItem( "daemon" ).nodeValue();
		    socketSettings->_daemon = socketSettings->_daemon.startsWith("/")
                ? socketSettings->_daemon
                : QString( "%1/%2" ).arg( applicationPath ).arg( socketSettings->_daemon );

			// read the logfile
			QDomNode logFile = IspComponents::Dom::getChildNodeByName( socket, "logFile" );	
			socketSettings->_logFile = logFile.attributes().namedItem("value").nodeValue();
		    socketSettings->_logFile = socketSettings->_logFile.startsWith("/")
                ? socketSettings->_logFile
                : QString( "%1/%2" ).arg( applicationPath ).arg( socketSettings->_logFile );

			// read the block-from
			QDomNode blockFrom = IspComponents::Dom::getChildNodeByName( socket, "blockFrom" );
			readBlockFrom( blockFrom, socketSettings->_blockFrom );

			_socketSettingsList.append( socketSettings );
		}

		socket = socket.nextSibling();
	}

	return true;
}

void InternetServerPlatform::readBlockFrom( QDomNode node, BlockFrom& blockFrom )
{
	Logging::logInfo( this, "readBlockFrom()" );
	
	if( node.isNull() )
		return;

	QDomNode domNode = node.firstChild();
	while( false == domNode.isNull() )
	{
		if( "ip" == domNode.nodeName() )
		{
			QString ip = domNode.attributes().namedItem("value").nodeValue();
			if( (false==blockFrom._blockedIpList.contains(ip)) && (false==ip.isEmpty()) )
			{
				blockFrom._blockedIpList.append( ip );
				Logging::logInfo( this, QString("blocked ip: %1").arg(ip) );
			}
		}

		else if( "ipRange" == domNode.nodeName() )
		{
			QStringList ipInterval = domNode.attributes().namedItem( "value"  ).nodeValue().split( "-" );
			QStringList exceptList = domNode.attributes().namedItem( "except" ).nodeValue().split( "," );
				
			if( 2 == ipInterval.size() )
			{
				IpAddressAsNumber startIP = QHostAddress( ipInterval.at(0) ).toIPv4Address();
				IpAddressAsNumber endIP   = QHostAddress( ipInterval.at(1) ).toIPv4Address();

				blockFrom._blockedIpIntervals.insert( startIP, endIP );

				Logging::logInfo( this, QString("blocked ip interval: %1").arg( domNode.attributes().namedItem( "value"  ).nodeValue() ) );
			}
			else
			{
				IpAddressAsNumber startIP = QHostAddress( (quint32)0        ).toIPv4Address();
				IpAddressAsNumber endIP   = QHostAddress( "255.255.255.255" ).toIPv4Address();
				
				blockFrom._blockedIpIntervals.insert( startIP, endIP );

				Logging::logInfo( this, QString("blocked ip interval: 0.0.0.0-255.255.255.255") );
			}

			if( false == exceptList.isEmpty() )
				foreach( QString ip, exceptList )
					blockFrom._exceptList.append( QHostAddress(ip).toIPv4Address() );
		}

		else if( "file" == domNode.nodeName() )
		{
/*
			QString file = domNode.attributes().namedItem("value").nodeValue();
			replaceVariables( file );

			Logging::logInfo( this, QString("file to monitor for blocked ip: %1").arg(file) );
						
			if( false == filesToMonitor.contains(file) )
				filesToMonitor.append( file );
*/		
		}
		
		domNode = domNode.nextSibling();
	} 
}

void InternetServerPlatform::readMood( QDomNode moodNode, Mood& mood )
{
	Logging::logInfo( this, "readMood()" );
	
	if( moodNode.isNull() )
		return;

	QString active = moodNode.attributes().namedItem("active").nodeValue(); 
	QDomNode node = Dom::getChildNodeByName( moodNode, active );
	if( false == node.isNull() )
	{
		if( "business" == active )
		{
			mood._active = Mood::Business;
			mood._business._explanatoryPage = node.attributes().namedItem("explanatoryPage").nodeValue();
			replaceVariables( mood._business._explanatoryPage );
		}
		else if( "hacker" == active )
		{
			mood._active = Mood::Hacker;
			mood._hacker._folderToStoreInfoFiles = node.attributes().namedItem("folderToStoreInfoFiles").nodeValue();
			mood._hacker._explanatoryPage = node.attributes().namedItem("explanatoryPage").nodeValue();
				
			replaceVariables( mood._hacker._folderToStoreInfoFiles );
			replaceVariables( mood._hacker._explanatoryPage );
		}
		else if( "hole" == active )
		{
			mood._active = Mood::Hole;
			Logging::logInfo( this, "active mood: Mood::Hole" );
		}
		else if( "brutal" == active )
		{
			mood._active = Mood::Brutal;
			mood._brutal._countPerSecond = node.attributes().namedItem("countPerSecond").nodeValue().toInt();
			mood._brutal._countPerMinute = node.attributes().namedItem("countPerMinute").nodeValue().toInt();
			mood._brutal._totalCount = node.attributes().namedItem("totalCount").nodeValue().toInt();
			mood._brutal._scriptForFirewall = node.attributes().namedItem("scriptForFirewall").nodeValue();
			mood._brutal._runWith = node.attributes().namedItem("runWith").nodeValue();

			replaceVariables( mood._brutal._scriptForFirewall );
			replaceVariables( mood._brutal._runWith );
		}
	}
}

void InternetServerPlatform::replaceVariables( QString& stringToOperateOn )
{
	while( stringToOperateOn.contains("@") )
	{
		int startPos = stringToOperateOn.indexOf( "@" );

		int shift = 0;
		if( stringToOperateOn.contains("/") )
			shift = stringToOperateOn.indexOf( "/" );
		else
			shift = stringToOperateOn.length();
		
		QString variableToLookFor = stringToOperateOn.mid( startPos+1, shift-1 );		
		if( false == _variableList.contains(variableToLookFor) )
			continue;
		
		stringToOperateOn.remove( startPos, shift );
		
		stringToOperateOn.insert( startPos, _variableList[variableToLookFor] );
	}	
}
