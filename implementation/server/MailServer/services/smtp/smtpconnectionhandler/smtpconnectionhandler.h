#ifndef _SMTP_CONNECTION_HANDLER_H_
#define _SMTP_CONNECTION_HANDLER_H_

// Qt headers
#include <QtCore/QThread>


class SmtpConnectionHandler : public QThread
{
	
	Q_OBJECT

public:
	SmtpConnectionHandler( int socketDescriptor );
	~SmtpConnectionHandler();


public:
	void run();


private:
	bool isMailAddressOnLocalServer( QString mailAddress );


private: // security checks
	bool checkMailAddress( QString& mailAddress );
	bool scanForVirus( QByteArray& content );
	bool scanForSpam( QByteArray& content );


private:
	int _socketDescriptor;

};

#endif
