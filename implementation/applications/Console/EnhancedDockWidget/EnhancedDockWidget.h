
#include <QtGui/QDockWidget>

class EnhancedDockWidget : public QDockWidget
{
    public:
        EnhancedDockWidget( QWidget* parent=0 );

    private:
        void closeEvent( QCloseEvent* event );

};

