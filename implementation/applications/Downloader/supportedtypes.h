#ifndef SUPPORTED_TYPES_H
#define SUPPORTED_TYPES_H


#define ONE_KILOBYTE 1024
#define ONE_MEGABYTE (1024*ONE_KILOBYTE)

// some common defines for an item in download
#define DOWNLOADING		"downloading"
#define USER_ABORT		"user abort"
#define ERROR			"error"
#define ERROR_HANDLE 	"error on getting handle"
#define ERROR_FILE		"error on writing to file"
#define FINISHED		"finished"

// definitions for the supported types
namespace SupportedTypes
{ 
	enum ID
		{ TORRENT =0, HTTP, FTP, UNSUPPORTED };

	static char* Name[UNSUPPORTED + 1]=
		{ "TORRENT", "HTTP", "FTP", "UNSUPPORTED" };
};
static SupportedTypes::ID getType( QString uri )
{
	QString str = uri.toUpper();
	for( int type = SupportedTypes::TORRENT; type < SupportedTypes::UNSUPPORTED; ++type )
	{
		QString typeValue = SupportedTypes::Name[type];
		if( (true == str.startsWith(typeValue)) || (true == str.endsWith(typeValue)) )
			return static_cast<SupportedTypes::ID>( type );
	}
	
	return SupportedTypes::UNSUPPORTED;
}


// definitions for the HTTP/FTP support
namespace HttpFtpColumns
{
	enum ID
		{ NAME = 0, STATUS, PROGRESS, SIZE, BYTESREAD, DOWNLOAD_SPEED, TIMEREMAIN, URL, LAST };
	
	static char* Name[LAST + 1]=
		{ "Name", "Status", "Progress",	"Size",	"Bytes Read", "Download Speed",	"Remain Time", "URL", "LAST" };
};


// definitions for the Torrents support
namespace TorrentColumns
{
	enum ID
		{ NAME = 0, STATUS, PROGRESS, TOTAL_SIZE, READ_SIZE, DOWNLOAD_SPEED, UPLOAD_SPEED, UPLOADED_SIZE, TORRENT_FILE, LAST };
	
	static char* Name[LAST + 1]=
		{ "Name", "Status", "Progress",	"Total Size", "Read Size", "Download Speed", "Upload Speed", "Uploaded Size", "Torrent File", "LAST" };
};

#endif
