#ifndef __SystemTrayIcon_H__
#define __SystemTrayIcon_H__

// Qt headers
#include <QtGui/QSystemTrayIcon>

// forwards
class QWidget;
class QMenu;

/****************************************************************************
*
*   This class adds some custom behaviour to the base sys tray class
*
****************************************************************************/
class SystemTrayIcon :  public QSystemTrayIcon
{
    Q_OBJECT
    
    public:
        SystemTrayIcon( QWidget* widget, QMenu* menu );
        virtual ~SystemTrayIcon();
    
    private slots:
        void sysTrayIconActivated( QSystemTrayIcon::ActivationReason activationReason );
    
    private:
        QWidget* _widget;
        QMenu*   _menu;
};

#endif
