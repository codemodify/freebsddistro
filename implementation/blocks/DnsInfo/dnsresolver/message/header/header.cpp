
// loca headers
#include "header.h"

Header::Header()
{
}

Header::~Header()
{
}
	
bool Header::fromByteArray( QByteArray& byteArray, int& index )
{
	if( byteArray.size() < sizeof(HeaderData) )
		return false;

	memcpy( (void*)&_data, (void*)(byteArray.data()+index), sizeof(HeaderData) );
	
	index+= sizeof(HeaderData);

	return true;
}

QByteArray Header::toByteArray()
{
	// this is a reserved field, and docs say to make sure it is zero
	setZ( 0 );
	
	return QByteArray::fromRawData( reinterpret_cast<const char*>(&_data), sizeof(HeaderData)  );
}

void Header::empty()
{
	memset( static_cast<void*>(&_data), 0, sizeof(HeaderData) );
}

quint16 Header::getID()     { return _data._id; }
quint8  Header::getQR()     { return ((_data._flagsHi & 0x80) >> 7); } //  10000000
quint8  Header::getOpcode() { return ((_data._flagsHi & 0x78) >> 3); } //  01111000
quint8  Header::getAA()     { return ((_data._flagsHi & 0x4 ) >> 2); } //  00000100
quint8  Header::getTC()     { return ((_data._flagsHi & 0x2 ) >> 1); } //  00000010
quint8  Header::getRD()     { return ((_data._flagsHi & 0x1 ) >> 0); } //  00000001
quint8  Header::getRA()     { return ((_data._flagsHi & 0x80) >> 7); } //  10000000
quint8  Header::getZ()      { return ((_data._flagsHi & 0x70) >> 4); } //  01110000
quint8  Header::getRcode()  { return ((_data._flagsHi & 0x1 ) >> 0); } //  00000001
quint16 Header::getQDcount(){ quint16 result = _data._qdCountLo; result |= ((quint16)_data._qdCountHi)<<8; return result; }
quint16 Header::getANcount(){ quint16 result = _data._anCountLo; result |= ((quint16)_data._anCountHi)<<8; return result; }
quint16 Header::getNScount(){ quint16 result = _data._nsCountLo; result |= ((quint16)_data._nsCountHi)<<8; return result; }
quint16 Header::getARcount(){ quint16 result = _data._arCountLo; result |= ((quint16)_data._arCountHi)<<8; return result; }

void Header::setID     ( quint16 id      ){        _data._id = id; }
void Header::setQR     ( quint8  qr      ){    _data._flagsHi|= ((qr    <<7) & 0x80); }
void Header::setOpcode ( quint8  opcode  ){    _data._flagsHi|= ((opcode<<3) & 0x78); }
void Header::setAA     ( quint8  aa      ){    _data._flagsHi|= ((    aa<<2) & 0x4 ); }
void Header::setTC     ( quint8  tc      ){    _data._flagsHi|= ((    tc<<1) & 0x2 ); }
void Header::setRD     ( quint8  rd      ){    _data._flagsHi|= ((    rd<<0) & 0x1 ); }
void Header::setRA     ( quint8  ra      ){    _data._flagsLo|= ((    ra<<7) & 0x80); }
void Header::setZ      ( quint8  z       ){    _data._flagsLo|= ((     z<<4) & 0x70); }
void Header::setRcode  ( quint8  rcode   ){    _data._flagsLo|= (( rcode<<0) & 0x1 ); }
void Header::setQDcount( quint16 qdCount ){ _data._qdCountHi = qdCount>>8; _data._qdCountLo = (quint8)qdCount; }
void Header::setANcount( quint16 anCount ){ _data._anCountHi = anCount>>8; _data._anCountLo = (quint8)anCount; }
void Header::setNScount( quint16 nsCount ){ _data._nsCountHi = nsCount>>8; _data._nsCountLo = (quint8)nsCount; }
void Header::setARcount( quint16 arCount ){ _data._arCountHi = arCount>>8; _data._arCountLo = (quint8)arCount; }
