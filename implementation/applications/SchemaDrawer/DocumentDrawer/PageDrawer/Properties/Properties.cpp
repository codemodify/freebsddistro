
// Qt headers
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QColorDialog>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGraphicsScene>
#include <QtGui/QToolButton>
#include <QtGui/QLabel>

// blocks
#include <IconProvider/ActionIconProvider/ActionIconProvider.h>

// local headers
#include "Properties.h"
#include "DocumentDrawer/PageDrawer/Shape/Shape.h"


Properties::Properties() :
	QObject(),
	QGraphicsRectItem(),
	_target( 0 ),
	_haveToResize( false )
{
	setHandlesChildEvents( true );
	setFlag( QGraphicsItem::ItemIsSelectable, true );

	_menu.addAction( ActionIconProvider().getIconForAction("duplicate"), "Duplicate"     , this, SLOT(duplicateTarget())    );
	_menu.addSeparator();
	_menu.addAction( ActionIconProvider().getIconForAction("forward")  , "Bring On Front", this, SLOT(bringTargetToFront()) );
	_menu.addAction( ActionIconProvider().getIconForAction("backward") , "Send To Back"  , this, SLOT(sendTargetToBack())   );
	_menu.addSeparator();
	_menu.addAction( ActionIconProvider().getIconForAction("delete")   , "Delete"        , this, SLOT(deleteTarget()) );


	_infoIcon   = ActionIconProvider().getIconForAction( "info"   );
	_resizeIcon = ActionIconProvider().getIconForAction( "resize" );
}

void Properties::setTarget( Shape* target )
{
	_target = target;

	if( 0 == _target )
	{
		hide();
		return;
	}

	show();

	QPointF point = _target->scenePos();
	setPos( point );

	_targetRect = _target->getGeometry();
	_targetRect.setX     ( _targetRect.x()      - 3 );
	_targetRect.setY     ( _targetRect.y()      - 3 );
	_targetRect.setWidth ( _targetRect.width()  + 3 );
	_targetRect.setHeight( _targetRect.height() + 3 );

	const int infoIconSize = 20;
	_infoIconRect.setX     ( _targetRect.topRight().x() - infoIconSize );
	_infoIconRect.setY     ( _targetRect.topRight().y() + 5            );
	_infoIconRect.setWidth ( infoIconSize               - 5            );
	_infoIconRect.setHeight( infoIconSize               - 5            );

	const int resizeIconSize = 20;
	_resizeIconRect.setX     ( _targetRect.bottomRight().x() - resizeIconSize );
	_resizeIconRect.setY     ( _targetRect.bottomRight().y() - resizeIconSize );
	_resizeIconRect.setWidth ( resizeIconSize                - 2              );
	_resizeIconRect.setHeight( resizeIconSize                - 2              );

	setRect( _targetRect );
}

Shape* Properties::target()
{
	return _target;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- overrides
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Properties::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	if( 0 == _target )
		return;

	QPen	pen;
		pen.setColor ( "black"      );
		pen.setWidthF( 1.5          );
		pen.setStyle ( Qt::DashLine );

	painter->setPen  ( pen                      );
	painter->drawRect( _targetRect              );
	
	_infoIcon.paint  ( painter, _infoIconRect   );
	_resizeIcon.paint( painter, _resizeIconRect );
}

void Properties::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
	_menu.exec( event->screenPos() );
}

void Properties::mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
	if( _resizeIconRect.contains(mouseEvent->pos().toPoint()) )
		_haveToResize = true;

	QGraphicsRectItem::mousePressEvent( mouseEvent );
}

void Properties::mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
	if( Qt::LeftButton == mouseEvent->buttons() )
	{
		if( _haveToResize )
		{
			// make sure we don't resize in minus (switch the coordinate systems)
			if(	( mouseEvent->scenePos().x() > mapToScene(_target->getGeometry().topLeft()).x() ) && 
				( mouseEvent->scenePos().y() > mapToScene(_target->getGeometry().topLeft()).y() ) )
			{
				qreal w = mouseEvent->pos().x() - mouseEvent->lastPos().x();
				qreal h = mouseEvent->pos().y() - mouseEvent->lastPos().y();

				_target->resize( w, h );

				setTarget( _target );
			}
		}
		else
		{
			moveBy(	mouseEvent->scenePos().x()-mouseEvent->lastScenePos().x(), 
					mouseEvent->scenePos().y()-mouseEvent->lastScenePos().y() );

			_target->moveBy(	mouseEvent->scenePos().x()-mouseEvent->lastScenePos().x(),
							mouseEvent->scenePos().y()-mouseEvent->lastScenePos().y() );
		}
	}

	QGraphicsRectItem::mouseMoveEvent( mouseEvent );
}

void Properties::mouseReleaseEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
	if( _infoIconRect.contains(mouseEvent->pos().toPoint()) )
		showProterties();

	_haveToResize = false;

	QGraphicsRectItem::mouseReleaseEvent( mouseEvent );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- internal handlers
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void Properties::currentColorChanged( const QColor& color )
{
	_currentColor = color;
}

void Properties::changeTextColor()
{
	_itemToSet = 0;

	_groupBoxColor->hide();

	_colorDialog->setCurrentColor( _textColor );
	_colorDialog->show();
}

void Properties::changeLineColor()
{
	_itemToSet = 1;

	_groupBoxColor->hide();

	_colorDialog->setCurrentColor( _fillColor );
	_colorDialog->show();
}

void Properties::changeFillColor()
{
	_itemToSet = 2;

	_groupBoxColor->hide();

	_colorDialog->setCurrentColor( _lineColor );
	_colorDialog->show();
}

void Properties::colorDialogFinished( int resultCode )
{
	if( QDialog::Accepted == resultCode )
	{
		if( 0 == _itemToSet ){ _textColorButton->setPalette( _currentColor ); _textColor = _currentColor; }
		if( 1 == _itemToSet ){ _lineColorButton->setPalette( _currentColor ); _lineColor = _currentColor; }
		if( 2 == _itemToSet ){ _fillColorButton->setPalette( _currentColor ); _fillColor = _currentColor; }
	}

	_colorDialog->hide();
	_groupBoxColor->show();
}

void Properties::duplicateTarget()
{
	Shape* shape = _target->duplicate();

	_target->scene()->addItem( shape );
}

void Properties::bringTargetToFront()
{
	_target->setZValue( _target->scene()->items().count() );
}

void Properties::sendTargetToBack()
{
	_target->setZValue( 0 );
}

void Properties::deleteTarget()
{
	_target->scene()->removeItem( _target );

	setTarget( 0 );

	delete _target;
}

void Properties::showProterties()
{
	if( 0 == _target )
		return;

	// text
	QGroupBox* groupBoxText = new QGroupBox( "Text" );

	QLineEdit*	text = new QLineEdit();
				text->setText( _target->getText() );

	QVBoxLayout*	vBoxLayout1 = new QVBoxLayout( groupBoxText );
				vBoxLayout1->addWidget( text );

	// color
	_groupBoxColor = new QGroupBox( "Color" );

	_textColorButton = new QToolButton();
	_textColorButton->setPalette( _target->getTextColor() );
	_textColor = _target->getTextColor();
	connect( _textColorButton, SIGNAL(released()), this, SLOT(changeTextColor()) );

	_lineColorButton = new QToolButton();
	_lineColorButton->setPalette( _target->getLineColor() );
	_lineColor = _target->getLineColor();
	connect( _lineColorButton, SIGNAL(released()), this, SLOT(changeLineColor()) );

	_fillColorButton = new QToolButton();
	_fillColorButton->setPalette( _target->getFillColor() );
	_fillColor = _target->getFillColor();
	connect( _fillColorButton, SIGNAL(released()), this, SLOT(changeFillColor()) );

	QGridLayout*	gridLayout = new QGridLayout( _groupBoxColor );
				gridLayout->addWidget( new QLabel("Text Color"), 0, 0 );
				gridLayout->addWidget( _textColorButton        , 0, 1 );

				gridLayout->addWidget( new QLabel("Line Color"), 1, 0 );
				gridLayout->addWidget( _lineColorButton        , 1, 1 );

				gridLayout->addWidget( new QLabel("Fill Color"), 2, 0 );
				gridLayout->addWidget( _fillColorButton        , 2, 1 );

	_colorDialog = new QColorDialog();
	_colorDialog->hide();
	connect( _colorDialog, SIGNAL(finished(int)),                      this, SLOT(colorDialogFinished(int))           );
	connect( _colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentColorChanged(const QColor&)) );
	
	// assemble
	QDialog dialog;

	QDialogButtonBox* dialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

	QObject::connect( dialogButtonBox, SIGNAL(accepted()), &dialog, SLOT(accept()) );
	QObject::connect( dialogButtonBox, SIGNAL(rejected()), &dialog, SLOT(reject()) );

	QVBoxLayout*	vBoxLayout = new QVBoxLayout( &dialog  );
				vBoxLayout->addWidget( groupBoxText    );
				vBoxLayout->addWidget( _groupBoxColor  );
				vBoxLayout->addWidget( _colorDialog   );
				vBoxLayout->addWidget( dialogButtonBox );

	if( QDialog::Accepted == dialog.exec() )
	{
		_target->setText ( text->text() );
		_target->setTextColor( _textColor );
		_target->setLineColor( _lineColor );
		_target->setFillColor( _fillColor );
	}
}
