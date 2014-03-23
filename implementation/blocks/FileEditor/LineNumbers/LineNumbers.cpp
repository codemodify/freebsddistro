
// Qt headers
#include <QtGui/QTextBlock>
#include <QtGui/QPaintEngine>
#include <QtGui/QPaintEvent>
#include <QtGui/QTextLayout>
#include <QtGui/QScrollBar>

// local headers
#include "LineNumbers.h"
#include "FileEditor/TextFileEditor/TextFileEditor.h"

LineNumbers::LineNumbers( TextFileEditor* textFileEditor ) :
	QWidget(),
	_textFileEditor( textFileEditor )
{
        setContentsMargins( 0, 0, 0, 0 );

	setAttribute( Qt::WA_StaticContents );
	//setAttribute( Qt::WA_NoBackground );

	setFixedWidth( 35 );

	connect( _textFileEditor->verticalScrollBar(),	SIGNAL(valueChanged(int)),	this, SLOT(update()) );
	connect( _textFileEditor,						SIGNAL(textChanged()),		this, SLOT(update()) );
}

void LineNumbers::paintEvent( QPaintEvent* event )
{
	QWidget::paintEvent( event );

	// getting the first text-block in the viewport
	QTextCursor textCursor	= _textFileEditor->cursorForPosition( QPoint(0,0) );
	QTextBlock  textBlock  = textCursor.block();

	QPainter	painter( this );
			painter.setPen( Qt::gray );
			painter.setFont( _textFileEditor->font() );
//			painter.eraseRect( rect() );

	QRect rect = _textFileEditor->cursorRect();
	int x = 0;
	int y = _textFileEditor->viewport()->pos().y();
	int w = width();
	int h = rect.height();

	while( textBlock.isValid() )
	{
		QRectF rect( x, y, w, h );

		painter.drawText
		(
			rect,
			Qt::AlignRight,
			QString::number( textBlock.blockNumber()+1 )
		);

		y += h;

		textBlock = textBlock.next();
	}
}

void LineNumbers::resizeEvent( QResizeEvent *e )
{
	QWidget::resizeEvent( e );
}
