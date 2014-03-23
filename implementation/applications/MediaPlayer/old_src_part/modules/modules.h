#ifndef _MODULES_H_
#define _MODULES_H_

// base module
#include "mod_base/mod_base.h"

// sound modules
//#include "mod_base/mod_basesound/mod_mp3/mod_mp3.h"
#include "mod_base/mod_basesound/mod_wav/mod_wav.h"
#include "mod_base/mod_basesound/mod_ogg/mod_ogg.h"

// graphics modules
#include "mod_base/mod_basegraphics/mod_bmp/mod_bmp.h"

// stores the registered data types, and their names
namespace RegisteredModule
{
	typedef enum Type
	{
		WAV,
		BMP,
		MP3,
		OGG,
		UNKNOWN
	};
	
	static
	const char* Name[ UNKNOWN + 1 ] =
	{
		"WAV",
		"BMP",
		"MP3",
		"OGG",
		"UNKNOWN"
	};
}

#endif

//RegisteredModule::Type Mediaplayer::chooseMediaType( QString fileToPlay )
//{
//	// checking for corresponding media type
//	Logger::getInstance()->addInfo( "Checking for corresponding registered media type", MEDIAPLAYER );
//	
//	int type = RegisteredModule::WAV;
//	for( ; type < RegisteredModule::UNKNOWN; ++type )
//		if( fileToPlay == RegisteredModule::Name[type] )
//		{
//			Logger::getInstance()->addInfo( QString("Type was found - ") + RegisteredModule::Name[type], MEDIAPLAYER );
//			return (RegisteredModule::Type)type;
//		}
//	
//	Logger::getInstance()->addInfo( QString("The type was not found for - ") + fileToPlay, MEDIAPLAYER );
//	return RegisteredModule::UNKNOWN;
//}