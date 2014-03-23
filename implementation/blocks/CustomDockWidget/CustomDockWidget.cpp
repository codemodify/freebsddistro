
// Qt headers
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QCloseEvent>

// local headers
#include "CustomDockWidget.h"
#include "CustomDockWidget.TitleBar.h"


CustomDockWidget::CustomDockWidget( QWidget* parent ) :
    QDockWidget( parent )
{
    setAllowedAreas ( Qt::AllDockWidgetAreas                );
    setFeatures     ( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    setFocusPolicy  ( Qt::StrongFocus                       );

    _titleBar = new CustomDockWidgetTitleBar();
    QDockWidget::setTitleBarWidget( _titleBar );
}

void CustomDockWidget::setTitleLyoutButtonsAtLeft()
{
    _titleBar->setTitleLyoutButtonsAtLeft();
}

void CustomDockWidget::setTitleLyoutButtonsAtRight()
{
    _titleBar->setTitleLyoutButtonsAtRight();
}

void CustomDockWidget::setTitleStyleSheet( QString titleBarStyleSheet )
{
    _titleBar->setStyleSheet( titleBarStyleSheet );
}

void CustomDockWidget::setTitleText( QString text )
{
    _titleBar->setTitleText( text );
}

void CustomDockWidget::setTitleCloseButton( QAction* action, QObject* slotOwner, const char* slot )
{
    _titleBar->setTitleCloseButton( action, slotOwner, slot );
}

void CustomDockWidget::addTitleButton( QAction* action, QObject* slotOwner, const char* slot )
{
    _titleBar->addTitleButton( action, slotOwner, slot );
}

void CustomDockWidget::addTitleWidget( QWidget* widget )
{
    _titleBar->addTitleWidget( widget );
}

void CustomDockWidget::addTitleSeparator( QIcon icon )
{
    _titleBar->addTitleSeparator( icon );
}

void CustomDockWidget::setContentWidget( QWidget* contentWidget )
{
    QDockWidget::setWidget( contentWidget );
}

void        CustomDockWidget::setTitleBarWidget ( QWidget* widget ) {           QDockWidget::setTitleBarWidget( widget );   }
QWidget*    CustomDockWidget::titleBarWidget    () const            { return    QDockWidget::titleBarWidget();              }
void        CustomDockWidget::setWidget         ( QWidget* widget ) {           QDockWidget::setWidget( widget );           }
QWidget*    CustomDockWidget::widget            () const            { return    QDockWidget::widget();                      }

void CustomDockWidget::closeEvent( QCloseEvent* closeEvent )
{
    hide();

    closeEvent->accept();
}

