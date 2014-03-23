
// local headers
#include "common/commonui.h"

class Clonner : public CommonUi
{

	Q_OBJECT

public:
	Clonner( QWidget* parent = 0 );
	~Clonner();


protected slots:
	bool start();
	QByteArray readOutput();


private:
	int _mediaSize;
	
};
