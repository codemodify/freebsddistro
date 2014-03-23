
// Qt's headers
#include <QtGui/QWidget>
#include <QtCore/QPointer>

class Panel : public QWidget
{

	Q_OBJECT

public:
	Panel( QWidget* parent=0 );
	~Panel();



private:
	QPointer<QHBoxLayout> _horizontaLayout;
	
};

