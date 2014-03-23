#ifndef NIKUSHORSITE_H
#define NIKUSHORSITE_H

#include <QtCore/QObject>
#include "website.h"

class NikushorSite : public QObject, public WebSite
{
	Q_OBJECT
	Q_INTERFACES( WebSite )
	
public:
	NikushorSite();
	~NikushorSite();
	
public: // here we have to implement the virtuals from base class
	bool initialize();
	void shutdown();
	
	QString getName();
	QString getDescription();
	
	QByteArray invoke( QByteArray request );

};

#endif
