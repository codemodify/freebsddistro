#ifndef _MAIL_SERVER_CONFIG_VARIABLES_
#define _MAIL_SERVER_CONFIG_VARIABLES_

#include <QtCore/QString>
#include <QtCore/QMap>

#include "maildatabase/maildatabase.h"


extern quint16 smtpPort;
extern quint16 popPort;

extern QString network;

extern QString mailStorage;

typedef QMap<QString,QString> Accounts;
typedef QMap<QString,Accounts*> MailServers;
extern MailServers mailServers;

extern MailDatabase* mailDatabase;

#endif
