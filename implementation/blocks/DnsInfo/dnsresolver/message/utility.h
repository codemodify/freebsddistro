#ifndef _UTILITY_H_
#define _UTILITY_H_


// this function extracts and converts a label from a DNS protocol formated buffer to a usual sequence of chars
// it takes into account the pointers that might be present in the buffer
// the 'offset' will be updated to show where the inside algo(read with french accent) has left-off
bool getStringFromDnsFormat( QByteArray& byteArray, int& offset, QByteArray& string );

// this function converts a usual string into a DNS formated representation of the string
// no pointers(compression) is used for this
bool convertStringToDnsFormat( QByteArray& string, QByteArray& stringInDnsFormat );

#endif
