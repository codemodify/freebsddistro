
// Qt's headers
#include <QtCore/QFile>

// mediaplayer's headers
#include "mod_mp3.h"

// common library headers
#include <Logger>

// header for 'libmad' library for MP3 decoding
#include <mad.h>

// header for 'libsdl' library for accessing the audio device
#include <SDL.h>

// used by Logger for identification
#define MODMP3 "mod_mp3"


ModMp3::ModMp3(QString fileToPlay):
	ModBaseSound(fileToPlay)
{
}

ModMp3::~ModMp3()
{
}

/**************************************************************************************************/
#define MADX_INPUT_BUFFER_SIZE	(5*1152*8)
#define MADX_OUTPUT_BUFFER_SIZE	 (1152*8)

enum madx_sig { 
	ERROR_OCCURED,
	MORE_INPUT,
	FLUSH_BUFFER,
	EOF_REACHED,
	CALL_AGAIN
};

struct madx_house {
	struct mad_stream 	stream;
	struct mad_frame 	frame;
	struct mad_synth 	synth;
	mad_timer_t 		timer;
	unsigned long		frame_cnt;
	unsigned char 		*output_ptr;
};

struct madx_stat {
	char 		msg[256];
	size_t 		write_size;
	int			is_eof;
	size_t		readsize;
	size_t		remaining;

	// Will reference some 
	// "middle part" of in_buffer:
	unsigned char 		*buffstart;		
};

#if (MAD_VERSION_MAJOR>=1) || \
    ((MAD_VERSION_MAJOR==0) && \
     (((MAD_VERSION_MINOR==14) && \
       (MAD_VERSION_PATCH>=2)) || \
      (MAD_VERSION_MINOR>14)))
#define MadErrorString(x) mad_stream_errorstr(x)
#else
static const char 
*MadErrorString(const struct mad_stream *Stream)
{
	switch(Stream->error)
	{
		// Generic unrecoverable errors. 
		case MAD_ERROR_BUFLEN:
			return("input buffer too small, or EOF");
		case MAD_ERROR_BUFPTR:
			return("invalid (null) buffer pointer");
		case MAD_ERROR_NOMEM:
			return("not enough memory");

		// Frame header related unrecoverable errors.
		case MAD_ERROR_LOSTSYNC:
			return("lost synchronization");
		case MAD_ERROR_BADLAYER:
			return("reserved header layer value");
		case MAD_ERROR_BADBITRATE:
			return("forbidden bitrate value");
		case MAD_ERROR_BADSAMPLERATE:
			return("reserved sample frequency value");
		case MAD_ERROR_BADEMPHASIS:
			return("reserved emphasis value");

		// Recoverable errors 
		case MAD_ERROR_BADCRC:
			return("CRC check failed");
		case MAD_ERROR_BADBITALLOC:
			return("forbidden bit allocation value");
		case MAD_ERROR_BADSCALEFACTOR:
			return("bad scalefactor index");
		case MAD_ERROR_BADFRAMELEN:
			return("bad frame length");
		case MAD_ERROR_BADBIGVALUES:
			return("bad big_values count");
		case MAD_ERROR_BADBLOCKTYPE:
			return("reserved block_type");
		case MAD_ERROR_BADSCFSI:
			return("bad scalefactor selection info");
		case MAD_ERROR_BADDATAPTR:
			return("bad main_data_begin pointer");
		case MAD_ERROR_BADPART3LEN:
			return("bad audio data length");
		case MAD_ERROR_BADHUFFTABLE:
			return("bad Huffman table select");
		case MAD_ERROR_BADHUFFDATA:
			return("Huffman data overrun");
		case MAD_ERROR_BADSTEREO:
			return("incompatible block_type for JS");

			
		// Unknown error. This switch may be out of 
		// sync with libmad's defined error codes.
		default:
			return("Unknown error code");
	}
}
#endif






// 
// Borrowed from audio.c in the madplay source.
// 32-bit pseudo-random number generator.
// 

static inline unsigned long 
psurand_gen(unsigned long state)
{
  return (state * 0x0019660dL + 0x3c6ef35fL) & 0xffffffffL;
}





// 
// scale_audio():
// 
// A scaling routine based on madplay's audio_linear_dither() 
// function. It performs dithering, which is the addition of 
// a random number to the least significant bits (LSB) of 
// the sample that targets the LSB at the 16 bit mark. 
// madx_read() uses this to perform its scaling before 
// output.
//

static inline signed short 
scale_audio( mad_fixed_t sample, mad_fixed_t *rndval_ptr )
{
	unsigned int scalebits;
	mad_fixed_t output, mask, rnd;

	// bias 
	output = sample + (1L << (MAD_F_FRACBITS + 1 - 16 - 1));	
	
  	scalebits = MAD_F_FRACBITS + 1 - 16;
  	mask = (1L << scalebits) - 1;

  	// dither 
	rnd = psurand_gen(*rndval_ptr);
  	output += (rnd & mask) - (*rndval_ptr & mask);
	*rndval_ptr = rnd;
	
  	// clip 
	if (output >= MAD_F_ONE)
    	output = MAD_F_ONE - 1;
	else if (output < -MAD_F_ONE)
    	output = -MAD_F_ONE;

  	// quantize 
  	output &= ~mask;

	// scale 
	return output >> scalebits;
}









//
// madx_init():
// 
// Initializes mxhouse members, open files and
// psuedo-files.
//   

int madx_init ( unsigned char *out_buffer, madx_house *mxhouse  )
{
	

	
	// Initialize libmad structures 
	mad_stream_init(&mxhouse->stream);
	mad_frame_init(&mxhouse->frame);
	mad_synth_init(&mxhouse->synth);
	mad_timer_reset(&mxhouse->timer);	


	// Assign pointer to output buffer. "output_ptr"
	// is used to determine when to flush the output 
	// buffer in madx_read().

	mxhouse->output_ptr = out_buffer;


	return(1);

	
}








// 
// madx_read():
// 
// Is called until EOF is reached or error 
// occurs. The calling code must inspect 
// the "madx_sig" return values to 
// determine what to do (e.g. call 
// madx_read() again, flush output, etc).
// 		

madx_sig madx_read (	unsigned char *in_buffer, 
	unsigned char *out_buffer, madx_house *mxhouse, madx_stat *mxstat)
{


	// "Random" seed for generating dither. Used 
	// by scale_audio() in preparing the samples 
	// for 16 bit output. 

	mad_fixed_t 	rnscale = 0xa8b9ff7e;

	unsigned char	*guard_ptr  = NULL;
	unsigned char	*readstart;

	// Clear mxstat->msg
	sprintf(mxstat->msg,"");
	


	mxstat->buffstart = 0;


	// Input file has been read completely
	if ( mxstat->is_eof && !mxstat->readsize )
	{

		sprintf(mxstat->msg,"End of input stream"); 
		return(EOF_REACHED); 
	}

	
	
	// Executes on initial loop

	if ( 	(mxstat->readsize == 0)  && 
			(mxstat->remaining == 0) &&
			mxhouse->stream.buffer == NULL 	)
	{

			mxstat->readsize = MADX_INPUT_BUFFER_SIZE;
			mxstat->remaining = 0;
			return(MORE_INPUT);
	}


	readstart = in_buffer;
	



	// Stream buffer

	if ( mxhouse->stream.buffer == NULL ||
			mxhouse->stream.error == MAD_ERROR_BUFLEN )
	{


		// BUFFER_GUARD
		// "mxstat->is_eof" must be set to
		// non-zero IMMEDIATELY (see "test.cpp")
		// once it is detected, so the following
		// may execute, padding the buffer.

		if( mxstat->is_eof )
		{
		
			guard_ptr = in_buffer + mxstat->readsize;
			memset( guard_ptr, 0, MAD_BUFFER_GUARD );
		
			mxstat->readsize += MAD_BUFFER_GUARD;
		}



		// Pipe the new buffer content to libmad's 
		// stream decoder facility.

		mad_stream_buffer( &mxhouse->stream, 
				in_buffer, mxstat->readsize + mxstat->remaining );
		
		mxhouse->stream.error = (mad_error)0;

	}




	


	// [Madlld comment]:
	// Decode the next MPEG frame. The streams is read from the
	// buffer, its constituents are broken down and stored in the 
	// mxhouse->frame structure, ready for examination/alteration 
	// or PCM synthesis. Decoding options are carried in the Frame
	// structure from the mxhouse->stream structure.
	// 
	// Error handling: mad_frame_decode() returns a non zero value
	// when an error occurs. The error condition can be checked in
	// the error member of the mxhouse->stream structure. A mad error is
	// recoverable or fatal, the error status is checked with the
	// MAD_RECOVERABLE macro.
	// 
	// {4} When a fatal error is encountered all decoding
	// activities shall be stopped, except when a MAD_ERROR_BUFLEN
	// is signaled. This condition means that the
	// mad_frame_decode() function needs more input to complete
	// its work. One shall refill the buffer and repeat the
	// mad_frame_decode() call. Some bytes may be left unused at
	// the end of the buffer if those bytes forms an incomplete
	// frame. Before refilling, the remaining bytes must be moved
	// to the beginning of the buffer and used for input for the
	// next mad_frame_decode() invocation. (See the comments
	// marked {2} earlier for more details.)
	// 
	// Recoverable errors are caused by malformed bit-streams, in
	// this case one can call again mad_frame_decode() in order to
	// skip the faulty part and re-sync to the next frame.


	
	if( mad_frame_decode(&mxhouse->frame, &mxhouse->stream) )
	{

		if( MAD_RECOVERABLE(mxhouse->stream.error) )
		{
			

			// Do not print a message if the error is 
			// a loss of synchronization or this loss 
			// is due to the end of stream guard bytes. 

			if( mxhouse->stream.error != MAD_ERROR_LOSTSYNC
				   	||
				mxhouse->stream.this_frame != guard_ptr 
					||
				// Suppress error if caused by ID3 tag.
			   (mxhouse->stream.this_frame[0] != 'I' &&		// ID3v2
				mxhouse->stream.this_frame[1] != 'D' &&
				mxhouse->stream.this_frame[2] != '3') 
			   		||					
			   (mxhouse->stream.this_frame[0] != 'T' &&		// ID3v1
				mxhouse->stream.this_frame[1] != 'A' &&
				mxhouse->stream.this_frame[2] != 'G') )
			{			
				sprintf(mxstat->msg,"Recoverable frame level error (%s)",
						MadErrorString(&mxhouse->stream));

			}	
			return(CALL_AGAIN); 


		}
		else		// Non-recoverable error
		{


			// MAD_ERROR_BUFLEN means that more input
			// is needed (it's not recoverable without
			// interaction from the calling code). The
			// bytes already in the buffer must be 
			// preserved. "mxstat->buffstart" is used
			// to point to the end of the preserved
			// bytes in "in_buffer", then MORE_INPUT 
			// is requested. "mxstat->buffstart" is set
			// to 0 every time "madx_read()" is 
			// called. The calling code checks this 
			// variable to determine if a full buffer 
			// read or a partial read is necessary.
			// (Not "0" signifies a partial read.)

			if( mxhouse->stream.error == MAD_ERROR_BUFLEN )
			{		
				
				
				sprintf(mxstat->msg,
					"Need more input (%s)",	MadErrorString(&mxhouse->stream));
				
				mxstat->remaining = 
					mxhouse->stream.bufend - mxhouse->stream.next_frame;
				
				memmove(in_buffer, 
						mxhouse->stream.next_frame, mxstat->remaining);
				
				mxstat->readsize = MADX_INPUT_BUFFER_SIZE - mxstat->remaining;


				// How far "forward" in_buffer to begin:
				mxstat->buffstart = in_buffer + mxstat->remaining;
			

				return(MORE_INPUT);			

			}
			else			// Error returned
			{
				sprintf(mxstat->msg,"Unrecoverable frame level error (%s).",
						MadErrorString(&mxhouse->stream));
				return(ERROR_OCCURED); 
			}
		}
	}







	// [Madlld comment]:
	// Accounting. The computed frame duration is in the frame
	// header structure. It is expressed as a fixed point number
	// whole data type is mad_timer_t. It is different from the
	// samples fixed point format and unlike it, it can't directly
	// be added or subtracted. The timer module provides several
	// functions to operate on such numbers. Be careful there, as
	// some functions of libmad's timer module receive some of
	// their mad_timer_t arguments by value!

	mxhouse->frame_cnt++;
	mad_timer_add( &mxhouse->timer, mxhouse->frame.header.duration );




	// Once decoded the frame is synthesized to 
	// PCM samples. No errors are reported by 
	// mad_synth_frame();

	mad_synth_frame( &mxhouse->synth, &mxhouse->frame );




	
	
	// [Madlld comment]:
	// Synthesized samples must be converted from libmad's fixed
	// point number to the consumer format. Here we use unsigned
	// 16 bit big endian integers on two channels. Integer samples
	// are temporarily stored in a buffer that is flushed when
	// full.
	//
	// Note: Code to computer "big-endian" has been preserved
	// below. It is simply commented out. This source produces
	// 16-bit "little-endian" PCM samples.

	
	for( int i=0; i < mxhouse->synth.pcm.length; i++ )
	{
		signed short	sample;

		// Left channel
		sample = scale_audio(mxhouse->synth.pcm.samples[0][i], &rnscale);

		
		// Originally big-endian:
		// *(mxhouse->output_ptr++)=sample>>8;      
		// *(mxhouse->output_ptr++)=sample&0xff;

		
		// 16-bit little-endian
		*(mxhouse->output_ptr++)=((sample>>0) & 0xff);
		*(mxhouse->output_ptr++)=((sample>>8) & 0xff);



		// Right channel. 
		// If the decoded stream is monophonic then 
		// the right output channel is the same as 
		// the left one.

		if(MAD_NCHANNELS(&mxhouse->frame.header)==2)
			sample = scale_audio(mxhouse->synth.pcm.samples[1][i], &rnscale);

		
		// Originally big-endian:
		// *(mxhouse->output_ptr++)=sample>>8;      
		// *(mxhouse->output_ptr++)=sample&0xff;


		// 16-bit little-endian
		*(mxhouse->output_ptr++)=((sample>>0) & 0xff);			
		*(mxhouse->output_ptr++)=((sample>>8) & 0xff);

	}
		

	// Tell calling code buffer size
	mxstat->write_size = mxhouse->output_ptr - out_buffer;


	// Indicate that buffer is ready to be
	// written.

	if (mxhouse->synth.pcm.length < 1152 
			||	
		MADX_OUTPUT_BUFFER_SIZE - mxstat->write_size  < 1152)
	{

		// Reset pointer
		mxhouse->output_ptr = out_buffer;  
		return(FLUSH_BUFFER);
	}
	else
		return(CALL_AGAIN); 		// Otherwise, continue


}




//
// madx_deinit():
//
// Close files, free memory allocated for
// structures.
// 
void madx_deinit (madx_house *mxhouse)
{

	// Mad is no longer used, the structures 
	// that were initialized must now be 
	// cleared.

	mad_synth_finish(&mxhouse->synth);
	mad_frame_finish(&mxhouse->frame);
	mad_stream_finish(&mxhouse->stream);

}

/**************************************************************************************************/
/********  SDL Chunk ******************************************************************************/
static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;
unsigned char 	in_buffer[ MADX_INPUT_BUFFER_SIZE + MAD_BUFFER_GUARD ];
unsigned char 	out_buffer[ MADX_OUTPUT_BUFFER_SIZE ];
FILE* in_file;
FILE* out_file;
size_t 			a;
size_t			in_size = MADX_INPUT_BUFFER_SIZE + MAD_BUFFER_GUARD;

// Structure necessary to use madxlib
madx_house		mxhouse;
madx_stat		mxstat;
madx_sig		mxsig; // Catch signals from madx_read()


void playBuffer( void *udata, Uint8 *stream, int len )
{
    // only play if we have data left
    if ( audio_len == 0 )
        qApp->exit(1);
        
		mxsig = madx_read( in_buffer, out_buffer, &mxhouse, &mxstat  );
		
		// if we got a critical error	
		if( ERROR_OCCURED == mxsig )
			qApp->exit(1);
			
		// fill buffer
		else if( MORE_INPUT == mxsig )
		{
			if( mxstat.buffstart )	// Fill partial buffer
			{
				a = fread(mxstat.buffstart, 1, mxstat.readsize, in_file);
				if( feof(in_file) )
				{
					mxstat.is_eof = 1;
					mxstat.readsize = a;
				}

			}
			else					// Read full buffer
			{
				fread(in_buffer, 1, mxstat.readsize, in_file);
				if( feof(out_file) )
				{
					mxstat.is_eof = 1;
					mxstat.readsize = a;
				}
			}
		}
		
		// output to file
		else if ( FLUSH_BUFFER == mxsig )
		{
			
			//if( mxstat.write_size == fwrite(out_buffer, 1, mxstat.write_size, out_file) )
			//	Logger::getInstance()->addInfo( "Writing buffer", MODMP3 );
			//else
			//	Logger::getInstance()->addError( "Writing buffer error", MODMP3 );
				// waiting to finish playing
			//obtained.size = mxstat.write_size;
			obtained.userdata = out_buffer;
			SDL_PauseAudio( 0 );
			while ( (false == m_haveToStop) && (SDL_GetAudioStatus() == SDL_AUDIO_PLAYING) )
				SDL_Delay(1000);
			
		}
		
		// input file EOF
		else if( EOF_REACHED == mxsig )
		{
			//a = fwrite(out_buffer,1,mxstat.write_size,out_file);
			//if( mxstat.write_size != a )
			//	Logger::getInstance()->addError( "Error with final write", MODMP3 );
			//else
			//	Logger::getInstance()->addInfo( "Finished.", MODMP3 );
			//obtained.size = mxstat.write_size;
			obtained.userdata = out_buffer;

		}


    // mix as much data as possible
    len = ( len > audio_len ? audio_len : len );
    SDL_MixAudio( stream, audio_pos, len, SDL_MIX_MAXVOLUME );
    audio_pos += len;
    audio_len -= len;
}

/**************************************************************************************************/

void ModMp3::run()
{
	SDL_AudioSpec desired, obtained, hardwareSpec;
	
	// setting the audio format
	desired.freq = 22050;
	desired.format = AUDIO_S16;
	desired.channels = 2;
	desired.samples = 1024;
	desired.callback = playBuffer;
	desired.userdata = NULL;
	
	SDL_Init(SDL_INIT_AUDIO);
	SDL_OpenAudio(&desired, &obtained); hardwareSpec = obtained;
	
	// Initialize madxlib
	madx_init(out_buffer, &mxhouse);
	
	// open the files
	in_file = fopen( m_fileToPlay.toAscii().data(), "rb");
	out_file = fopen("/test.pcm", "wb");

	SDL_PauseAudio( 0 );	
	while ( (false == m_haveToStop) && (SDL_GetAudioStatus() == SDL_AUDIO_PLAYING) )
		SDL_Delay(1000);
	
	// clean-up
	madx_deinit( &mxhouse );
	
	fclose( in_file );
	fclose( out_file );

SDL_CloseAudio();
SDL_Quit();
}
