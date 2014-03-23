#ifndef _MOD_WAV_H_
#define _MOD_WAV_H_

#include "../mod_basesound.h"

class ModWav : public ModBaseSound
{
public:
	// constructor
	ModWav(QString fileToPlay);
	
	// destructor
	~ModWav();
	
	// reimplementation of QThread::run()
	void run();
	
public slots:
	// sets the progress as a request by user
	void setProgress( int percent );
	
	// sets the volume as a request by user
	void setVolume( int percent );

};

#endif
