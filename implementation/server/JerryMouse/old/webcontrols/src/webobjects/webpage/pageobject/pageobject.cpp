
#include "pageobject.h"

PageObject::PageObject( bool destroyChildren ) :
	_destroyChildren( destroyChildren )
{}

PageObject::~PageObject()
{
	if( true == _destroyChildren )
		foreach( PageObject* pageObject, _children )
			delete pageObject;
	
	_children.clear();
}

QString PageObject::getChildrenAsHtml()
{
	QString asHtml;

	foreach( PageObject* pageObject, _children )
		asHtml += pageObject->toHtml();
	
	return asHtml;
}
