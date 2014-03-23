
// local headers
#include "common/commonui.h"

class Burner : public CommonUi
{

	Q_OBJECT

public:
	Burner( QWidget* parent = 0 );
	~Burner();

	void setImageType( QString imageType );


protected slots:
	bool start();
	void choosePath();
	QByteArray readOutput();


private:
	QString _imageType;

};

