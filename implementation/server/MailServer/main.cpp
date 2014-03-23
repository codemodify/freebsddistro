
// local headers
#include "mailserver/mailserver.h"

int main( int argc, char** argv )
{	
	// creating mail-server object, and run it
	MailServer mailServer( argc, argv );
	if( false == mailServer.init() )
		return -1;
	
	return mailServer.exec();
}
