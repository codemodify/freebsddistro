
// local headers
#include "question.h"
#include "dnsinfo/dnsresolver/message/utility.h"

Question::Question()
{}

Question::~Question()
{}

int Question::getSize()
{
	int size = _qname.size();
	size+= 2 * sizeof( quint16 );

	return size;
}

bool Question::fromByteArray( QByteArray& byteArray, int& index )
{
	// getting the query name
	if( false == getStringFromDnsFormat(byteArray,index,_qname) )
		return false;

	// qetting the type
	_qtype  = byteArray.at(index++);
	_qtype  = _qtype << 8;
	_qtype |= byteArray.at(index++); 
	
	// getting the class
	_qclass  = byteArray.at(index++);
	_qclass  = _qclass << 8;
	_qclass |= byteArray.at(index++); 

	return true;
}

QByteArray Question::toByteArray()
{
	QByteArray byteArray;
	
	// adding the QNAME
	convertStringToDnsFormat( _qname, byteArray );
	
	// adding the QTYPE
	quint16 tempValue = _qtype<<8;
	byteArray += QByteArray::fromRawData( reinterpret_cast<const char*>(&tempValue), sizeof(_qtype) );

	// adding the QCLASS
	tempValue = _qclass<<8;
	byteArray += QByteArray::fromRawData( reinterpret_cast<const char*>(&tempValue), sizeof(_qclass) );

	return byteArray;
}

QByteArray Question::getQname()
{
	return _qname;
}

quint16 Question::getQtype()
{
	return _qtype;
}

quint16 Question::getQclass()
{
	return _qclass;
}

void Question::setQname( QByteArray qname )
{
	_qname = qname;
}

void Question::setQtype( quint16 qtype )
{
	_qtype = qtype;
}

void Question::setQclass( quint16 qclass )
{
	_qclass = qclass;
}
