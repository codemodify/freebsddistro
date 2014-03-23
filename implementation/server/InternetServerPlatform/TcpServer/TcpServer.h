#ifndef __TcpServer_H__
#define __TcpServer_H__

// Qt headers
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
//#include <QtNetwork/QSslSocket>


class TcpServer : public QTcpServer
{
    Q_OBJECT

	public:
		TcpServer();

	public:
		QTcpSocket* getSoketFromDescriptor		( int socketDescriptor );
//		QSslSocket* getSecureSoketFromDescriptor	( int socketDescriptor );

	signals:
		void newIncommingConnection( int socketDescriptor );

	protected:    
		void incomingConnection( int socketDescriptor );

};

#endif
