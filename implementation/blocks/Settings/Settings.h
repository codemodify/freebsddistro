#ifndef __Settings_H__
#define __Settings_H__


// Qt headers
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtXml/QDomDocument>



/****************************************************************************
*
*   This class implements a easy interface for an application
*   to store/read settings or any data that needs to survive
*   for the next time when the app is run.
*
*   Usage example:
*
*       Settings settings;
*       settings.setFile( "settings.xml" );
*
*
*       settings.setValueForKey( "showSplashScreen", "yes" );
*       settings.setValueForKey( "windows/main/width", "600" );
*       settings.setValueForKey( "windows/ouptuconsole/width", "200" );
*       settings.setValuesForKey( "application/openedRecentFiles", valuesList );
*
*
*       QString keyVlaue;
*       settings.getValueForKey( "showSplashScreen", keyVlaue );
*
*       QStringList keyVlaues;
*       settings.getValuesForKey( "application/openedRecentFiles", keyVlaues );
*
*
*       settings.save();
*
****************************************************************************/
class Settings
{
    public:
        Settings();
        virtual ~Settings();

        static QString  getSettingsFileForApp();


    // allowing users to do this
    public:
        bool setFile( QString settingsFilePath, bool createIfNotExist=true );
        bool save();

        void addKey         ( QString keyName  );
        void removeKey      ( QString keyName  );

        void setValueForKey ( QString keyName, QString  keyValue, bool addKeyIfNotPresent=true );
        void getValueForKey ( QString keyName, QString& keyValue                               );

        void setValuesForKey( QString keyName, QStringList& valuesList, bool addKeyIfNotPresent=true   );
        void getValuesForKey( QString keyName, QStringList& valueList                                  );
        void getChildrenNamesForKey( QString keyName, QStringList& valueList                           );


    // some internal data to work with
    private:
        QDomDocument    _domDocument;
        QString         _settingsFilePath;

};

#endif
