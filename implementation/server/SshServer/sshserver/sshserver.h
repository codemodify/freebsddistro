#ifndef sshserver_h
#define sshserver_h

// Qt headers
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslError>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslCipher>
#include <QtCore/QList>


// ISP headers
#include <isp/modules/daemon/daemon.h>

// local headers
#include "configuration.h"

// some forwards
class QDomNode;


/****************************************************************************************
*
*   This class defines a daemon that will expose remote shell services in a secure way.
*
*   A layer of security is used from openssl.org,
*   second one is inherited from Internet-Server-Platform,
*   third is baes on its features and behaviuor.
*
*   All these are combined in a way to get the best "gets-the-job-done" effect.  
*
****************************************************************************************/
class SshServer : public Daemon
{
    Q_OBJECT
    Q_INTERFACES( Daemon )
    
    // this thing here allows us to build custom sockets from its descriptor
    ISP_DAEMON_WITH_SOCKET_ACCESS

	
    public:
        SshServer();
        ~SshServer();


    private: // this is used for reading the daemon's settings
        bool readSshServerSettings( QDomNode parentNode );
        bool readAcceptedShellCommands( QDomNode parentNode, AcceptedShellCommands& acceptedShellCommands );
        bool readUser( QDomNode userNode, UserSettings& userSettings );

	
    private:
        // some internally stored data
        SshServerSettings _sshServerSettings;
        UserList _userList;

        // now we go ahead and implement the thigs that we must as a module
        bool initialize();
        bool shutdown();
        QString getDescription();
        QString getName();
        

    private slots:
        void slot_newIncommingConnection( int socketDescriptor );
         
        // some slots for the SSL-socket events to land in
        void slot_SuccessfulConnected();
        void slot_UnSuccessfulConnected( const QList<QSslError>& sslErrors );
        void slot_IncommingData();
        
        
        void slot_ConnectionEcrypted();
        void slot_ConnectionSateChanged( QSslSocket::SslMode sslMode );
        void slot_ConnectionSslErrors( const QList<QSslError>& sslErrors );
        void slot_ConnectionConnected();
        void slot_ConnectionDisconnected();
        void slot_ConnectionError( QAbstractSocket::SocketError socketError );
        void slot_ConnectionHostFound();
        void slot_ConnectionStateChanged( QAbstractSocket::SocketState socketState );
        void slot_ConnectionStatereadyRead();
        

};

#endif
