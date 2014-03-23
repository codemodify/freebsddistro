#ifndef TORRENTDOWNLOADER_H_
#define TORRENTDOWNLOADER_H_


#include <QtCore/QProcess>


class QTableWidgetItem;
class QString;

class TorrentDownloader : public QProcess
{
	
	// using QT's signal/slots framework
	Q_OBJECT
	


public:
	TorrentDownloader( QString torrentFile, QString saveTo, QTableWidgetItem* rowID );
	~TorrentDownloader();
	
	// gets the row-id
	QTableWidgetItem* getRowID(){ return _rowID; }
	
	// starts the download
	void startDownload();



public slots:
	void stopRequested( QTableWidgetItem* rowID );



signals:

	// reflects the current progress of downloading
	void progress( QString readBytes, QString totalBytes, QString percentage, QString downloadSpeed, QString uploadSpeed, QString uploadedSize );
	
	// reflect the current status of downloading
	void status( QString status );

	void terminated();
	void finished();



private slots:
	void wrappedApplicationhasOutput();



private:
	// row ID, used to store the row this object corresponds to
	QTableWidgetItem* _rowID;
	
	// stores the URL that should be downloaded
	QString _torrentFile;
	
	// folder to save to
	QString _saveTo;

	
private:
	QString _totalSize;
	
	bool _finished;
};

#endif
