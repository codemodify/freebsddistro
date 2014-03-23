
// Qt headers
#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>
#include <QtCore/QProcess>

// blocks
#include <Settings/Settings.h>

// local headers
#include "FileLauncher.h"


void FileLauncher::launchFile( QString file )
{
    QStringList handlerApps;

    Settings    settings;
                settings.setFile( QCoreApplication::applicationDirPath() + "/FileLauncher.settings" );
    
    // if this is a folder then look for a file-manager
    if( QFileInfo(file).isDir() )
    {
        QString fileManager;
        settings.getValueForKey( "fileManager", fileManager );

        handlerApps = fileManager.split( ";" );
    }
    
    // if this is a file then look for a registered app to open it in
    else
    {
        QString registeredApp;
        settings.getValueForKey( QFileInfo(file).suffix(), registeredApp );
        
        handlerApps = registeredApp.split( ";" );
    }

    // run the registered app with the file as parameter
    QString registeredApp = handlerApps.value( 0 );
    if( handlerApps.size() > 1 )
    {
        qDebug( "FIXME: FileLauncher::launchFile()" );
        qDebug( "FIXME: promt a dialog to let user choose with what app to run the file" );
    }
    
    QProcess::startDetached( registeredApp, QStringList()<<file );
}

void FileLauncher::printFile( QString /*file*/, bool /*recursive*/ )
{
    qDebug( "FIXME: FileLauncher::printFile()" );
}

void FileLauncher::exploreFile( QString /*file*/ )
{
    qDebug( "FIXME: FileLauncher::exploreFile()" );
}

