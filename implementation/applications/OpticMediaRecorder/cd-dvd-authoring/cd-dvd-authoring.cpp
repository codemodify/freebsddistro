
// Qt's headers
#include <QtGui/QCDEStyle>
#include <QtCore/QPointer>

// local headers
#include "cd-dvd-authoring.h"
#include "opticaldisc/OpticalDisc"
#include "opticaldiscimage/OpticalDiscImage"

CDDVDAuthoring::CDDVDAuthoring( QWidget* parent ):
	QWidget(parent)
{
}

CDDVDAuthoring::~CDDVDAuthoring()
{
}

void CDDVDAuthoring::initEnvironement()
{
	QApplication::setStyle( new QCDEStyle() );

	setupUi( this );

	connect( _dvdBurnImage, SIGNAL(clicked()), this, SLOT(dvdBurnImage()) );
	connect( _dvdCreateImage, SIGNAL(clicked()), this, SLOT(dvdCreateImage()) );

	connect( _cdBurnImage, SIGNAL(clicked()), this, SLOT(cdBurnImage()) );
	connect( _cdCreateImage, SIGNAL(clicked()), this, SLOT(cdCreateImage()) );

	connect( _opticaldiscClone, SIGNAL(clicked()), this, SLOT(opticaldiscClone()) );
	connect( _opticaldiscTest, SIGNAL(clicked()), this, SLOT(opticaldiscTest()) );
	connect( _opticaldiscErase, SIGNAL(clicked()), this, SLOT(opticaldiscErase()) );

	connect( _imageConvert, SIGNAL(clicked()), this, SLOT(imageConvert()) );
	connect( _imageGenerateHash, SIGNAL(clicked()), this, SLOT(imageGenerateHash()) );
}

void CDDVDAuthoring::dvdBurnImage()
{
	QPointer<Burner> burner = new Burner();
	burner->setImageType( "DVD" );
	burner->show();
}

void CDDVDAuthoring::dvdCreateImage()
{
	QPointer<Creator> creator = new Creator();
	creator->setImageType( "DVD" );
	creator->show();
}

void CDDVDAuthoring::cdBurnImage()
{
	QPointer<Burner> burner = new Burner();
	burner->setImageType( "CD" );
	burner->show();
}

void CDDVDAuthoring::cdCreateImage()
{
	QPointer<Creator> creator = new Creator();
	creator->setImageType( "CD" );
	creator->show();
}

void CDDVDAuthoring::opticaldiscClone()
{
	QPointer<Clonner> clonner = new Clonner();
	clonner->show();
}

void CDDVDAuthoring::opticaldiscTest()
{
	QPointer<Tester> tester= new Tester();
	tester->show();
}

void CDDVDAuthoring::opticaldiscErase()
{
	QPointer<Eraser> eraser = new Eraser();
	eraser->show();
}

void CDDVDAuthoring::imageConvert()
{}

void CDDVDAuthoring::imageGenerateHash()
{
	QPointer<HashGenerator> hashGenerator = new HashGenerator();
	hashGenerator->show();
}

