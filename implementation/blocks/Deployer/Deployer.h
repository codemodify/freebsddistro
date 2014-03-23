#ifndef __Deployer_H__
#define __Deployer_H__

// Qt headers
#include <QtXml/QDomDocument>

// forwards
class QString;


class Deployer
{
	public:
		Deployer( QString configurationFileName="" );

	public:
		void setSourceAndTarget( QString source, QString target );
		void deploy( QString scenarioName );

	private:
		QDomDocument _domDocument;

		QString _source;
		QString _target;
};

#endif
