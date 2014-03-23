
// Qt's headers
#include <QtCore/QTime>
#include <QtGui/QApplication>

// common library's headers
#include "Logger"

// ensure the zero pointing for the static variable
Logger* Logger::s_logger = 0;

// used by Logger for identification
#define LOGGER "Logger"

Logger::Logger()
{}

Logger::~Logger()
{
	// closing the log-file
	m_logFile.close();
}

bool Logger::createInstance()
{
	// checking if logger is not valid
	if( 0 == s_logger )
	{
		try
		{
			// getting memory for logger object
			Logger::s_logger = new Logger();
		}
		catch(...)
		{
			// return failure
			return false;
		}
	}
	
	// return success
	return true;
}

Logger* Logger::getInstance()
{
	// create a instance if there is no any
	if( 0 == Logger::s_logger )
	{
		if( false == Logger::createInstance() )
			return 0;

		if( false == Logger::s_logger->setLogFile() )
			return 0;
	}
	
	// return the instance
	return Logger::s_logger;
}

bool Logger::setLogFile(QString fileName)
{
	// checking if have to generate a file name
	if( true == fileName.isEmpty() )
		fileName = qApp->applicationFilePath() + ".log";
	
	// setting the file name
	m_logFile.setFileName( fileName );
	
	// opening the file, and chek if we can write
	if( false == m_logFile.open(QIODevice::Append) )
		return false;
	
	addRaw("\n\n#####################################################################################################");
	addInfo("Logging framework is up and running. Time - " + QTime::currentTime().toString(Qt::LocalDate), LOGGER );
		
	// returning success
	return true;
}

void Logger::addMessage( QString message, QString sender )
{
	// preparing the sender
	if( false == sender.isEmpty() )
		sender += " - ";
	
	m_mutex.lock();
	
	// writting to the file	
	m_logFile.write( QString( sender + message + "...\n" ).toAscii() );
	
	// ensuring the data is written to the file imideately, this is important if the calling app will crash, then the logs will be available for analyzing
	m_logFile.flush();
	
	m_mutex.unlock();
}

void Logger::addInfo( QString message, QString sender )
{
	// preparing the message
	message = "Info: " + message;
	
	// adding the message to the file
	addMessage( message, sender );
}

void Logger::addError( QString message, QString sender )
{
	// preparing the message
	message = "Error: " + message;
	
	// adding the message to the file
	addMessage( message, sender );
}

void Logger::addWarning( QString message, QString sender )
{
	// preparing the message
	message = "Warning: " + message;
	
	// adding the message to the file
	addMessage( message, sender );
}

void Logger::addDebug( QVariant debug, QString sender )
{
	// preparing the message
	QString message = "D_E_B_U_G: got value = " + debug.toString() + ", QT type of the value = " + debug.typeToName( debug.type() );
	
	// adding the message to the log file
	addMessage( message, sender );
}

void Logger::addRaw( QString message )
{
	m_mutex.lock();
	m_logFile.write( QString( message + "\n" ).toAscii() );	
	m_mutex.unlock();
}

void Logger::destroyInstance()
{
	// checking if we have any running instances
	if( 0 == Logger::s_logger )
		return;

	// writing log
	Logger::getInstance()->addInfo("Logging framework is going down. Time - " + QTime::currentTime().toString(Qt::LocalDate), LOGGER );
	
	// destroying the logger
	delete Logger::s_logger;
	
	// setting the pointer to zero
	Logger::s_logger = 0;	
}
