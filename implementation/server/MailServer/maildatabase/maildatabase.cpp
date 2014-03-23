
// Qt headers
#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QVariant>

// local headers
#include "maildatabase.h"

#define pathForDomain       ( DOMAIN)           ( mailStorage + "/" + DOMAIN )
#define trashPathForAccount ( DOMAIN, ACCOUNT ) ( pathForDomain(DOMAIN) + "/" + ACCOUNT + "/trash" )
#define mailPathForAccount  ( DOMAIN, ACCOUNT ) ( pathForDomain(DOMAIN) + "/" + ACCOUNT + "/mail"  )


MailDatabase::MailDatabase( QString& mailStoragePath )
{
	_mailStoragePath = mailStoragePath;
}

MailDatabase::~MailDatabase()
{}

QString MailDatabase::getPathForMail( QString domain, QString account )
{
	return QString("%1/%2/%3/mail").arg(_mailStoragePath).arg(domain).arg(account);	
}

QString MailDatabase::getPathForTrash( QString domain, QString account )
{
	return QString("%1/%2/%3/trash").arg(_mailStoragePath).arg(domain).arg(account);
}

MailDatabase::MailId MailDatabase::generateMailId( QString& domain, QString& account )
{
	MailIdList mailIdList;
	getInfoAboutAllMails( domain, account, mailIdList );
	
	char* mailIdFormat =  "0000000000";
	quint64 maxMailIndex = 4000000000 ;
	
	QString valueAsString = mailIdList.isEmpty() ? mailIdFormat : mailIdList.at(mailIdList.size()-1);
	QVariant variant( valueAsString );
	
	quint64 mailIndex = variant.toULongLong();
	if( mailIndex > maxMailIndex )
		mailIndex = 0;

	mailIndex++;
	
	QString mailId = QString("%1").arg(mailIndex);
	for( int i=(10-mailId.length()); i>0; i-- )
		mailId.insert( 0, "0" );

	return mailId;
}

void MailDatabase::addNewMail( QString& domain, QString& account, QByteArray& mailContent )
{
	QString mailFilePath = getPathForMail( domain, account );
	QString mailFile = QString("%1/%2").arg(mailFilePath).arg(generateMailId(domain,account));

	QFile file( mailFile );
	if( false == file.open(QIODevice::WriteOnly) )
		return;
		
	file.write( mailContent );
	file.close();
}

void MailDatabase::deleteMail( QString& domain, QString& account, MailId& mailID )
{
	QString mailFilePath = getPathForMail( domain, account );
	QString trashFilePath = getPathForMail( domain, account );
	
	QString mailFile = QString("%1/%2").arg(mailFilePath).arg(mailID);
	QString trashFile = QString("%1/%2").arg(trashFilePath).arg(mailID);
	
	QFile::copy( mailFile, trashFile );
	QFile( mailFile ).remove();
}

QByteArray MailDatabase::getMail( QString& domain, QString& account, MailId& mailID )
{
	QString mailFilePath = getPathForMail( domain, account );
	QString mailFile = QString("%1/%2").arg(mailFilePath).arg(mailID);

	QFile file( mailFile );

	if( false == file.open(QFile::ReadOnly) )
		return QByteArray();

	QByteArray mailContent = file.readAll();
	
	return mailContent;
}

void MailDatabase::getInfoAboutAllMails( QString& domain, QString& account, MailIdList& mailIdList )
{
	QString mailFilePath = getPathForMail( domain, account );
	
	mailIdList = QDir(mailFilePath).entryList( QDir::Files | QDir::NoDotAndDotDot );
}

quint64 MailDatabase::getInfoAboutMail( QString& domain, QString& account, MailId& mailId )
{
	QString mailFilePath = getPathForMail( domain, account );
	QString mailFile = QString("%1/%2").arg(mailFilePath).arg(mailId);

	return QFileInfo( mailFile ).size();
}
