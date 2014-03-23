
// Qt headers
#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QDebug>

// local headers
#include "Components/Components.h"
#include "Logger/Logger.h"

static QHash< QObject*, Logger* > s_loggers;

void IspComponents::Logging::registerForLogging( QObject* pointerToCaller, QString logFilePath )
{
	if( 0 == pointerToCaller )
		return;

	Logger*	logger = new Logger( logFilePath );
	
	s_loggers.insert( pointerToCaller, logger );
}

void IspComponents::Logging::logInfo( QObject* pointerToCaller, QString message )
{
	if(	s_loggers.contains(pointerToCaller) )
		s_loggers[ pointerToCaller ]->addInfo( message );

	else
		qDebug() << message;
}

void IspComponents::Logging::logWarning( QObject* pointerToCaller, QString message )
{
	if(	s_loggers.contains(pointerToCaller) )
		s_loggers[ pointerToCaller ]->addWarning( message );

	else
		qDebug() << message;
}

void IspComponents::Logging::logError( QObject* pointerToCaller, QString message )
{
	if(	s_loggers.contains(pointerToCaller) )
		s_loggers[ pointerToCaller ]->addError( message );

	else
		qDebug() << message;
}

void IspComponents::Logging::logFatal( QObject* pointerToCaller, QString message )
{
	if(	s_loggers.contains(pointerToCaller) )
		s_loggers[ pointerToCaller ]->addFatal( message );

	else
		qDebug() << message;
}
