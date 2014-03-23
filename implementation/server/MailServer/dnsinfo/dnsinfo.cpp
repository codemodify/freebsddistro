
// qt headers
#include <QtNetwork/QHostInfo>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

// local headers
#include "dnsinfo.h"
#include "dnsresolver/dnsresolver.h"


DnsInfo::DnsInfo():
	_isCacheEnabled( true ),
	_allowSystemNameServers( true ),
	_socketType( UDP )
{
	// here we add to the name server list the root internet nameservers, this will be used as last option when nothing else is useful
	_defaultNameServers.append( "A.GTLD-SERVERS.net" );
	_defaultNameServers.append( "B.GTLD-SERVERS.net" );
	_defaultNameServers.append( "C.GTLD-SERVERS.net" );
	_defaultNameServers.append( "D.GTLD-SERVERS.net" );
	_defaultNameServers.append( "E.GTLD-SERVERS.net" );
	_defaultNameServers.append( "F.GTLD-SERVERS.net" );
	_defaultNameServers.append( "G.GTLD-SERVERS.net" );
	_defaultNameServers.append( "H.GTLD-SERVERS.net" );
	_defaultNameServers.append( "I.GTLD-SERVERS.net" );
	
	_nameServers = _defaultNameServers;
}

DnsInfo::~DnsInfo()
{
	_nameServers.clear();
	_defaultNameServers.clear();
	_resolvedDomains.clear();
}

void DnsInfo::setCacheEnabled( bool enableCache )
{
	_isCacheEnabled = enableCache;
}

void DnsInfo::setPreferredNameServers( QStringList& nameServers )
{
	// add the servers preffered by the user first
	_nameServers = nameServers;
	
	// next we add the system configured name servers
	if( true == _allowSystemNameServers )
	{
		QStringList systemNameservers;
		getSystemConfiguredNameServers( systemNameservers );
		_nameServers += systemNameservers;
	}
	
	// add some last option nameservers 
	_nameServers += _defaultNameServers;
}

void DnsInfo::setAllowUsageOfSystemConfiguredNameServers( bool allow )
{
	_allowSystemNameServers = allow;
}

void DnsInfo::getSystemConfiguredNameServers( QStringList& systemNameServers )
{
	// here we look into '/etc/resolv.conf'
	QFile file( "/etc/resolv.conf" );
	if( false == file.open(QIODevice::ReadOnly) )
		return;

	QTextStream textStream( &file );
	QString textLine;
	while( false == textStream.atEnd() )
	{
		textLine = textStream.readLine();
		if( true == textLine.startsWith('#') )
			continue;
		
		QStringList stringList = textLine.split( " " );
		if( stringList.size() < 2 )
			continue;
			
		QString type = stringList[0].toLower();
		QString value = stringList[1].toLower();
		if( "nameserver" == type )
		{
			QHostAddress hostAddress;
			if( true == hostAddress.setAddress(value) )
				systemNameServers.append( hostAddress.toString() );
		}
	}
	
	file.close();
	
}

bool DnsInfo::getDomainNameForIP( QString ip, QString& domainName, QString& errorString )
{
	return false;
}

bool DnsInfo::getIpAddressesForDomain( QString domainName, QStringList& ipAddresses, QString& errorString )
{
	// try to get them from cache
	if( true == _isCacheEnabled )
	{
		bool result = getCachedIpAddressesForDomain( domainName, ipAddresses );
		if( true == result )
			return true;
	}

	// once we're here then we have to build a request to the DNS system to ask for help
	DnsResolver dnsResolver( _socketType );
	foreach( QString nameServer, _nameServers )
	{
		bool result = dnsResolver.requestARecord( nameServer, domainName, ipAddresses, errorString );
		if( true == result )
		{
			addIpAddressesForDomainToCache( domainName, ipAddresses );
			return true;
		}
	}

	return false;
}

bool DnsInfo::getMailServersForDomain( QString domainName, QStringList& mailServers, QString& errorString )
{
	// try to get them from cache
	if( true == _isCacheEnabled )
	{
		bool result = getCachedMailServersForDomain( domainName, mailServers );
		if( true == result )
			return true;
	}
	
	// once we're here then we have to build a request to the DNS system to ask for help on the mater
	DnsResolver dnsResolver( _socketType );
	foreach( QString nameServer, _nameServers )
	{
		QStringList unresolvedMailServers;
		bool result = dnsResolver.requestMXRecord( nameServer, domainName, unresolvedMailServers, errorString );
		if( true == result )
		{
			foreach( QString unresolvedMailServer, unresolvedMailServers )
			{
				QStringList ipList;
				result = getIpAddressesForDomain( unresolvedMailServer, ipList, errorString );
				if( (true==result) && (false==ipList.isEmpty()) )
					mailServers.append( ipList.at(0) );
			}
			
			addMailServersForDomainToCache( domainName, mailServers );
			
			return true;
		}
	}

	return false;
}



// OoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo
// O    Cache related stuff
// OoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo
void DnsInfo::addIpAddressesForDomainToCache( QString& domainName, QStringList& ipAddresses )
{
	// adding the domain to the list
	bool result = _resolvedDomains.contains( domainName );
	if( false == result )
		_resolvedDomains.insert( domainName, DomainInfo() );

	// adding the domain's IP addresses
	_resolvedDomains[domainName]._ipAddresses = ipAddresses;
}

void DnsInfo::addMailServersForDomainToCache( QString& domainName, QStringList& mailServers )
{
	// adding the domain to the list
	bool result = _resolvedDomains.contains( domainName );
	if( false == result )
		_resolvedDomains.insert( domainName, DomainInfo() );
		
	// adding the domain's mail servers
	_resolvedDomains[domainName]._mailServers = mailServers;
}

bool DnsInfo::getCachedIpAddressesForDomain( QString& domainName, QStringList& ipAddresses )
{
	// find the domain
	bool result = _resolvedDomains.contains( domainName );
	if( false == result )
		return false;

	// get the IP addresses for the domain
	if( _resolvedDomains[domainName]._ipAddresses.isEmpty() )
		return false;

	ipAddresses = _resolvedDomains[domainName]._ipAddresses;

	return true;
}

bool DnsInfo::getCachedMailServersForDomain( QString& domainName, QStringList& mailServers )
{
	// find the domain
	bool result = _resolvedDomains.contains( domainName );
	if( false == result )
		return false;

	// get the IP addresses for the domain
	if( _resolvedDomains[domainName]._mailServers.isEmpty() )
		return false;

	mailServers = _resolvedDomains[domainName]._mailServers;

	return true;
}
