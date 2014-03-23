
// Qt headers
#include <QtNetwork/QTcpSocket>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

// local headers
#include "popconnectionhandler.h"
#include "services/mailserverconfigvariables.h"

PopConnectionHandler::PopConnectionHandler( int socketDescription ):
	QThread(),
	_socketDescription( socketDescription )
{
	_tcpSocket = new QTcpSocket();
	connect( this, SIGNAL(finished()), this, SLOT(deleteLater()) );
	start();
}

PopConnectionHandler::~PopConnectionHandler()
{
	_tcpSocket->disconnectFromHost();
	_tcpSocket->close();
}

typedef enum { USER=0, PASS, TRANSACTION_STATE, STAT, LIST, RETR, DELE, NOOP, RSET, UIDL, TOP, QUIT, IGNORED_COMMAND, APOP, ERROR_COMMAND } PopCommand;
PopCommand getPopCommandFromString( QByteArray& command );

bool doesUserExist( QString domain, QString account, QString password );

void PopConnectionHandler::run()
{
	_tcpSocket->setSocketDescriptor( _socketDescription );
	
	const int TIME_OUT = 3000;
	
	#define SEND_OK()  _tcpSocket->write("+OK\r\n"); if(!_tcpSocket->waitForBytesWritten(TIME_OUT))return;
	#define SEND_ERR() _tcpSocket->write("-ERR\r\n"); if(!_tcpSocket->waitForBytesWritten(TIME_OUT))return;
	
	#define SEND_OK_With_Data(aditionalData)  _tcpSocket->write(QString("+OK%1\r\n").arg(aditionalData).toAscii()); if(!_tcpSocket->waitForBytesWritten(TIME_OUT))return;

	
	// send the greeting message
	SEND_OK();
	
	//OoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo
    // handling USER message
    int expectedCommand = USER;
    bool result = _tcpSocket->waitForReadyRead( TIME_OUT );
    if( false == result )
        return;

    QByteArray data = _tcpSocket->readAll();
    PopCommand currentCommand = getPopCommandFromString( data );
    if( currentCommand != expectedCommand )
        return;

    SEND_OK();

	QString user = data;
	user.remove( "USER ", Qt::CaseInsensitive );
	user.remove( "\r\n" );
	
	QString domain = user.mid( user.indexOf("@")+1 );
	
	user.remove(domain);
	user.remove( "@" );
	
	//OoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo
    // handling PASS message
	expectedCommand = PASS;
    result = _tcpSocket->waitForReadyRead( TIME_OUT );
    if( false == result )
        return;

	data = _tcpSocket->readAll();
	currentCommand = getPopCommandFromString( data );
    if( currentCommand != expectedCommand )
        return;

	SEND_OK();

	QString pass = data;
	pass.remove( "PASS ", Qt::CaseInsensitive );
	pass.remove( "\r\n" );
	
	//OoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo
	// cheking if we have this user on served domains
	result = doesUserExist( domain, user, pass );
	if( false ==  result )
		return;
		
	//OoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo
	// enter TRANSACTION_STATE state
	int failedTimeouts = 0;
	while( 2 != failedTimeouts )
	{
		result = _tcpSocket->waitForReadyRead( TIME_OUT );
		if( false == result )
		{
			++failedTimeouts;
			continue;
		}
		
		data = _tcpSocket->readAll();
		currentCommand = getPopCommandFromString( data );
		if( (currentCommand<=TRANSACTION_STATE) || (currentCommand>=IGNORED_COMMAND) )
			return;
			
		// read listing of user's mail
		MailDatabase::MailIdList mailIdList;
		mailDatabase->getInfoAboutAllMails( domain, user, mailIdList );

		// here we manage user's mail depending what it requests
		if( STAT == currentCommand )
		{
			qint64 totalMailSize = 0;
			foreach( MailDatabase::MailId mailId, mailIdList )
				totalMailSize+= mailDatabase->getInfoAboutMail( domain, user, mailId );
			
			QString dataToSend = QString(" %1 %2").arg( mailIdList.count() ).arg( totalMailSize );
			SEND_OK_With_Data( dataToSend );
		}
		else if( LIST == currentCommand )
		{
			QString listArgs = data;
			listArgs.remove( "LIST", Qt::CaseInsensitive );
			listArgs = listArgs.trimmed();
			
			if( true == listArgs.isEmpty() )
			{
				SEND_OK();

				quint64 mailCount = 0;
				foreach( MailDatabase::MailId mailId, mailIdList )
				{
					mailCount++;
					quint64 mailSize = mailDatabase->getInfoAboutMail( domain, user, mailId );
					_tcpSocket->write( QString("%1 %2\r\n").arg(mailCount).arg(mailSize).toAscii() );
					result = _tcpSocket->waitForBytesWritten(TIME_OUT);
					if( false == result )
						return;
				}
				_tcpSocket->write( ".\r\n" );
				result = _tcpSocket->waitForBytesWritten(TIME_OUT);
				if( false == result )
					return;
			}
			else
			{
				int mailIndex = listArgs.toInt();
				if( mailIndex < mailIdList.count() )
				{
					quint64 mailSize = mailDatabase->getInfoAboutMail( domain, user, mailIdList[mailIndex] );
					QString dataToSend = QString(" %1 %2").arg( mailIndex ).arg( mailSize );
					SEND_OK_With_Data( dataToSend );
				}
				else
					SEND_ERR();
			}
		}
		else if( RETR == currentCommand )
		{
			QString retrArgs = data;
			retrArgs.remove( "RETR", Qt::CaseInsensitive );
			retrArgs = retrArgs.trimmed();
			
			if( retrArgs.isEmpty() )
			{
				SEND_ERR();
				continue;
			}

			int messageNumber = retrArgs.toInt();
			if( (messageNumber > mailIdList.count()) || (messageNumber < 0) )
			{
				SEND_ERR();
				continue;
			}
			
			SEND_OK();

			messageNumber--;
			QByteArray mailContent = mailDatabase->getMail( domain, user, mailIdList[messageNumber] );
			_tcpSocket->write( mailContent );
			result = _tcpSocket->waitForBytesWritten(TIME_OUT);
			if( false == result )
					return;
			
			_tcpSocket->write( ".\r\n" );
			result = _tcpSocket->waitForBytesWritten(TIME_OUT);
			if( false == result )
				return;
		}
		if( DELE == currentCommand )
		{
			QString deleArgs = data;
			deleArgs.remove( "DELE ", Qt::CaseInsensitive );
			deleArgs.remove( "\r\n" );
			
			if( deleArgs.isEmpty() )
			{
				SEND_ERR();
				continue;
			}
			
			int messageNumber = deleArgs.toInt();
			if( (messageNumber >= mailIdList.count()) || (messageNumber < 0) )
			{
				SEND_ERR();
				continue;
			}
			
			SEND_OK();
			
			mailDatabase->deleteMail( domain, user, mailIdList[messageNumber] );
		}
		if( NOOP == currentCommand )
		{
			SEND_OK();
		}
		if( RSET == currentCommand )
		{
			SEND_OK();
		}
		if( QUIT == currentCommand )
		{
			SEND_OK();
			break;
		}
		if( UIDL == currentCommand )
		{
			QString uidlArgs = data;
			uidlArgs.remove( "UIDL", Qt::CaseInsensitive );
			uidlArgs = uidlArgs.trimmed();

			if( true == uidlArgs.isEmpty() )
			{
				SEND_OK();

				quint64 mailCount = 0;
				foreach( MailDatabase::MailId mailId, mailIdList )
				{
					mailCount++;
					_tcpSocket->write( QString("%1 %2\r\n").arg(mailCount).arg(mailId).toAscii() );
					result = _tcpSocket->waitForBytesWritten(TIME_OUT);
					if( false == result )
						return;
				}
				_tcpSocket->write( ".\r\n" );
				result = _tcpSocket->waitForBytesWritten(TIME_OUT);
				if( false == result )
					return;
			}
			else
			{
				int mailIndex = uidlArgs.toInt();
				if( mailIndex < mailIdList.count() )
				{
					QString dataToSend = QString(" %1 %2").arg( mailIndex ).arg( mailIdList[mailIndex] );
					SEND_OK_With_Data( dataToSend );
				}
				else
					SEND_ERR();
			}		
		}
		if( TOP == currentCommand )
		{
			SEND_ERR();
		}
		
	} // while
}

PopCommand getPopCommandFromString( QByteArray& command )
{
	if( command.toUpper().startsWith("USER") )
		return USER;

	if( command.toUpper().startsWith("PASS") )
		return PASS;

	if( command.toUpper().startsWith("STAT") )
		return STAT;
		
	if( command.toUpper().startsWith("LIST") )
		return LIST;
		
	if( command.toUpper().startsWith("RETR") )
		return RETR;
			
	if( command.toUpper().startsWith("DELE") )
		return DELE;

	if( command.toUpper().startsWith("QUIT") )
		return QUIT;

	if( command.toUpper().startsWith("TOP") )
		return TOP;

	if( command.toUpper().startsWith("UIDL") )
		return UIDL;

	if( command.toUpper().startsWith("APOP") )
		return IGNORED_COMMAND;

	return ERROR_COMMAND;
}

bool doesUserExist( QString domain, QString account, QString password )
{
	if( mailServers.contains(domain) )
	{
		Accounts* accounts = mailServers[domain];
		if( accounts->contains(account) )
			if( (*accounts)[account] == password )
				return true;
	}
		
	return false;
}
