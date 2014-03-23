#ifndef MAIN_H
#define MAIN_H

#include <QtGui/QApplication.h>


class MyApp : public QApplication
{

	Q_OBJECT


public:
	MyApp( int &argc, char **argv, const QString& rAppName );
	virtual bool x11EventFilter( XEvent * );


public slots:
	void quit();  // What's up with this?
	void saveOptions();
};

#endif
