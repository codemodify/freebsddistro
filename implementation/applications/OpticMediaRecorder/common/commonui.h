#ifndef COMMONUI_H
#define COMMONUI_H

// Qt's headers
#include <QtGui/QWidget>
#include <QtCore/QProcess>

// local headers
#include "ui_commonui.h"
#include "commoncode.h"

class CommonUi: public CommonCode, public Ui::CommonUi
{

	Q_OBJECT

public:
	CommonUi( QWidget* parent = 0 );
	virtual ~CommonUi();


protected slots:
	virtual void stop();
	virtual bool start();
	virtual void choosePath();


protected slots:
	QByteArray readOutput();
	void processStarted();
	void processFinished();



protected:
	void addLogMessage( QString message, QString colorName="black" );

};

#endif
