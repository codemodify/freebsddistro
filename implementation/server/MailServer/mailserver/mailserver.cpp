
// Qt headers
#include <QtXml/QDomDocument>
#include <QtXml/QDomNamedNodeMap>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QDir>

// local headers
#include "mailserver.h"
#include "services/smtp/smtp.h"
#include "services/pop/pop.h"
#include "services/mailserverconfigvariables.h"


quint16 smtpPort = 50025;
quint16 popPort = 50110;
QString network = "any";
QString mailStorage = "/var/spool/mail";
MailServers mailServers;
MailDatabase* mailDatabase;


MailServer::MailServer( int argc, char** argv ):
	QCoreApplication(argc,argv)
{
	_configFilePath = argv[1];
}

bool MailServer::init()
{
	bool result = loadConfigFile();
	if( false == result )
		return false;

	mailDatabase = new MailDatabase( mailStorage );		

	result = prepareStorage();
	if( false == result )
		return false;

	_smtpService = new SmtpService();
	if( true == _smtpService.isNull() )
		return false;
	
	_popService = new PopService();
	if( true == _popService.isNull() )
		return false;
		
	return true;
}

MailServer::~MailServer()
{
	MailServers::const_iterator it = mailServers.constBegin();
	while( it != mailServers.constEnd() )
	{
		Accounts* accounts = it.value();
		accounts->clear();
		
		delete accounts;
		
		++it;
	}
	
	mailServers.clear();
	
	delete mailDatabase;
}

bool MailServer::prepareStorage()
{
	MailServers::const_iterator it = mailServers.constBegin();
	while( it != mailServers.constEnd() )
	{
		QString domain = it.key();
		Accounts* accounts = it.value();
		
		foreach( QString account, *accounts )
		{
			QDir().mkpath( mailDatabase->getPathForMail(domain,account) );
			QDir().mkpath( mailDatabase->getPathForTrash(domain,account) );
		}
		
		++it;
	}
	
	return true;
}

bool MailServer::loadConfigFile()
{
	// checking if we can open file for reading
	QFile file( _configFilePath );
	if( false == file.open(QIODevice::ReadOnly) )
		return false;

	QDomDocument domDocument;
	domDocument.setContent( &file );
	file.close();
	
	// getting the server config options
	QDomNode serverNode = domDocument.documentElement();	
	QDomNamedNodeMap serverNodeAttributes = serverNode.attributes();
	
	network = serverNodeAttributes.namedItem("network").nodeValue();
	smtpPort = serverNodeAttributes.namedItem("smtpPort").nodeValue().toInt();
	popPort = serverNodeAttributes.namedItem("popPort").nodeValue().toInt();
	mailStorage = serverNodeAttributes.namedItem("mailStorage").nodeValue();
	
	// getting the domains
	QDomNode domainNode = serverNode.firstChild();
	while( false == domainNode.isNull() )
	{
		QDomNamedNodeMap domainNodeAttributes = domainNode.attributes();
		QString domainName = domainNodeAttributes.namedItem("name").nodeValue();
		
		// here we get the accounts configured for the domain
		Accounts* accounts = new Accounts();
		QDomNode accountNode = domainNode.firstChild();
		while( false == accountNode.isNull() )
		{
			QDomNamedNodeMap accountNodeAttributes = accountNode.attributes();
			QString account = accountNodeAttributes.namedItem("account").nodeValue();
			QString password = accountNodeAttributes.namedItem("account").nodeValue();
			QString info = accountNodeAttributes.namedItem("info").nodeValue();
			
			accounts->insert( account, password );
			
			accountNode = accountNode.nextSibling();
		}
		
		mailServers.insert( domainName, accounts );
		
		domainNode = domainNode.nextSibling();
	}
	
	return true;
}
