#ifndef _CORE_H_
#define _CORE_H_

// qt headers
#include <QtCore/QObject>
#include <QtCore/QMap>

class QDomDocument;

class CoreInstaller : public QObject
{
	Q_OBJECT
	
public:
	CoreInstaller();
	~CoreInstaller();
	
public:
	bool setInstallFile( QString installFilePath );

signals:
	void satusUpdate( QString message );

	
private: // debug related
	bool _weAreInDebugMode;
	
private: // variables related
	QMap<QString,QString> _variables;
	void readVariables( QDomDocument& installFileAsXml );

protected: // stuff for user interface interaction
	//virtual QString showDialog(  );
	
	
};

#endif
