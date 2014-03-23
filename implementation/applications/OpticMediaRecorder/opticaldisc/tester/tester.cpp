
// local headers
#include "tester.h"

#define DVD_SIZE 4470
#define CD_SIZE 700

Tester::Tester( QWidget* parent ):
	CommonUi(parent)
{
	setWindowTitle( windowTitle() + "disc tester" );

	_choosePath->setEnabled( false );
	_path->setEnabled( false );
	_path->setText( "not applicable" );
}

Tester::~Tester()
{
}

bool Tester::start()
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

	_process.start( QString("dcfldd if=%1 of=/dev/null").arg(_device->text()) );

	return true;
}

QByteArray Tester::readOutput()
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

