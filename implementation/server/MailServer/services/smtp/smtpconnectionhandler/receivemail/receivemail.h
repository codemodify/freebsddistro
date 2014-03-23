#ifndef _RECEIVE_MAIL_H_
#define _RECEIVE_MAIL_H_

// Qt headers
#include <QtCore/QList>
#include <QtCore/QStringList>

class QTcpSocket;
class QByteArray;

typedef QList<QByteArray> ByteArrayList;

// this function speaks SMTP(conforming to RFC) and receives a mail from a SMTP speaking client.
bool receiveMail( QTcpSocket& tcpSocket, 
                  QString& from, QString& replyAddress, QStringList& destinations, 
				  QByteArray& mailContent, ByteArrayList& attachments );

#endif
