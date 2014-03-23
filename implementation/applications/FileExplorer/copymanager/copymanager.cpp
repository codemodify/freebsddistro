

// Qt's headers
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

// local headers
#include "copymanager.h"
#include "FileManager"
#include "BaloonNotificationManager"

// common's headers
#include <Logger>

#define COPYMANAGER "CopyManager"

CopyManager* CopyManager::s_copyManager = 0;
QMutex CopyManager::s_mutex;

CopyManager::CopyManager()
{
	Logger::getInstance()->addInfo( "CopyManager is up", COPYMANAGER );
}

CopyManager::~CopyManager()
{
	Logger::getInstance()->addInfo( "CopyManager is down", COPYMANAGER );
	
	s_mutex.lock();
	s_copyManager = 0;
	s_mutex.unlock();
}

void CopyManager::addToCopyList( QString source, QString destination, bool move, Filemanager* fileManager )
{
	// creating an instance of the copymanager object
	if( false == CopyManager::isCopyingFiles() )
	{
		// starting a new instance for the copy-manager object
		s_mutex.lock();
		s_copyManager = new CopyManager();
		s_mutex.unlock();
	}

	// connecting the out-side world to internal events
	connect( s_copyManager, SIGNAL(itemDone()), fileManager, SLOT(copyManagerFinished()) );
	connect( s_copyManager, SIGNAL(error(QString)), fileManager, SLOT(copyManagerHasErrors(QString)) );
	
	// adding a new item to the copy list
	s_mutex.lock();
	
	CopyItem copyItem;
	copyItem.source = source;
	copyItem.destination = destination;
	copyItem.move = move;
	
	s_copyManager->m_itemsToCopy.enqueue( copyItem );
	
	// starting the thread if it is not running
	if( false == CopyManager::s_copyManager->isRunning() )
		CopyManager::s_copyManager->start();
	
	s_mutex.unlock();
}

void CopyManager::run()
{
	// copying the items while they are in the list
	while( s_mutex.lock(),false == m_itemsToCopy.isEmpty() )
	{
		s_mutex.unlock();
		doFileCopy( m_itemsToCopy.dequeue() );
		emit itemDone();
	}
	
	s_mutex.unlock();
	
	disconnect();
	
	deleteLater();
	
	s_copyManager = 0;
}

void CopyManager::informAboutTheError( QString errorString )
{
	Logger::getInstance()->addError(errorString, COPYMANAGER);
	emit error( errorString );
}

bool CopyManager::isCopyingFiles()
{
	// the result to return
	bool result = false;
	
	// getting the status for copy-manager object
	s_mutex.lock();
	result = (0 != s_copyManager);
	s_mutex.unlock();
	
	// returning the result
	return result;
}

void CopyManager::doFileCopy( CopyItem copyItem )
{
	// the result for an operation
	bool result = false;
	
	// if we have to deal with a file we simply copy it
	if( QFileInfo(copyItem.source).isFile() )
	{
		// copying the file
		QString destination = copyItem.destination + "/" + QFileInfo(copyItem.source).fileName();
		if( false == copyFile(copyItem.source,destination) )
			informAboutTheError( QString(ERROR_ON_FILE_COPY).arg(copyItem.source,copyItem.destination) );
		
		// removing the source if this is required
		if( (true==copyItem.move)&&(false==removeFile(copyItem.source)) )
			informAboutTheError( QString(ERROR_ON_FILE_DELTE).arg(copyItem.source) );
	}
	
	// dealing with a folder as a source
	else
	{
		if( false == copyFolder(copyItem.source,copyItem.destination) )
			informAboutTheError( QString(ERROR_ON_FOLDER_COPY).arg(copyItem.source,copyItem.destination) );
				
		// we're are done with copy of this "CopyItem", now remove or not the source
		if( (true==copyItem.move)&&(false==removeFolder(copyItem.source)) )
			informAboutTheError( QString(ERROR_ON_FOLDER_DELETE).arg(copyItem.source) );
	}
}
