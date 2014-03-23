#ifndef collaborativeschemaserver_h
#define collaborativeschemaserver_h

// Qt headers
#include <QtCore/QList>


// ISP headers
#include <isp/modules/daemon/daemon.h>

// local headers
#include "configuration.h"

class QTcpSocket;

/****************************************************************************************
*
*   This class defines a daemon that will expose schema-drawing services 
*   in a collaborative way.
*
****************************************************************************************/
class CollaborativeSchemaServer : public Daemon
{
    Q_OBJECT
    Q_INTERFACES( Daemon )
    
    // this thing here allows us to build custom sockets from its descriptor
    ISP_DAEMON_WITH_SOCKET_ACCESS

	
    public:
        CollaborativeSchemaServer();
        ~CollaborativeSchemaServer();


    private: // we implement the thigs that we must as a module
        bool initialize();
        bool shutdown();
        QString getDescription();
        QString getName();
        
    private slots: // we implement the thigs that we must as a module
        void slot_newIncommingConnection( int socketDescriptor );
        
    private slots:
        void slot_IncommingData();
        void slot_ClientDiconnected();
        
    private:
        QList<QTcpSocket*> _clients;
        
};

#endif
