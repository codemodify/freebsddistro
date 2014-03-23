#ifndef SITESERVER_H
#define SITESERVER_H


#include "webserver/configuration.h"

class Logger;

class DomainServer
{

public:
	DomainServer( DomainConfig* domainConfig );
	~DomainServer();

public:
	bool hasToBeBlocked( QString& host );
	QByteArray getContentForRequest( QString& resourceWanted );

private:
	DomainConfig* _domainConfig;
	Logger* _logger;
	
	QByteArray getFileContent( QString fileName );
	
};

#endif
