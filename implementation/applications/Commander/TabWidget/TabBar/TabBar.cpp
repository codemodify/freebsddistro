
// Qt headers
#include <QtGui/QWheelEvent>


// local headers
#include "TabBar.h"


TabBar::TabBar() :
    QTabBar(),
    _trackMouseMove( false ),
    _mouseX( 0 )
{
    setUsesScrollButtons( true );
}

TabBar::~TabBar()
{}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//  interface
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void TabBar::addTab( QString label, QWidget* widget )
{
    _widgets.append( widget );

    addTab( label );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//  walk trough the tabs
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void TabBar::wheelEvent( QWheelEvent* event )
{
    // if there is one or no tabs then no actions happens
    if( count() <= 1 )
        return;

    // walk trough the tabs
    int index = currentIndex();

    if( event->delta() > 0 )
        index = ( index == count()-1 ) ? index : index + 1;
    else
        index = ( index == 0         ) ? index : index - 1;

    setCurrent( index);

    // mark that we did handle the event
    event->accept();
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//  close a tab
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void TabBar::mouseDoubleClickEvent( QMouseEvent* event )
{
    // if there is one or no tabs then no actions happens
    if( count() <= 1 )
        return;

    int index = currentIndex();

    removeTab( index );

    QWidget* widget = _widgets.takeAt( index );
    delete widget;

    // mark that we did handle the event
    event->accept();
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//  switch tabs with places
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void TabBar::mouseMoveEvent( QMouseEvent* event )
{
    if( count() <= 1 )
        return;

    int index = currentIndex();
    int destintionIndex = 0;

    // move to righ
    if( _mouseX > event->x() )
        destintionIndex = ( index == count()-1 ) ? index : index + 1;

    // move to left
    else
        destintionIndex = ( index == 0         ) ? index : index - 1;

    if( index != destintionIndex )
    {
        QString label = tabText( index );

        removeTab( index );
        insertTab( destintionIndex, label );

        _widgets.move( index, destintionIndex );
    }

    _mouseX = event->x();

    // mark that we did handle the event
    event->accept();
}

void TabBar::mousePressEvent( QMouseEvent* event )
{
    _trackMouseMove = true;
    _mouseX = event->x();

    // mark that we did handle the event
    event->accept();
}

void TabBar::mouseReleaseEvent( QMouseEvent* event )
{
    _trackMouseMove = false;

    // mark that we did handle the event
    event->accept();
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//  popup a context menu
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void TabBar::contextMenuEvent( QContextMenuEvent* event )
{
    QMenu*  contextMenu = new QMenu();
            contextMenu->addAction( "close", this, SIGNAL(closeCurrentTab()) );

    emit contextMenuRequested( contextMenu, event->globalPos() );
}

void TabBar::closeCurrentTab()
{
    QMouseEvent* event = new QMouseEvent();

    mouseDoubleClickEvent( event );

    delete event;
}
