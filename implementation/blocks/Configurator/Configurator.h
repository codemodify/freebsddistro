#ifndef __Configurator_H__
#define __Configurator_H__

// Qt headers
#include <QtGui/QDialog>
#include <QtXml/QDomDocument>

// forwards
class QString;
class QListWidgetItem;
class QDomNode;
class QListWidget;
class QFrame;
class QToolBox;
class QPushButton;
class PluginManager;

class Configurator : public QDialog
{
    Q_OBJECT
    
    // constructors
    public:
        Configurator( QStringList parameter );
        Configurator( QStringList parameter, PluginManager* pluginManager );
        ~Configurator();

    private:
        void constructorHelper();
    

    // interface
    public:
        static QString getAppConfigurationFile();
               QString getActiveSectionItem();
    

    protected:
        void closeEvent( QCloseEvent* event );
    
    signals:
        void configurationChanged();
    
    // init stuff
    private:
        void initUi             ();
        void initUiFromSettings ();
        bool loadSettingsFile   ();
        bool loadSection        ( QDomNode section								);
        bool loadSectionItem    ( QDomNode sectionItem, QListWidget& listWidget	);
    
    private slots:
        void saveSettings();
    
    // handlers for controls
    private slots:
        void currentChanged     ( int index );
        void currentItemChanged ( QListWidgetItem* current, QListWidgetItem* previous );
        void loadItem           ( QListWidgetItem* item );
        void saveItem           ( QListWidgetItem* item );
    
        void fontChoose ();
        void colorChoose();
        void pathChoose ();
    
        QWidget* createFontChooser  ( QString controlName, QString controlText, QString controlValue );
        QWidget* createColorChooser ( QString controlName, QString controlText, QString controlValue );
        QWidget* createPathChooser  ( QString controlName, QString controlText, QString controlValue );
        QWidget* createRadioBoxes   ( QString controlName, QString controlText, QString controlValue, QDomNode radioNode );
        QWidget* createCheckBoxes   ( QString controlName, QString controlText, QString controlValue, QDomNode cehckBoxNode );
        QWidget* createTextViewer   ( QString controlName, QDomNode control );
        QWidget* createLineInput    ( QString controlName, QString controlText, QString controlValue );
        QWidget* createPlugins      ( QString controlName, QString controlValue );
    
        void saveFontChooser    ( QString controlName, QDomNamedNodeMap controlAttributes   );
        void saveColorChooser   ( QString controlName, QDomNamedNodeMap controlAttributes   );
        void savePathChooser    ( QString controlName, QDomNamedNodeMap controlAttributes   );
        void saveRadioBoxes     ( QString controlName, QDomNode	radioNode                   );
        void saveCheckBoxes     ( QString controlName, QDomNode radioNode                   );
        void saveLineInput      ( QString controlName, QDomNamedNodeMap controlAttributes   );
        void savePlugins        ( QString controlName, QDomNamedNodeMap controlAttributes   );

    // internal stuff
    private:
        QToolBox*           _sections               ;
        QFrame*             _configOptions          ;
        QPushButton*        _save                   ;
        QPushButton*        _close                  ;
    
        QDomDocument        _domDocument            ;
        QListWidgetItem*    _currentWigetItem       ;
        QString             _configurationFileName  ;
        QString             _startSectionName       ;
        QString             _startSectionItemName   ;

        PluginManager*      _pluginManager;
        bool                _localPluginManager;
};

#endif
