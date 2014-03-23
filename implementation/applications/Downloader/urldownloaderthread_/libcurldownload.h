#ifndef _LIBCURLDOWNLOAD_H_
#define _LIBCURLDOWNLOAD_H_

// Qt's headers
#include <QtCore/QThread>
#include <QtCore/QMutex>

// CURL's headers
#include <curl/curl.h>

class LibCurlDownload : public QThread
{
public:
	LibCurlDownload( CURL* curlHandle );
	~LibCurlDownload();
	void run();
	CURLcode getResult(){ return m_result; }
private:
	CURLcode m_result;
	CURL* m_curlHandle;
};

#endif //_LIBCURLDOWNLOAD_H_
