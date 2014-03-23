
// Qt headers
#include <QtCore/QMetaObject>
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QDesktopWidget>
#include <QtGui/QFont>

// blocks
#include <Settings/Settings.h>

// local headers
#include "Styling.h"
#include "SystemTrayIcon/SystemTrayIcon.h"


Styling::Styling( QWidget& widget ) :
    _widget( widget )
{
    _className = _widget.metaObject()->className();
}

Styling::~Styling()
{}

void Styling::setTitle()
{
    _widget.setWindowTitle( _className );
}

void Styling::setIcon()
{
    _widget.setWindowIcon( QIcon(QApplication::applicationFilePath() + ".png") );
}

void Styling::saveOnScreenPosition()
{
    Settings    settings;
                settings.setFile( Settings::getSettingsFileForApp() );
                settings.setValueForKey( _className + "/geometry/maximized",    QString().number(_widget.isMaximized()) );
                settings.setValueForKey( _className + "/geometry/x",            QString().number(_widget.x())           );
                settings.setValueForKey( _className + "/geometry/y",            QString().number(_widget.y())           );
                settings.setValueForKey( _className + "/geometry/width",        QString().number(_widget.width())       );
                settings.setValueForKey( _className + "/geometry/height",       QString().number(_widget.height())      );

                settings.save();
}

void Styling::loadOnScreenPosition()
{
    QString maximized, x, y, width, height;

    Settings    settings;
                settings.setFile( Settings::getSettingsFileForApp() );
                settings.getValueForKey( _className + "/geometry/maximized",    maximized );
                settings.getValueForKey( _className + "/geometry/x",            x       );
                settings.getValueForKey( _className + "/geometry/y",            y       );
                settings.getValueForKey( _className + "/geometry/width",        width   );
                settings.getValueForKey( _className + "/geometry/height",       height  );

    int maximizedAsInt  = maximized.toInt();
    int xAsInt          = x.toInt();
    int yAsInt          = y.toInt();
    int widthAsInt      = width.toInt();
    int heightAsInt     = height.toInt();

    QWidget* desktopWidget = QDesktopWidget().screen();

    xAsInt      = ( (xAsInt         < 0) || (xAsInt     > desktopWidget->width() )   ) ? 0   : xAsInt;
    yAsInt      = ( (yAsInt         < 0) || (yAsInt     > desktopWidget->height())   ) ? 0   : yAsInt;
    widthAsInt  = ( (widthAsInt    <= 0) || (widthAsInt > desktopWidget->width() )   ) ? 800 : widthAsInt;
    heightAsInt = ( (heightAsInt   <= 0) || (heightAsInt> desktopWidget->height())   ) ? 600 : heightAsInt;

    if( maximizedAsInt )
        _widget.showMaximized();
    else
        _widget.setGeometry( xAsInt, yAsInt, widthAsInt, heightAsInt );
}
        
void Styling::saveFonts()
{
    qDebug( "FIXME: Styling::saveFonts()" );
/*
    Settings    settings;
                settings.setFile( Settings::getSettingsFileForApp() );
                settings.setValueForKey( _className + "/geometry/x",      QString().number(_widget.x())         );
                settings.setValueForKey( _className + "/geometry/y",      QString().number(_widget.y())         );
                settings.setValueForKey( _className + "/geometry/width",  QString().number(_widget.width())     );
                settings.setValueForKey( _className + "/geometry/height", QString().number(_widget.height())    );

                settings.save();
*/
}

void Styling::loadFonts()
{
    QString family, size;

    Settings    settings;
                settings.setFile( Settings::getSettingsFileForApp() );
                settings.getValueForKey( _className + "/font/family",  family  );
                settings.getValueForKey( _className + "/font/size",    size    );

    int sizeAsInt = size.toInt();

    _widget.setFont( QFont(family,sizeAsInt) );
}

QSystemTrayIcon* Styling::installSystrayIcon( QIcon* icon, QMenu* menu )
{
    if( false == QSystemTrayIcon::isSystemTrayAvailable() )
        return new QSystemTrayIcon();

	SystemTrayIcon*	sysTray = new SystemTrayIcon( &_widget, menu );
					sysTray->setIcon( *icon );
					sysTray->show();

	return sysTray;
}
