#ifndef _MESSAGE_H_
#define _MESSAGE_H_

// Qt headers
#include <QtCore/QByteArray>
#include <QtCore/QList>


class Header;
class Question;
class ResourceRecord;

class Message
{

public:
	Message();
	~Message();

public:
	bool fromByteArray( QByteArray& byteArray );
	QByteArray toByteArray();

	
public: // these interfaces are preffered for usage, 
		// if some speciffics are needed then there is provided access to the declarations below
	void buildQueryMessagetForARecord( QByteArray domainName );
	void buildQueryMessagetForMXRecord( QByteArray domainName );
	
	void getServers( QStringList& serverList );
	

public:
	typedef QList<ResourceRecord*> ResourceRecords;
	
	Header*         _header;
	Question*       _question;
	ResourceRecords _answers;
	ResourceRecords _authorities;
	ResourceRecords _additionals;


private:
	void cleanUp();
	void init();

};

#endif
