
// local headers
#include "clonner.h"

#define DVD_SIZE 4470
#define CD_SIZE 700

Clonner::Clonner( QWidget* parent ):
	CommonUi(parent)
{
	setWindowTitle( windowTitle() + "disc clonner" );
}

Clonner::~Clonner()
{
}

bool Clonner::start()
{
	if( false == CommonUi::start() )
		return false;

	// first of all trying to detect what kind of media we have to clone
	QProcess mediaDetector;
	mediaDetector.start( QString("growisofs -dry-run -Z %1=/dev/null").arg(_device->text()) );
	mediaDetector.waitForFinished();
	QByteArray detectedMedia = mediaDetector.readAll();
	
	if( detectedMedia.indexOf("media is not recognized as recordable DVD") > -1 )
		_mediaSize = CD_SIZE;
	else
		_mediaSize = DVD_SIZE;

	_process.start( QString("dcfldd if=%1 of=\"%2\"").arg(_device->text()).arg(_path->text()) );

	return true;
}

QByteArray Clonner::readOutput()
{
	QByteArray byteArray = CommonUi::readOutput();
	
	int startPos = byteArray.indexOf("blocks (");
	startPos += 8;
	int endPos = byteArray.indexOf("Mb) written");

	QString writtenSize = byteArray.mid( startPos, endPos-startPos );

	int percent = (writtenSize.trimmed().toInt() * 100) / _mediaSize;
	_progress->setValue( percent );

	return "";
}

