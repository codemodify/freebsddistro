#include <Logger>
#include "mod_base.h"

#define MODBASE "mod_basesound"

ModBase::ModBase( QString fileToPlay ):
	m_fileToPlay(fileToPlay),
	m_haveToStop(false)
{
}

ModBase::~ModBase()
{
}

void ModBase::stopPlaying()
{
	// informing the thread it should stop playing
	Logger::getInstance()->addInfo( "stoping playing thread - ", MODBASE );
	m_mutex.lock();
	m_haveToStop = true;
	m_mutex.unlock();
}

bool ModBase::haveToStop()
{
	m_mutex.lock();
	bool result = m_haveToStop;
	m_mutex.unlock();
	
	return result;
}

void ModBase::setProgress( int percent )
{}
	
void ModBase::setVolume( int percent )
{}
