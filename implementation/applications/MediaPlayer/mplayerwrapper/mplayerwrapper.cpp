
// local headers
#include "mplayerwrapper.h"


#define ONE_SECOND 1000
#define HALF_SECOND 500
#define INFINITE -1

MplayerWrapper::MplayerWrapper( QWidget* parent ):
	QX11EmbedContainer(parent)
{
	// setting timer on the update job
	connect( &m_mplayerProcess, SIGNAL(readyReadStandardOutput()),this, SLOT(mplayerHasOutput()));
	connect( &m_mplayerProcess, SIGNAL(started()), this, SLOT(mplayerStarted()) );
	connect( &m_mplayerProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(mplayerFinished()) );
	connect( &m_timer, SIGNAL(timeout()), this, SLOT(requestOutputFromMplayer()) );
}

MplayerWrapper::~MplayerWrapper()
{
	stop();
}

void MplayerWrapper::play( QString filePath )
{
	m_mplayerProcess.start( QString("mplayer -slave -wid %1 \"%2\"").arg(winId()).arg(filePath) );
	m_mplayerProcess.waitForStarted( INFINITE );
	m_timer.start( ONE_SECOND );
}

void MplayerWrapper::stop()
{
	m_timer.stop();
	emit mediaProgress( 0 );
	m_mplayerProcess.write( "quit\n" );
	m_mplayerProcess.kill();
	m_mplayerProcess.waitForFinished( ONE_SECOND );
}

void MplayerWrapper::requestOutputFromMplayer()
{
	// requesting the 'mplayer' to write to 'stdout' the current progress percent
	m_mplayerProcess.write( "get_percent_pos\n" );
}

void MplayerWrapper::mplayerHasOutput()
{
	QByteArray output = m_mplayerProcess.readAllStandardOutput();
	
	// checking for progress information
	QRegExp percentProgress( "ANS_PERCENT_POSITION=(\\d+)" );
	if( percentProgress.indexIn(output) > -1 )
	{
		emit mediaProgress( percentProgress.cap(1).toInt() );
	}
	
	// checking for video width:heigth
	QRegExp rxdim( "VO:.*=>\\s(\\d+)x(\\d+)" );
	if (rxdim.indexIn(output)>-1)
	{
		emit mediaVideoResolution( rxdim.cap(1).toInt(), rxdim.cap(2).toInt() );
	}
}

void MplayerWrapper::setProgress( int percent )
{
	m_mplayerProcess.write( QString( "seek %1 1 \n" ).arg(percent).toAscii() );
}

void MplayerWrapper::pause()
{
	if( true == m_timer.isActive() )		// have to pause
	{
		m_timer.stop();
		m_mplayerProcess.write( "pause\n" );
	}
	else		// have to resume
	{
		m_mplayerProcess.write( "qweasd\n" );
		m_timer.start( ONE_SECOND );		
	}
}

void MplayerWrapper::mplayerStarted()
{
	emit mediaStarted();
}

void MplayerWrapper::mplayerFinished()
{
	emit mediaFinished();
}
