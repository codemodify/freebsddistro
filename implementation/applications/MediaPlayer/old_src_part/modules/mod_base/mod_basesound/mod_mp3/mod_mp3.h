#ifndef _MOD_MP3_H_
#define _MOD_MP3_H_

#include "../mod_basesound.h"

class ModMp3 : public ModBaseSound
{
public:
	// constructor
	ModMp3(QString fileToPlay);
	
	// destructor
	~ModMp3();
	
	// reimplementation
	void run();
};

#endif
