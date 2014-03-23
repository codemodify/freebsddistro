
// local headers
#include "protocol.h"

Protocol::Protocol()
{
}

Protocol::~Protocol()
{
}

void Protocol::conect( QString user, QString password )
{}

void Protocol::sendMessage( QString contactID, QString message )
{}

void Protocol::sendFile( QString contactID, QString file )
{}

void Protocol::changeStatus( int statusID )
{}

ProtocolID Protocol::getProtocolID()
{
	return unknown;
}
