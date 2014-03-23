#ifndef DOWNLOADER_H
#define DOWNLOADER_H

// Qt's headers
#include <QtGui/QMainWindow>


// downloader's headers
#include "ui_downloader.h"
#include "torrentdownloader/torrentdownloader.h"


/*
 *	The starting point of everything is implemented in "Downloader"
 */
class Downloader : public QMainWindow, public Ui::Downloader
{
	
	// using QT's signal/slot architecture
	Q_OBJECT



public:

	// the default constructor
	Downloader();

	// destructor
	~Downloader();

	// this method sets-up the needed GUI/nonGUI settings for a correct start
	void initEnvironement();




public slots:

	// handles the click on "Add" button
	void download();

	// handles the click on "Start" button
	void start();

	// handles the click on "Stop" button
	void stop();

	// handles the click() on "Delete" button
	void remove();

	// HTTP / FTP handling
	//void httpFtpProgressUpdate( double readBytes, double totalBytes, double downloadSpeed, double elapsedTime, double remainTime );
	//void httpFtpStatusUpdate( QString status );
	//void httpFtpDownloadFinished();
	
	// Torrents handling
	void torrentProgressUpdate( QString readBytes, QString totalBytes, QString percentage, QString downloadSpeed, QString uploadSpeed, QString uploadedSize );
	void torrentStatusUpdate( QString status );
	void torrentDownloadFinished();



	
	void saveAndExit();
	void settings();



private slots:
	
	void settingsChanged();



signals:
	//void stopHttpFtpDownload( QTableWidgetItem* rowID );
	void stopTorrentDownload( QTableWidgetItem* rowID );



private:		// stuff for settings

	void readSettings();
	QString _downloadFolder;


private:		// stuff for HTTP / FTP

	// allocates memory and returns the pointer to newly created thread object
	//URLDownloaderThread* createHttpFtpDownload( QString url, bool startit=true );

	// checks if a given number of row is valid
	//bool isFtpHttpRowValid( int row );
	
	// removes a given row, and free all resources used by that row
	//void removeHttpFtpRow( int row );
	
	
private:		// stuff fo the torrents
	
	TorrentDownloader* createTorrentDownload( QString url, bool startit=true );
	bool isTorrentRowValid( int row );
	void removeTorrentRow( int row );
	
};

#endif
