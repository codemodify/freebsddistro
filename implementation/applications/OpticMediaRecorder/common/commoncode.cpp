
// Qt's headers
#include <QtGui/QCloseEvent>

// local headers
#include "commonui.h"

CommonCode::CommonCode( QWidget* parent ):
	QWidget(parent)
{
	// setting up the process component
	_process.setReadChannelMode( QProcess::MergedChannels );
	connect( &_process, SIGNAL(readyRead()), this, SLOT(readOutput()) );
	connect( &_process, SIGNAL(started()), this, SLOT(processStarted()) );
	connect( &_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished()) );
}

CommonCode::~CommonCode()
{}

void CommonCode::closeEvent( QCloseEvent* closeEvent )
{
	closeEvent->accept();
	deleteLater();
}

QByteArray CommonCode::readOutput()
{
	QByteArray byteArray = _process.readAll();
	return byteArray;
}

void CommonCode::processStarted()
{
}

void CommonCode::processFinished()
{
}

