#ifndef _QUESTION_H_
#define _QUESTION_H_

// Qt headers
#include <QtCore/QByteArray>



/******************************************************************************
*
*	This class works with a Question at low level.
*	It does not have knowledge about the type of the Question, its class etc, it just 
*	returns/sets the bytes that it was asked to, in a way taht will conform 
*	to the DNS protocol.
*
******************************************************************************/
class Question
{

public:
	Question();
	~Question();


public:
	int getSize();
	
	bool       fromByteArray( QByteArray& byteArray, int& index );
	QByteArray toByteArray();

	QByteArray getQname();
	quint16    getQtype();
	quint16    getQclass();

	void setQname( QByteArray qname );
	void setQtype( quint16 qtype );
	void setQclass( quint16 qclass );
	
	struct QTYPE { enum{ A=1, NS, MD, MF, CNAME, SOA, MB, MG, MR, NUL, WKS, PTR, HINFO, MINFO, MX, TXT, AXFR=252, MAILB, MAILA, ALL }; };
	struct QCLASS{ enum{ IN=0, CS, CH, HS, CLASS_ALL=255 }; };


private:
	QByteArray _qname;
	quint16    _qtype;
	quint16    _qclass;

};

#endif
