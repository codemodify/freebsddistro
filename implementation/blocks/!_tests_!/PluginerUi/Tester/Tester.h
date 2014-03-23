#ifndef __Tester_H__
#define __Tester_H__

// Qt headers
#include <QtGui/QWidget>

// blocks
#include <Pluginer/PluginManager.h>
#include <PluginerUi/PluginerUi.h>


class Tester : public QWidget
{
    Q_OBJECT

    public:
        Tester();
        ~Tester();

    private slots:
        void slot_showPluginManagerUi();
        void slot_showQWidgetDerivedObjectFromTheLoadedPlugin();

    private:
        PluginManager*  _pluginManager;
        PluginerUi*     _pluginerUi;
};

#endif
