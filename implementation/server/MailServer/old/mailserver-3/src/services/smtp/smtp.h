#ifndef _SMTP_SERVICE_H_
#define _SMTP_SERVICE_H_

// Qt headers
#include <QtNetwork/QTcpServer>

class SmtpService : public QTcpServer
{

public:
	SmtpService();
	~SmtpService();

private:
    void incomingConnection( int socketDescriptor );

};

#endif
