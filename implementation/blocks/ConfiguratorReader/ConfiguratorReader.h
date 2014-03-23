#ifndef __ConfiguratorReader_H__
#define __ConfiguratorReader_H__

// Qt headers
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtGui/QFont>
#include <QtGui/QColor>

// forwards
class QDomNode;
class QDomDocument;


/***************************************************************************
*
*   "pathToObject" = interface/fonts/editorFont
*
***************************************************************************/
class ConfiguratorReader
{
    public:
        ConfiguratorReader( QString configurationFileName="" );
        ~ConfiguratorReader();
        
        static QString getAppConfigurationFile();
    
    // interface
    public:
        QFont       getFont             ( QString pathToObject );
        QColor      getColor            ( QString pathToObject );
        QString     getFileSystemPath   ( QString pathToObject );
        QString     getCheckedRadioBox  ( QString pathToObject );
        QStringList getCheckedCheckBoxes( QString pathToObject );
        QString     getLineInput        ( QString pathToObject );
        QStringList getPlugins          ( QString pathToObject );
    
    // helpers
    private:
        QDomNode getControlNodeFromPath( QString pathToObject );
    
    private:
        QDomDocument _configurationFile;

};

#endif

