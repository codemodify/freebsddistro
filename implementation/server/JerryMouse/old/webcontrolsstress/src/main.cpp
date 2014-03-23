
#include "webobjects/webpage/webpage.h"
#include "webobjects/webpage/pageobject/label/label.h"

int main( int argc, char** argv )
{
	WebPage webPage;
	webPage.setTitle( "Web Pages in C++" );
	
	// add a simple text
	Label label;
	label.setText( "This is a simple Label" );
	label.setBold( true );
	label.setUnderlined( true );
	webPage.addObject( &label );
	
	QString webPageAsHtml = webPage.toHtml();
	qDebug( webPageAsHtml.toAscii().data() );
	
	return 0;
}
