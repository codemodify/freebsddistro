#ifndef DOWNLOADENGINE_H_
#define DOWNLOADENGINE_H_

class DownloadEngine
{
	// using QT's signal/slots framework
	Q_OBJECT
	
public:
	DownloadEngine();
	virtual ~DownloadEngine();
	
	
	
signals:
	void downloadProgress( QString downloadedSize, QString totalSize,  );
	
	void downloadFinished();
	void downloadStopped( QString reason )
};

#endif

