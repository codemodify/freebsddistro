#ifndef __CommunicationCenterServer_H__
#define __CommunicationCenterServer_H__

// Qt headers
#include <QtCore/QList>

// ISP headers
#include <InternetServerPlatformLibrary/Modules/SocketDaemon.h>
#include <InternetServerPlatformLibrary/Components/Components.h>
using namespace IspComponents;

// local headers
#include "Configuration.h"


class CommunicationCenterServer : public SocketDaemon
{
	Q_OBJECT

	public:
		CommunicationCenterServer();
		~CommunicationCenterServer();

	// implement inherited stuff
	public:
		bool initialize();
		bool shutdown();

		QString getName();
		QString getDescription();

		void connectionRequest( QTcpSocket* socket );
		void disconnectionRequest( QTcpSocket* socket );
		void dataRequest( QByteArray& request, QByteArray& response, bool& requestIsValid );

	// helpers
	public slots:
		void sendCurrentUserListToPeers();

	// internal
	private:
		Bottles::ID _idInsideBottlesWorld;
		Bottles::ID _rootBottleId;

};

#endif
