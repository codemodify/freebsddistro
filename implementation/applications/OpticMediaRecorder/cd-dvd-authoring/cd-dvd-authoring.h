
// Qt's headers
#include <QtGui/QWidget>

// local headers
#include "ui_cd-dvd-authoring.h"

class CDDVDAuthoring : public QWidget, public Ui::CDDVDAuthoring
{

	Q_OBJECT

public:
	CDDVDAuthoring( QWidget* parent = 0 );
	~CDDVDAuthoring();

	void initEnvironement();



private slots:
	void dvdBurnImage();
	void dvdCreateImage();

	void cdBurnImage();
	void cdCreateImage();

	void opticaldiscClone();
	void opticaldiscTest();
	void opticaldiscErase();

	void imageConvert();
	void imageGenerateHash();
};

