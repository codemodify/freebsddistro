
// local headers
#include "pop.h"
#include "popconnectionhandler/popconnectionhandler.h"
#include "services/mailserverconfigvariables.h"

PopService::PopService():
	QTcpServer()
{
	listen( QHostAddress::Any, popPort );
}

PopService::~PopService()
{}

void PopService::incomingConnection( int socketDescriptor )
{
	new PopConnectionHandler( socketDescriptor );
}
