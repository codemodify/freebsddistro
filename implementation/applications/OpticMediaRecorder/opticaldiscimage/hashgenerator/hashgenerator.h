
// local headers
#include "common/commonui.h"

class HashGenerator : public CommonUi
{

	Q_OBJECT

public:
	HashGenerator( QWidget* parent = 0 );
	~HashGenerator();


protected slots:
	bool start();
	void choosePath();

};

