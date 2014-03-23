#ifndef _RESOURCE_RECORD_H_
#define _RESOURCE_RECORD_H_

// Qt headers
#include <QtCore/QByteArray>



/******************************************************************************
*
*	This class works with a RR at low level.
*	It does not have knowledge about the type of the RR, its class etc, it just 
*	returns/sets the bytes that it was asked to, in a way that will conform 
*	to the DNS protocol.
*
******************************************************************************/
class ResourceRecord
{

public:
	ResourceRecord();
	~ResourceRecord();


public:
	bool       fromByteArray( QByteArray& byteArray, int& index );
	QByteArray toByteArray();
	
	QByteArray getName();
	quint16    getType();
	quint16    getClass();
	quint32    getTTL();
	quint16    getRDataLength();
	QByteArray getRData();
	
	void setName( QByteArray& name );
	void setType( quint16 type );
	void setClass( quint16 theClass );
	void setTTL( quint32 ttl );
	void setRDataLength( quint16 rdLength );
	void setRData( QByteArray& rData );
	

protected:
	QByteArray _name;
	quint16    _type;
	quint16    _class;
	quint32    _ttl;
	quint16    _rdLength;
	QByteArray _rdata;

};

#endif
