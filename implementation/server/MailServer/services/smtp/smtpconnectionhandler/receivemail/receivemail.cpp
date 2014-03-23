
// Qt headers
#include <QtNetwork/QTcpSocket>
#include <QtCore/QByteArray>

// local headers
#include "receivemail.h"

// this defines the SMTP commands specified by RFC, we add for convenience the "IGNORED_COMMAND", "ERROR_COMMAND"
typedef enum { HELO=0, EHLO=0, MAIL, RCPT, DATA, IGNORED_COMMAND, QUIT, HELP, RSET, EXPN, VRFY, SOML, SAML, ERROR_COMMAND } SmtpCommand;

// this returns the command code depending on the data received by the user
SmtpCommand getCommandFromString( QByteArray& command );

bool receiveMail( QTcpSocket& tcpSocket, QString& from, QString& replyAddress, QStringList& destinations, QByteArray& mailContent, ByteArrayList& attachments )
{
    const int TIME_OUT = 3000;

	#define SEND_READY()   ( tcpSocket.write("220\r\n"), tcpSocket.waitForBytesWritten(TIME_OUT) )
	#define SEND_OK()      ( tcpSocket.write("250\r\n"), tcpSocket.waitForBytesWritten(TIME_OUT) )
	#define SEND_DATA_OK() ( tcpSocket.write("354\r\n"), tcpSocket.waitForBytesWritten(TIME_OUT) )
	#define SEND_CLOSE()   ( tcpSocket.write("221\r\n"), tcpSocket.waitForBytesWritten(TIME_OUT) )

    // send the greeting message
    bool result = SEND_READY();
    if( false == result )
		return false;

    //OoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo
    // handling HELO/EHLO message
    int expectedCommand = HELO;
    result = tcpSocket.waitForReadyRead( TIME_OUT );
    if( false == result )
        return false;
        
    QByteArray data = tcpSocket.readAll();
    SmtpCommand currentCommand = getCommandFromString( data );
    if( currentCommand != expectedCommand )
        return false;

    result = SEND_OK();
    if( false == result )
        return false;

    //OoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo
    // handling the MAIL message
    expectedCommand++;

    result = tcpSocket.waitForReadyRead( TIME_OUT );
    if( false == result )
        return false;
        
	data = tcpSocket.readAll();
	currentCommand = getCommandFromString( data );
    if( currentCommand != expectedCommand )
        return false;
    
    result = SEND_OK();
    if( false == result )
        return false;

	from = data;
	from.remove( "MAIL FROM:", Qt::CaseInsensitive );
	from.remove( "<" );
	from.remove( ">" );
	from.remove( "\r\n" );
				
	replyAddress = from;

    //OoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo
    // handling the RCPT message
    expectedCommand++;

    result = tcpSocket.waitForReadyRead( TIME_OUT );
    if( false == result )
        return false;
        
	data = tcpSocket.readAll();
	currentCommand = getCommandFromString( data );
    if( currentCommand != expectedCommand )
        return false;
    
    result = SEND_OK();
    if( false == result )
        return false;

	QString to = data;
	to.remove( "RCPT TO:", Qt::CaseInsensitive );
	to.remove( "<" );
	to.remove( ">" );				
	to.remove( "\r\n" );
				
	destinations.append( to );

    //OoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo
    // handling the DATA message
    expectedCommand++;

    result = tcpSocket.waitForReadyRead( TIME_OUT );
    if( false == result )
        return false;
        
	data = tcpSocket.readAll();
	currentCommand = getCommandFromString( data );
    if( currentCommand != expectedCommand )
        return false;
    
    result = SEND_DATA_OK();
    if( false == result )
        return false;
    
    // here we'll be expecting for mail content from the client
    int failedTimeOuts = 0; // we use this as protection against DoS attacks
    while( 2 != failedTimeOuts )
    {
        result = tcpSocket.waitForReadyRead( TIME_OUT );
		if( false == result )
		{
			failedTimeOuts++;
			continue;
		}
		
		data = tcpSocket.readAll();
        mailContent.append( data );
        if( data.endsWith( ".\r\n" ) )
		{
            result = SEND_OK();
            if( false == result )
                return false;
                
            break;		
		}

    }
	
	return true;
}

SmtpCommand getCommandFromString( QByteArray& command )
{
	if( command.toUpper().startsWith("HELO") )
		return HELO;

	if( command.toUpper().startsWith("EHLO") )
		return EHLO;

	if( command.toUpper().startsWith("MAIL FROM:") )
		return MAIL;
		
	if( command.toUpper().startsWith("RCPT TO:") )
		return RCPT;
		
	if( command.toUpper().startsWith("DATA") )
		return DATA;
			
	if( command.toUpper().startsWith("QUIT") )
		return IGNORED_COMMAND;

	if( command.toUpper().startsWith("HELP") )
		return IGNORED_COMMAND;

	if( command.toUpper().startsWith("RSET") )
		return IGNORED_COMMAND;

	if( command.toUpper().startsWith("EXPN") )
		return IGNORED_COMMAND;

	if( command.toUpper().startsWith("VRFY") )
		return IGNORED_COMMAND;

	if( command.toUpper().startsWith("SOML") )
		return IGNORED_COMMAND;

	if( command.toUpper().startsWith("SAML") )
		return IGNORED_COMMAND;

	return ERROR_COMMAND;
}
