#ifndef _MOD_BMP_H_
#define _MOD_BMP_H_

#include "../mod_basegraphics.h"

class ModBmp : public ModBaseGraphics
{
public:
	ModBmp(QString fileToPlay);
	~ModBmp();
	
	// reimplementation of QThread::run()
	void run();
};

#endif
