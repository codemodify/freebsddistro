#ifndef _MOD_BASEGRAPHIC_H_
#define _MOD_BASEGRAPHIC_H_

#include "../mod_base.h"

class ModBaseGraphics : public ModBase
{
public:
	// constructor
	ModBaseGraphics(QString fileToPlay);
	
	// destructor
	virtual ~ModBaseGraphics();
};

#endif
