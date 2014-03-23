#ifndef COPYMANAGER_H_
#define COPYMANAGER_H_

// Qt's headers
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QQueue>
#include <QtCore/QString>

#include "FileSystemOperations"

class Filemanager;

class CopyManager : public QThread
{
	// using the signal/slots framework
	Q_OBJECT
	
public:
	
	~CopyManager();
	
	// adds a new item to the copy list, starts a new copy-manager instance if there is no one	
	static void addToCopyList( QString source, QString destination, bool move, Filemanager* fileManager );

	// checks if there are some copy operations
	static bool isCopyingFiles();
	
	// reimplementation from QThread
	void run();

signals:

	// emited when all copy/move operations are done
	void itemDone();
	void error(QString error);

private:

	// the set of properties for a item to be copied
	struct CopyItem { QString source; QString destination; bool move; };
	
	// stores the list of all items to be copied
	QQueue<CopyItem> m_itemsToCopy;
	
	// used for singleton pattern implementation
	static CopyManager* s_copyManager;
	
	// used  to sync the access to the "s_copyManager" resource
	static QMutex s_mutex;
	
	// disabling the direct creation of the object
	CopyManager();
	
	// copy files routine
	void doFileCopy( CopyItem copyItem );
	void informAboutTheError( QString errorString );
};

#endif
