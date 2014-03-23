#ifndef __WebServer_H__
#define __WebServer_H__

// blocks
#include <Pluginer/PluginImplementation.h>

// ISP headers
#include <InternetServerPlatformLibrary/Modules/ISocketDaemon.h>
#include <InternetServerPlatformLibrary/Components/Components.h>

// local headers
#include "Configuration.h"


class WebServer : public QObject, public AbstractPlugin, public ISocketDaemon
{
    Q_OBJECT
    Q_INTERFACES( IPlugin )
    Q_PLUGINABLE( WebServer )

	// obligatory from IPlugin
	public:
        bool doInit();
        void unInit();

    // obligatory from ISocketDaemon
    public:
        ISP_NO_TRACK_FOR_CONNECTIONS();
		void dataRequest( QByteArray& request, QByteArray& response, bool& requestIsValid );

	// helpers
	private:
		bool readWebserverSettings	( QDomNode parentNode									);
		void readDefaultFileList	( QDomNode parentNode, DefaultFileList& defaultFileList	);
		void readAliases			( QDomNode parentNode, Aliases& sliases					);
		void readActions			( QDomNode parentNode, Actions& actions					);
	
		void fetchResourceContent( QByteArray& response, DomainSettings& domainSettings, QString& resource );
	
	private:
		WebServerSettings	_webServerSettings;
		DomainList		_domainList;
};

#endif
