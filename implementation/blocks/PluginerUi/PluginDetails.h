#ifndef __PluginDetails_H__
#define __PluginDetails_H__

// Qt headers
#include <QtGui/QDialog>

class IPlugin;


class PluginDetails : public QDialog
{
    public:
        PluginDetails( IPlugin& plugin );

    protected:
        void closeEvent( QCloseEvent* event );

};

#endif
