#include "mod_bmp.h"
#include <SDL.h>
#include <QtCore/QByteArray>
#include <Logger>

#define MODBMP "mod_bmp"

ModBmp::ModBmp(QString fileToPlay):
	ModBaseGraphics(fileToPlay)
{
}

ModBmp::~ModBmp()
{
}

void ModBmp::run()
{
	SDL_Surface *screen;
	
	/* Initialize the SDL library */
    if( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        Logger::getInstance()->addInfo(QString("Couldn't initialize SDL - ") + SDL_GetError(), MODBMP );
		return;
    }
	
    /* Have a preference for 8-bit, but accept any depth */
    screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE|SDL_ANYFORMAT);
    if ( screen == NULL )
    {
        Logger::getInstance()->addInfo(QString("Couldn't set 640x480x8 video mode - ") + SDL_GetError(), MODBMP );
        SDL_Quit();
        return;
    }	
	
	SDL_Surface *image;
	   
	/* Load the BMP file into a surface */
    image = SDL_LoadBMP( m_fileToPlay.toAscii().data() );
    if (image == NULL)
    {
        Logger::getInstance()->addInfo("Couldn't load - " + m_fileToPlay, MODBMP );
        SDL_Quit();
        return;
    }

    /*
     * Palettized screen modes will have a default palette (a standard
     * 8*8*4 colour cube), but if the image is palettized as well we can
     * use that palette for a nicer colour matching
     */
    if (image->format->palette && screen->format->palette)
    {
    	SDL_SetColors( screen, image->format->palette->colors, 0, image->format->palette->ncolors );
    }

    /* Blit onto the screen surface */
    if( SDL_BlitSurface(image, NULL, screen, NULL) < 0 )
    	Logger::getInstance()->addInfo(QString("BlitSurface error: - ") + SDL_GetError(), MODBMP );

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);

    /* Free the allocated BMP surface */
    SDL_FreeSurface(image);	
    
    SDL_Quit();
}
