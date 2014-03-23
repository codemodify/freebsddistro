
#include "layout.h"

Layout::Layout():
	PageObject()
{}

Layout::~Layout()
{}

QString Layout::toHtml()
{
	return getContainedObjectsAsHTML();
}
