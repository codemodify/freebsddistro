#ifndef _MOD_BASE_H_
#define _MOD_BASE_H_

#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QMutex>

class ModBase : public QThread
{
	Q_OBJECT
		
public:
	ModBase(QString fileToPlay);
	virtual ~ModBase();
	
	// sets the stop flag, to terminate the thread
	void stopPlaying();

public slots:
	// sets the progress as a request by user
	virtual void setProgress( int percent );
	
	// sets the volume as a request by user
	virtual void setVolume( int percent );
	
signals:
	// reflects the current progress in playing media
	void progress( int percent );
	
protected:

	// gets the user's request to stop or not
	bool haveToStop();
	
	// stores the file that should be played
	QString m_fileToPlay;
	
	// thread stop flag
	bool m_haveToStop;
	
	// mutex used to manage access to the 'm_haveToStop'
	QMutex m_mutex;
};

#endif
