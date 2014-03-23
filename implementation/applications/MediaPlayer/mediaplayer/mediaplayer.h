#ifndef _MEDIAPLAYER_H_
#define _MEDIAPLAYER_H_

// Qt's headers
#include <QtGui/QDirModel>
#include <QtGui/QMenu>


// local headers
#include "ui_mediaplayer.h"
#include "mplayerwrapper.h"

class QStringList;
class QPalette;


/**********************************************************************************************************
 * 
 * The starting point of everything is implemented in Mediaplayer
 * 
 *********************************************************************************************************/
class Mediaplayer : public QMainWindow, public Ui::Mediaplayer
{
	// using Qt's signal/slots framework
	Q_OBJECT


	
public:

	Mediaplayer();
	~Mediaplayer();
	
	// setups the parameters for the instance to run correclty
	void initEnvironement();





protected:	// Main window related stuff. Overriding some default implementations

	void resizeEvent( QResizeEvent* event );
	void keyReleaseEvent( QKeyEvent* event );
	void enterEvent( QEvent* event ); 	// work-around to not let the mplayer to handle keyboard
	void leaveEvent( QEvent* event );
	void timerEvent ( QTimerEvent * event );

private slots:	// some aditional stuff

	void resizePlayingMedia();



private slots:	// handling the menu
	
	void playerOptions();
	void playerHelpMe();
	void playListClear();
	void playListSave();
	void playListOpen();
	void playListAddItems( QStringList& items );
	void playListCopyToPortablePlayer();
	void playListSyncFromPortablePlayer();
	void playSelectedPlaylistItem(QListWidgetItem* item);




private slots:	// handles the action when user press the 'Play/Stop/Next/Previous' button
	
	void play();
	void stop();
	void next();
	void previous();




private slots:	// file-browser implementation
	
	void fileBrowserRightClick(const QPoint&);
	void fileBrowserPlay();
	void fileBrowserEnqueue();
	
private:
	QDirModel m_fileBrowserModel;
	QMenu m_fileBrowserMenu;	





private:		// embedding the "mplayer"

	MplayerWrapper* m_mplayer;

private slots:
	void mediaVideoResolution( int width, int heigth );
	void mediaType( MplayerWrapper::MediaType mediaType );
	void mediaStarted();
	void mediaFinished();
	
	void setProgress();




private: // handling the volume
	void getSystemVolume();

private slots:
	void setSystemVolume();





private:		// implementing the autoplay-next feature

	bool m_autoPlayMode;
	void autoNext(); // method used when there is no user input, and the auto-playing mode is on
	int m_currentPlayingRow;





private:		// implementing the fullscreen feature

	bool m_isFullScreen;
	QSize m_size;
	QPoint m_pos;

	// saves the color palette for the output window, used when restoring from fullscreen mode to restore the palete from black
	QPalette m_savePalette;





private:		// used internally to store data sent by mplayer-wraper
	int m_movieWidth;
	int m_movieHeight;
	int m_timerID;
	
};

#endif
