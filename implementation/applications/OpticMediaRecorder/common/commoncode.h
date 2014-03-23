#ifndef COMMON_CODE_H
#define COMMON_CODE_H

// Qt's headers
#include <QtGui/QWidget>
#include <QtCore/QProcess>

class CommonCode : public QWidget
{

	Q_OBJECT

public:
	CommonCode( QWidget* parent=0 );
	~CommonCode();



protected slots:
	virtual QByteArray readOutput();
	virtual void processStarted();
	virtual void processFinished();



protected:
	void closeEvent( QCloseEvent* closeEvent );



protected:
	QProcess _process;

};

#endif
