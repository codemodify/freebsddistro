#ifndef __IspComponents_H__
#define __IspComponents_H__

// Qt headers
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QUuid>
#include <QtCore/QHash>
#include <QtXml/QDomNode>
#include <QtXml/QDomDocument>
#include <QtNetwork/QTcpSocket>


namespace IspComponents
{
	namespace Dom
	{
		QDomNode getChildNodeByName	( QDomNode parentNode, QString childTagNameToLookFor );
		QDomNode loadXmlFileAndGetRootNode( QString settingsFilePath, QDomDocument& settingsFileAsXml );		
	}

	namespace Logging
	{
		void registerForLogging( QObject* pointerToCaller, QString logFilePath );

		void logInfo   ( QObject* pointerToCaller, QString message );
		void logWarning( QObject* pointerToCaller, QString message );
		void logError  ( QObject* pointerToCaller, QString message );
		void logFatal  ( QObject* pointerToCaller, QString message );
	}

	namespace Bottles
	{
		typedef QUuid ID;

		typedef struct _Peer
		{
			void*		_data;
			QStringList	_sslCertificates;
			QTcpSocket*	_socket;

			_Peer(){ _data = 0; _socket = 0; }
		} Peer;
		typedef QHash< ID, Peer > PeerList;

		typedef struct
		{
			QString  _name;
			ID       _creator;
			PeerList _peerList;
		} Bottle;		
		typedef QHash< ID, Bottle > BottleList;

		typedef bool ( *PeersCompareEngine )( const Peer& peer, void* searchData );


		// some methods to work with bottles
		void registerForBottles( QObject* caller, ID& idInsideBottlesWorld );
		void unRegisterForBottles( QObject* caller );
		void setPeersCompareEngine( PeersCompareEngine peersCompareEngine );

		void createBottle	( QObject* caller, ID& bottleCreator, QString bottleName, ID& newBottleId );
		void destroyBottle	( QObject* caller, ID& bottleId, ID& bottleCreator                        );
		void getBottleInfo	( QObject* caller, ID& bottleId, Bottle& bottleResultInfo                 );

		// some methods to work with bottle's flora and fauna
		void addPeerToBottle		( QObject* caller, ID& bottleId, Peer& peer, ID& newPeerId );
		void removePeerFromBottle	( QObject* caller, ID& bottleId, ID& peerId                );

		void searchPeer			( QObject* caller, ID& bottleId, void* searchData, PeerList& foundPeers );
		void searchPeer			( QObject* caller,               void* searchData, PeerList& foundPeers );
		void searchPeer			(                                void* searchData, PeerList& foundPeers );

		void getPeer				( QObject* caller, ID& bottleId, ID& peerId, Peer&     peer         );
		void getPeers				( QObject* caller, ID& bottleId,             PeerList& foundPeers   );
		void getPeers				( QObject* caller,                           PeerList& foundPeers   );
		void getPeers				(                                            PeerList& foundPeers   );

		// some methods that implement a transport mechanism inside the 'bottles'
		void sendToPeer		( QObject* caller, ID& bottleId, ID& peerId, QByteArray& data );
		void sendToAllPeers	( QObject* caller, ID& bottleId,             QByteArray& data );
		void sendToAllPeers	( QObject* caller,                           QByteArray& data );
		void sendToAllPeers	(                                            QByteArray& data );
	}

}

#endif
