
#include <QtGui/QTextEdit>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>


#include "sshclient.h"

SshClient::SshClient() :
	QWidget()
{
	// setup the UI
	_outputData = new QTextEdit();
	_outputData->setReadOnly( true );	
	
	_inputData = new QLineEdit();
	_inputData->setEnabled( false );
	
	_sendButton = new QPushButton( "send" );
	_sendButton->setEnabled( false );
	connect( _sendButton, SIGNAL(released()), this, SLOT(slot_Send()) );

	QHBoxLayout* horizontalLayout = new QHBoxLayout();
	horizontalLayout->addWidget( _inputData );
	horizontalLayout->addWidget( _sendButton );
	
	QVBoxLayout* vertcalLayout = new QVBoxLayout( this );
	vertcalLayout->addWidget( _outputData );
	vertcalLayout->addLayout( horizontalLayout );
	
	// setup the connection
	_connection = new QSslSocket();
	connect( _connection, SIGNAL(encrypted()), this, SLOT(slot_ConnectionEcrypted()) );
	//connect( _connection, SIGNAL(modeChanged(QSslSocket::SslMode)), this, SLOT(slot_ConnectionSateChanged(QSslSocket::SslMode)) );
	connect( _connection, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(slot_ConnectionSslErrors(const QList<QSslError>&)) );

	//connect( _connection, SIGNAL(connected()),                                this, SLOT(slot_ConnectionConnected()) );
	//connect( _connection, SIGNAL(disconnected()),                             this, SLOT(slot_ConnectionDisconnected()) );
	//connect( _connection, SIGNAL(error(QAbstractSocket::SocketError)),        this, SLOT(slot_ConnectionError(QAbstractSocket::SocketError)) );
	//connect( _connection, SIGNAL(hostFound()),                                this, SLOT(slot_ConnectionHostFound()) );
	//connect( _connection, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(slot_ConnectionStateChanged(QAbstractSocket::SocketState)) );
	//connect( _connection, SIGNAL(readyRead()),                                this, SLOT(slot_ConnectionStatereadyRead()) );

    _connection->setLocalCertificate( "/usr/home/nicu/gen-cryptic-info/client.id.rsa.pem.certificate" );
	_connection->setPrivateKey( "/usr/home/nicu/gen-cryptic-info/client.id.rsa.pem.private" );

	_connection->connectToHostEncrypted( "localhost", 8022 );
}

SshClient::~SshClient()
{} 

void SshClient::slot_Send()
{
	QString data = _inputData->text();
	_inputData->clear();
	
	_outputData->append( data );
	
	_connection->write( data.toAscii() );
}

void SshClient::slot_ConnectionEcrypted()
{
	_inputData->setEnabled( true );
	_sendButton->setEnabled( true );
}

void SshClient::slot_ConnectionSateChanged( QSslSocket::SslMode sslMode )
{
	if( sslMode == QSslSocket::UnencryptedMode )
		_outputData->append( "current ssl mode: Unencrypted" );
	else if( sslMode == QSslSocket::SslClientMode )
		_outputData->append( "current ssl mode: SslClient" );
	else if( sslMode == QSslSocket::SslServerMode )
		_outputData->append( "current ssl mode: SslServer" );
}

void SshClient::slot_ConnectionSslErrors( const QList<QSslError>& sslErrors )
{
	foreach( QSslError sslError, sslErrors )
		_outputData->append( QString("ssl-error: %1").arg(sslError.errorString()) );
}

void SshClient::slot_ConnectionConnected()
{
	_outputData->append( "connected" );
}

void SshClient::slot_ConnectionDisconnected()
{
	_outputData->append( "disconnected" );
}

void SshClient::slot_ConnectionError( QAbstractSocket::SocketError socketError )
{
	if( QAbstractSocket::ConnectionRefusedError == socketError )
		_outputData->append( "socketError: ConnectionRefusedError" );

	else if( QAbstractSocket::RemoteHostClosedError == socketError )
		_outputData->append( "socketError: RemoteHostClosedError" );

	else if( QAbstractSocket::HostNotFoundError == socketError )
		_outputData->append( "socketError: HostNotFoundError" );

	else if( QAbstractSocket::SocketAccessError == socketError )
		_outputData->append( "socketError: SocketAccessError" );

	else if( QAbstractSocket::SocketResourceError == socketError )
		_outputData->append( "socketError: SocketResourceError" );

	else if( QAbstractSocket::SocketTimeoutError == socketError )
		_outputData->append( "socketError: SocketTimeoutError" );

	else if( QAbstractSocket::DatagramTooLargeError == socketError )
		_outputData->append( "socketError: DatagramTooLargeError" );

	else if( QAbstractSocket::NetworkError == socketError )
		_outputData->append( "socketError: NetworkError" );

	else if( QAbstractSocket::AddressInUseError == socketError )
		_outputData->append( "socketError: AddressInUseError" );

	else if( QAbstractSocket::SocketAddressNotAvailableError == socketError )
		_outputData->append( "socketError: SocketAddressNotAvailableError" );

	else if( QAbstractSocket::UnsupportedSocketOperationError == socketError )
		_outputData->append( "socketError: UnsupportedSocketOperationError" );

	else if( QAbstractSocket::UnknownSocketError == socketError )
		_outputData->append( "socketError: UnknownSocketError" );
}

void SshClient::slot_ConnectionHostFound()
{
	_outputData->append( "hostfound" );
}

void SshClient::slot_ConnectionStateChanged( QAbstractSocket::SocketState socketState )
{
	if( QAbstractSocket::UnconnectedState == socketState )
		_outputData->append( "socketState: UnconnectedState" );
	else if( QAbstractSocket::HostLookupState == socketState )
		_outputData->append( "socketState: HostLookupState" );
	else if( QAbstractSocket::ConnectingState == socketState )
		_outputData->append( "socketState: ConnectingState" );
	else if( QAbstractSocket::ConnectedState == socketState )
		_outputData->append( "socketState: ConnectedState" );
	else if( QAbstractSocket::BoundState == socketState )
		_outputData->append( "socketState: BoundState" );
	else if( QAbstractSocket::ClosingState == socketState )
		_outputData->append( "socketState: ClosingState" );
	else if( QAbstractSocket::ListeningState == socketState )
		_outputData->append( "socketState: ListeningState" );
}

void SshClient::slot_ConnectionStatereadyRead()
{
	_outputData->append( "ready-read" );
}
