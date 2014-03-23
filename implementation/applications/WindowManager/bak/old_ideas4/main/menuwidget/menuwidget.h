#ifndef MENU_WIDGET_H
#define MENU_WIDGET_H

#include <QtGui/QWidget>

class wmMenuWidget : public QWidget
{

        Q_OBJECT

public:
        wmMenuWidget( QWidget *parent=0, const char *name=0 );

signals:
        void switchVD();
   
public slots:
        void run();
        void restart();
        void page1();
        void page2();
        void page3();
        void page4();
        void kwmhelp();
        void about();
        void addMenuWin(Client *); 
        void removeMenuWin(int);
        void nabWindow();
        void transpMove();
        void opaqueMove();
        void changeTitlebar();
        void vdRequest(int);
   
private:
        QMenuBar *menu;
        QPopupMenu *view;
        QPopupMenu *windows;
        TitleOptions *ot;
};

#endif
