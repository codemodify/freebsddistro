

// ISP headers
#include <isp/components/components.h>
using namespace IspComponents::Dom;
using namespace IspComponents::Logging;

// local headers
#include "sshserver.h"

Q_EXPORT_PLUGIN2( sshServer, SshServer )

SshServer::SshServer():Daemon(){}

SshServer::~SshServer(){}

bool SshServer::initialize()
{
    logInfo( this, QString("init module %1 ---- ---- ---- ---- ---- ----").arg(getDescription()), true );
    
	QString settingsFilePath = _pathToModule + ".settings";
	logInfo( this, QString("settings file to use: %1").arg(settingsFilePath), true );
	
	QDomDocument settingsFileAsXml;
	QDomNode rootNode = IspComponents::Dom::loadXmlFileAndGetRootNode(settingsFilePath,settingsFileAsXml);

	// reading the server's settings
	if( false == readSshServerSettings(rootNode) )
		return false;
	
	// seeking for defined users
	QDomNode users = IspComponents::Dom::getChildNodeByName( rootNode, "users" );
	if( true == users.isNull() )
    {
		logInfo(this,"no users defined",true);
		return false;
    }
		
	// reading the deafaults for a user
	logInfo( this, "reading defaults >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", true );
	AcceptedShellCommands acceptedShellCommands;
	QDomNode defaultValues = IspComponents::Dom::getChildNodeByName( users, "defaultValues" );
	if( true == defaultValues.isNull() )
    	logInfo( this, "no defaults defined", true );
    else
    {
		readAcceptedShellCommands( defaultValues, acceptedShellCommands );
		if( acceptedShellCommands.isEmpty() )
        {
			logInfo( this, "acceptedShellCommands is empty", true );
			return false; 
        }
    }
	
	// now read the configured users
	logInfo( this, "reading users >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", true );
	QDomNode user = IspComponents::Dom::getChildNodeByName( users, "user" );
	if( true == user.isNull() )
    {
		logInfo( this, "no users defined", true );
		return false;
    }
		
	while( false == user.isNull() )
    {
		if( "#comment" != user.nodeName() )
        {
			logInfo( this, QString("reading settings for user: %1").arg(user.nodeName()), true );			
			UserSettings* userSettings = new UserSettings();
			if( false == readUser(user,*userSettings) )
				return false;
			if( userSettings->_acceptedShellCommands.isEmpty() )
				userSettings->_acceptedShellCommands = acceptedShellCommands;				
			_userList.append(userSettings);
        }
		user = user.nextSibling();
    }


//    QFile certificateFile( "/usr/home/nicu/gen-cryptic-info/client.id.rsa.pem.certificate" );
//    QSslCertificate certificate( &certificateFile );
//    _caCertificates.append( certificate );

	logInfo( this, "SshServer is up and ready for serving" );		
	return true;
}

bool SshServer::shutdown()
{
	logInfo( this, "Webserver is going down" );
    
    return true;
}

QString SshServer::getDescription()
{
	return "Internet Server Platform - SshServer module";
}

QString SshServer::getName()
{
    return "SshServer";
}

bool SshServer::readSshServerSettings( QDomNode parentNode )
{
	logInfo( this, "reading SSH server settings", true );
	
	QDomNode sshServerSettingsNode = IspComponents::Dom::getChildNodeByName( parentNode, "sshServerSettings" );
	if( true == sshServerSettingsNode.isNull() )
		return false;

	// read ssl version		
	QDomNode versionNode = IspComponents::Dom::getChildNodeByName( sshServerSettingsNode, "version" );
	if( true == versionNode.isNull() )
		return false;

	_sshServerSettings._version = versionNode.attributes().namedItem("value").nodeValue();
	logInfo( this, QString("version=%1").arg(_sshServerSettings._version), true );

	// read cipher
	QDomNode cipherNode = IspComponents::Dom::getChildNodeByName( sshServerSettingsNode, "cipher" );
	if( true == cipherNode.isNull() )
		return false;

	_sshServerSettings._cipher = cipherNode.attributes().namedItem("value").nodeValue();
	logInfo( this, QString("chiper=%1").arg(_sshServerSettings._cipher), true );
	
	// read ceritificate
	QDomNode certificateNode = IspComponents::Dom::getChildNodeByName( sshServerSettingsNode, "certificate" );
	if( true == certificateNode.isNull() )
		return false;
    
    _sshServerSettings._certificate = certificateNode.attributes().namedItem("value").nodeValue();
    logInfo( this, QString("certificate=%1").arg(_sshServerSettings._certificate), true );
        

    // read public key
    QDomNode publicKeyNode = IspComponents::Dom::getChildNodeByName( sshServerSettingsNode, "publicKey" );
    if( true == publicKeyNode.isNull() )
        return false;
    
	_sshServerSettings._publicKey = publicKeyNode.attributes().namedItem("value").nodeValue();
	logInfo( this, QString("publicKey=%1").arg(_sshServerSettings._publicKey), true );

    // read private key
    QDomNode privateKeyNode = IspComponents::Dom::getChildNodeByName( sshServerSettingsNode, "privateKey" );
    if( true == privateKeyNode.isNull() )
        return false;
    	
	_sshServerSettings._privateKey = privateKeyNode.attributes().namedItem("value").nodeValue();
	logInfo( this, QString("privateKey=%1").arg(_sshServerSettings._privateKey), true );

    // read format
    QDomNode formatNode = IspComponents::Dom::getChildNodeByName( sshServerSettingsNode, "format" );
    if( true == formatNode.isNull() )
        return false;
    
	_sshServerSettings._format = formatNode.attributes().namedItem("value").nodeValue();
	logInfo( this, QString("format=%1").arg(_sshServerSettings._format), true );

    // we're done here        
    return true;
}

bool SshServer::readAcceptedShellCommands( QDomNode parentNode, AcceptedShellCommands& acceptedShellCommands )
{
	logInfo( this, "reading accepted Shell Commands", true );
	
	QDomNode acceptedShellCommandsNode = IspComponents::Dom::getChildNodeByName( parentNode, "acceptedShellCommands" );
	if( true == acceptedShellCommandsNode.isNull() )
		return false;
		
	acceptedShellCommands = acceptedShellCommandsNode.attributes().namedItem("value").nodeValue().split(",");
	foreach( QString shellCommand, acceptedShellCommands )
		logInfo( this, QString("shellCommand=%1").arg(shellCommand), true );
        
    return true; 
}

bool SshServer::readUser( QDomNode userNode, UserSettings& userSettings )
{
	userSettings._name = userNode.attributes().namedItem("name").nodeValue();
	logInfo( this, QString("name=%1").arg(userSettings._name), true );
	if( userSettings._name.isEmpty() )
		return false;

	userSettings._home=userNode.attributes().namedItem("home").nodeValue();
	logInfo( this, QString("home=%1").arg(userSettings._home), true );
	if( userSettings._home.isEmpty() )
		return false;
	
	userSettings._lockInHome=(userNode.attributes().namedItem("lockInHome").nodeValue()=="yes");
	logInfo( this, QString("lockInHome=%1").arg(userSettings._lockInHome?"yes":"no"), true );
	
	userSettings._publicKey=userNode.attributes().namedItem("publicKey").nodeValue();
	logInfo( this, QString("publicKey=%1").arg(userSettings._publicKey), true );
	if( userSettings._publicKey.isEmpty() )
		return false;
	
	userSettings._publicKeyFormat=userNode.attributes().namedItem("publicKeyFormat").nodeValue();
	logInfo( this, QString("publicKeyFormat=%1").arg(userSettings._publicKeyFormat), true );
	if( userSettings._publicKeyFormat.isEmpty() )
		return false;
	
	userSettings._acceptedShellCommands=userNode.attributes().namedItem("value").nodeValue().split(",");
	foreach( QString shellCommand, userSettings._acceptedShellCommands )
		logInfo( this, QString("shellCommand=%1").arg(shellCommand), true );
        
    return true;
}

void SshServer::slot_newIncommingConnection( int socketDescriptor )
{
    QSslSocket* sslSocket = new QSslSocket();
	
	// before the handshake, we need to adjust some security parameters for SSL
    
	QSsl::SslProtocol sslProtocol;
	if( "SSL-v3" == _sshServerSettings._version )
		sslProtocol = QSsl::SslV3;
    else if( "TLS-v1" == _sshServerSettings._version )
		sslProtocol = QSsl::TlsV1;
	else
    {
		logError( this, "no valid SSL version to use" );
		delete sslSocket;        
		return;
    }
    QSsl::EncodingFormat ecodingFormat = ("PER"==_sshServerSettings._format) ? QSsl::Pem : QSsl::Der;
    QSsl::KeyAlgorithm algorithm = ("RSA"==_sshServerSettings._cipher) ? QSsl::Rsa : QSsl::Dsa;
    QByteArray password;

    // setting the SSL version to use
    sslSocket->setProtocol( sslProtocol );
    
    // ensure that the peer's certificate will be verified 
    sslSocket->setPeerVerifyMode( QSslSocket::VerifyPeer );
    
    // ensure that the peer's cerficiate and its issuer's certificate will be verified
    sslSocket->setPeerVerifyDepth( 2 );

    
    // setting server's certificate
    sslSocket->setLocalCertificate( _sshServerSettings._certificate, ecodingFormat );
    
    // setting server's private key
    sslSocket->setPrivateKey( _sshServerSettings._privateKey, algorithm, ecodingFormat, password );
    
    // setting the CA ceritificate
    QList<QSslCertificate> caCertificates = QSslCertificate::fromPath( _sshServerSettings._certificate, ecodingFormat );
    sslSocket->setDefaultCaCertificates( caCertificates );
    
    // setup some traps for the socket events
	connect( sslSocket, SIGNAL(disconnected()), sslSocket, SLOT(deleteLater()) );
	connect( sslSocket, SIGNAL(encrypted()), SLOT(slot_SuccessfulConnected()) );
	connect( sslSocket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(slot_UnSuccessfulConnected(const QList<QSslError>&)) );
    connect( sslSocket, SIGNAL(readyRead()), this, SLOT(slot_IncommingData()) );
    
	// start the handshake
	bool result = sslSocket->setSocketDescriptor( socketDescriptor );
    if( false == result )
    {
        logError( this, QString("failed to set socket descriptor: %1").arg(sslSocket->errorString()) );
        delete sslSocket;
        return;
    }

    sslSocket->startServerEncryption();
}

void SshServer::slot_SuccessfulConnected()
{
	QSslSocket* sslSocket = qobject_cast<QSslSocket*>( sender() );
	if( 0== sslSocket )
		return;
        
    logError( this, "slot_SuccessfulConnected", true );
}

void SshServer::slot_UnSuccessfulConnected( const QList<QSslError>& sslErrors )
{
	QSslSocket* sslSocket = qobject_cast<QSslSocket*>(sender());
	if( 0 == sslSocket )
		return;

    // inform the ISP that the peer is a bad guy
	emit markPeerAsPotentialIntruder( sslSocket );

    // check fo the error occured
    foreach( QSslError sslError, sslErrors )
        logError( this, sslError.errorString(), true );
}

void SshServer::slot_IncommingData()
{
    QSslSocket* sslSocket = qobject_cast<QSslSocket*>(sender());
    if( 0 == sslSocket )
        return;
    
    logInfo( this, sslSocket->readAll(), true );
}
