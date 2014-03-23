#ifndef __Logger_H__
#define __Logger_H__

// Qt headers
#include <QtCore/QString>
#include <QtCore/QFile>


class Logger : public QObject
{
	public:
		Logger( QString logFilePath );
		~Logger();

	public:
		void addInfo		( QString message );
		void addWarning	( QString message );
		void addError		( QString message );
		void addFatal		( QString message );

	public:
		void addMessage	( QString& message );

	private:
		QFile	_logFile;
//		QString	_logOwner;

};

#endif
