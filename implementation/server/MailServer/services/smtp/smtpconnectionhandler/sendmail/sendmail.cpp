
// Qt headers
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>


// local headers
#include "sendmail.h"
#include "dnsinfo/dnsinfo.h"

bool sendMail( QString& from, QString& replyAddress, QString destination, QByteArray& mailContent, ByteArrayList& attachments )
{
	const int TIME_OUT = 3000;
	QTcpSocket tcpSocket;
	bool result;

	QString targetServer = destination.mid( destination.indexOf("@")+1);
	
	// getting the mail servers for the domain, and try to connect to one of them and deliver the mail
	QStringList mailServers;
	QString errorString;
	DnsInfo dnsInfo;
	
	if( false == dnsInfo.getIpAddressesForDomain(targetServer,mailServers,errorString) )
		return false;
	
	foreach( QString ipAddress, mailServers )		
	{
		tcpSocket.connectToHost( ipAddress, 25 );
		result = tcpSocket.waitForConnected( TIME_OUT );
		if( true == result )
			break;
	}
	if( false == result  )
		return false;	
	
	// reading the greeting message	
	if( !tcpSocket.waitForReadyRead(TIME_OUT) ) 
	{
		tcpSocket.abort();
		return false;
	}
	QByteArray data = tcpSocket.readAll();
	if( !data.startsWith("220") )
	{
		tcpSocket.abort();
		return false;
	}

	// seding EHLO
	//tcpSocket->write( "EHLO\r\n" );
	tcpSocket.write( "HELO there\r\n" );
	if( !tcpSocket.waitForReadyRead(TIME_OUT) )
	{
		tcpSocket.abort();
		return false;
	}
	data = tcpSocket.readAll();
	if( !data.startsWith("250") )
	{
		tcpSocket.abort();
		return false;
	}
		
	// sending MAIL FROM
	QString mailFromString = QString("MAIL FROM: <%1>\r\n").arg(from);
	tcpSocket.write( mailFromString.toAscii() );
	if( !tcpSocket.waitForReadyRead(TIME_OUT) )
	{
		tcpSocket.abort();
		return false;
	}
	data = tcpSocket.readAll();
	if( !data.startsWith("250") )
	{
		tcpSocket.abort();
		return false;
	}
		
	// sending RCPT TO
	QString mailToString = QString("RCPT TO: <%1>\r\n").arg(destination);
	tcpSocket.write( mailToString.toAscii() );
	if( !tcpSocket.waitForReadyRead(TIME_OUT) )
	{
		tcpSocket.abort();
		return false;
	}
	data = tcpSocket.readAll();
	if( !data.startsWith("250") )
	{
		tcpSocket.abort();
		return false;
	}
	
	// sending DATA
	tcpSocket.write( "DATA\r\n" );
	if( !tcpSocket.waitForReadyRead(TIME_OUT) )
	{
		tcpSocket.abort();
		return false;
	}
	data = tcpSocket.readAll();
	if( !data.startsWith("354") )
	{
		tcpSocket.abort();
		return false;
	}
	
	tcpSocket.write( mailContent );
	if( !tcpSocket.waitForBytesWritten() )
	{
		tcpSocket.abort();
		return false;
	}
	tcpSocket.write( ".\r\n" );
	if( !tcpSocket.waitForReadyRead(TIME_OUT) )
	{
		tcpSocket.abort();
		return false;
	}
	data = tcpSocket.readAll();
	if( !data.startsWith("250") )
	{
		tcpSocket.abort();
		return false;
	}
	
	// sending QUIT
	tcpSocket.write( "QUIT\r\n" );
	if( !tcpSocket.waitForBytesWritten() )
	{
		tcpSocket.abort();
		return false;
	}
	
	// cleaning up
	tcpSocket.disconnectFromHost();
	tcpSocket.abort();
	
	return true;
}
