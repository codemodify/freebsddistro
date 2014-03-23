#ifndef __PluginerUi_H__
#define __PluginerUi_H__

// Qt headers
#include <QtGui/QWidget>

// blocks headers
#include <Pluginer/PluginManager.h>

class QListWidget;
class QListWidgetItem;

class PluginerUi : public QWidget
{
    Q_OBJECT

    public:
        PluginerUi( PluginManager& pluginManager );
        
    private slots:
        void pluginDoubleClicked( QListWidgetItem* item );
        void pluginCheckedUnchecked( QListWidgetItem* item );

        void refreshPluginList();
    
    private:
        QString         _pluginDirectory;
        PluginManager&  _pluginManager;
        QListWidget*    _listWidget;
};

#endif
