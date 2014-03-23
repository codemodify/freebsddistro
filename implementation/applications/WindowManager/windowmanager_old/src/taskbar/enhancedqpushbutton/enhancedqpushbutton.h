#ifndef ENHANCED_QPUSHBUTTON_H
#define ENHANCED_QPUSHBUTTON_H

// Qt headers
#include <QtGui/QPushButton>

class EnhancedQPushButton : public QPushButton
{

public:
	EnhancedQPushButton();

	void* _dataPointer;
};

#endif
