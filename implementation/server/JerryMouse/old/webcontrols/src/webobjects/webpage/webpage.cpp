
#include <QtCore/QFile>

#include "webpage.h"

#include "pageobject/pageobject.h"
#include "pageobject/htmlelements/taghtml/taghtml.h"
#include "pageobject/htmlelements/taghead/taghead.h"
#include "pageobject/htmlelements/tagtitle/tagtitle.h"
#include "pageobject/htmlelements/tagbody/tagbody.h"
#include "pageobject/htmlelements/text/text.h"

WebPage::WebPage() :
	WebObject()
{}

WebPage::~WebPage()
{
	empty();
}

void WebPage::setTitle( QString title )
{
	_title = title;
}

void WebPage::setBackgroundColor( QString backgroundColor )
{
	_backgroundColor = backgroundColor;
}

void WebPage::setBackgroundImage( QString backgroundImage )
{
	_backgroundImage = backgroundImage;
}

void WebPage::setTextColor( QString textColor )
{
	_textColor = textColor;
}

void WebPage::setLinksColor( QString linksColor )
{
	_linksColor = linksColor;
}

void WebPage::setVisitedLinksColor( QString visitedLinksColor )
{
	_visitedLinksColor = visitedLinksColor;
}

void WebPage::setHoveredLinksColor( QString hoveredLinksColor )
{
	_hoveredLinksColor = hoveredLinksColor;
}

void WebPage::addObject( PageObject* pageObject )
{
	_pageObjects.append( pageObject );
}

void WebPage::empty()
{
	_pageObjects.clear();
}

QString WebPage::toHtml()
{
	HtmlElements::Text text;
	text.setText( _title );
	
	HtmlElements::TagTitle tagTitle;
	tagTitle.setContent( text.toHtml() );
	
	HtmlElements::TagHead tagHead;
	tagHead.setContent( tagTitle.toHtml() );
		
	// get the controls content
	QString objectsAsHtml;
	foreach( PageObject* pageObject, _pageObjects )
		objectsAsHtml += pageObject->toHtml();
	
	HtmlElements::TagBody tagBody;
	tagBody.setContent( objectsAsHtml );

	HtmlElements::TagHtml tagHtml;
	tagHtml.setContent( tagHead.toHtml() + tagBody.toHtml() );

	return tagHtml.toHtml();
}

void WebPage::saveToFile( QString fullFilePath )
{
	QFile file( fullFilePath );
	if( !file.open(QFile::ReadOnly) )
		return;
		
	file.write( toHtml().toAscii() );
	file.close();
}
