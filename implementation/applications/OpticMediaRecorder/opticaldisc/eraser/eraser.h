
// local headers
#include "common/commonui.h"

class Eraser : public CommonUi
{

	Q_OBJECT

public:
	Eraser( QWidget* parent = 0 );
	~Eraser();


protected slots:
	bool start();

};

