#include "mod_ogg.h"

#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>

#include <SDL.h>

#include <Logger>

#define MOD_OGG "mod_ogg"

// file handle
FILE* file;

// the OGG file definition
OggVorbis_File oggFile;


ModOgg::ModOgg( QString fileToPlay ):
	ModBaseSound(fileToPlay)
{
	Logger::getInstance()->addInfo( "Ogg module is up", MOD_OGG );
}

ModOgg::~ModOgg()
{
	Logger::getInstance()->addInfo( "Ogg module is going down", MOD_OGG );
}

void oggInitAndReadInfo( bool& stereo, long& freq )
{

	// stores information about the OGG file like freq, channels etc
	vorbis_info* oggInfo;
	
	// filling the file definition structure
	ov_open( file, &oggFile, NULL, 0);
	
	// getting the technical info about the file
	oggInfo = ov_info( &oggFile, -1 );
	
	// getting the channels mode
	stereo = ( oggInfo->channels != 1 );
	
	// getting the frequency
	freq = oggInfo->rate;
}

// reads a chunk of data from the OGG file into a buffer
// after this operation the buffer is ready to play on audio-device format
// returns the number of read bytes, 0 if there is no more to read
#define BYTES_TO_READ 4096
long oggReadBuffer( char* buffer )
{
	
	buffer = (char*)malloc( sizeof(char) * BYTES_TO_READ );
	
	long readBytes;
	
	int bitStream;
	
	readBytes = ov_read( &oggFile, buffer, BYTES_TO_READ, 0, 2, 1, &bitStream );
}

void oggFinish()
{
	ov_clear( &oggFile );
}

void sdlFillBuffer( void *unused, Uint8 *stream, int len )
{
	len = (int)oggReadBuffer( (char*)stream );
	
	// if we reached the end of the file, or some error, ther stop playing
	if( 0 > len  )
		SDL_PauseAudio( 1 );
}

void ModOgg::run()
{
	// init the SDL library
	Logger::getInstance()->addInfo( "init the SDL library", MOD_OGG );
	if ( SDL_Init(SDL_INIT_AUDIO) < 0 )
	{
		Logger::getInstance()->addInfo( "SDL was not init", MOD_OGG );
		return;
	}
	
	file = fopen( m_fileToPlay.toAscii().data(), "rb" );
	
	// reading the OGG technical information
	long freq;
	bool stereo;
	oggInitAndReadInfo( stereo, freq );
	
	// setting the required audio-hardware specifications
	SDL_AudioSpec specifications;
	specifications.freq = freq;
	specifications.format = AUDIO_S16MSB;
	specifications.channels = stereo ? 2 : 1;
	//specifications.silence = 
	specifications.samples = 8192;
	//specifications.size = 
	specifications.callback = sdlFillBuffer;
	specifications.userdata = 0;
	
	Logger::getInstance()->addInfo( "opening audio device", MOD_OGG );
	if( -1 == SDL_OpenAudio( &specifications, 0 ) )
		Logger::getInstance()->addInfo( "audio device was not opened", MOD_OGG );
	else
	{
		// start to play audio
		Logger::getInstance()->addInfo( "playing the audio file - " + m_fileToPlay, MOD_OGG );	
		SDL_PauseAudio( 0 );	
	
		// waiting to finish playing, or for user's request to stop
		while ( (false == haveToStop()) && (SDL_GetAudioStatus() == SDL_AUDIO_PLAYING) )
			SDL_Delay(1000);
			
		// stop playing
		SDL_PauseAudio( 1 );
	}
	
	// finishing with the OGG library
	oggFinish();
	
	// file finished playing, closing audio device
	Logger::getInstance()->addInfo( "file finished playing, closing audio device", MOD_OGG );
	SDL_CloseAudio();
	
	// deinit the SDL
	SDL_Quit();
}

