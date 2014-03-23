#ifndef _DNS_INFO_H_
#define _DNS_INFO_H_

// Qt headers
#include <QtCore/QStringList>
#include <QtCore/QMap>


class QString;

/**************************************************************************************
*
*	This class represents a client to the DNS system.
*	The functionalities that it incorporates are:
*	- resolve IP to domain-name
*	- domain-name to IPs
*	- get the l"MX" records for a server
*
**************************************************************************************/
class DnsInfo
{

public:
	DnsInfo();
	~DnsInfo();


public:
	// sets the list of name servers to make the queries to, if nothing set then the root-servers of the internet are used
	void setPreferredNameServers( QStringList& nameServers );

	// ecnable/disables the cache usage
	void setCacheEnabled( bool enableCache = true );
	
	// chooses between the transport type to use to contact a DNS server
	enum SocketType{ UDP, TCP };
	void setSocketType( SocketType socketType );
	
	// enables the usage of the system configured DNS servers
	void setAllowUsageOfSystemConfiguredNameServers( bool allow );
	void getSystemConfiguredNameServers( QStringList& systemNameServers );
			
	// get the domain-name for the given IP, returns false in case of failure and an error explanation in "errorString"
	// blocking method
	bool getDomainNameForIP( QString ip, QString& domainName, QString& errorString );

	// get the IP addresses for the given domain-name, returns false in case of failure and an error explanation in "errorString"
	// blocking method
	bool getIpAddressesForDomain( QString domainName, QStringList& ipAddresses, QString& errorString );
	
	// get the Mail Servers for the given domain-name, returns false in case of failure and an error explanation in "errorString"
	// blocking method
	bool getMailServersForDomain( QString domainName, QStringList& mailServers, QString& errorString );


private: // here we'll store name-servers to refer to when asking teh DNS system for help
	QStringList _nameServers;
	QStringList _defaultNameServers;
	bool _allowSystemNameServers;


private: // this thing here will implement a cache technique in case there is a second call for the same domain
	bool _isCacheEnabled;

	typedef struct _DomainInfo
	{
		~_DomainInfo()
		{
			_ipAddresses.clear();
			_mailServers.clear();
		}
		QStringList _ipAddresses;
		QStringList _mailServers;
	} DomainInfo;

	typedef QMap<QString,DomainInfo> Domains;

	Domains _resolvedDomains;
	
	bool getCachedIpAddressesForDomain( QString& domainName, QStringList& ipAddresses );
	bool getCachedMailServersForDomain( QString& domainName, QStringList& mailServers );

	void addIpAddressesForDomainToCache( QString& domainName, QStringList& ipAddresses );
	void addMailServersForDomainToCache( QString& domainName, QStringList& mailServers );


private: // this thing here will choose between the TCP or UDP to carry the data on, default is TCP
	SocketType _socketType;

};

#endif
