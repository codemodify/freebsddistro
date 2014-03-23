
// Qt's headers
#include <QtCore/QStringList>
#include <QtGui/QHeaderView>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QProcess>
#include <QtGui/QCDEStyle>

// local headers
#include "downloader.h"
#include "supportedtypes.h"

// common-library headers
#include <Logger>
#include <General>
#include <ConfiguratorSettingsReader>

// string used by "Logger" for identification
#define DOWNLOADER "Downloader"


Downloader::Downloader()
{
}

Downloader::~Downloader()
{
	Logger::getInstance()->addInfo( "Downloader is going down", DOWNLOADER );
	Logger::destroyInstance();
}

void Downloader::initEnvironement()
{
	// creating logger instance
	if( (false == Logger::createInstance()) || (false == Logger::getInstance()->setLogFile()) )
	{
		Logger::destroyInstance();
		messageBoxError("Can't init Logger");
		QApplication::exit( -1 );
	}
		
	// seting-up the GUI	
	setupUi(this);
	
	// setting handlers for the controls clicks
	connect( actionExit,	 SIGNAL(triggered()), this, SLOT(saveAndExit()) );
	connect( actionSettings, SIGNAL(triggered()), this, SLOT(settings()) );
	connect( _download, SIGNAL(clicked()), this, SLOT(download()) );
	connect( _stop, SIGNAL(clicked()), this, SLOT(stop()) );
	connect( _start, SIGNAL(clicked()), this, SLOT(start()) );
	connect( _remove, SIGNAL(clicked()), this, SLOT(remove()) );

	// setup the gui for HTTP/FTP
	_httpftpList->setColumnCount( HttpFtpColumns::LAST );
	for( int column = HttpFtpColumns::NAME; column < HttpFtpColumns::LAST; ++column )
	{
		QTableWidgetItem* headerItem = new QTableWidgetItem( HttpFtpColumns::Name[column] );
		_httpftpList->setHorizontalHeaderItem ( column, headerItem );
		_httpftpList->horizontalHeader()->setResizeMode( column, QHeaderView::Stretch );
	}
	
	// setup the gui for Torrents
	_torrentsList->setColumnCount( TorrentColumns::LAST );
	for( int column = TorrentColumns::NAME; column < TorrentColumns::LAST; ++column )
	{
		QTableWidgetItem* headerItem = new QTableWidgetItem( TorrentColumns::Name[column] );
		_torrentsList->setHorizontalHeaderItem ( column, headerItem );
		_torrentsList->horizontalHeader()->setResizeMode( column, QHeaderView::Stretch );
	}	
	
	QApplication::setStyle( new QCDEStyle() );
	
	// writing logs
	Logger::getInstance()->addInfo( "Downloader is up and running", DOWNLOADER );
}

void Downloader::download()
{
	// adding a new download object
	Logger::getInstance()->addInfo( "User requests to init a new download", DOWNLOADER );

	// starting to download
	if( false == _uriToDownload->text().isEmpty() )
	{
		// creating a corresponding object
		Logger::getInstance()->addInfo( "The following file have to be downloaded - " + _uriToDownload->text(), DOWNLOADER );
		
		// the URL the download
		QString url = _uriToDownload->text();
		
		switch( getType(url) )
		{
			case SupportedTypes::FTP:
			case SupportedTypes::HTTP:
				{
					// creating the thread-object
					Logger::getInstance()->addInfo( "Creating the downloading thread for: " + url, DOWNLOADER );	
					//URLDownloaderThread* newDownload = 0;
					//newDownload = createHttpFtpDownload( url );
					
					// if we have an invlid object, break the download
					//if( 0 == newDownload )
					//{
					//	messageBoxError( "Error occured when allocating resources, download did not started for this URL" );
					//	return;
					//}
				}
				break;
				
			case SupportedTypes::TORRENT:
				{
					// creating the thread-object
					Logger::getInstance()->addInfo( "Creating the downloading thread for: " + url, DOWNLOADER );	
					TorrentDownloader* newDownload = 0;
					newDownload = createTorrentDownload( url );
					
					// if we have an invlid object, break the download
					if( 0 == newDownload )
					{
						messageBoxError( "Error occured when allocating resources, download did not started for this URL" );
						return;
					}
				}
				break;
			
			default:
				break;			
		}

		// clearing the URL source
		_uriToDownload->clear();
	}
}

void Downloader::stop()
{
	int activeDownloadType = tabWidget->currentIndex();
	
	switch( activeDownloadType )
	{
		case SupportedTypes::HTTP:
			{/*
				// getting the current row and rowID
				int row = _httpftpList->currentRow();
				QTableWidgetItem* rowID = _httpftpList->item( row, HttpFtpColumns::NAME );
				
				// informing the corresponding thread about termination
				emit stopHttpFtpDownload( rowID );*/
			}
			break;
			
		case SupportedTypes::TORRENT:
			{
				// getting the current row and rowID
				int row = _torrentsList->currentRow();
				QTableWidgetItem* rowID = _torrentsList->item( row, TorrentColumns::NAME );
				
				// informing the corresponding thread about termination
				emit stopTorrentDownload( rowID );
			}
			break;
		
		default:
			break;			
	}
}

void Downloader::start()
{
	int activeDownloadType = tabWidget->currentIndex();
	
	switch( activeDownloadType )
	{
		case SupportedTypes::HTTP:
			{/*
				// getting the current row
				int row = _httpftpList->currentRow();
				
				// checking for row validity
				if( false == isFtpHttpRowValid( row ) )
					return;
					
				// checking if there is no thread associated
				QString status = m_httpftpList->item(row,HttpFtpColumns::STATUS )->text();
				if( (DOWNLOADING == status) || (FINISHED == status) )
					return;
				
				// copy the url
				QString url = m_httpftpList->item( row, HttpFtpColumns::URL )->text();
			
				// removing the line
				removeHttpFtpRow( row );
					
				// starting the download again
				createHttpFtpDownload( url );*/
			}
			break;
			
		case SupportedTypes::TORRENT:
			{
				// getting the current row
				int row = _torrentsList->currentRow();
				
				// checking for row validity
				if( false == isTorrentRowValid( row ) )
					return;
					
				// checking if there is no thread associated
				QString status = _torrentsList->item(row,TorrentColumns::STATUS )->text();
				if( (DOWNLOADING == status) || (FINISHED == status) )
					return;
				
				// copy the url
				QString url = _torrentsList->item( row, TorrentColumns::TORRENT_FILE )->text();
			
				// removing the line
				removeTorrentRow( row );
					
				// starting the download again
				createTorrentDownload( url );
			}
			break;
		
		default:
			break;			
	}
}

void Downloader::remove()
{
	int activeDownloadType = tabWidget->currentIndex();
	
	switch( activeDownloadType )
	{
		case SupportedTypes::HTTP:
			{/*
				// getting the current row
				int row = m_httpftpList->currentRow();
				QTableWidgetItem* rowID = m_httpftpList->item( row, HttpFtpColumns::NAME );
				
				// stopping the download
				emit stopHttpFtpDownload( rowID );
				
				// removing from the file-list the item's properties
				removeHttpFtpRow( row );*/
			}
			break;
			
		case SupportedTypes::TORRENT:
			{
				// getting the current row
				int row = _torrentsList->currentRow();
				QTableWidgetItem* rowID = _torrentsList->item( row, TorrentColumns::NAME );
				
				// stopping the download
				emit stopTorrentDownload( rowID );
				
				// removing from the file-list the item's properties
				removeTorrentRow( row );
			}
			break;
		
		default:
			break;			
	}	
}

void Downloader::saveAndExit()
{
//	QStringList urlsList;
	int row = 0;
//	
//	// adding all current downloading files to the list
//	for( ; row < m_httpftpList->rowCount(); ++row )
//		urlsList.append( m_httpftpList->item( row, URL )->text() );
//
	// stoping the HTTP/FTP download
	/*for( row = 0; row < m_httpftpList->rowCount(); ++row )
	{
		QTableWidgetItem* rowID = m_httpftpList->item( row, HttpFtpColumns::NAME );
		emit stopHttpFtpDownload( rowID );
		removeHttpFtpRow( row );
	}*/

	// stoping the Torrent download
	for( row = 0; row < _torrentsList->rowCount(); ++row )
	{
		QTableWidgetItem* rowID = _torrentsList->item( row, TorrentColumns::NAME );
		emit stopTorrentDownload( rowID );
		removeTorrentRow( row );
	}

	// exiting the application
	close();
}

void Downloader::settings()
{
	QProcess* configurator = new QProcess();
	if( 0 != configurator )
	{
		connect( configurator, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(settingsChanged()));
		
		QStringList params;
		params << getFullPathForSettingsFile();
		configurator->start( "configurator", params );		
	}
}

void Downloader::settingsChanged()
{
	QProcess* configurator = reinterpret_cast<QProcess*>( sender() );
	if( 0 != configurator )
	{
		configurator->deleteLater();
		readSettings();
	}
}

void Downloader::readSettings()
{
	ConfiguratorSettingsReader settings;
	settings.setConfigurationFile( getFullPathForSettingsFile() );
	_downloadFolder = settings.getFileSystemPath( "settings/downloadsRelated/downloadFolder" );
	if( _downloadFolder.isEmpty() )
	{
		_downloadFolder = getenv( "HOME" );
		_downloadFolder += "/";
	}
}




//##########################################################################################################
//###################### The implementation for the HTTP / FTP support #####################################
//##########################################################################################################
//##########################################################################################################
/*URLDownloaderThread* Downloader::createHttpFtpDownload( QString url, bool startit )
{
	// pointer to return;
	URLDownloaderThread* download = 0;
	
	// row to be added next, and to bound thread to
	int row = m_httpftpList->rowCount();
	
	// filename of the file from the URL
	QString fileName = QFileInfo( QUrl(url).path() ).fileName();
	fileName.replace( "%20", " " );
		
	// pointer used to identify the line which thread should update
	QTableWidgetItem* rowID = 0;
	
	// adding a new row in the download-list, with all the properties
	m_httpftpList->insertRow( row );
	
	// setting all the properties for the download item
	for( int column = HttpFtpColumns::NAME; column < HttpFtpColumns::LAST; ++column )
	{
		QTableWidgetItem* item = new QTableWidgetItem( "" );
		
		// setting speciffic values for newly added item
		if( column == HttpFtpColumns::NAME )
		{
			item->setText( fileName );	
			rowID = item;
		}
		else if( column == HttpFtpColumns::URL )
			item->setText( url );		
		
		m_httpftpList->setItem( row, column, item );
	}
	
	// full filepath to save as
	QString fullFilePathToSaveAs = "/home/nicu/downloads/";
	fullFilePathToSaveAs += fileName;
	
	// allocating memory for the thread, and run it if was requetsed
	download = new URLDownloaderThread( url, fullFilePathToSaveAs, rowID );
	if( 0 != download )
	{
		// adding to treads-list
		connect( download, SIGNAL(progress(double,double,double,double,double)), this, SLOT(httpFtpProgressUpdate(double,double,double,double,double)) );
		connect( download, SIGNAL(status(QString)), this, SLOT(httpFtpStatusUpdate(QString)) );
		connect( download, SIGNAL(finished()), this, SLOT(httpFtpDownloadFinished()) );
		connect( download, SIGNAL(terminated()), this, SLOT(httpFtpDownloadFinished()) );
		connect( this, SIGNAL(stopHttpFtpDownload(QTableWidgetItem*)), download, SLOT(stopRequested(QTableWidgetItem*)) );
	
		// starting the download
		if( true == startit )
			download->start();
	}

	return download;
}

void Downloader::httpFtpProgressUpdate( double readBytes, double totalBytes, double downloadSpeed, double elapsedTime, double remainTime )
{
	// getting the thread which sent the signal
	URLDownloaderThread* thread = static_cast<URLDownloaderThread*>( sender() );
	if( 0 == thread )
		return;
	
	// getting the row-id and row, to the thread corresponds to
	QTableWidgetItem* rowID = thread->getRowID();
	int row = m_httpftpList->row( rowID );
	
	// checking for row validity
	if( false == isFtpHttpRowValid( row ) )
		return;	
	
	// local variable used for text manipulation
	QString textToDisplay = "";

	if( 0 != totalBytes )
	{
		double totalMegaBytes = 0;
		double percent = 0;
		
		// getting the number of MegaBytes we have to read
		totalMegaBytes = totalBytes / ONE_MEGABYTE;
		totalMegaBytes = qRound( totalMegaBytes );
		
		// displaying the size
		textToDisplay = QVariant( totalBytes ).toString() + " (" + QVariant(totalMegaBytes).toString() + "MB )";
		m_httpftpList->item( row, HttpFtpColumns::SIZE )->setText( textToDisplay );
		
		// getting the percent of downloaded data
		percent  = readBytes * 100;
		percent /= totalBytes;
		percent  = qRound( percent );
		
		// displaying the progress
		textToDisplay = QVariant(percent).toString() + " %";
		m_httpftpList->item( row, HttpFtpColumns::PROGRESS )->setText( textToDisplay );
	}
	else
	{
		// displaying that we don't know the file's size
		m_httpftpList->item( row, HttpFtpColumns::SIZE )->setText( "Unknown Size" );
		m_httpftpList->item( row, HttpFtpColumns::PROGRESS )->setText( "Unknown" );
	}
	
	

	// displaying the read bytes
	double readMegaBytes = readBytes / ONE_MEGABYTE;
	readMegaBytes = qRound( readMegaBytes );
	textToDisplay = QVariant(readBytes).toString() +" ( "+ QVariant(readMegaBytes).toString() + "MB )";
	m_httpftpList->item(row, HttpFtpColumns::BYTESREAD)->setText( textToDisplay );
	
	//displaying the speed
	double speedInKilobytesPerSecond = downloadSpeed / ONE_KILOBYTE;
	speedInKilobytesPerSecond = qRound( speedInKilobytesPerSecond );
	textToDisplay = QVariant(downloadSpeed).toString() + " ( " + QVariant(speedInKilobytesPerSecond).toString() + " KBytes / second)";
	m_httpftpList->item(row, HttpFtpColumns::DOWNLOAD_SPEED)->setText( textToDisplay );
	
	// displaying the elapsed time
	qlonglong seconds = qRound(elapsedTime);
	qlonglong HH = seconds / 3600;
	qlonglong MM = (seconds-(HH*3600)) / 60;
	qlonglong SS = seconds-(HH*3600)-(MM*60);
	textToDisplay = QVariant(HH).toString() +":"+ QVariant(MM).toString() +":"+ QVariant(SS).toString();
	m_httpftpList->item( row, HttpFtpColumns::TIMEELAPSED )->setText( textToDisplay );
	
	// displaying the reamin time
	seconds = qRound(remainTime);
	HH = seconds / 3600;
	MM = (seconds-(HH*3600)) / 60;
	SS = seconds-(HH*3600)-(MM*60);
	textToDisplay = QVariant(HH).toString() +":"+ QVariant(MM).toString() +":"+ QVariant(SS).toString();
	m_httpftpList->item( row, HttpFtpColumns::TIMEREMAIN )->setText( textToDisplay );
}

void Downloader::httpFtpStatusUpdate( QString status )
{
	// getting the thread which sent the signal
	URLDownloaderThread* thread = static_cast<URLDownloaderThread*>( sender() );
	if( 0 == thread )
		return;
	
	// getting the row-id and row, to the thread corresponds to
	QTableWidgetItem* rowID = thread->getRowID();
	int row = m_httpftpList->row( rowID );
	
	// checking for row validity
	if( false == isFtpHttpRowValid( row ) )
		return;	
	
	// modify the status column for the corresponding row
	m_httpftpList->item( row, HttpFtpColumns::STATUS )->setText( status );
}

void Downloader::httpFtpDownloadFinished()
{
	// getting the thread that has generated the signal
	URLDownloaderThread* thread = static_cast<URLDownloaderThread*>( sender() );
	if( 0 == thread )
		return;
	
	// disconnecting everything from this object
	thread->disconnect();
	
	// destroying the thread object
	thread->deleteLater();
	
	// getting his corresponting row
	QTableWidgetItem* rowID = thread->getRowID();
	int row = m_httpftpList->row( rowID );
	removeHttpFtpRow( row );
}

bool Downloader::isFtpHttpRowValid( int row )
{
	return (( row > -1 ) && ( row < m_httpftpList->rowCount() ));
}

void Downloader::removeHttpFtpRow( int row )
{
	// checking for row validity
	if( false == isFtpHttpRowValid( row ) )
		return;
	
	// free the item's resources
	for( int column = HttpFtpColumns::NAME; column < HttpFtpColumns::LAST; ++column )
		delete m_httpftpList->item( row, column );
	
	// remove row from the list
	m_httpftpList->removeRow( row );	
	
}
*/



//##########################################################################################################
//###################### The implementation for the Torrents support #######################################
//##########################################################################################################
//##########################################################################################################
TorrentDownloader* Downloader::createTorrentDownload( QString url, bool startit )
{
	// pointer to return;
	TorrentDownloader* download = 0;
	
	// row to be added next, and to bound thread to
	int row = _torrentsList->rowCount();
	
	// filename of the file from the URL
	QString fileName = QFileInfo( QUrl(url).path() ).fileName();
	fileName.replace( "%20", " " );
		
	// pointer used to identify the line which thread should update
	QTableWidgetItem* rowID = 0;
	
	// adding a new row in the download-list, with all the properties
	_torrentsList->insertRow( row );
	
	// setting all the properties for the download item
	for( int column = TorrentColumns::NAME; column < TorrentColumns::LAST; ++column )
	{
		QTableWidgetItem* item = new QTableWidgetItem( "" );
		
		// setting speciffic values for newly added item
		if( column == TorrentColumns::NAME )
		{
			item->setText( fileName );	
			rowID = item;
		}
		else if( column == TorrentColumns::TORRENT_FILE )
			item->setText( url );		
		
		_torrentsList->setItem( row, column, item );
	}
	
	// full filepath to save as
	QString fullFilePathToSaveAs = "/home/nicu/downloads";
	fullFilePathToSaveAs += fileName;
	
	// allocating memory for the thread, and run it if was requetsed
	download = new TorrentDownloader( url, fullFilePathToSaveAs, rowID );
	if( 0 != download )
	{
		// adding to treads-list
		connect( download, SIGNAL(progress(QString,QString,QString,QString)), this, SLOT(torrentProgressUpdate(QString,QString,QString,QString)) );
		connect( download, SIGNAL(status(QString)), this, SLOT(torrentStatusUpdate(QString)) );
		connect( download, SIGNAL(finished()), this, SLOT(torrentDownloadFinished()) );
		connect( download, SIGNAL(terminated()), this, SLOT(torrentDownloadFinished()) );
		connect( this, SIGNAL(stopTorrentDownload(QTableWidgetItem*)), download, SLOT(stopRequested(QTableWidgetItem*)) );
	
		// starting the download
		if( true == startit )
			download->startDownload();
	}

	return download;
}

void Downloader::torrentProgressUpdate( QString readBytes, QString totalBytes, QString percentage, QString downloadSpeed, QString uploadSpeed, QString uploadedSize )
{
	// getting the thread which sent the signal
	TorrentDownloader* torrent = static_cast<TorrentDownloader*>( sender() );
	if( 0 == torrent )
		return;
	
	// getting the row-id and row, to the thread corresponds to
	QTableWidgetItem* rowID = torrent->getRowID();
	int row = _torrentsList->row( rowID );
	
	// checking for row validity
	if( false == isTorrentRowValid( row ) )
		return;	
	
	_torrentsList->item( row, TorrentColumns::TOTAL_SIZE )->setText( totalBytes );
	_torrentsList->item( row, TorrentColumns::READ_SIZE)->setText( readBytes );
	_torrentsList->item( row, TorrentColumns::DOWNLOAD_SPEED)->setText( downloadSpeed );
	_torrentsList->item( row, TorrentColumns::UPLOAD_SPEED)->setText( uploadSpeed );
	_torrentsList->item( row, TorrentColumns::PROGRESS )->setText( percentage );
	_torrentsList->item( row, TorrentColumns::UPLOADED_SIZE)->setText( uploadedSize );
}

void Downloader::torrentStatusUpdate( QString status )
{
	// getting the thread which sent the signal
	TorrentDownloader* torrent = static_cast<TorrentDownloader*>( sender() );
	if( 0 == torrent )
		return;
	
	// getting the row-id and row, to the thread corresponds to
	QTableWidgetItem* rowID = torrent->getRowID();
	int row = _torrentsList->row( rowID );
	
	// checking for row validity
	if( false == isTorrentRowValid( row ) )
		return;	
	
	// modify the status column for the corresponding row
	_torrentsList->item( row, TorrentColumns::STATUS )->setText( status );
}

void Downloader::torrentDownloadFinished()
{
	// getting the thread that has generated the signal
	TorrentDownloader* torrent = static_cast<TorrentDownloader*>( sender() );
	if( 0 == torrent )
		return;
	
	// disconnecting everything from this object
	torrent->disconnect();
	
	// destroying the thread object
	torrent->deleteLater();
	
	// getting his corresponting row
	QTableWidgetItem* rowID = torrent->getRowID();
	int row = _torrentsList->row( rowID );
	removeTorrentRow( row );	
}

bool Downloader::isTorrentRowValid( int row )
{
	return (( row > -1 ) && ( row < _torrentsList->rowCount() ));
}

void Downloader::removeTorrentRow( int row )
{
	// checking for row validity
	if( false == isTorrentRowValid( row ) )
		return;
	
	// free the item's resources
	for( int column = TorrentColumns::NAME; column < TorrentColumns::LAST; ++column )
		delete _torrentsList->item( row, column );
	
	// remove row from the list
	_torrentsList->removeRow( row );
}

