
// Qt's headers
#include <QtGui/QColor>
#include <QtGui/QTextCharFormat>
#include <QtGui/QFileDialog>

// local headers
#include "commonui.h"

CommonUi::CommonUi( QWidget* parent ):
	CommonCode(parent)
{

	setupUi( this );
	
	connect( _start, SIGNAL(clicked()), this, SLOT(start()) );
	connect( _stop, SIGNAL(clicked()), this, SLOT(stop()) );
	connect( _choosePath, SIGNAL(clicked()), this, SLOT(choosePath()) );
}

CommonUi::~CommonUi()
{
}

void CommonUi::choosePath()
{
	QString path = QFileDialog::getSaveFileName();
	if( path.isEmpty() )
		return;

	_path->setText( path );
}

bool CommonUi::start()
{
	_logWindow->clear();

	if( _path->text().isEmpty() || _device->text().isEmpty() )
	{
		addLogMessage( ".", "red" );
		addLogMessage( "Be sure to indicate a image file and burning device.","red" );
		return false;
	}

	addLogMessage( ".", "green" );
	addLogMessage( "Start - ###########################################################", "green" );

	return true;
}

void CommonUi::stop()
{
	_process.terminate();
	addLogMessage( "Stopped by user - ###########################################################", "red" );
}

QByteArray CommonUi::readOutput()
{
	QByteArray byteArray = CommonCode::readOutput();

	addLogMessage( byteArray );

	return byteArray;
}

void CommonUi::addLogMessage( QString message, QString colorName )
{
	// formatting the log messages
	QColor color;
	color.setNamedColor( colorName );

	QTextCharFormat textCharFormat;
	textCharFormat.setForeground( QBrush(color) );

	// add a new log message
	_logWindow->setCurrentCharFormat( textCharFormat );
	_logWindow->append( message.trimmed() );
}

void CommonUi::processStarted()
{
	_start->setEnabled( false );
	_stop->setEnabled( true );
}

void CommonUi::processFinished()
{
	_start->setEnabled( true );
	_stop->setEnabled( false );

	addLogMessage( "Finished - ###########################################################", "green" );
}

