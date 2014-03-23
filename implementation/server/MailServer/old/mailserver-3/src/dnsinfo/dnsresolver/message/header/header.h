#ifndef _HEADER_H_
#define _HEADER_H_

// local headers
#include <QtCore/QByteArray>

/******************************************************************************
*
*	This class works with a Header at low level.
*	It does not have knowledge about the logics or meanings of what is contains,
*	it just returns/sets the bytes that it was asked to, in a way taht will conform 
*	to the DNS protocol.
*
******************************************************************************/
class Header
{

public:
	Header();
	~Header();
	
	bool       fromByteArray( QByteArray& byteArray, int& index );
	QByteArray toByteArray();
	void       empty();
	
	quint16 getID();
	quint8  getQR();
	quint8  getOpcode();
	quint8  getAA();
	quint8  getTC();
	quint8  getRD();
	quint8  getRA();
	quint8  getZ();
	quint8  getRcode();
	quint16 getQDcount();
	quint16 getANcount();
	quint16 getNScount();
	quint16 getARcount();

	void setID     ( quint16 id );
	void setQR     ( quint8  qr );
	void setOpcode ( quint8  opcode );
	void setAA     ( quint8  aa );
	void setTC     ( quint8  tc );
	void setRD     ( quint8  rd );
	void setRA     ( quint8  ra );
	void setZ      ( quint8  z );
	void setRcode  ( quint8  rcode );
	void setQDcount( quint16 qdCount );
	void setANcount( quint16 anCount );
	void setNScount( quint16 nsCount );
	void setARcount( quint16 arCount );
	
	struct ID    { static quint16 generateID(){ return 1; } };
	struct QR    { enum { Query=0, Response }; };
	struct OPCODE{ enum { StandardQuery=0, InverseQuery, ServerStatus }; };
	struct AA    { enum { NonAuthoritativeAnswer=0, AuthoritativeAnswer }; };
	struct TC    { enum { NoTruncation=0, Truncation }; };
	struct RD    { enum { NoRecursionDesired=0, RecursionDesired }; };
	struct RA    { enum { NoRecursionAvailable=0, RecursionAvailable }; };
	struct RCODE { enum { NoError=0, FormatError, ServerFailure, NameError, NotImplemented, Refused }; };
	

private:
	struct HeaderData
	{
		quint16 _id;
                        // | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  |
		quint8 _flagsHi; // | QR |      Opcode       | AA | TC | RD |
		quint8 _flagsLo; // | RA |       Z      |       RCODE       |

		quint8 _qdCountHi; // number of entries in the question section
		quint8 _qdCountLo; //
		
		quint8 _anCountHi; // number of resource records in the answer section
		quint8 _anCountLo; //

		quint8 _nsCountHi; // number of name server resource records in the authority records section
		quint8 _nsCountLo; //

		quint8 _arCountHi; // number of resource records in the additional records section
		quint8 _arCountLo; //
	};	

	HeaderData _data;

};

#endif
