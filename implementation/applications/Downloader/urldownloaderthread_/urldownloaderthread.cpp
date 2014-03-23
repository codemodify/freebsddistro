
// Qt's headers
#include <QtCore/QFileInfo>

// common-library headers
#include <Logger>

// downloader's headers
#include "urldownloaderthread.h"
#include "libcurldownload.h"
#include "supportedtypes.h"

// string used by "Logger" for identification
#define URLDOWNLOADERTHREAD "URDownloaderThread"
	
URLDownloaderThread::URLDownloaderThread( QString url, QString saveTo, QTableWidgetItem* row ):
	m_saveTo( saveTo ),
	m_row( row ),
	m_curlHandle( 0 ),
	m_userStopRequest( false )
{
	Logger::getInstance()->addInfo( "Thread is up", URLDOWNLOADERTHREAD );
	
	// setting the url to download
	m_url = new char[ url.length() ];
	memset( m_url, 0, url.length() );
	strcpy( m_url, url.toAscii().data() );
}

URLDownloaderThread::~URLDownloaderThread()
{
	Logger::getInstance()->addInfo( "Thread is going down", URLDOWNLOADERTHREAD );
}

void URLDownloaderThread::run()
{
	// here goes the CURL init steps
		
	// getting the handle 
	curl_global_init( CURL_GLOBAL_ALL );
	m_curlHandle = curl_easy_init();
	if( 0 == m_curlHandle )
	{
		Logger::getInstance()->addError( "Error when getting the CURL handle", URLDOWNLOADERTHREAD );
		
		// setting the operation's status
		emit status( ERROR_HANDLE );
			
		return;
	}

	// opening the file to save data
	m_file = fopen( m_saveTo.toAscii().data(), "ab" );
	if( NULL == m_file )
	{
		Logger::getInstance()->addError( "Error on opening the save file for writing", URLDOWNLOADERTHREAD );
		emit status( ERROR_FILE );
		return;
	}
	
	// setting the CURL properties
	char errorMessage[ CURL_ERROR_SIZE ] = {0};
	QFileInfo fileInfo( m_saveTo );
	curl_easy_setopt( m_curlHandle, CURLOPT_URL, m_url );
	curl_easy_setopt( m_curlHandle, CURLOPT_RESUME_FROM, fileInfo.size() );
	curl_easy_setopt( m_curlHandle, CURLOPT_NOPROGRESS, 0 );
	curl_easy_setopt( m_curlHandle, CURLOPT_WRITEDATA, m_file );	
	curl_easy_setopt( m_curlHandle, CURLOPT_ERRORBUFFER, &errorMessage );
	
	// start downloading
	Logger::getInstance()->addInfo( QString("Start download for: ") + m_url, URLDOWNLOADERTHREAD );
	LibCurlDownload downloadThread( m_curlHandle );
	downloadThread.start();

	emit status( DOWNLOADING );
	
	// updating the download  status
	while( (true == downloadThread.isRunning()) && (false == getUserStopReguest()) )
	{
		sleep(1);
		
		// getting the total downloaded bytes
		double readBytes = 0;
		curl_easy_getinfo( m_curlHandle, CURLINFO_SIZE_DOWNLOAD, &readBytes );
		
		// getting the total size of the file
		double totalBytes = 0;
		curl_easy_getinfo( m_curlHandle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &totalBytes );
		
		// getting the download speed
		double speed = 0;
		curl_easy_getinfo( m_curlHandle, CURLINFO_SPEED_DOWNLOAD, &speed );

		// getting the time elapsed
		double elapsedTime = 0;
		curl_easy_getinfo( m_curlHandle, CURLINFO_TOTAL_TIME, &elapsedTime );
		
		// getting the time elapsed
		double remainTime = (totalBytes-readBytes) / speed;
		
		emit progress( readBytes, totalBytes, speed, elapsedTime, remainTime );
	}
	
	// checking of we have to terminate the downloading thread
	if( downloadThread.isRunning() && (true == getUserStopReguest()) )
		downloadThread.terminate();
	
	// waiting for thread to terminate
	downloadThread.wait( ULONG_MAX );
	
	if( 0 != downloadThread.getResult() )
	{
		Logger::getInstance()->addError( QString("Error when downloading: ") + ( getUserStopReguest() ? "User abort" : errorMessage ), URLDOWNLOADERTHREAD );
		
		// setting the success status to false
		m_success = false;
		
		// changing the status to error
		emit status( getUserStopReguest() ? USER_ABORT : ERROR );
		
	}
	else
	{
		Logger::getInstance()->addInfo( QString("Download finished for: ") + m_url, URLDOWNLOADERTHREAD );

		// setting the success status to false
		m_success = true;
		
		// changing the status to finished
		emit status( FINISHED );
		
	}
	
	// cleaning up
	cleanUp();
	
	return;
	
}

void URLDownloaderThread::cleanUp()
{
	// free allocated memory
	delete m_url;
	
	// closing the opened file
	fclose( m_file );
	
	// clean-up the CURL
	curl_easy_cleanup( m_curlHandle );
	
	// global CRUL cleanup
	curl_global_cleanup();
}

bool URLDownloaderThread::getUserStopReguest()
{
	// getting the user's stop request
	m_mutexUserStopRequest.lock();
	bool request = m_userStopRequest;
	m_mutexUserStopRequest.unlock();
	
	return request;
}

void URLDownloaderThread::stopRequested( QTableWidgetItem* rowID )
{
	// checking if this message was targeted for us
	if( rowID != getRowID() )
		return;
		
	// stoping the tread's execution
	m_mutexUserStopRequest.lock();
	m_userStopRequest = true;
	m_mutexUserStopRequest.unlock();	
}
