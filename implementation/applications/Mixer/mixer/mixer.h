#ifndef MIXER_H_
#define MIXER_H_

// Qt's headers
#include <QtGui/QWidget>

// local headers
#include "ui_mixer.h"

class Mixer : public QWidget, public Ui::Mixer
{
	
	// using Qt's signals/slots framework
	Q_OBJECT
	
public:
	Mixer();
	~Mixer();
	bool initEnvironement();
	
private slots:
	void mixerDeviceChangedValue( int newValue );
	void mixerRecordSourceChanged( bool setSourceRecord );

private:
	int m_mixerDevice;
	int m_existMask;
	int m_canRecordMask;
	int m_isRecordMask;
	int m_stereoMask;
	
	int getMixerDeviceValue( int mixerDevice );

};

#endif
