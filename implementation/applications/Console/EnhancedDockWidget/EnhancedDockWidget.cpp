
#include "EnhancedDockWidget.h"

EnhancedDockWidget::EnhancedDockWidget( QWidget* parent ):
    QDockWidget( parent )
{
    setFeatures( QDockWidget::DockWidgetClosable );
}

void EnhancedDockWidget::closeEvent( QCloseEvent* event )
{
    deleteLater();
}
