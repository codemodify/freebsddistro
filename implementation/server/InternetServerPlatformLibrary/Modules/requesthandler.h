#ifndef requesthandler_h
#define requesthandler_h


// local headers
#include "../module.h" 


/****************************************************************************************
*
*	This class defines a request handler
*
****************************************************************************************/
class RequestHandler : public Module
{
public:
	virtual ~RequestHandler(){};
	
public:
	virtual QByteArray hadleRequest( QByteArray& request )

};

Q_DECLARE_INTERFACE( ContentHandler, "mysd.isp.modules.RequestHandler/1.0" );

#endif
