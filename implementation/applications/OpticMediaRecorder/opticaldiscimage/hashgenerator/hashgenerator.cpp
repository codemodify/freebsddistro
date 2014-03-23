
// Qt's headers
#include <QtGui/QFileDialog>

// local headers
#include "hashgenerator.h"

HashGenerator::HashGenerator( QWidget* parent ):
	CommonUi(parent)
{
	setWindowTitle( windowTitle() + "image hash generator" );

	_device->setText( "none" );
	_device->setEnabled( false );
}

HashGenerator::~HashGenerator()
{}

bool HashGenerator::start()
{
	if( false == CommonUi::start() )
		return false;

	_process.start( QString("md5 \"%1\"").arg(_path->text()) );
	_process.waitForFinished( -1 );

	_process.start( QString("sha1 \"%1\"").arg(_path->text()) );
	_process.waitForFinished( -1 );
	
	_process.start( QString("sha256 \"%1\"").arg(_path->text()) );

	return true;
}

void HashGenerator::choosePath()
{
	QString path = QFileDialog::getOpenFileName();
	if( path.isEmpty() )
		return;

	_path->setText( path );
}

