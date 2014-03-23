#include "libcurldownload.h"

LibCurlDownload::LibCurlDownload( CURL* curlHandle ):
	m_curlHandle( curlHandle )
{
}

LibCurlDownload::~LibCurlDownload()
{
}

void LibCurlDownload::run()
{
	m_result = curl_easy_perform( m_curlHandle );
}
