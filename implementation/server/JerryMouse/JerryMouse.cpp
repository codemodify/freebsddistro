
// Qt headers
#include <QtNetwork/QHttpRequestHeader>
#include <QtCore/QFileInfo>

// local headers
#include "JerryMouse.h"

using namespace IspComponents;


bool JerryMouse::doInit()
{
    // populate data about the plugin
    _pluginInfo.name            = "JerryMouse";
    _pluginInfo.description     = "JerryMouse is an ISP module to publish data using HTTP";
    _pluginInfo.version         = "1.0";
    _pluginInfo.compatVersion   = "1.0";
    _pluginInfo.vendor          = "abcdefghi";
    _pluginInfo.copyright       = "abcdefghi (C) (TM) (R) and all the rest of smart letters";
    _pluginInfo.license         = "absolutely free";
    _pluginInfo.url             = "www.nikushor.eu";

    // do the resource init
	QString settingsFilePath = getPluginInfo().pluginPath + ".config";
	Logging::logInfo( this, QString("config file: %1").arg(settingsFilePath) );
/*
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
*/

	return true;
}

void JerryMouse::unInit()
{}

void JerryMouse::dataRequest( QByteArray& request, QByteArray& response, bool& requestIsValid )
{
/*
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
*/
}

SetPluginImplementation( JerryMouse )
