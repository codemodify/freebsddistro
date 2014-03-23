#ifndef requestvalidator_h
#define requestvalidator_h


// local headers
#include "../module.h" 


/*************************************************************************
*
*	This class defines a request validator 
*
*************************************************************************/
class RequestValidator : public Module
{
public:
	virtual ~RequestValidator(){};
	
public:

	// gets the type of the request that this module will validate
	virtual QString getRequestType() = 0;
	
	// this method is called to validate the request, if false
	// is returned the client will not be served
	virtual bool isRequestValid( QByteArray& request ) = 0;

};

Q_DECLARE_INTERFACE( RequestValidator, "mysd.isp.modules.RequestValidator/1.0" );

#endif
