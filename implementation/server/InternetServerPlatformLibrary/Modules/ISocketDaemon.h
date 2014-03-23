#ifndef __ISocketDaemon_H__
#define __ISocketDaemon_H__

// Qt headers
#include <QtNetwork/QTcpSocket>

// blocks
#include <Pluginer/PluginDefinition.h>


#define ISP_NO_TRACK_FOR_CONNECTIONS(...)                                   \
    public:     virtual void connectionRequest( QTcpSocket* socket ){}      \
    public:     virtual void disconnectionRequest( QTcpSocket* socket ){}



/****************************************************************************************
*
*	This class defines the interface for for a socket daemon.
*
****************************************************************************************/
DeclarePluginInterface
(
    ISocketDaemon,

    /* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
        A daemon needs to implement this if it wants to keep track of the connected clients
    ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */
    virtual void connectionRequest( QTcpSocket* socket ) = 0;


    /* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
        A daemon needs to implement this if it wants to keep track of the disconnected clients
    ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */
    virtual void disconnectionRequest( QTcpSocket* socket ) = 0;


    /* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
        A daemon needs to implement this method in order to be able to process data
    	'request'        - data read from the socket.
    	'response'       - data that will be sent to the peer.
    	'requestIsValid' - verified by ISP, false means the peer is marked as 
                           potential intruder and no data is sent.
    ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */
    virtual void dataRequest( QByteArray& request, QByteArray& response, bool& requestIsValid ) = 0;
)

#endif
