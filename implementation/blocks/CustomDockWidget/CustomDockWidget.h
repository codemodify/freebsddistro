#ifndef __CustomDockWidget_H__
#define __CustomDockWidget_H__

// Qt headers
#include <QtGui/QDockWidget>
#include <QtGui/QIcon>

// forwards
class CustomDockWidgetTitleBar;


class CustomDockWidget : public QDockWidget
{
    public:
        CustomDockWidget( QWidget* parent=0 );

    public:
        void setTitleLyoutButtonsAtLeft ();
        void setTitleLyoutButtonsAtRight();
        void setTitleStyleSheet( QString titleBarStyleSheet );

        void setTitleText               ( QString text );
        void setTitleCloseButton        ( QAction* action, QObject* slotOwner, const char* slot );
        void addTitleButton             ( QAction* action, QObject* slotOwner, const char* slot );
        void addTitleWidget             ( QWidget* widget );
        void addTitleSeparator          ( QIcon icon = QIcon() );
        
        void setContentWidget           ( QWidget* contentWidget );

    
    private: // hide some iherited methods
        void        setTitleBarWidget   ( QWidget* widget );
        QWidget*    titleBarWidget      () const;
        void        setWidget           ( QWidget* widget );
        QWidget*    widget              () const;


	protected:
		void closeEvent( QCloseEvent* event );


    private:
        CustomDockWidgetTitleBar* _titleBar;

};

#endif

