#ifndef __WebServer_H__
#define __WebServer_H__

// blocks
#include <Pluginer/PluginImplementation.h>

// ISP headers
#include <InternetServerPlatformLibrary/Modules/ISocketDaemon.h>
#include <InternetServerPlatformLibrary/Components/Components.h>


class JerryMouse : public QObject, public AbstractPlugin, public ISocketDaemon
{
    Q_OBJECT
    Q_INTERFACES( IPlugin )
    Q_PLUGINABLE( JerryMouse )

	// obligatory from IPlugin
	public:
        bool doInit();
        void unInit();

    // obligatory from ISocketDaemon
    public:
        ISP_NO_TRACK_FOR_CONNECTIONS();
		void dataRequest( QByteArray& request, QByteArray& response, bool& requestIsValid );

};

#endif
