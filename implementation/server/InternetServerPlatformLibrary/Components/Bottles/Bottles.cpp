
// local headers
#include "Components/Components.h"


// convenience stuff
#define MAKE_SURE_OBJECT_IS_REGISTERED( qobject )    \
    if( false == _objects.keys().contains(qobject) ) \
        return;

#define MAKE_SURE_BOTTLE_EXISTS( qobject, bottleId )    \
    if( false == _objects[qobject].contains(bottleId) ) \
        return;

#define MAKE_SURE_PEER_EXISTS( qobject, bottleId, peerId )                  	\
	if																\
	(																\
		(false == (_objects[qobject][bottleId])._peerList.contains(peerId))	\
		&&															\
		(false == (_objectsId[qobject]==bottleId)	)						\
	)																\
		return;


// store all the object <->  bottles corresponance
QHash< QObject*, IspComponents::Bottles::ID         > _objectsId;
QHash< QObject*, IspComponents::Bottles::BottleList > _objects;
IspComponents::Bottles::PeersCompareEngine            _peersCompareEngine = 0;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ----  some methods to work with bottles
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void IspComponents::Bottles::registerForBottles( QObject* caller, ID& idInsideBottlesWorld )
{
	idInsideBottlesWorld = QUuid::createUuid();

    _objects.insert( caller, BottleList() );
	_objectsId.insert( caller, idInsideBottlesWorld );
}

void IspComponents::Bottles::unRegisterForBottles( QObject* caller )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );

	BottleList& bottleList = _objects[ caller ];

	BottleList::const_iterator begin = bottleList.begin();
	BottleList::const_iterator end   = bottleList.end();
	BottleList::const_iterator it    = begin;
	
	for( ; it != end; ++it )
	{
		const Bottle& bottle = it.value();

		PeerList& peerList = (PeerList&) bottle._peerList;
				 peerList.clear();
	}

	bottleList.clear();
	_objects.remove( caller );
	_objectsId.remove( caller );
}

void IspComponents::Bottles::setPeersCompareEngine( PeersCompareEngine peersCompareEngine )
{
	_peersCompareEngine = peersCompareEngine;
}

void IspComponents::Bottles::createBottle( QObject* caller, ID& bottleCreator, QString bottleName, ID& newBottleId )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );

	Bottle	bottle;
			bottle._name		= bottleName;
			bottle._creator	= bottleCreator;

	newBottleId = QUuid::createUuid();

	BottleList&	bottleList = _objects[ caller ];
    				bottleList.insert( newBottleId, bottle );
}

void IspComponents::Bottles::destroyBottle( QObject* caller, ID& bottleId, ID& bottleCreator )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );
	MAKE_SURE_BOTTLE_EXISTS( caller, bottleId );
	MAKE_SURE_PEER_EXISTS( caller, bottleId, bottleCreator );

	// check if the bottleCreator is the creator of the bottle
	BottleList& bottleList = _objects[ caller ];

	Bottle& bottle = bottleList[ bottleId ];
	if( bottleCreator != bottle._creator )
		return;

	PeerList& peerList = bottle._peerList;

	// FIXME: send a notification to the bottle-members about the bottle removal
	// ...

	peerList.clear();
	bottleList.remove( bottleId );
}

void IspComponents::Bottles::getBottleInfo( QObject* caller, ID& bottleId, Bottle& bottleResultInfo )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );
	MAKE_SURE_BOTTLE_EXISTS( caller, bottleId );

	// getting the object's ISP info
	BottleList& bottleList = _objects[ caller ];

	bottleResultInfo = bottleList[ bottleId ];
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- some methods to work with bottle's flora and fauna
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void IspComponents::Bottles::addPeerToBottle( QObject* caller, ID& bottleId, Peer& peer, ID& newPeerId )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );
	MAKE_SURE_BOTTLE_EXISTS( caller, bottleId );

	BottleList& bottleList = _objects[ caller ];
	Bottle&     bottle     = bottleList[ bottleId ];
	PeerList&   peerList   = bottle._peerList;

	newPeerId = QUuid::createUuid();

	peerList.insert( newPeerId, peer );
}

void IspComponents::Bottles::removePeerFromBottle( QObject* caller, ID& bottleId, ID& peerId )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );
	MAKE_SURE_BOTTLE_EXISTS( caller, bottleId );
	MAKE_SURE_PEER_EXISTS( caller, bottleId, peerId );

	BottleList& bottleList = _objects[ caller ];
	Bottle&     bottle     = bottleList[ bottleId ];
	PeerList&   peerList   = bottle._peerList;

	peerList.remove( peerId );
}

void IspComponents::Bottles::searchPeer( QObject* caller, ID& bottleId, void* searchData, PeerList& foundPeers )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );
	MAKE_SURE_BOTTLE_EXISTS( caller, bottleId );

	BottleList& bottleList = _objects[ caller ];
	Bottle&     bottle     = bottleList[ bottleId ];
	PeerList&   peerList   = bottle._peerList;

	PeerList::const_iterator begin = peerList.begin();
	PeerList::const_iterator end   = peerList.end();
	PeerList::const_iterator it    = begin;

	for( ; it != end; ++it )
	{
		const ID&   id   = it.key();
		const Peer& peer = it.value();

		if( _peersCompareEngine(peer,searchData) )
			foundPeers.insert( id, peer );
	}
}

void IspComponents::Bottles::searchPeer( QObject* caller, void* searchData, PeerList& foundPeers )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );

	BottleList& bottleList = _objects[ caller ];

	BottleList::const_iterator begin = bottleList.begin();
	BottleList::const_iterator end   = bottleList.end();
	BottleList::const_iterator it    = begin;
	
	for( ; it != end; ++it )
	{
		const ID& bottleId = it.key();

		searchPeer( caller, (ID&)bottleId, searchData, foundPeers );
	}
}

void IspComponents::Bottles::searchPeer( void* searchData, PeerList& foundPeers )
{
	QHash< QObject*, BottleList >::const_iterator begin = _objects.begin();
	QHash< QObject*, BottleList >::const_iterator end   = _objects.end();
	QHash< QObject*, BottleList >::const_iterator it    = begin;
	
	for( ; it != end; ++it )
	{
		QObject* object = it.key();

		searchPeer( object, searchData, foundPeers );
	}
}

void IspComponents::Bottles::getPeer( QObject* caller, ID& bottleId, ID& peerId, Peer& peer )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );
	MAKE_SURE_BOTTLE_EXISTS( caller, bottleId );
	MAKE_SURE_PEER_EXISTS( caller, bottleId, peerId );

	peer = _objects[caller][bottleId]._peerList[peerId];
}

void IspComponents::Bottles::getPeers( QObject* caller, ID& bottleId, PeerList& foundPeers )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );
	MAKE_SURE_BOTTLE_EXISTS( caller, bottleId );

	foundPeers = _objects[caller][bottleId]._peerList;
}

void IspComponents::Bottles::getPeers( QObject* caller, PeerList& foundPeers )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );

	BottleList& bottleList = _objects[ caller ];

	BottleList::const_iterator begin = bottleList.begin();
	BottleList::const_iterator end   = bottleList.end();
	BottleList::const_iterator it    = begin;
	
	for( ; it != end; ++it )
	{
		const Bottle& bottle = it.value();

		qCopy( bottle._peerList.begin(), bottle._peerList.end(), foundPeers.end() );
	}
}

void IspComponents::Bottles::getPeers( PeerList& foundPeers )
{
	QHash< QObject*, BottleList >::const_iterator begin = _objects.begin();
	QHash< QObject*, BottleList >::const_iterator end   = _objects.end();
	QHash< QObject*, BottleList >::const_iterator it    = begin;
	
	for( ; it != end; ++it )
	{
		QObject* object = it.key();

		getPeers( object, foundPeers );
	}
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- some methods that implement a transport mechanism inside the 'bottles'
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void IspComponents::Bottles::sendToPeer( QObject* caller, ID& bottleId, ID& peerId, QByteArray& data )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );
	MAKE_SURE_BOTTLE_EXISTS( caller, bottleId );
	MAKE_SURE_PEER_EXISTS( caller, bottleId, peerId );

	Peer peer;
	getPeer( caller, bottleId, peerId, peer );

	peer._socket->write( data );
}

void IspComponents::Bottles::sendToAllPeers( QObject* caller, ID& bottleId, QByteArray& data )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );
	MAKE_SURE_BOTTLE_EXISTS( caller, bottleId );

	BottleList& bottleList = _objects[ caller ];
	Bottle&     bottle     = bottleList[ bottleId ];
	PeerList&   peerList   = bottle._peerList;

	PeerList::const_iterator begin = peerList.begin();
	PeerList::const_iterator end   = peerList.end();
	PeerList::const_iterator it    = begin;

	for( ; it != end; ++it )
	{
		const Peer& peer = it.value();

		//if( peer._socket )
			peer._socket->write( data );
	}
}

void IspComponents::Bottles::sendToAllPeers( QObject* caller, QByteArray& data )
{
	MAKE_SURE_OBJECT_IS_REGISTERED( caller );

	BottleList& bottleList = _objects[ caller ];

	BottleList::const_iterator begin = bottleList.begin();
	BottleList::const_iterator end   = bottleList.end();
	BottleList::const_iterator it    = begin;
	
	for( ; it != end; ++it )
	{
		const ID& bottleId = it.key();

		sendToAllPeers( caller, (ID&)bottleId, data );
	}
}

void IspComponents::Bottles::sendToAllPeers( QByteArray& data )
{
	QHash< QObject*, BottleList >::const_iterator begin = _objects.begin();
	QHash< QObject*, BottleList >::const_iterator end   = _objects.end();
	QHash< QObject*, BottleList >::const_iterator it    = begin;
	
	for( ; it != end; ++it )
	{
		QObject* object = it.key();

		sendToAllPeers( object, data );
	}
}
