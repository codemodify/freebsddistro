#ifndef MPLAYERWRAPPER_H_
#define MPLAYERWRAPPER_H_

// Qt's header files
#include <QtCore/QProcess>
#include <QtCore/QTimer>
#include <QtGui/QX11EmbedContainer>




/*******************************************************************************************************************
 * 
 *	This class is designed to wrap the 'mplayer', by making easier to send/receive information to/from mplayer,
 *	and by presenting it as a widget, ready to use in any application.
 * 
 *******************************************************************************************************************/
class MplayerWrapper : public QX11EmbedContainer
{
	
	// using the Qt's signal/slots framework
	Q_OBJECT


public:	// defines the media type that we can play
	enum MediaType
	{
		audio,
		video,
		dvd,
		vcd,
		networkStream
	};




public:
	MplayerWrapper( QWidget* parent = 0 );
	~MplayerWrapper();




signals:		// information that we need to know about the current playing media

	void mediaProgress( int percent );
	void mediaVideoResolution( int width, int heigth );
	void mediaType( MediaType mediaType );
	void mediaStarted();
	void mediaFinished();



public slots:

	void play( QString filePath );
	void stop();
	void pause();
	void setProgress( int percent );




private slots:

	void requestOutputFromMplayer();
	void mplayerHasOutput();
	void mplayerStarted();
	void mplayerFinished();
	



private:
	QProcess m_mplayerProcess;
	QTimer m_timer;

};

#endif

