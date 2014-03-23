
// Qt headers
#include <QtCore/QProcess>
#include <QtGui/QKeyEvent>
#include <QtGui/QX11EmbedContainer>
#include <QtGui/QColor>
#include <QtGui/QTabBar>

// blocks
#include <IconProvider/ActionIconProvider/ActionIconProvider.h>
#include <Styling/Styling.h>

// local headers
#include "Console.h"
#include "EnhancedDockWidget/EnhancedDockWidget.h"


Console::Console() :
    QTabWidget()
{}

Console::~Console()
{}

void Console::initEnvironement()
{
    setContextMenuPolicy( Qt::CustomContextMenu );
    setTabPosition( QTabWidget::South );

    // init the context menu
    QAction*            action              = 0;
    ActionIconProvider actionIconProvider;

    action = _contextMenu.addAction( "tab",         this, SLOT(newTab())        ); action->setIcon( actionIconProvider.getIconForAction("add")       );
    action = _contextMenu.addAction( "tab",         this, SLOT(closeTab())      ); action->setIcon( actionIconProvider.getIconForAction("remove")    );
    action = _contextMenu.addAction( "next",        this, SLOT(nextTab())       ); action->setIcon( actionIconProvider.getIconForAction("forward")   );
    action = _contextMenu.addAction( "prev",        this, SLOT(previousTab())   ); action->setIcon( actionIconProvider.getIconForAction("backward")  );
    action = _contextMenu.addAction( "fullscreen",  this, SLOT(fullScreen())    ); action->setIcon( actionIconProvider.getIconForAction("fullscreen"));

    connect(    this, SIGNAL(customContextMenuRequested(const QPoint&)),
                this, SLOT(rightClick(const QPoint&))                   );

    newTab();

    Styling styling( *this );
            styling.loadOnScreenPosition();
}

void Console::newTab()
{
    QX11EmbedContainer* xtermWidget = new QX11EmbedContainer();

    EnhancedDockWidget* dockWidget = new EnhancedDockWidget();
                        dockWidget->setWidget( xtermWidget );

    int index = addTab( dockWidget, "xterm" );
    tabBar()->setTabTextColor( index, QColor::fromHsv(rand() % 256,255,190) );

    setCurrentIndex( index );

    const QString c_xtermCommand = QString( "xterm -fn 9x15 -fg green -bg black -into %1" ).arg( xtermWidget->winId() );
    QProcess*   xterm = new QProcess();
                xterm->start( c_xtermCommand );

    // handling the close actions
    connect( xtermWidget,   SIGNAL(clientClosed()),         this, SLOT(closeTab())  );
    connect( dockWidget,    SIGNAL(destroyed(QObject*)),    xterm, SLOT(kill())     );
    connect( dockWidget,    SIGNAL(destroyed(QObject*)),    this, SLOT(closeTab())  );
}

void Console::rightClick( const QPoint& point )
{
    _contextMenu.exec( mapToGlobal(point) );
}

void Console::closeTab()
{
    removeTab( currentIndex() );
}

void Console::fullScreen()
{
    if( isFullScreen() )
        showNormal();
    else
        showFullScreen();
}

void Console::nextTab()
{
    int index = currentIndex();

    if( ++index >= count() )
        index = 0;

    setCurrentIndex( index );
}

void Console::previousTab()
{
    int index = currentIndex();

    if( --index < 0 )
        index = count() - 1;

    setCurrentIndex( index );
}

void Console::closeEvent( QCloseEvent* event )
{
    Styling styling( *this );
        styling.saveOnScreenPosition();

    QTabWidget::closeEvent( event );
}
