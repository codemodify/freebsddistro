#ifndef __Console_H__
#define __Console_H__

// Qt headers
#include <QtGui/QTabWidget>
#include <QtGui/QMenu>


class Console : public QTabWidget
{
    Q_OBJECT

    public:
        Console();
        ~Console();
        void initEnvironement();

    private slots:
        void newTab();
        void closeTab();
        void fullScreen();
        void nextTab();
        void previousTab();
        void rightClick( const QPoint& point );

    protected:
        void closeEvent( QCloseEvent* event );

    private:
        QMenu _contextMenu;

};

#endif
