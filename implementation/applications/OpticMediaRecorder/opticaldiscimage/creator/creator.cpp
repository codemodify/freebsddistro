
// Qt's headers
#include <QtGui/QFileDialog>
#include <QtGui/QTextCharFormat>
#include <QtCore/QUuid>
#include <QtGui/QCloseEvent>

// common's headers
#include <FileSystemOperations>

// local headers
#include "creator.h"


#define DVD_MAX_SIZE 4291 
#define CD_MAX_SIZE 700


Creator::Creator( QWidget* parent ):
	CommonCode(parent),
	_imageType("")
{
	setupUi( this );


	// setting up the clicks on buttons
	connect( _start, SIGNAL(clicked()), this, SLOT(start()) );
	connect( _stop, SIGNAL(clicked()), this, SLOT(stop()) );
	connect( _choosePath, SIGNAL(clicked()), this, SLOT(choosePath()) );


	// setting up the files view component
	_dirModelFiles.setFilter( QDir::AllEntries | QDir::Hidden );
	_dirModelFiles.setSorting( QDir::DirsFirst );

	_filesView->setModel( &_dirModelFiles );
	connect( _filesView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(filesContextMenu(const QPoint&)) );
	connect( _filesView, SIGNAL(collapsed(const QModelIndex&)), this, SLOT(resizeColumnToContents()) );
	connect( _filesView, SIGNAL(expanded(const QModelIndex&)), this, SLOT(resizeColumnToContents()) );

	_filesMenu.addAction( "Add to image", this, SLOT(addFilesToImage()) );
	_filesMenu.addAction( "Refresh", this, SLOT(refreshFiles()) );


	// setting up the image view component
	QString uuid = QUuid::createUuid().toString();
	QString folderPath = getenv("HOME");
	folderPath += "/" + uuid;
	createFolder( folderPath );
	_folderPath = folderPath;

	_dirModelImage.setFilter( QDir::AllEntries | QDir::Hidden );
	_dirModelImage.setSorting( QDir::DirsFirst );

	_imageView->setModel( &_dirModelImage );
	_imageView->setRootIndex( _dirModelImage.index(_folderPath) );
	connect( _imageView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(imageContextMenu(const QPoint&)) );
	connect( _imageView, SIGNAL(collapsed(const QModelIndex&)), this, SLOT(resizeColumnToContents()) );
	connect( _imageView, SIGNAL(expanded(const QModelIndex&)), this, SLOT(resizeColumnToContents()) );

	_imageMenu.addAction( "Remove from image", this, SLOT(removeFromImage()) );
}

Creator::~Creator()
{
	removeFolder( _folderPath );
}

void Creator::setImageType( QString imageType )
{
	_imageType = imageType;

	if( _imageType == "DVD" )
	{
		setWindowTitle( QString("%1 %2").arg(windowTitle()).arg("DVD image creator") );
		_imageSize->setMaximum( DVD_MAX_SIZE );
	}

	else if( _imageType == "CD" )
	{
		setWindowTitle( QString("%1 %2").arg(windowTitle()).arg("CD image creator") );
		_imageSize->setMaximum( CD_MAX_SIZE );
	}

	else
		addLogMessage( "An error occured. It is not clear how this diloag was started.");
}

void Creator::choosePath()
{
	QString path = QFileDialog::getSaveFileName();
	if( path.isEmpty() )
		return;

	_path->setText( path );
}

void Creator::start()
{
	if( _path->text().isEmpty() )
	{
		addLogMessage( ".\n", "red" );
		addLogMessage( "Be sure to indicate a image file to save as.","red" );
		return;
	}

	addLogMessage( ".\n", "green" );
	addLogMessage( "Starting - ###########################################################", "green" );
	addLogMessage( QString("mkisofs -f -R -J -iso-level 3 -o %1 %2").arg(_path->text()).arg(_folderPath) );
	_process.start( QString("mkisofs -f -R -J -iso-level 3 -o %1 %2").arg(_path->text()).arg(_folderPath) );
}

void Creator::stop()
{
	_process.terminate();
	addLogMessage( "Stopped by user - ###########################################################", "red" );
}

QByteArray Creator::readOutput()
{
	QByteArray byteArray = CommonCode::readOutput();

	if( byteArray.indexOf("%") > -1 )
	{
		QString percent = byteArray.mid(1,2);
		_progress->setValue( percent.toInt() );
	}
	
	addLogMessage( byteArray );

	return "";
}

void Creator::addLogMessage( QString message, QString colorName )
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

void Creator::filesContextMenu( const QPoint& point )
{
	QPoint screenPosition = _filesView->viewport()->mapToGlobal( point );

	_filesMenu.exec( screenPosition );
}

void Creator::addFilesToImage()
{
	QString fullPath = _dirModelFiles.filePath( _filesView->currentIndex() );
	QString name = _dirModelFiles.fileName( _filesView->currentIndex() );
	createSymbolicLink( fullPath, _folderPath+"/"+name );

	_dirModelImage.refresh( _dirModelImage.index(_folderPath) );
	_imageView->setRootIndex( _dirModelImage.index(_folderPath) );

	calculateImageSize();
}

void Creator::resizeColumnToContents()
{
	QObject* object = sender();
	if( 0 == object )
		return;

	QTreeView* treeView = qobject_cast<QTreeView*>( object );
	if( 0 == treeView )
		return;

	treeView->resizeColumnToContents( 0 );
}

void Creator::imageContextMenu( const QPoint& point )
{
	QPoint screenPosition = _imageView->viewport()->mapToGlobal( point );

	_imageMenu.exec( screenPosition );
}

void Creator::removeFromImage()
{
	QString fullPath = _dirModelImage.fileName( _imageView->currentIndex() );
	removeFile( _folderPath + "/" + fullPath );

	_dirModelImage.refresh( _dirModelImage.index(_folderPath) );
	_imageView->setRootIndex( _dirModelImage.index(_folderPath) );

	calculateImageSize();
}

void Creator::processStarted()
{
	_start->setEnabled( false );
	_stop->setEnabled( true );
}

void Creator::processFinished()
{
	_start->setEnabled( true );
	_stop->setEnabled( false );

	addLogMessage( "Finished - ###########################################################", "green" );
}

void Creator::calculateImageSize()
{
	QProcess diskUsage;
	diskUsage.start( QString("du -L -d 0 \"%1\"").arg(_folderPath) );
	diskUsage.waitForFinished();

	QByteArray byteArray = diskUsage.readAll();
	byteArray.remove( byteArray.indexOf(_folderPath), _folderPath.length() );

	_imageSize->setValue( byteArray.trimmed().toInt() );
	_currentSize->setText( QString("%1 MB").arg(byteArray.trimmed().data()) );

	int totalSize = (_imageType=="DVD") ? DVD_MAX_SIZE : CD_MAX_SIZE;

	_availableSize->setText( QString("%1 MB").arg(totalSize) );
	if( byteArray.trimmed().toInt() >= totalSize )
	{
		_imageSize->setValue( _imageSize->maximum() );
		_sizeExceeded->setText( QString("%1 MB").arg(byteArray.trimmed().toInt()-totalSize) );
		
	}
}

void Creator::refreshFiles()
{
	_dirModelFiles.refresh( _filesView->currentIndex() );
}

