
// Qt headers
#include <QtCore/QStringList>
#include <QtCore/QByteArray>

// local headers
#include "utility.h"

// check if a label size actually represents a pointer to a label
#define IS_BYATE_A_POINTER(byte) (byte & 0xC0)

// some DNS limits agreed on RFC
#define MAX_DOMAIN_NAME_LENGTH    255
#define MAX_DOMAIN_LABEL_LENGTH   63
#define MAX_DOMAIN_POINTERS_COUNT 65530


bool getStringFromDnsFormat( QByteArray& byteArray, int& offset, QByteArray& name )
{
	if( offset >= byteArray.size() )
		return false;
	if( 0 == byteArray.at(offset++) )
		return false;

	quint16 localOffset = --offset;
	quint16 pointersCount = 0;
	quint8 labelSize = byteArray.at( localOffset );
	while( labelSize > 0 )
	{
		if( IS_BYATE_A_POINTER(labelSize) ) // some trash data claims to be a pointer
		{
			// getting the pointer value so we can find the real label
			if( (localOffset+2) > byteArray.size() )
				return false;

			quint8 v1 = byteArray.at(localOffset++) & 63;
			quint8 v2 = byteArray.at(localOffset);			
			quint16 pointerValue = ((quint16)v1) << 8;
			pointerValue |= v2;
			
			
			if( 0 == pointerValue ) 
				return false;
			else if( ++pointersCount > MAX_DOMAIN_POINTERS_COUNT )
				return false;
			
			// we update the global offset only at first pointer encounter
			if( 1 == pointersCount )
				offset += 2;

			localOffset = pointerValue;
			labelSize = byteArray.at( localOffset );
		}
		// check if somebody is fucking us
		if( labelSize > MAX_DOMAIN_LABEL_LENGTH )
			return false;
		if( labelSize+localOffset > byteArray.size() )
			return false;
			
		localOffset++;
		
		name += byteArray.mid( localOffset, labelSize );
		name += '.';
		localOffset += labelSize;
		labelSize = byteArray.at( localOffset );
	}
	
	// removing the last '.' char added in the algorithm
	name.truncate( name.size()-1 );
	
	// in case no pointers have been met, setting the offset so the caller will know from where to continue
	if( 0 == pointersCount )
		offset = localOffset+1;

	return true;
}

bool convertStringToDnsFormat( QByteArray& string, QByteArray& stringInDnsFormat )
{
	QStringList labels = QString(string).split(".");
	foreach( QString label, labels )
	{
		stringInDnsFormat += label.length();
		stringInDnsFormat += label.toAscii();
	}
	
	// adding a byte that contains a zero
	stringInDnsFormat += (char)0;
	
	return true;
}
