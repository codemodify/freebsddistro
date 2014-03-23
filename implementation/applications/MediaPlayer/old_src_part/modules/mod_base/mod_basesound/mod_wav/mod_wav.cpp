#include <SDL.h>
#include <QtCore/QByteArray>
#include <Logger>
#include "mod_wav.h"

#define MODWAV "mod_wav"

ModWav::ModWav(QString fileToPlay):
	ModBaseSound(fileToPlay)
{
	Logger::getInstance()->addInfo( "Wav module is up and runing", MODWAV );
}

ModWav::~ModWav()
{
	Logger::getInstance()->addInfo( "Wav module is going down", MODWAV );
}

struct {
	SDL_AudioSpec spec;
	Uint8   *sound;			/* Pointer to wave data */
	Uint32   soundlen;		/* Length of wave data */
	int      soundpos;		/* Current play position */
} wave;

void fillerup(void *unused, Uint8 *stream, int len)
{
	Uint8 *waveptr;
	int    waveleft;

	/* Set up the pointers */
	waveptr = wave.sound + wave.soundpos;
	waveleft = wave.soundlen - wave.soundpos;

	SDL_MixAudio(stream, waveptr, len, SDL_MIX_MAXVOLUME);
	wave.soundpos += len;
	if( wave.soundpos >= wave.soundlen  )
		SDL_PauseAudio(1);
}

void ModWav::run()
{
	Logger::getInstance()->addInfo( "init the SDL library", MODWAV );
	if ( SDL_Init(SDL_INIT_AUDIO) < 0 )
	{
		Logger::getInstance()->addInfo( "SDL was not init", MODWAV );
		return;
	}
	
	memset((void*)&wave, 0, sizeof(wave));

	Logger::getInstance()->addInfo( "loading file", MODWAV );
	if( 0 == SDL_LoadWAV( m_fileToPlay.toAscii().data(), &wave.spec, &wave.sound, &wave.soundlen ) )
	{
		Logger::getInstance()->addInfo( "file was not loaded", MODWAV );
		return;
	}
	wave.spec.callback = fillerup;
	
	Logger::getInstance()->addInfo( "opening audio device", MODWAV );
	if( -1 == SDL_OpenAudio( &wave.spec, 0 ) )
	{
		Logger::getInstance()->addInfo( "audio device was not opened", MODWAV );
		SDL_FreeWAV(wave.sound);
		return;
	}
	
	// start to play audio
	Logger::getInstance()->addInfo( "playing the audio file - " + m_fileToPlay, MODWAV );	
	SDL_PauseAudio( 0 );
	
	// waiting to finish playing
	while ( (false == m_haveToStop) && (SDL_GetAudioStatus() == SDL_AUDIO_PLAYING) )
		SDL_Delay(1000);
	
	// file finished playing, closing audio device
	Logger::getInstance()->addInfo( "file finished playing, closing audio device", MODWAV );
	SDL_CloseAudio();
	
	// freeing audio buffer
	SDL_FreeWAV( wave.sound );
	SDL_Quit();
}

void ModWav::setProgress( int percent )
{
	Logger::getInstance()->addInfo( "Setting the progress on media , requested by the user", MODWAV );
}


void ModWav::setVolume( int percent )
{
	Logger::getInstance()->addInfo( "Setting the volume on media , requested by the user", MODWAV );
}
