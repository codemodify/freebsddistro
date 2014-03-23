
// local headers
#include "smtp.h"
#include "smtpconnectionhandler/smtpconnectionhandler.h"
#include "services/mailserverconfigvariables.h"

SmtpService::SmtpService():
	QTcpServer()
{
	listen( QHostAddress::Any, smtpPort );
}

SmtpService::~SmtpService()
{
	close();
}

void SmtpService::incomingConnection( int socketDescriptor )
{
	new SmtpConnectionHandler( socketDescriptor );
}

