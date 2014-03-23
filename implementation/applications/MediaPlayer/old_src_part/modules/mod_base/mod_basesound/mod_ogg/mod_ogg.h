#ifndef _MOD_GG_H_
#define _MOD_GG_H_

#include "../mod_basesound.h"

class ModOgg : public ModBaseSound
{
public:
	ModOgg( QString fileToPlay );
	~ModOgg();
	void run();
};

#endif
