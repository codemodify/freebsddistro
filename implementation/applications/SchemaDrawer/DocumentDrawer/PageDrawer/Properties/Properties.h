#ifndef __Properties_H__
#define __Properties_H__

// Qt headers
#include <QtCore/QObject>
#include <QtGui/QGraphicsRectItem>
#include <QtGui/QIcon>
#include <QtGui/QMenu>

// forwards
class Shape;
class QToolButton;
class QColorDialog;
class QGroupBox;


class Properties : public QObject, public QGraphicsRectItem
{
	Q_OBJECT

	public:
		Properties();

		void 	setTarget( Shape* target );
		Shape*	target();


	// overrides
	public:
		void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

	protected:
		void contextMenuEvent ( QGraphicsSceneContextMenuEvent* event );

		void mousePressEvent  ( QGraphicsSceneMouseEvent* mouseEvent );
		void mouseMoveEvent   ( QGraphicsSceneMouseEvent* mouseEvent );
		void mouseReleaseEvent( QGraphicsSceneMouseEvent* mouseEvent );


	// internal handlers
	private slots:
		void duplicateTarget();
		void bringTargetToFront();
		void sendTargetToBack();
		void deleteTarget();

		void currentColorChanged( const QColor& color );
		void changeTextColor();
		void changeLineColor();
		void changeFillColor();
		void colorDialogFinished( int resultCode );

	private:
		void showProterties();


	private:
		QMenu	_menu;

		Shape*	_target;
		QRectF	_targetRect;

		QIcon	_infoIcon;
		QRect	_infoIconRect;

		QIcon	_resizeIcon;
		QRect	_resizeIconRect;
		bool		_haveToResize;

		// stores shape's properties
		QGroupBox*	_groupBoxColor;
		QColorDialog*	_colorDialog;

		int			_itemToSet;
		QToolButton*	_textColorButton;
		QToolButton*	_lineColorButton;
		QToolButton*	_fillColorButton;

		QColor		_currentColor;
		QColor		_textColor;
		QColor		_lineColor;
		QColor		_fillColor;
};

#endif
