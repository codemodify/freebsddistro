#ifndef __FileLauncher_H__
#define __FileLauncher_H__


// Qt headers
#include <QtCore/QString>


/****************************************************************************
*
*   This class implements a easy interface for an application
*   to act on a file/folder.
*
*   Usage example:
*
*       FileLauncher::launchFile( "/Users/nicu/Movies/TheMovie.avi" );
*       FileLauncher::launchFile( "/Users/nicu/Movies" );
*
*       FileLauncher::printFile( "/Users/nicu/Documents/ThisYearReport.text" );
*       FileLauncher::printFile( "/Users/nicu/Documents", true );
*
*       FileLauncher::exploreFile( "/Users/nicu/Downloads/ReadMe.text" );
*       FileLauncher::exploreFile( "/Users/nicu/Downloads" );
*
*
****************************************************************************/
class FileLauncher
{
    public:

        // launches the file with a registered app, or opens the folder if 'file' is a folder
        static void launchFile( QString file );

        // prints the 'file' or prints the folder content if 'file' is a folder
        static void printFile( QString file, bool recursive=false );

        // opens file/folder in a file manager and selects it to be visible to the user
        static void exploreFile( QString file );

};

#endif
