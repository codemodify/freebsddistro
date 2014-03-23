#include "torrentdownloader.h"
#include "supportedtypes.h"


TorrentDownloader::TorrentDownloader( QString torrentFile, QString saveTo, QTableWidgetItem* rowID ):
	_saveTo( saveTo ),
	_torrentFile( torrentFile ),
	_rowID( rowID ),
	_finished( false )
{
}

TorrentDownloader::~TorrentDownloader()
{
}

void TorrentDownloader::startDownload()
{
	connect( this, SIGNAL(readyReadStandardOutput()), this, SLOT(wrappedApplicationhasOutput()) );
	setWorkingDirectory( _saveTo );
	start( QString("aria2c -T \"%1\"").arg(_torrentFile) );
}

void TorrentDownloader::stopRequested( QTableWidgetItem* rowID )
{
	// checking if this message was targeted for us
	if( rowID != getRowID() )
		return;
	
	emit terminated();
	terminate();
}

void TorrentDownloader::wrappedApplicationhasOutput()
{
	// the vars we're going to collect data into
	QString readBytes, totalBytes, percentage, downloadSpeed, uploadSpeed, uploadedSize;

	// reading the application's output
	QByteArray output = readAllStandardOutput();

	// 0/252,783,452B 0% - D:0.00 U:0.00(0) 0 peers

	QString downloadCompletedPattern("Download Completed");

	QString downloadedSize("/");
	QString totalSizePattern("B");

	QString percentPattern("%");

	QString downloadSpeedPattern("D:");

	QString uploadSpeedPattern("U:");
	QString uploadedSizePattern("(");
	
	// checking if we have finished or still downloading
	int index = output.indexOf( downloadCompletedPattern );
	if( -1 == index )
		emit status(DOWNLOADING);
	else
		emit status(FINISHED);

	// checking for the downloaded size
	index = output.indexOf( downloadedSize );
	if( -1 != index )
	{
		readBytes = output.mid(0,index);
	}

	// checking for the total size
	index = output.indexOf( totalSizePattern );
	if( -1 != index )
	{
		int localIndex = output.indexOf("/");
		totalBytes = output.mid( localIndex, index - localIndex );
	}
	
	// checking for the progress information
	index = output.indexOf(percentPattern);
	if( -1 != index )
	{
		int localIndex = output.indexOf("B");
		percentage = output.mid( localIndex, index - localIndex );
	}	

	// checking for the download speed
	index = output.indexOf(downloadSpeedPattern);
	if( -1 != index )
	{
		int localIndex = output.indexOf("U:");
		downloadSpeed = output.mid( index, localIndex - index );
	}

	// checking for the upload speed
	index = output.indexOf(uploadSpeedPattern);
	if( -1 != index )
	{
		int localIndex = output.indexOf("(");
		uploadSpeed = output.mid( index, localIndex - index );
	}

	// checking for the uploaded size
	index = output.indexOf(uploadedSizePattern);
	if( -1 != index )
	{
		int localIndex = output.indexOf(")");
		uploadedSize = output.mid( index, localIndex - index );
	}

	emit progress( readBytes, totalBytes, percentage, downloadSpeed, uploadSpeed, uploadedSize );
}
