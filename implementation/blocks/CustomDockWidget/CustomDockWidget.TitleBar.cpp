
// Qt headers
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>

// local headers
#include "CustomDockWidget.TitleBar.h"


CustomDockWidgetTitleBar::CustomDockWidgetTitleBar() :
    QWidget()
{
    _text = new QLabel();

    _additionalsLayout = new QHBoxLayout();
    _additionalsLayout->setSpacing( 0 );
    _additionalsLayout->setContentsMargins( 0, 0, 0, 0 );

    _thisLayout2 = new QHBoxLayout();
    _thisLayout2->setSpacing( 0 );
    _thisLayout2->setContentsMargins( 0, 0, 0, 0 );
    setTitleLyoutButtonsAtRight();

    _thisLayout = new QHBoxLayout( this );
    _thisLayout->setSpacing( 0 );
    _thisLayout->setContentsMargins( 0, 0, 0, 0 );
    _thisLayout->addLayout( _thisLayout2 );
}

void CustomDockWidgetTitleBar::setTitleLyoutButtonsAtLeft()
{
    while( 0 != _thisLayout2->takeAt(0) )
        ;

    _thisLayout2->addLayout( _additionalsLayout );
    _thisLayout2->addStretch();
    _thisLayout2->addWidget( _text );
}

void CustomDockWidgetTitleBar::setTitleLyoutButtonsAtRight()
{
    while( 0 != _thisLayout2->takeAt(0) )
        ;

    _thisLayout2->addWidget( _text );
    _thisLayout2->addStretch();
    _thisLayout2->addLayout( _additionalsLayout );
}

void CustomDockWidgetTitleBar::setTitleText( QString text )
{
    _text->setText( text );
}

void CustomDockWidgetTitleBar::setTitleCloseButton( QAction* action, QObject* slotOwner, const char* slot )
{
    _thisLayout->addWidget( createButton(action,slotOwner,slot) );
}

void CustomDockWidgetTitleBar::addTitleButton( QAction* action, QObject* slotOwner, const char* slot )
{
    _additionalsLayout->addWidget( createButton(action,slotOwner,slot) );
}

void CustomDockWidgetTitleBar::addTitleWidget( QWidget* widget )
{
    _additionalsLayout->addWidget( widget );
}

void CustomDockWidgetTitleBar::addTitleSeparator( QIcon icon )
{
    QLabel* label = new QLabel();
            label->setPixmap( icon.pixmap(icon.availableSizes().value(0)) );

    _additionalsLayout->addWidget( label );
}

QWidget* CustomDockWidgetTitleBar::createButton( QAction* action, QObject* slotOwner, const char* slot )
{
    QToolButton*    toolButton = new QToolButton();
                    toolButton->setAutoRaise( true );
                    toolButton->setDefaultAction( action );

    connect( toolButton, SIGNAL(released()), slotOwner, slot );

    return toolButton;
}

