#ifndef __Architector_H__
#define __Architector_H__

// Qt headers
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>

class PluginManager;


class Architector : public QMainWindow
{
    Q_OBJECT

    // default stuff
    public:
        Architector();
        ~Architector();

    // this takes care of the menu
    private:
        void setupMenu();
        
    private slots:
        void helpMe();
        void newArchitecture();
        void openArchitecture();
        
    protected:
        void closeEvent( QCloseEvent* event );

    // this takes care of the plugins
    private:
        void setupPlugins();

    private:
         PluginManager* _pluginManager;
        
        
    // helpers
    private:
        void openArchitectureByFilePath( QString filePath );
    
    private slots:
        void readConfiguration();

    // members
    private:
        QMdiArea* _mdiArea;

};

#endif

