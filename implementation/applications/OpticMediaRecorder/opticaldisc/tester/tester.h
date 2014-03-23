

// local headers
#include "common/commonui.h"

class Tester : public CommonUi
{

	Q_OBJECT

public:
	Tester( QWidget* parent = 0 );
	~Tester();


protected slots:
	bool start();
	QByteArray readOutput();


private:
	int _mediaSize;

};

