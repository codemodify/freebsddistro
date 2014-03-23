#ifndef configuration_h
#define configuration_h

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QPair>
#include <QtCore/QMap>

typedef struct
{
	QString _version;
	QString _cipher;
    QString _certificate;
	QString _publicKey;
	QString _privateKey;
	QString _format;
} SshServerSettings;


typedef QStringList AcceptedShellCommands;
typedef struct
{
	QString _name;
	QString _home;
	bool _lockInHome;
	QString _publicKey;
	QString _publicKeyFormat;
	
	AcceptedShellCommands _acceptedShellCommands;
} UserSettings;

typedef QList<UserSettings*> UserList;

#endif
