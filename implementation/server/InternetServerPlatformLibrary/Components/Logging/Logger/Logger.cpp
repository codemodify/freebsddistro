
// Qt headers
#include <QtCore/QDateTime>
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

// local headers
#include "Logger.h"


Logger::Logger( QString logFilePath )
{
	_logFile.setFileName( logFilePath );
	_logFile.open( QFile::Append );

	addInfo( "#####################################################################################################" );
	addInfo( "Logging framework is up and running." );
}

Logger::~Logger()
{
	addInfo( "Logging framework is going down." );

	_logFile.close();
}

void Logger::addMessage( QString& message )
{
	const QString c_time = QDateTime::currentDateTime().toString( "yyyy-MM-dd:hh-mm-ss" );

	_logFile.write
	(
		QString( "[%1] - %2\n" ).arg( c_time ).arg( message ).toAscii()
	);

	_logFile.flush();
}

void Logger::addInfo( QString message )
{
	message = "[info   ] " + message;

	addMessage( message );
}

void Logger::addWarning( QString message )
{
	message = "[warning] " + message;

	addMessage( message );
}

void Logger::addError( QString message )
{
	message = "[error  ] " + message;

	addMessage( message );
}

void Logger::addFatal( QString message )
{
	message = "[fatal  ] " + message;

	addMessage( message );
}
