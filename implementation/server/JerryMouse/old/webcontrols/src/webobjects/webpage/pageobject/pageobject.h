#ifndef pageobject_h
#define pageobject_h

#include <QtCore/QString> 
#include <QtCore/QList>

/*****************************************************
*
*	Defines an object that can be on a web-page 	
*
*****************************************************/
class PageObject
{
public:
	PageObject( bool destroyChildren=false );
	virtual ~PageObject();
	
	virtual QString toHtml() = 0;

protected: // child items
	bool _destroyChildren;
	QList<PageObject*> _children;
	QString getChildrenAsHtml();
public:
	void addObject( PageObject* htmlObject );

};

#endif
