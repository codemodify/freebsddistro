#ifndef BUFFER_H_
#define BUFFER_H_

// Qt's includes
#include <QtCore/QByteArray>

// local headers
#include "bulshit.h"

class Buffer : public QByteArray
{
public:
	Buffer();
	~Buffer();



public: // ICQ speciffics

   //-- Big Endian --
   char *PackBE(CBuffer *);
   char *PackBE(const char *data, int size);
   char *PackStringBE(const char *data, unsigned short max = 0);
   char *PackUnsignedShortBE(unsigned short data);
   char *PackUnsignedLongBE(unsigned long data);

   char *Pack(CBuffer *);
   char *Pack(const char *data, int size);
   char *CBuffer::PackLNTS(const char *);
   char *PackString(const char *data, unsigned short max = 0);
   char *PackUnsignedShort(unsigned short data);
   char *PackUnsignedLong(unsigned long data);
   char *PackChar(char data);
   void Copy(CBuffer *);
   char *print(char *&);
   void Clear();
   void Reset();
   bool Empty();
   bool Full();
   bool End()  { return ( getDataPosRead() >= (getDataStart() + getDataSize()) ); }
   void Create(unsigned long _nDataSize = 0);

   //-- Big Endian --
   unsigned long UnpackUnsignedLongBE();
   unsigned short UnpackUnsignedShortBE();

   CBuffer& operator >> (char &in);
   CBuffer& operator >> (unsigned char &in);
   CBuffer& operator >> (unsigned short &in);
   CBuffer& operator >> (unsigned long &in);
   char *UnpackRaw(char *, unsigned short);
   char *UnpackString(char *, unsigned short);
   char *UnpackString();                // Need to delete[] returned string
   char *UnpackStringBE(char *, unsigned short);
   char *UnpackStringBE();              // Need to delete[] returned string
   char *UnpackUserString();            // Need to delete[] returned string
   unsigned long UnpackUnsignedLong();
   unsigned long UnpackUinString();
   unsigned short UnpackUnsignedShort();
   char UnpackChar();

   char *getDataStart() const           { return m_pDataStart; };
   char *getDataPosRead() const         { return m_pDataPosRead; };
   char *getDataPosWrite() const        { return m_pDataPosWrite; };
   unsigned long getDataSize() const    { return m_pDataPosWrite - m_pDataStart; };
   unsigned long getDataMaxSize() const { return m_nDataSize; };

   void setDataSize(unsigned long _nDataSize)  { m_nDataSize = _nDataSize; };
   void setDataPosWrite(char *_pDataPosWrite)  { m_pDataPosWrite = _pDataPosWrite; };
   void setDataPosRead(char *_pDataPosRead)  { m_pDataPosRead = _pDataPosRead; };
   void incDataPosWrite(unsigned long c)  { m_pDataPosWrite += c; };
   void incDataPosRead(unsigned long c)  { m_pDataPosRead += c; };

   //--- OSCAR Related Functions ------

   bool readTLV(int count = -1, int bytes = -1); // This should be called automatically if m_pTLV == 0
   void cleanupTLV();

   void PackTLV(unsigned short, unsigned short, const char *);
   void PackTLV(unsigned short, unsigned short, CBuffer *);

   unsigned short getTLVLen(unsigned short);
   bool hasTLV(unsigned short);

   unsigned long UnpackUnsignedLongTLV(unsigned short);
   unsigned short UnpackUnsignedShortTLV(unsigned short);
   unsigned char UnpackCharTLV(unsigned short);
   char *UnpackStringTLV(unsigned short); // Need to delete[] returned string
   CBuffer UnpackTLV(unsigned short);

private:
   CBuffer& operator=(const CBuffer&);

protected:

   char *m_pDataStart,
        *m_pDataPosWrite,
        *m_pDataPosRead;
   unsigned long m_nDataSize;
   SOscarTLV_Chain *m_pTLV;

   SOscarTLV *getTLV(unsigned short _nType);
   void antiwarning() { NetworkIpToPacketIp(PacketIpToNetworkIp(127)); }
	
};

#endif
