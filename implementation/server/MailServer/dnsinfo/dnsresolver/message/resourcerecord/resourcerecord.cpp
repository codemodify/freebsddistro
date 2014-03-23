
// local headers
#include "resourcerecord.h"
#include "dnsinfo/dnsresolver/message/utility.h"

ResourceRecord::ResourceRecord()
{}

ResourceRecord::~ResourceRecord()
{}

bool ResourceRecord::fromByteArray( QByteArray& byteArray, int& index )
{
	// getting the "Name"
	if( false == getStringFromDnsFormat(byteArray,index,_name) )
		return false;

	// getting the Type
	_type  = byteArray.at(index++);
	_type  = _type << 8;
	_type |= byteArray.at(index++);
	
	// getting the Class
	_class  = byteArray.at(index++);
	_class  = _class << 8;
	_class |= byteArray.at(index++);

	// getting the TTL
	quint32 ttl1 = 0; ttl1 = byteArray.at(index++);
	quint32 ttl2 = 0; ttl2 = byteArray.at(index++);
	quint32 ttl3 = 0; ttl3 = byteArray.at(index++);
	quint32 ttl4 = 0; ttl4 = byteArray.at(index++);
	_ttl = (ttl1<<24) | (ttl2<<16) | (ttl3<<8) | ttl4;

	// getting the RDLength
	_rdLength  = byteArray.at(index++);
	_rdLength  = _rdLength << 8;
	_rdLength |= byteArray.at(index++);

	// get the RData
	_rdata = byteArray.mid( index, _rdLength );
	
	// adjusting the index so the caller will know where we left-off
	index += _rdLength;
	
	return true;
}

QByteArray ResourceRecord::toByteArray()
{
	QByteArray byteArray;

	// adding the name
	convertStringToDnsFormat( _name, byteArray );

	// adding the type
	byteArray += QByteArray::fromRawData( reinterpret_cast<const char*>(_type), sizeof(_type) );

	// adding the class
	byteArray += QByteArray::fromRawData( reinterpret_cast<const char*>(_class), sizeof(_class) );

	// adding the ttl
	byteArray += QByteArray::fromRawData( reinterpret_cast<const char*>(_ttl), sizeof(_ttl) );

	// addint the rdLength
	byteArray += QByteArray::fromRawData( reinterpret_cast<const char*>(_rdLength), sizeof(_rdLength) );

	// adding the rdata
	byteArray += _rdata;

	return byteArray;
}

QByteArray ResourceRecord::getName()
{
	return _name;
}

quint16 ResourceRecord::getType()
{
	return _type;
}

quint16 ResourceRecord::getClass()
{
	return _class;
}

quint32 ResourceRecord::getTTL()
{
	return _ttl;
}

quint16 ResourceRecord::getRDataLength()
{
	return _rdLength;
}

QByteArray ResourceRecord::getRData()
{
	return _rdata;
}
	
void ResourceRecord::setName( QByteArray& name )
{
	_name = name;
}

void ResourceRecord::setType( quint16 type )
{
	_type = type;
}

void ResourceRecord::setClass( quint16 theClass )
{
	_class = theClass;
}

void ResourceRecord::setTTL( quint32 ttl )
{
	_ttl = ttl;
}

void ResourceRecord::setRDataLength( quint16 rdLength )
{
	_rdLength = rdLength;
}

void ResourceRecord::setRData( QByteArray& rData )
{
	_rdata = rData;
}
