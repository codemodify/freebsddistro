#ifndef __InternetServerPlatform_H__
#define __InternetServerPlatform_H__

// Qt headers
#include <QtCore/QCoreApplication>
#include <QtCore/QHash>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QMapIterator>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QPointer>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

// blocks
#include <Pluginer/PluginManager.h>

// ISP
#include <InternetServerPlatformLibrary/Modules/ISocketDaemon.h>
#include <InternetServerPlatformLibrary/Components/Components.h>

// local headers
#include "TcpServer/TcpServer.h"
#include "ISP.SettingsDefinitions.h"

using namespace IspComponents;


/***************************************************************************************

	This class defines an Ineternet Server Platform that will serve the world

****************************************************************************************/
class InternetServerPlatform : public QCoreApplication
{
	Q_OBJECT

	public:
		InternetServerPlatform( int argc, char** argv );
		~InternetServerPlatform();

		int run();


	// related to settings reading
	private:
		bool loadSettings();

		void readVariables			( QDomNode node );
		bool readIspSettings		( QDomNode node );
		bool readSockets			( QDomNode node );

		void	readBlockFrom	( QDomNode node, BlockFrom& blockFrom );
		void	readMood		( QDomNode node, Mood& mood );

		void replaceVariables	( QString& stringToOperateOn );
		void startMonitorFiles ( QStringList& filesToMonitor, BlockFrom& blockFrom );


	// related to the actions when a new connection occures
	private:
		bool isPeerInBlockList	( QTcpSocket* tcpSocket, BlockFrom& blockFrom );
		void applyIspMood		( QTcpSocket* tcpSocket );
		void dumpClient		( QTcpSocket* tcpSocket, bool accepted );

	private slots:
		void slot_newConnection(int);
		void slot_clientDisconnected();
		void slot_readClientRequest();
		void markPeerAsPotentialIntruder( QTcpSocket* tcpSocket );


	// internal data used by the ISP
	private:
		VariableList				_variableList;
		IspSettings				_ispSettings;
		SocketSettingsList			_socketSettingsList;

		SocketDaemonSettingsList	_socketDaemonSettingsList;

		QMap< QFileSystemWatcher*, BlockFrom* > 	_filesWatchers;

	private slots:
		void slot_fileChanged( const QString& filePath );

    private:
        PluginManager* _pluginManager;

};

#endif
