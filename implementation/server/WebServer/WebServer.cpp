
// Qt headers
#include <QtNetwork/QHttpRequestHeader>
#include <QtCore/QFileInfo>

// local headers
#include "WebServer.h"
#include "HttpCodes.h"

using namespace IspComponents;


bool WebServer::doInit()
{
    // populate data about the plugin
    _pluginInfo.name            = "WebServer";
    _pluginInfo.description     = "WebServer is an ISP module to publish data using HTTP";
    _pluginInfo.version         = "1.0";
    _pluginInfo.compatVersion   = "1.0";
    _pluginInfo.vendor          = "abcdefghi";
    _pluginInfo.copyright       = "abcdefghi (C) (TM) (R) and all the rest of smart letters";
    _pluginInfo.license         = "absolutely free";
    _pluginInfo.url             = "www.nikushor.eu";

    // do the resource init
	QString settingsFilePath = getPluginInfo().pluginPath + ".config";
	Logging::logInfo( this, QString("config file: %1").arg(settingsFilePath) );

	// get pointers to all the key DOM nodes
	QDomDocument	settingsFileAsXml;
	QDomNode		webserver			= Dom::loadXmlFileAndGetRootNode( settingsFilePath, settingsFileAsXml );
	QDomNode		webserverSettings	= IspComponents::Dom::getChildNodeByName( webserver,	"webserverSettings"	);
	QDomNode		domains 			= IspComponents::Dom::getChildNodeByName( webserver,	"domains"				);
	QDomNode		defaultValues		= IspComponents::Dom::getChildNodeByName( domains,		"defaultValues"		);
	QDomNode		domain			= IspComponents::Dom::getChildNodeByName( domains,		"domain"				);		

	// reading the webserver's settings
	if( false == readWebserverSettings(webserverSettings) )
		return false;

	// reading the deafaults
	DefaultFileList defaultFiles;
	readDefaultFileList( defaultValues, defaultFiles   );

	Actions defaultActions;
	readActions( defaultValues, defaultActions );		
	
	// read the configured domains
	while( false == domain.isNull() )
	{
		if( QDomNode::CommentNode != domain.nodeType() )
		{
			QString domainName    = domain.attributes().namedItem( "name"    ).nodeValue();
			QString domainContent = domain.attributes().namedItem( "content" ).nodeValue();
			Logging::logInfo( this, QString("domain: [%1] -> %2").arg(domainName).arg(domainContent) );

			// build domain settigns
			DomainSettings* domainSettings = new DomainSettings();

			readDefaultFileList( domain, domainSettings->_defaultFileList );
			if( domainSettings->_defaultFileList.isEmpty() )
				domainSettings->_defaultFileList = defaultFiles;
						
			readActions( domain, domainSettings->_actions );
			if( domainSettings->_actions.isEmpty() )
				domainSettings->_actions = defaultActions;

			domainSettings->_content = domainContent;

			// adding domain to the list
			_domainList.insert( domainName, domainSettings );

			Aliases aliases;
			readAliases( domain, aliases );

			foreach( QString alias, aliases )
				_domainList.insert( alias, domainSettings );
		}
		
		domain = domain.nextSibling();
	}
		
	return true;
}

void WebServer::unInit()
{}

void WebServer::dataRequest( QByteArray& request, QByteArray& response, bool& requestIsValid )
{
	// this is a trick to confuse intruders, whatever he asks we say OK
	response = HTTP_OK;

	// check for request validity
	QHttpRequestHeader httpRequestHeader( request );

	if( requestIsValid = httpRequestHeader.isValid(), false == requestIsValid )
		return;

	// getting the asked 'host' and 'resource' 
	QString domain = httpRequestHeader.value( "host" );
	if(	domain.contains(":") )
		domain.resize( domain.indexOf(":") );
	
	QString path = httpRequestHeader.path();	
	
	// log some brief info about the request
	Logging::logInfo( this, "---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----" );
	Logging::logInfo( this, "---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----" );
	Logging::logInfo( this, QString( "%1: %2 %3" ).arg( httpRequestHeader.method() ).arg( domain ).arg( path )               );
	
	// dump the request in raw form if requested
	if( _webServerSettings._dumpHttpHeaders )
	{
		Logging::logInfo( this, "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo dumpHttpHeaders() start" );

		Logging::logInfo( this, QString("RAW:\n %1").arg(request.data()) );

		Logging::logInfo( this, "PARSED:" );
		QList< QPair<QString,QString> > values = httpRequestHeader.values();
		for( int index=0; index < values.size(); ++index )
		{
			QPair<QString,QString> pair = values.at( index );
			Logging::logInfo( this, QString("%1 : %2").arg(pair.first).arg(pair.second) );
		}
		values.clear();

		Logging::logInfo( this, "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo dumpHttpHeaders() end" );
	}
	
	// geting requested resource
	if( false == _domainList.contains(domain) )
		return; 

	DomainSettings* domainSettings = _domainList[ domain ];

	fetchResourceContent( response, *domainSettings, path );
}


void WebServer::fetchResourceContent( QByteArray& response, DomainSettings& domainSettings, QString& resource )
{
	QString fullResourcePath = domainSettings._content + "/" + resource;
	Logging::logInfo( this, QString("fetching: %1").arg(fullResourcePath) );
	 
	bool result = QFileInfo(fullResourcePath).isFile();
	if( false == result )
	{
		Logging::logInfo( this, "not found, looking for some default values" );
		foreach( QString resource, domainSettings._defaultFileList )
		{
			fullResourcePath = domainSettings._content + "/" + resource;
			Logging::logInfo( this, QString("fetching: %1").arg(fullResourcePath) );
			
			result = QFileInfo(fullResourcePath).isFile();
			if( result )
				break;
			
			Logging::logInfo( this, "not valid" );
		}
	}
	
	if( false == result )
	{
		Logging::logInfo( this, "nothing valid have been found" );
		return;
	}

	QFile file( fullResourcePath );
	if( !file.open(QFile::ReadOnly) )
		return;
		
	response += file.readAll();
	file.close();
}

bool WebServer::readWebserverSettings( QDomNode parentNode )
{
	QDomNode dumpHttpHeadersNode = IspComponents::Dom::getChildNodeByName( parentNode, "dumpHttpHeaders" );
	if( dumpHttpHeadersNode.isNull() )
	{
		Logging::logError( this, "DOM : webserver.webserverSettings.dumpHttpHeaders is null" );
		return false;
	}

	QString dumpHttpHeaders =  dumpHttpHeadersNode.attributes().namedItem("value").nodeValue();
	Logging::logInfo( this, QString("dumpHttpHeaders=%1").arg(dumpHttpHeaders) );

	_webServerSettings._dumpHttpHeaders = (dumpHttpHeaders == "yes");

	return true;
}

void WebServer::readDefaultFileList( QDomNode parentNode, DefaultFileList& defaultFileList )
{
	QDomNode defaultFileListNode = IspComponents::Dom::getChildNodeByName( parentNode, "defaultFileList" );
	if( defaultFileListNode.isNull() )
	{
		Logging::logInfo( this, QString("DOM : webserver.domains.%1.defaultFileList is null").arg(parentNode.nodeName()) );
		return;
	}

	defaultFileList = defaultFileListNode.attributes().namedItem("value").nodeValue().split(",");
	foreach( QString file, defaultFileList )
		Logging::logInfo( this, QString("default file to look for: %1").arg(file) );
}

void WebServer::readAliases( QDomNode parentNode, Aliases& aliases )
{
	QDomNode aliasesNode = IspComponents::Dom::getChildNodeByName( parentNode, "aliases" );
	if( aliasesNode.isNull() )
	{
		Logging::logInfo( this, QString("DOM : webserver.domains.%1.aliases is null").arg(parentNode.nodeName()) );
		return;
	}

	aliases = aliasesNode.attributes().namedItem("value").nodeValue().split(",");
	foreach( QString alias, aliases )
		Logging::logInfo( this, QString("alias: %1").arg(alias) );		
}

void WebServer::readActions( QDomNode parentNode, Actions& actions )
{
	QDomNode actionsNode = IspComponents::Dom::getChildNodeByName( parentNode, "actions" );
	if( actionsNode.isNull() )
	{
		Logging::logInfo( this, QString("DOM : webserver.domains.%1.actions is null").arg(parentNode.nodeName()) );
		return;
	}
		
	QDomNode action = actionsNode.firstChild();
	while( false == action.isNull() )
	{
		if( QDomNode::CommentNode != actionsNode.nodeType() )
		{
			QString executable	= action.attributes().namedItem( "executable" ).nodeValue();
			QString params	= action.attributes().namedItem( "params"     ).nodeValue();
			
			Action theAction = qMakePair( executable, params );
			
			FileSuffix fileSuffix = actionsNode.nodeName();
			actions.insert( fileSuffix, theAction );
			
			Logging::logInfo( this, QString("action for file-suffix: %1 -> %2 %3").arg(fileSuffix).arg(executable).arg(params) );			
		}
		
		action = action.nextSibling();
	}
}

SetPluginImplementation( WebServer )
