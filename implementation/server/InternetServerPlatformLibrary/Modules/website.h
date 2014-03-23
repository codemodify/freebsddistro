#ifndef WEBSITE_H
#define WEBSITE_H

// Qt headers
#include <QtCore/QByteArray>
#include <QtPlugin>
#include <QtCore/QString>

// local headers
//#include "../module.h" 

/****************************************************************************************
*
*	This class defines a web-site.
*
*	To build your own website inherit this class.
*
****************************************************************************************/
class WebSite
{
public:
	virtual ~WebSite(){};

public: // these methods are used by the webserver at starting and stoping of a module  
	virtual bool initialize() = 0;
	virtual void shutdown() = 0;

public:
	virtual QString getName() = 0;
	virtual QString getDescription() = 0;
 
public: // this method is used to pass the requests from the client to the site
	virtual QByteArray invoke( QByteArray request ) = 0;

};

Q_DECLARE_INTERFACE( WebSite, "mysd.plugins.webserver.modules.WebSite/1.0" );

#endif
