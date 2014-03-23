
// Qt's headers
#include <QtGui/QFileDialog>

// local headers
#include "burner.h"

Burner::Burner( QWidget* parent ):
	CommonUi(parent),
	_imageType("")
{
}

Burner::~Burner()
{
}

bool Burner::start()
{
	if( false == CommonUi::start() )
		return false;

	if( _imageType == "DVD" )
	{
		addLogMessage( QString("growisofs -speed=4 -dvd-compat -Z %1=%2").arg(_device->text()).arg(_path->text()) );
		_process.start( QString("growisofs -speed=4 -dvd-compat -Z %1=%2").arg(_device->text()).arg(_path->text()) );
	}
	else if( _imageType == "CD" )
	{
		addLogMessage( QString("burncd -e -s 4 -v -f %1 data %2 fixate").arg(_device->text()).arg(_path->text()) );
		_process.start( QString("burncd -e -s 4 -v -f %1 data %2 fixate").arg(_device->text()).arg(_path->text()) );
	}

	return true;
}

void Burner::setImageType( QString imageType )
{
	_imageType = imageType;

	if( _imageType == "DVD" )
		setWindowTitle( QString("%1 %2").arg(windowTitle()).arg("DVD image burning") );

	else if( _imageType == "CD" )
		setWindowTitle( QString("%1 %2").arg(windowTitle()).arg("CD image burning") );

	else
	{
		addLogMessage( "An error occured. It is not clear how this diloag was started.","red" );
		addLogMessage( "Any burnings attemtps will not take place.","red" );
	}
}

QByteArray Burner::readOutput()
{
	QByteArray byteArray = CommonUi::readOutput();

	if( byteArray.indexOf("%") > -1 )
	{
		QString percent = byteArray.mid( byteArray.indexOf("%")-4, 2 );
		_progress->setValue( percent.trimmed().toInt() );
	}

	return "";
}

void Burner::choosePath()
{
	QString path = QFileDialog::getOpenFileName();
	if( path.isEmpty() )
		return;

	_path->setText( path );
}
