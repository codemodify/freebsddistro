#ifndef __CustomDockWidgetTitleBar_H__
#define __CustomDockWidgetTitleBar_H__

// Qt headers
#include <QtGui/QWidget>

class QLabel;
class QHBoxLayout;
class QToolButton;


class CustomDockWidgetTitleBar : public QWidget
{
    public:
        CustomDockWidgetTitleBar();

        void setTitleLyoutButtonsAtLeft ();
        void setTitleLyoutButtonsAtRight();

        void setTitleText               ( QString text );
        void setTitleCloseButton        ( QAction* action, QObject* slotOwner, const char* slot );
        void addTitleButton             ( QAction* action, QObject* slotOwner, const char* slot );
        void addTitleWidget             ( QWidget* widget );
        void addTitleSeparator          ( QIcon icon );

    private:
        QWidget* createButton( QAction* action, QObject* slotOwner, const char* slot );

    private:
        QHBoxLayout*    _thisLayout;
        QHBoxLayout*    _thisLayout2;
        QLabel*         _text;
        QHBoxLayout*    _additionalsLayout;

};

#endif
