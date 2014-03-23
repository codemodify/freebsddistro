
// local headers
#include "eraser.h"

Eraser::Eraser( QWidget* parent ):
	CommonUi(parent)
{
	setWindowTitle( windowTitle() + "disc eraser" );

	_choosePath->setEnabled( false );
	_progress->setVisible( false );
}

Eraser::~Eraser()
{
}

bool Eraser::start()
{
	if( false == CommonUi::start() )
		return false;

	if( _path->text().toLower() == "cd" )
	{
		addLogMessage( QString("burncd -f %1 blank").arg(_device->text()) );
		_process.start( QString("burncd -f %1 blank").arg(_device->text()) );
	}
	else
	{
		addLogMessage( QString("dvd+rw-format -blank %1").arg(_device->text()) );
		_process.start( QString("dvd+rw-format -blank %1").arg(_device->text()) );
	}

	return true;
}

