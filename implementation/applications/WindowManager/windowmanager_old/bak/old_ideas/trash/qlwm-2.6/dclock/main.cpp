#include <qapplication.h>
#include "dclock.h"

int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	dclock *dc = new dclock;
	a.setMainWidget(dc);
	dc->show();
	return a.exec();
}
