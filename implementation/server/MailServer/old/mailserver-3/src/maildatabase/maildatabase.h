#ifndef _MAIL_DATABASE_H_
#define _MAIL_DATABASE_H_

class QString;
class QByteArray;
class QStringList;

class MailDatabase 
{

public:

	MailDatabase( QString& mailStoragePath );
	~MailDatabase();


public:
	typedef QString MailId;
	typedef QStringList MailIdList;


public:
	void addNewMail( QString& domain, QString& account, QByteArray& mailContent );
	void deleteMail( QString& domain, QString& account, MailId& mailId );
	QByteArray getMail( QString& domain, QString& account, MailId& mailId );

	void getInfoAboutAllMails( QString& domain, QString& account, MailIdList& mailIdList );
	quint64 getInfoAboutMail( QString& domain, QString& account, MailId& mailId );


private: // paths related
	QString _mailStoragePath;
public:
	QString getPathForMail( QString domain, QString account );
	QString getPathForTrash( QString domain, QString account );
	
private: // mail-id related
	MailId generateMailId( QString& domain, QString& account );

};

#endif
