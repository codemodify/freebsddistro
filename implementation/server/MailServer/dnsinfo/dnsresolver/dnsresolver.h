#ifndef _DNS_RESOLVER_H_
#define _DNS_RESOLVER_H_



/*****************************************************************************************************
*
*	This class uses a UDP or TCP socket to connect to a name-server and collect some data from it.
*
******************************************************************************************************/
class DnsResolver
{

public:
	DnsResolver( bool isSocketTcpType );
	~DnsResolver();

public:
	// nameserver = IP
	// domainName = 'domain.com' or '192.168.1.1'
	bool requestARecord( QString& nameServer, QString& domainName, QStringList& ipAddresses, QString& errorString );

	// nameserver = IP
	// domainName = 'domain.com' or '192.168.1.1'
	bool requestMXRecord( QString& nameServer, QString& domainName, QStringList& mailServers, QString& errorString );

private:
	bool _isSocketTcpType;
	bool sendUsingTcp( QString& nameServer, QByteArray& messageToSend, QByteArray& responseMessage, QString& errorString );
	bool sendUsingUdp( QString& nameServer, QByteArray& messageToSend, QByteArray& responseMessage, QString& errorString);
	
};

#endif
