
// qt headers
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>

// local headers
#include "domainserver.h"
#include "components/logger/logger.h"

#define LOGGER_ID "DomainServer"

DomainServer::DomainServer( DomainConfig* domainConfig )
{
	_domainConfig = domainConfig;

	_logger = new Logger();
	_logger->setLogFile( _domainConfig->_logFile );
}

DomainServer::~DomainServer()
{
	delete _logger;
}

bool DomainServer::hasToBeBlocked( QString& host )
{
	return _domainConfig->_blockFrom.contains( host );
}

QByteArray getFileContent( QString fileName );
QByteArray DomainServer::getContentForRequest( QString& resourceWanted )
{
	QString fullPathToResource = QString("%1/%2").arg(_domainConfig->_content).arg(resourceWanted);
	_logger->addInfo( QString("resource: %1, fullPath: %2").arg(resourceWanted).arg(fullPathToResource), LOGGER_ID );
	
	// check if this is a file
	if( QFileInfo(fullPathToResource).isFile() ) 
		return getFileContent( fullPathToResource );

	// if not then look for a file	
	_logger->addInfo( "failed to find requesed resource, looking for some default files", LOGGER_ID );
		
	foreach( QString defaultFile, _domainConfig->_defaultFile )
	{
		fullPathToResource = QString("%1/%2").arg(_domainConfig->_content).arg(defaultFile);
		_logger->addInfo( QString("fullPath: %1").arg(fullPathToResource), LOGGER_ID );
		
		if( !QFileInfo(fullPathToResource).isFile() )
		{
			_logger->addInfo( "file not found", LOGGER_ID );				
			continue;
		}

		_logger->addInfo( "file found", LOGGER_ID );
		return getFileContent( fullPathToResource );
	}
	
	return QByteArray();
}

QByteArray DomainServer::getFileContent( QString fileName )
{
	_logger->addInfo( QString("getting content for: %1").arg(fileName), LOGGER_ID );

	// run the CGI	
	if( _domainConfig->_actions.contains(QFileInfo(fileName).suffix()) )
	{
		QString executable = _domainConfig->_actions[QFileInfo(fileName).suffix()].first;
		QString flags = _domainConfig->_actions[QFileInfo(fileName).suffix()].second;

		QString toRun = QString("%1 %2 %3").arg(executable).arg(flags).arg(fileName);
		_logger->addInfo( QString("exec: %1").arg(toRun), LOGGER_ID );
		
		QProcess process;
		process.start( toRun );
		process.waitForFinished();
		
		QByteArray byteArray = process.readAllStandardOutput();
		
		return byteArray;
	}

	// just return the content of the file
	QFile file( fileName );
	if( !file.open(QFile::ReadOnly) )
		return QByteArray();

	QByteArray byteArray = file.readAll();
	file.close();
	
	byteArray = "HTTP/1.0 200 Ok\r\n\r\n" + byteArray;
	return byteArray;
}
