#ifndef _MOD_BASE_H_
#define _MOD_BASE_H_

// Qt's headers
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtGui/QTableWidget>

// CURL's headers
#include <curl/curl.h>



/*
 *	This is the base class for all the implemented protocols.
 * 	It is designed to provide all required interfaces to easy interact with the main "Downloader" module
 */
class URLDownloaderThread : public QThread
{
	// using QT's signal/slots framework
	Q_OBJECT
			
public:

	// constructor:
	// url - the URL to download, 
	// saveTo - the full path of the file to save as
	// row - the row from the download-list, which have to be updated with information like: percent, status, etc ...
	URLDownloaderThread( QString url, QString saveTo, QTableWidgetItem* row );
	
	// destructor
	~URLDownloaderThread();
	
	// reimplementation of QThread::run()
	void run();
	
	// gets the error termination status
	bool isSuccessfulTerminated(){ return m_success; }
	
	// gets the row-id
	QTableWidgetItem* getRowID(){ return m_row; }
	
public slots:
	void stopRequested( QTableWidgetItem* rowID );

signals:

	// reflects the current progress of downloading
	void progress( double readBytes, double totalBytes, double speed, double elapsedTime, double remainTime );
	
	// reflect the current status of downloading
	void status( QString status );
	
private:

	// stores the tread termination error status
	bool m_success;

	// row ID, used to store the row this object corresponds to
	QTableWidgetItem* m_row;
	
	// handle for our transfer operation
	CURL* m_curlHandle;
	
	// stores the URL that should be downloaded
	char* m_url;
	
	// stores the path where to save the downloading file
	QString m_saveTo;
	
	// the file object to work with
	FILE* m_file;
	
	// cleanup the all used resources
	void cleanUp();
	
	// informs the thread it should stop, as a request of user
	bool m_userStopRequest;

	// mutex to manage the access to the "m_userStopRequest"
	QMutex m_mutexUserStopRequest;

	// gets the user stop request
	bool getUserStopReguest();

};

#endif
