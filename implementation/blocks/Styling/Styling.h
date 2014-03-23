#ifndef __Styling_H__
#define __Styling_H__

// Qt headers
#include <QtGui/QSystemTrayIcon>

// forwards
class QWidget;
class QMenu;
class QIcon;

/****************************************************************************
*
*   This class
*
****************************************************************************/
class Styling
{
    public:
        Styling( QWidget& widget );
        virtual ~Styling();


    public: // widget related
        void setTitle();
        void setIcon();
        void saveOnScreenPosition();
        void loadOnScreenPosition();
        
        void saveFonts();
        void loadFonts();

        QSystemTrayIcon* installSystrayIcon( QIcon* icon, QMenu* menu );
        

    private: // the widget to work with
        QWidget&    _widget;
        QString     _className;

};

#endif
