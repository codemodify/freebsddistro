#ifndef __Logger_H__
#define __Logger_H__

// Qt headers
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QVariant>
#include <QtCore/QMutex>

/*******************************************************************************
*	Singleton Pattern Design.
* 	Thread safe.
* 
*    Usage example:
* 
*		Logger::createInstance();
*		Logger::getInstance() -> setLogFile( "logfile.log" );
*		Logger::getInstance() -> addInfo( "Application started" );
*		Logger::destroyInstance();
*
********************************************************************************/
class Logger
{
public:
	
	// the destructor
	~Logger();

	// creates instance of the  logger
	// true - if success
	// false - if fail
	static bool createInstance();
	
	// gets the instance of the logger object, creates one if there is no any
	// pointer - on success
	static Logger* getInstance();	

	// sets the log file, if none given then log will be stored under application path
	// true - on success
	// false - on error
	bool setLogFile( QString fileName = "");
	
	// adds a new "Info/Error/Warning/Debug/Raw information" to the log file
	// message - the string that should appear in the log file
	// sender - the identification string, to show in log file who was writting the log-line, for example it could be a thread or main app
	void addInfo( QString message, QString sender="" );
	void addError( QString message, QString sender="" );
	void addWarning( QString message, QString sender="" );
	void addDebug( QVariant debug, QString sender="" );
	void addRaw( QString message );
	
	// destroyiong the logger's instance
	static void destroyInstance();	
	
	// used to implement the singleton pattern design
	// this is not intented to be used directly
	static Logger* s_logger;
	
private:

	// disabling the direct creation of the Logger object
	Logger();
	
	// used to work with log-file on the storage
	QFile _logFile;

	// adds a log-string to the log file
	void addMessage(QString message, QString sender);

	// used for thread-safety
	QMutex _mutex;
};

#endif
