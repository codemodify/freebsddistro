#ifndef _MAIL_SERVER_H_
#define _MAIL_SERVER_H_

#include <QtCore/QCoreApplication>
#include <QtCore/QPointer>


class SmtpService;
class PopService;
class QStringList;

class MailServer : public QCoreApplication
{
	Q_OBJECT

public:
	MailServer( int argc, char** argv );
	~MailServer();

	bool init();

private: // config file related
	QString _configFilePath;
	bool loadConfigFile();
	bool prepareStorage();

private: // started serices
	QPointer<SmtpService> _smtpService;
	QPointer<PopService> _popService;

};

#endif
