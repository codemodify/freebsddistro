#ifndef __TabBar_H__
#define __TabBar_H__

// Qt headers
#include <QtCore/QList>
#include <QtGui/QTabBar>
#include <QtGui/QMenu>


class TabBar : public QTabBar
{
    Q_OBJECT

    public:
        TabBar();
        ~TabBar();


    // interface
    public:
        void addTab( QString label, QWidget* widget );


    // mouse events
    protected:
        // walk trough the tabs
        void wheelEvent             ( QWheelEvent*          event );

        // close a tab
        void mouseDoubleClickEvent  ( QMouseEvent*          event );

        // switch tabs with places
        void mouseMoveEvent         ( QMouseEvent*          event );
        void mousePressEvent        ( QMouseEvent*          event );
        void mouseReleaseEvent      ( QMouseEvent*          event );


    // popup a context menu
    protected:
        void contextMenuEvent       ( QContextMenuEvent*    event );
    signals:
        void contextMenuRequested( QMenu* menu, const QPoint& position );
    private slots:
        void closeCurrentTab();


    private:
        QList<QWidget*> _widgets;

        bool    _trackMouseMove;
        int     _mouseX;

};

#endif
