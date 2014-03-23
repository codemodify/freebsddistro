
// Qt's headers
#include <QtCore/QStringList>
#include <QtGui/QFileDialog>
#include <QtGui/QKeyEvent>
#include <QtGui/QPalette>
#include <QtGui/QCDEStyle>

// common's headers
#include <Logger>
#include <General>
//#include <Settings>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/soundcard.h>

// local headers
#include "mediaplayer.h"

// used by 'Logger' for identification
#define MEDIAPLAYER "Mediaplayer"

Mediaplayer::Mediaplayer():
	m_mplayer(0),
	m_movieWidth(0),
	m_movieHeight(0),
	m_isFullScreen(false),
	m_autoPlayMode(false),
	m_currentPlayingRow(-1)
{
}

Mediaplayer::~Mediaplayer()
{
	stop();
	delete m_mplayer;
	
	Logger::getInstance()->addInfo( "Mediaplayer is going down", MEDIAPLAYER );
	Logger::destroyInstance();
}

void Mediaplayer::initEnvironement()
{
	// creating logger instance
	if( (false == Logger::createInstance()) || (false == Logger::getInstance()->setLogFile()) )
	{
		Logger::destroyInstance();
		messageBoxError("Can't init Logger");
		QApplication::exit(-1);
	}


	// seting-up the GUI	
	setupUi(this);
	QApplication::setStyle( new QCDEStyle() );
	

	// assigning handlers to user actions for Play/Stop/Next/Previous
	connect( m_play,		SIGNAL(clicked()), this, SLOT(play()) );
	connect( m_stop,		SIGNAL(clicked()), this, SLOT(stop()) );
	connect( m_next,		SIGNAL(clicked()), this, SLOT(next()) );	
	connect( m_previous,	SIGNAL(clicked()), this, SLOT(previous()) );


	// handling the file browser
	m_fileBrowserModel.setFilter( QDir::AllEntries );
	m_fileBrowser->setModel( &m_fileBrowserModel );
	int column = m_fileBrowserModel.columnCount( QModelIndex() );
	for( --column; column > 0; --column )
	{
		// hiding all the collumns except the 'Name'
		m_fileBrowser->hideColumn( column );
	}

	connect( m_fileBrowser, SIGNAL(customContextMenuRequested(const QPoint&)),	this, SLOT(fileBrowserRightClick(const QPoint&))				);

	m_fileBrowserMenu.addAction("Play", this, SLOT(fileBrowserPlay()));
	m_fileBrowserMenu.addSeparator();
	m_fileBrowserMenu.addAction("Enqueue", this, SLOT(fileBrowserEnqueue()));


	// handling the menu actions
	connect( actionQuit, SIGNAL(triggered()), this, SLOT(close()) );
	connect( actionOptions, SIGNAL(triggered()), this, SLOT(playerOptions()) );
	connect( actionHelp_Me, SIGNAL(triggered()), this, SLOT(playerHelpMe()) );

	connect( actionSave_As, SIGNAL(triggered()), this, SLOT(playListSave()) );
	connect( actionOpen, SIGNAL(triggered()), this, SLOT(playListOpen()) );
	connect( actionClear, SIGNAL(triggered()), this, SLOT(playListClear()) );
	connect( actionCopy_To_My_Portable_Music_Player, SIGNAL(triggered()), this, SLOT(playListCopyToPortablePlayer()) );
	connect( actionSync_from_My_Portable_Music_Player, SIGNAL(triggered()), this, SLOT(playListSyncFromPortablePlayer()) );


	// handling mouse-events for the play-list
	connect( m_playList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(playSelectedPlaylistItem(QListWidgetItem*)) );


	// init the mplayer
	m_videoOutput->setAutoFillBackground(true);
	m_videoOutput->setPalette(QColor(0,0,0));
	m_mplayer = new MplayerWrapper(m_videoOutput);


	// handling the media information
	connect( m_mplayer, SIGNAL(mediaStarted()), this, SLOT(mediaStarted()) );
	connect( m_mplayer, SIGNAL(mediaFinished()), this, SLOT(mediaFinished()) );
	connect( m_mplayer, SIGNAL(mediaProgress(int)), m_progress, SLOT(setValue(int)) );
	connect( m_mplayer, SIGNAL(mediaVideoResolution(int,int)), this, SLOT(mediaVideoResolution(int,int)) );


	connect( splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(resizePlayingMedia()) );

	// handling the volume
	getSystemVolume();
	connect( m_volume, SIGNAL(sliderReleased()), this, SLOT(setSystemVolume()) );


	// setting the progress that user requested
	connect( m_progress, SIGNAL(sliderReleased()), this, SLOT(setProgress()) );
	

	Logger::getInstance()->addInfo( "Mediaplayer is up and running", MEDIAPLAYER );
}

void Mediaplayer::mediaStarted()
{
	m_play->setEnabled(false);
	m_stop->setEnabled(true);
}

void Mediaplayer::mediaFinished()
{
	m_play->setEnabled(true);
	m_stop->setEnabled(false);
	
	if( true == m_autoPlayMode )
		autoNext();
}

void Mediaplayer::play()
{
	// checking if we have any playlist-items
	if( !m_playList->count() )
		return;

	// start playing
	if( m_currentPlayingRow < 0 )
		m_currentPlayingRow = 0;
	
	m_playList->setCurrentRow( m_currentPlayingRow );
	
	QString fileToPlay = m_playList->currentItem()->data(Qt::DisplayRole).toString();
	m_mplayer->play( fileToPlay );
	Logger::getInstance()->addInfo( QString("playing file: %1").arg(fileToPlay), MEDIAPLAYER );
	
	// starting the auto-play mode
	m_autoPlayMode = true;
	
	m_timerID = startTimer(1000);
}

void Mediaplayer::stop()
{
	killTimer(m_timerID);
	
	// stopping the auto-play mode
	m_autoPlayMode = false;

	// stopping playing media
	m_mplayer->stop();
	m_movieHeight = 0;
	m_movieWidth = 0;	
}

void Mediaplayer::previous()
{
	// stopping current playing
	stop();
	
	// checking if we have any playlist-items
	if( !m_playList->count() )
		return;
	
	if( m_playList->currentRow() <= 0 )
		m_currentPlayingRow = m_playList->count() - 1;
	else
		m_currentPlayingRow--;
	
	// playing new item
	play();
}

void Mediaplayer::next()
{
	// stopping current playing
	stop();
	
	// checking if we have any playlist-items
	if( !m_playList->count() )
		return;
	
	// getting the next item to play
	if( m_currentPlayingRow >= (m_playList->count() - 1) )
		m_currentPlayingRow = 0;
	else
		m_currentPlayingRow++;

	// playing next media
	play();
}

void Mediaplayer::fileBrowserRightClick(const QPoint& point)
{
	QPoint position = m_fileBrowser->mapToGlobal(point);
	position.setY(position.y()+30); //BUG: the 'y' - position is not translated correctly, so we have to workaround this
	m_fileBrowserMenu.exec( position );
}

void Mediaplayer::fileBrowserPlay()
{
	// stoping playing, and clearing the playlist
	playListClear();
	
	// adding new items to playlist
	QStringList items;
	QModelIndexList selectedItems = m_fileBrowser->selectionModel()->selectedIndexes();
	for( QModelIndexList::iterator item = selectedItems.begin(); item != selectedItems.end(); ++item )
	{
		QString itemDisplayName = m_fileBrowserModel.filePath(*item);
		items.append( itemDisplayName );
	}
	playListAddItems( items );
	
	// start playing newly added media
	play();
}

void Mediaplayer::fileBrowserEnqueue()
{
	// adding selected items
	QStringList items;
	
	QModelIndexList selectedItems = m_fileBrowser->selectionModel()->selectedIndexes();
	for( QModelIndexList::iterator item = selectedItems.begin(); item != selectedItems.end(); ++item )
	{
		QString itemDisplayName = m_fileBrowserModel.filePath(*item);
		items.append( itemDisplayName );
	}
	
	playListAddItems( items );
}

void Mediaplayer::playListClear()
{
	// stop playing media
	stop();
	
	// clearing the playlist
	m_playList->clear();
	m_currentPlayingRow = -1;
}

void Mediaplayer::playListSave()
{/*
	// getting the file name to save as
	QString fileName = "";
	fileName = QFileDialog::getSaveFileName( this, "Save Playlist as ...", "/home", "Playlist (*.playlist)");
	if( fileName.isEmpty() )
		return;
		
	// building the items list
	QStringList items;
	for( int index = 0; index < m_playList->count(); ++index )
		items.append(  (m_playList->item(index))->data(Qt::DisplayRole).toByteArray()  );

	// saving the playlist
	Settings settings;
	settings.setSettingsFile( fileName );
	settings.addKey( "playlistItems" );
	settings.setKeyWithValues( "playlistItems", items );
	settings.saveSettings();
*/
}

void Mediaplayer::playListOpen()
{/*
	// getting the file name to load media from
	QString fileName = "";
	fileName = QFileDialog::getOpenFileName( this, "Open a Playlist ...", "/home", "Playlist (*.playlist)");
	if( fileName.isEmpty() )
		return;
	
	// loading all the items found
	Settings settings;
	settings.setSettingsFile( fileName );
	QStringList* values = settings.getValuesForKey("playlistItems");
	
	// adding loaded items into playlist 
	playListClear();
	playListAddItems( *values );
	
	// the returned object from the library is up to us to destroy
	values->clear();
	delete values;*/
}

void Mediaplayer::playListAddItems( QStringList& items )
{
	m_playList->addItems( items );
}

void Mediaplayer::playListCopyToPortablePlayer()
{
	messageBoxInfo( "not implemented" );
}

void Mediaplayer::playListSyncFromPortablePlayer()
{
	messageBoxInfo( "not implemented" );
}

void Mediaplayer::playerOptions()
{
	// getting the configuration file for Mediaplayer and runing the configurator
	QString settingsFile = getFullPathForSettingsFile();
	QProcess::startDetached(QString("configurator %1").arg(settingsFile));
}

void Mediaplayer::playerHelpMe()
{
	// getting the configuration file for Mediaplayer and runing the configurator
	QString settingsFile = getFullPathForSettingsFile();
	Logger::getInstance()->addInfo(settingsFile);
	QProcess::startDetached(QString("configurator \"%1\" startupSection=\"helpMe\"").arg(settingsFile));
}

void Mediaplayer::autoNext()
{
	// checking if we have any playlist-items
	if( !m_playList->count() )
		return;
	
	// getting the next item to play
	if( m_currentPlayingRow >= (m_playList->count() - 1) )
		m_currentPlayingRow = 0;
	else
		m_currentPlayingRow++;
	m_playList->setCurrentRow( m_currentPlayingRow );

	QString fileToPlay = m_playList->currentItem()->data(Qt::DisplayRole).toString();
	m_mplayer->play( fileToPlay );
}

void Mediaplayer::getSystemVolume()
{
	int _mixerfd = open("/dev/mixer", O_RDWR);
	int vol = 0;
	int res = -1;
	res = ioctl (_mixerfd, MIXER_READ(SOUND_MIXER_PCM), &vol);
	if( res == -1 )
		return;
	m_volume->setSliderPosition( vol & 0x7f );
	::close( _mixerfd );
}

void Mediaplayer::setSystemVolume()
{
	int _mixerfd = open("/dev/mixer", O_RDWR);
	int v = m_volume->value();
	int vol = 0;
	int res = -1;
	if (v <= 0) v = 1;
	vol = ((v << 8) & 0x7f00) | (v & 0x007f);
	res = ioctl (_mixerfd, MIXER_WRITE(SOUND_MIXER_PCM), &vol);
	::close( _mixerfd );
}

void Mediaplayer::resizePlayingMedia()
{	
	if( 0 != m_mplayer)
	{
		QSize videoWindowSize = m_videoOutput->size();
		int videoWindowWidth = videoWindowSize.width();
		int videoWindowHeight = videoWindowSize.height();

		bool isMovieWide = m_movieWidth > m_movieHeight;
		bool isVideoWindowWide = videoWindowWidth > videoWindowHeight;

		int scale_x, scale_y;
		if( isMovieWide )
		{
			scale_x = videoWindowWidth;
			double factor = static_cast<double>(m_movieWidth) / static_cast<double>(m_movieHeight);				
			scale_y = videoWindowWidth / factor;
			
			if( scale_y > videoWindowHeight )
			{
				scale_y = videoWindowHeight;
			}
		}
		else
		{
			scale_y = videoWindowHeight;
			double factor = static_cast<double>(m_movieHeight) / static_cast<double>(m_movieWidth);
			scale_x = videoWindowHeight / factor;
			
			if( scale_x > videoWindowWidth )
			{
				scale_x = videoWindowWidth;
			}
		}
		
		if( (scale_x<0) || (scale_y<0) )
		{
			scale_x = videoWindowWidth;
			scale_y = videoWindowHeight;
		}

		int startPointX = (videoWindowWidth - scale_x ) / 2;
		int startPointY = (videoWindowHeight - scale_y ) / 2;

		m_mplayer->setGeometry( startPointX, startPointY, scale_x, scale_y );
	}
}

void Mediaplayer::resizeEvent( QResizeEvent* event )
{
	resizePlayingMedia();
}

void Mediaplayer::mediaVideoResolution( int width, int heigth )
{
	m_movieWidth = width;
	m_movieHeight = heigth;
}

void Mediaplayer::playSelectedPlaylistItem(QListWidgetItem* item)
{
	stop();
	m_currentPlayingRow = m_playList->currentRow();
	play();
}

void Mediaplayer::keyReleaseEvent( QKeyEvent* event )
{
	Qt::Key key = static_cast<Qt::Key>( event->key() );
	switch( key )
	{
		// play
		case Qt::Key_Return:
			play();
		break;
		
		// pause
		case Qt::Key_Pause:		
		case Qt::Key_Space:
			m_mplayer->pause();
		break;
		
		// stop
		case Qt::Key_S:
			stop();
		break;
		
		// replay
		case Qt::Key_R:
			stop();
			play();
		break;
		
		// quit
		case Qt::Key_Q:
			stop();
			QApplication::quit();
		break;

		// previous
		case Qt::Key_P:
			previous();
		break;
		
		// next
		case Qt::Key_N:
			next();
		break;

		// show/hide playlist
		case Qt::Key_H:
			toolBox->setVisible( !toolBox->isVisible() );
		break;
		
		// kontrols
		case Qt::Key_K:
			frame->setVisible( !frame->isVisible() );
			frame_2->setVisible( !frame_2->isVisible() );
		break;
		
		// fullscreen
		case Qt::Key_F:
		{
			menubar->setVisible( m_isFullScreen );
			toolBox->setVisible( m_isFullScreen );
			frame->setVisible( m_isFullScreen );
			frame_2->setVisible( m_isFullScreen );
			
			if( !m_isFullScreen )
			{
				m_savePalette = palette();
				m_size = size();
				m_pos  = pos();
				setPalette( QColor(0,0,0) );
				setWindowState( Qt::WindowFullScreen );	
				m_isFullScreen = true;		
			}
			else
			{
				setPalette( m_savePalette );
				setWindowState( Qt::WindowNoState );	
				resize( m_size );
				move( m_pos );
				setWindowFlags( Qt::Window );
				showNormal();
				m_isFullScreen = false;		
			}
		}
		break;
		
		// ontop
		case Qt::Key_O:
			startTimer( 100 );
		break;
		
		// thin window
		case Qt::Key_T:
			keyReleaseEvent( &QKeyEvent(QEvent::KeyRelease,Qt::Key_K,Qt::NoModifier) );
			keyReleaseEvent( &QKeyEvent(QEvent::KeyRelease,Qt::Key_H,Qt::NoModifier) );
			//setWindowFlags( Qt::FramelessWindowHint );
		break;
		
		// disabling some keys
		case Qt::Key_Escape:
		break;
		
		// from QT docs, if we don't handle this key, we have to call this method
		default:
			event->ignore();
	}
}

void Mediaplayer::enterEvent( QEvent* event )
{
	grabKeyboard();
}

void Mediaplayer::leaveEvent( QEvent* event )
{
	releaseKeyboard();
}

void Mediaplayer::mediaType( MplayerWrapper::MediaType mediaType )
{
	
}

void Mediaplayer::setProgress()
{	
	m_mplayer->setProgress( m_volume->value() );
}

void Mediaplayer::timerEvent ( QTimerEvent * event )
{
	resizePlayingMedia();
}
