
// local headers
#include "InternetServerPlatform.h"


void InternetServerPlatform::startMonitorFiles( QStringList& filesToMonitor, BlockFrom& blockFrom )
{
	if( true == filesToMonitor.isEmpty() )
		return;
		
	QFileSystemWatcher* fileSystemWatcher = new QFileSystemWatcher();
	connect( fileSystemWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(slot_fileChanged(const QString&)) );
	
	_filesWatchers.insert( fileSystemWatcher, &blockFrom );
	
	foreach( QString file, filesToMonitor )
	{
		QFile fileToWatch(file);
		if( false == fileToWatch.open(QFile::Append) )
		{
			Logging::logError( this, QString("file to monitor does not exist, and was not abble to create it: [%1]").arg(file) );
			continue;
		}
		fileToWatch.close();
		fileSystemWatcher->addPath( file );
	}
}

void InternetServerPlatform::slot_fileChanged( const QString& filePath )
{
	Logging::logInfo( this, QString("One of monitored files changed: [%1]").arg(filePath) );
	
	BlockFrom* blockFrom = _filesWatchers.value( qobject_cast<QFileSystemWatcher*>(sender()) );
	if( 0 != blockFrom )
	{
		QFile file( filePath );
		if( false == file.open(QFile::ReadOnly) )
			return;

		blockFrom->_blockedIpListFromFile.clear();
 		while( false == file.atEnd() )
		{
			QByteArray line = file.readLine();
			blockFrom->_blockedIpListFromFile.append( line.trimmed().data() );
			Logging::logInfo( this, QString("the following IP has been added to the block-list: [%1]").arg(line.trimmed().data()) );
     	}
     	
     	file.close();
	}	
}
