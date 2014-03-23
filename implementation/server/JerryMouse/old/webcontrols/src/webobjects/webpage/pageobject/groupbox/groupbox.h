#ifndef GROUP_BOX_H
#define GROUP_BOX_H

#include "../pageobject.h"

class GroupBox : public PageObject
{
public:
	GroupBox();
	~GroupBox();


public:
	QString toHtml();

};

#endif
