#ifndef _MOD_BASESOUND_H_
#define _MOD_BASESOUND_H_

#include "../mod_base.h"

class ModBaseSound  : public ModBase
{
public:
	// constructor
	ModBaseSound(QString fileToPlay);
	
	// destructor
	virtual ~ModBaseSound();
};

#endif
