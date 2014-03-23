#ifndef LAYOUT_H
#define LAYOUT_H

#include "../pageobject.h"

class Layout : public PageObject
{
public:
	Layout();
	~Layout();
	
public:
	QString toHtml();


};

#endif
