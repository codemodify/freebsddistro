
#include "label.h"

#include "../htmlelements/text/text.h"
#include "../htmlelements/font/font.h"
#include "../htmlelements/b/b.h"
#include "../htmlelements/i/i.h"
#include "../htmlelements/u/u.h"
#include "../htmlelements/del/del.h"

Label::Label() :
	PageObject()
{
	_text = "text";
	_bold = false;
	_italic = false;
	_underlined = false;
	_crossed = false;
	_size = 10;
	_font = "Curier";
	_color = "Black";
}

Label::~Label()
{}

QString Label::toHtml()
{
	QString asHtml;
	
	HtmlElements::Text text;
	text.setText( _text );
	asHtml = text.toHtml();
	
	// setting the font
	if( _size || !_font.isEmpty() || !_color.isEmpty() )
	{
		HtmlElements::Font font;
		font.setColor( _color );
		font.setFace( _font );
		font.setSize( QString("%1").arg(_size) );
		font.setContent( asHtml );
		
		asHtml = font.toHtml();
	}
	
	// setting the bold
	if( _bold )
	{
		HtmlElements::B b;
		b.setContent( asHtml );
		
		asHtml = b.toHtml();
	}
	
	// setting the italic
	if( _italic )
	{
		HtmlElements::I i;
		i.setContent( asHtml );
		
		asHtml = i.toHtml();
	}

	// setting the underline
	if( _underlined )
	{
		HtmlElements::U u;
		u.setContent( asHtml );
		
		asHtml = u.toHtml();
	}
	
	// setting the underline
	if( _crossed )
	{
		HtmlElements::Del del;
		del.setContent( asHtml );
		
		asHtml = del.toHtml();
	}
	
	return asHtml;	
}

void Label::setText( QString text ){ _text = text; }
void Label::setBold( bool bold ){ _bold = bold; }
void Label::setItalic( bool italic ){ _italic = italic; }
void Label::setUnderlined( bool underlined ){ _underlined = underlined; }
void Label::setCrossed( bool crossed ){ _crossed = crossed; }
void Label::setSize( int size ){ _size = size; }
void Label::setFont( QString font ){ _font = font; }
void Label::setColor( QString color ){ _color = color; }
