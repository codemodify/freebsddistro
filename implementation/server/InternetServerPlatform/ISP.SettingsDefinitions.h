#ifndef __Configuration_H__
#define __Configuration_H__

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QList>

// convenience
typedef QString 							IpAddressAsString;
typedef quint32 							IpAddressAsNumber;
typedef quint16 							Port;
typedef QPair < IpAddressAsString, Port >		Socket1;
typedef QPair < IpAddressAsNumber, Port >		Socket2;



// reflecting settings
typedef QMap< QString, QString > VariableList;

typedef QString LogFile;

typedef struct
{
	QStringList								_blockedIpList;
	QMap <IpAddressAsNumber,IpAddressAsNumber>	_blockedIpIntervals;
	QList<IpAddressAsNumber>					_exceptList;
	QStringList								_blockedIpListFromFile;

} BlockFrom;

typedef struct
{
	struct{ QString _explanatoryPage;                                                                                } _business;
	struct{ QString _folderToStoreInfoFiles; QString _explanatoryPage;                                               } _hacker;
	struct{                                                                                                          } _hole;
	struct{ int _countPerSecond; int _countPerMinute; int _totalCount; QString _scriptForFirewall; QString _runWith; } _brutal;
	enum  { Business, Hacker, Hole, Brutal                                                                           } _active;

} Mood;

typedef struct
{
	LogFile		_logFile;
	BlockFrom		_blockFrom;
	Mood			_mood;

} IspSettings;

typedef struct
{
	QString		_on;
	QString		_daemon;
	LogFile		_logFile;
	BlockFrom		_blockFrom;

} SocketSettings;

typedef QList< SocketSettings* > SocketSettingsList;

class ISocketDaemon;
typedef struct
{
	SocketSettings* _socketSettings;
	ISocketDaemon*  _socketDaemon;

} SocketDaemonSettings;

typedef QHash< Socket2, SocketDaemonSettings > SocketDaemonSettingsList;

#endif
