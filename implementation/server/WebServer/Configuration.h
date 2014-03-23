#ifndef configuration_h
#define configuration_h

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QPair>
#include <QtCore/QHash>

typedef struct
{
	bool _dumpHttpHeaders;
} WebServerSettings;


typedef QStringList					DefaultFileList;
typedef QStringList					Aliases;
typedef QString						FileSuffix;
typedef QPair < QString   , QString >	Action;
typedef QHash < FileSuffix, Action  >	Actions;

typedef struct
{
	DefaultFileList	_defaultFileList;
	Actions			_actions;
	QString			_content;
} DomainSettings; 

typedef QString Domain; 
typedef QHash < Domain, DomainSettings* > DomainList;

#endif
