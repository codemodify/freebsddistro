#ifndef _SEND_MAIL_H_
#define _SEND_MAIL_H_

#include "services/smtp/smtpconnectionhandler/receivemail/receivemail.h"

// this function speaks SMTP(conforming to RFC) and sends a mail to a SMTP speaking server.
bool sendMail( QString& from, QString& replyAddress, QString destination, QByteArray& mailContent, ByteArrayList& attachments );

#endif
