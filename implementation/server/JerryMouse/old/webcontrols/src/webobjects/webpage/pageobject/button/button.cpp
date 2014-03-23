
#include "button.h"

#include "../_xhtml_elements_/button/button.h"
#include "../_xhtml_elements_/text/text.h"
#include "../_xhtml_elements_/img/img.h"

Button::Button() :
	PageObject()
{}

Button::~Button()
{}

QString Button::toHtml()
{
	XHtmlElements::Text text;
	text.setText( _text );
	
	XHtmlElements::Img image;
	image.setSrc( _imagePath );
	
	XHtmlElements::ButtonImage buttonImage;
	buttonImage.setContent( image.toHtml() + text.toHtml() );

	return buttonImage.toHtml();
}

void Button::setText( QString text ){ _text = text; }
void Button::setImage( QString imagePath ){ _imagePath = imagePath; }