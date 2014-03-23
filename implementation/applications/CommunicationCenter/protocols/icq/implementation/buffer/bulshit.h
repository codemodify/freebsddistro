
// Takes an ip from the buffer class and converts it to network byte order
unsigned long PacketIpToNetworkIp(unsigned long l)
{
  return htonl((l << 24) | ((l & 0xff00) << 8) | ((l & 0xff0000) >> 8) | (l >> 24));
}


// Takes an ip in network order and converts it to the packet class format
unsigned long NetworkIpToPacketIp(unsigned long l)
{
  l = ntohl(l);
  return (l << 24) | ((l & 0xff00) << 8) | ((l & 0xff0000) >> 8) | (l >> 24);
}


// Endianness utility routines: Unlike Real Internet Protocols, this
// heap of dung uses little-endian byte sex.  With the new v7 and above
// this heap of SHIT uses little-endian and big-endian byte sex.

unsigned short get_be_short(char *p)
{
  unsigned char *q = (unsigned char *)p;
  return (q[0] << 8) + q[1];
}

unsigned int get_be_int(char *p)
{
  unsigned char *q = (unsigned char *)p;
  return (q[0] << 24) + (q[1] << 16) + (q[2] << 8) + q[3];
}

unsigned long get_be_long(char *p)
{
  unsigned char *q = (unsigned char *)p;
  return (((unsigned long)(q[0]) << 24) +
    ((unsigned long)(q[1]) << 16) +
    ((unsigned long)(q[2]) << 8) +
    ((unsigned long)q[3]));
}

void put_be_short(char *p, unsigned short x)
{
  unsigned char *q = (unsigned char *)p;
  q[0] = ((x & 0xff00) >> 8);
  q[1] = (x & 0x00ff);
}

void put_be_int(char *p, unsigned int x)
{
  unsigned char *q = (unsigned char *)p;
  q[0] = ((x & 0xff000000) >> 24);
  q[1] = ((x & 0x00ff0000) >> 16);
  q[2] = ((x & 0x0000ff00) >> 8);
  q[3] = ((x & 0x000000ff));
}

void put_be_long(char *p, unsigned long x)
{
  unsigned char *q = (unsigned char *)p;
  q[0] = ((x & 0xff000000) >> 24);
  q[1] = ((x & 0x00ff0000) >> 16);
  q[2] = ((x & 0x0000ff00) >> 8);
  q[3] = ((x & 0x000000ff));
}

// return short (16-bit) stored in little-endian format, possibly unaligned
unsigned short get_le_short(char *p)
{
   unsigned char *q = (unsigned char *)p;
   return q[0] + (q[1] << 8);
}

// return int (32-bit) stored in little-endian format, possibly unaligned
unsigned int get_le_int(char *p)
{
   unsigned char *q = (unsigned char *)p;
   return q[0] + (q[1] << 8) + (q[2] << 16) + (q[3] << 24);
}

// return long (32-bit) stored in little-endian format, possibly unaligned
unsigned long get_le_long(char *p)
{
   unsigned char *q = (unsigned char *)p;
   // $C6.1 Promotions: unsigned char gets converted to int by default
   return ((unsigned long)q[0]) +
       (((unsigned long)q[1]) << 8) +
       (((unsigned long)q[2]) << 16) +
       (((unsigned long)q[3]) << 24);

}

// store 16-bit short in little-endian format, possibly unaligned
void put_le_short(char *p, unsigned short x)
{
   unsigned char *q = (unsigned char*)p;
   q[0] = x & 0xff;
   q[1] = (x >> 8) & 0xff;
}

// store 32-bit int in little-endian format, possibly unaligned
void put_le_int( char *p, unsigned int x)
{
   unsigned char *q = (unsigned char*)p;
   q[0] = x & 0xff;
   q[1] = (x >> 8) & 0xff;
   q[2] = (x >> 16) & 0xff;
   q[3] = (x >> 24) & 0xff;
}

// store 32-bit int in little-endian format, possibly unaligned
void put_le_long(char *p, unsigned long x)
{
   unsigned char *q = (unsigned char*)p;
   q[0] = x & 0xff;
   q[1] = (x >> 8) & 0xff;
   q[2] = (x >> 16) & 0xff;
   q[3] = (x >> 24) & 0xff;
}

//---- Reverse Endianism Functions -------------

void rev_e_short(unsigned short &x)
{
  unsigned short tmp = 0;

  tmp = ((x & 0x00ff) << 8);
  tmp |= ((x & 0xff00) >> 8);

  x = tmp;
}

void rev_e_long(unsigned long &x)
{
  unsigned long tmp = 0;

  tmp = ((x & 0x000000ff) << 24);
  tmp |= ((x & 0x0000ff00) << 8);
  tmp |= ((x & 0x00ff0000) >> 8);
  tmp |= ((x & 0xff000000) >> 24);

  x = tmp;
}

//=====TLV Structs =============================================================
struct SOscarTLV
{
  unsigned short nType;
  unsigned short nLen;
  unsigned char *pData;
};

struct SOscarTLV_Chain
{
        SOscarTLV *pTLV;
        SOscarTLV_Chain *pNext;
};
