#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QtCore/QList>
#include <QtCore/QString>

#include "../webobject.h"

class PageObject;

class WebPage : public WebObject
{
public:
	WebPage();
	~WebPage();

public:
	void setTitle( QString title );

	void addPageObject( PageObject* pageObject );
	void empty();
	
	QString toHtml();
	
	void saveToFile( QString fullFilePath );

private:
	QString _title;

private:	
	QList<PageObject*> _pageObjects;	
};

#endif
