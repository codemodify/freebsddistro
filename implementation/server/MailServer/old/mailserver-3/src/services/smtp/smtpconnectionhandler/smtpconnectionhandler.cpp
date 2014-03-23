
// Qt headers
#include <QtNetwork/QTcpSocket>
#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QUuid>
#include <QtCore/QFile>


// local headers
#include "smtpconnectionhandler.h"
#include "services/mailserverconfigvariables.h"
#include "receivemail/receivemail.h"
#include "sendmail/sendmail.h"

SmtpConnectionHandler::SmtpConnectionHandler( int socketDescriptor ):
	QThread(),
	_socketDescriptor( socketDescriptor )
{
	connect( this, SIGNAL(finished()), this, SLOT(deleteLater()) );
	start();
}

SmtpConnectionHandler::~SmtpConnectionHandler()
{
}


// mail delivery functionality
void deliverToMailbox( QString& from, QString& replyAddress, QString& destination, QByteArray& mailContent, ByteArrayList& attachments );

void SmtpConnectionHandler::run()
{
	QTcpSocket tcpSocket; 
	tcpSocket.setSocketDescriptor( _socketDescriptor );

	QString from;
	QString replyAddress;
	QStringList destinations;
	QByteArray mailContent;
	ByteArrayList attachments;

	bool result = receiveMail( tcpSocket, from, replyAddress, destinations, mailContent, attachments );
	tcpSocket.abort();
	if( false == result )
		return;


	// at this point we make some checks to ensure that all the parameters we have are ok

	result = checkMailAddress( from );
	if( false == result )
		return;

	result = checkMailAddress( replyAddress );
	if( false == result )
		return;

	foreach( QString destination, destinations )
	{
		result = checkMailAddress( destination );
		if( false == result )
			return;
	}

	result = scanForVirus( mailContent );
	if( false == result )
		return;

	result = scanForSpam( mailContent );
	if( false == result )
		return;

	for( int attachmentIndex=0; attachmentIndex < attachments.size(); ++attachmentIndex )
	{
		result = scanForVirus( attachments[attachmentIndex] );
		if( false == result )
			return;
	}

	// if we are still here at this moment, this means that this mail is a safe one, and we can deliver it to a local mailbox or to a corresponding mail servers
	foreach( QString destination, destinations )
	{
		if( true == isMailAddressOnLocalServer(destination) )
			deliverToMailbox( from, replyAddress, destination, mailContent, attachments );
		else
			sendMail( from, replyAddress, destination, mailContent, attachments );
	}
}

bool SmtpConnectionHandler::checkMailAddress( QString& mailAddress )
{
	return true;
}

bool SmtpConnectionHandler::scanForVirus( QByteArray& content )
{
	return true;
}

bool SmtpConnectionHandler::scanForSpam( QByteArray& content )
{
	return true;
}

bool SmtpConnectionHandler::isMailAddressOnLocalServer( QString mailAddress )
{
	int atPos = mailAddress.indexOf( "@" );

	QString domain = mailAddress.mid( atPos+1 );

	MailServers::const_iterator it = mailServers.constBegin();
	while( it != mailServers.constEnd() )
	{
		QString localDomain = it.key();

		if( domain == localDomain )
			return true;

		++it;
	}

	return false;
}

void deliverToMailbox( QString& from, QString& replyAddress, QString& destination, QByteArray& mailContent, ByteArrayList& attachments )
{
	// getting the domain
	QString domain = destination.mid( destination.indexOf("@")+1 );
	
	// getting the destination account
	QString account = destination;
	account.remove( domain );
	account.remove( "@" );
	
	mailDatabase->addNewMail( domain, account, mailContent );
}










