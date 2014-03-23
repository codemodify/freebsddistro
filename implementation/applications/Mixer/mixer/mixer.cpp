
// system headers
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <fcntl.h>

// Qt's headers
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QFrame>
#include <QtGui/QRadioButton>
#include <QtGui/QPlastiqueStyle>

// local headers
#include "mixer.h"

Mixer::Mixer():
	m_mixerDevice(0),
	m_existMask(0),
	m_canRecordMask(0),
	m_isRecordMask(0),
	m_stereoMask(0)
{
}

Mixer::~Mixer()
{
	if( m_mixerDevice )
	{
		::close( m_mixerDevice );
	}
}


char* mixer="/dev/mixer";
char deviceLabels[SOUND_MIXER_NRDEVICES][80]=SOUND_DEVICE_LABELS;
char deviceNames [SOUND_MIXER_NRDEVICES][80]=SOUND_DEVICE_NAMES;

bool Mixer::initEnvironement()
{
	// open the mixer device
	m_mixerDevice = open( mixer, O_RDWR );
	if( 0 > m_mixerDevice )
		return false;

	int ioctlResult = 0;
	ioctlResult = ioctl( m_mixerDevice, SOUND_MIXER_READ_DEVMASK, &m_existMask );
	if( (-1==ioctlResult) || (0==m_existMask) )
		return false;

	ioctlResult = ioctl( m_mixerDevice, SOUND_MIXER_READ_RECMASK, &m_canRecordMask );
	if( -1 == ioctlResult )
		return false;

	ioctlResult = ioctl( m_mixerDevice, SOUND_MIXER_READ_RECSRC, &m_isRecordMask );
	if( -1 == ioctlResult )
		return false;

	ioctlResult = ioctl( m_mixerDevice, SOUND_MIXER_READ_STEREODEVS, &m_stereoMask );
	if( -1 == ioctlResult )
		m_stereoMask = 0;
		
	// adding the layout on the frame, for feature widgets
	QVBoxLayout* generalLayout = new QVBoxLayout( this );
	QFrame* frame = new QFrame();
	frame->setFrameShape( QFrame::StyledPanel );
	generalLayout->addWidget( frame );
	
	QGridLayout* gridLayout = new QGridLayout( frame );
		
	// adding sliders on the form, depending on the number of the mixer-devices
	for( int deviceNumber = 0, gridRow = 0; deviceNumber < SOUND_MIXER_NRDEVICES; ++deviceNumber )
	{
		// check for a valid device
		if( m_existMask & (1<<deviceNumber) )
		{
			// build the device-name
			QLabel* deviceName = new QLabel( deviceLabels[deviceNumber] );
			deviceName->setAlignment( Qt::AlignRight );
			
			// build the device-value
			QSlider* deviceValue = new QSlider( Qt::Horizontal );
			deviceValue->setObjectName( deviceNames[deviceNumber] );
			deviceValue->setTickPosition( QSlider::TicksBelow );
			deviceValue->setSliderPosition( getMixerDeviceValue(deviceNumber) );
			connect( deviceValue, SIGNAL(sliderMoved(int)), this, SLOT(mixerDeviceChangedValue(int)) );
			
			// build the device's availability to be used as source for recording
			QRadioButton* sourceRecord = new QRadioButton();
			sourceRecord->setEnabled( (m_canRecordMask&(1<<deviceNumber))? true:false );
			sourceRecord->setChecked( (m_isRecordMask&(1<<deviceNumber))? true:false );
			sourceRecord->setObjectName( deviceNames[deviceNumber] + QString("_record") );
			connect( sourceRecord, SIGNAL(toggled(bool)), this, SLOT(mixerRecordSourceChanged(bool)) );
			
			// showing all that above
			gridLayout->addWidget( deviceName, gridRow, 0, Qt::AlignHorizontal_Mask );
			gridLayout->addWidget( deviceValue, gridRow, 1, Qt::AlignHorizontal_Mask );
			gridLayout->addWidget( sourceRecord, gridRow, 2, Qt::AlignHorizontal_Mask );
			
			++gridRow;
		}
	}
	
	setMinimumWidth(400);
	QApplication::setStyle(new QPlastiqueStyle());
	return true;
}

void Mixer::mixerDeviceChangedValue( int newValue )
{
	// getting the slider that sent the signal
	QSlider* slider = static_cast<QSlider*>( sender() );
	
	// getting the slider's name, to find the device we have to change
	QString deviceName = slider->objectName();
	
	for( int deviceNumber = 0; deviceNumber < SOUND_MIXER_NRDEVICES; ++deviceNumber )
	{
		if( (m_existMask & (1<<deviceNumber)) && (deviceName == deviceNames[deviceNumber]) )
		{
			int value = newValue;
			value += value * 256;
			ioctl( m_mixerDevice, MIXER_WRITE(deviceNumber), &value );
			break;
		}
	}
}

int Mixer::getMixerDeviceValue( int mixerDevice )
{
	int value = 0;
	ioctl( m_mixerDevice, MIXER_READ(mixerDevice), &value );
	
	return (value & 255);
}

void Mixer::mixerRecordSourceChanged( bool setSourceRecord )
{
	// getting the slider that sent the signal
	QRadioButton* sourceRecord = static_cast<QRadioButton*>( sender() );
	
	// getting the slider's name, to find the device we have to change
	QString deviceName = sourceRecord->objectName();
	deviceName.remove("_record");
	
	for( int deviceNumber = 0; deviceNumber < SOUND_MIXER_NRDEVICES; ++deviceNumber )
	{
		if( (m_existMask & (1<<deviceNumber)) &&
			(m_canRecordMask & (1<<deviceNumber)) &&
			(deviceName == deviceNames[deviceNumber])  )
		{
			if( true == setSourceRecord )
				m_isRecordMask |= (1<<deviceNumber);
			else
				m_isRecordMask &= ~(1<<deviceNumber);
			ioctl( m_mixerDevice, SOUND_MIXER_WRITE_RECSRC, &m_isRecordMask );
			break;
		}
	}
}
