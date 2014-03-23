
// Qt headers
#include <QtCore/QFile>
#include <QtGui/QListView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QStatusBar>
#include <QtGui/QMenu>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtGui/QBitmap>
#include <QtGui/QPaintEvent>
#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QScrollBar>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebFrame>

// blocks
#include <Styling/Styling.h>
#include <Settings/Settings.h>
#include <IconProvider/ActionIconProvider/ActionIconProvider.h>
#include <FileLauncher/FileLauncher.h>
#include <Configurator/Configurator.h>
#include <ConfiguratorReader/ConfiguratorReader.h>
#include <Skinning/SkinWidget.h>
#include <Skinning/SkinWindow.h>

// local headers
#include "CommunicationCenter.h"



CommunicationCenter::CommunicationCenter() :
	QWidget( 0 )
{
	// self cosmetics
	setWindowTitle( "Communication Center" );
	setWindowIcon( QIcon(QApplication::applicationFilePath() + ".png") );

	Styling	styling( *this );
			styling.loadOnScreenPosition();
			styling.loadFonts();

	// setup controls
	setupSysTray();
	setupCentralWidget();

	// some init
	_dataFolder = "/home/nicu/Temp";

	// network communication
	_communicationCenterServerClient = new CommunicationCenterServerClient();

	connect( _communicationCenterServerClient, SIGNAL(newSystemMessage(QString,QString)	), this, SLOT(newSystemMessage(QString,QString) ) );
	connect( _communicationCenterServerClient, SIGNAL(newTextMessage(QString,QString)      ), this, SLOT(newTextMessage(QString,QString)   ) );
	connect( _communicationCenterServerClient, SIGNAL(newFileMessage(QString,QString)      ), this, SLOT(newFileMessage(QString,QString)   ) );
	connect( _communicationCenterServerClient, SIGNAL(userListAvailable(CommunicationCenterServerClient::ShortUserInfoList*) ), this, SLOT(userListAvailable(CommunicationCenterServerClient::ShortUserInfoList*) ) );
	connect( _communicationCenterServerClient, SIGNAL(userInfoAvailable(CommunicationCenterServerClient::FullUserInfo*)      ), this, SLOT(userInfoAvailable(CommunicationCenterServerClient::FullUserInfo*)      ) );
	connect( _communicationCenterServerClient, SIGNAL(netowrkStatusUpdate(CommunicationCenterServerClient::NetworkStatus)    ), this, SLOT(netowrkStatusUpdate(CommunicationCenterServerClient::NetworkStatus)    ) );

	_communicationCenterServerClient->setIpPort( "localhost", 7000 );
}

CommunicationCenter::~CommunicationCenter()
{}

void CommunicationCenter::closeEvent( QCloseEvent* event )
{
    Styling styling( *this );
            styling.saveOnScreenPosition();

    QWidget::closeEvent( event );
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- sys tray
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void CommunicationCenter::setupSysTray()
{
	_sysTrayMenu = new QMenu();
	_sysTrayMenu->addAction( ActionIconProvider().getIconForAction("help"), "Help Me!", this,                     SLOT(helpMe()) );
	_sysTrayMenu->addAction( ActionIconProvider().getIconForAction("exit"), "Exit",     QApplication::instance(), SLOT(quit())   );

	QIcon icon( QApplication::applicationFilePath() + ".png" );

	Styling	styling( *this );
			styling.installSystrayIcon( &icon, _sysTrayMenu );
}

void CommunicationCenter::helpMe()
{
    QStringList configuratorParameter;
                configuratorParameter << Configurator::getAppConfigurationFile();
                configuratorParameter << "helpMe";
                configuratorParameter << "quickHelp";

	Configurator*	configurator = new Configurator( configuratorParameter );
	connect( configurator, SIGNAL(configurationChanged()), this, SLOT(readConfiguration()) );

	configurator->exec();
}

void CommunicationCenter::readConfiguration()
{
	ConfiguratorReader configuratorReader;

	QString ip   = configuratorReader.getLineInput( "network/server/ip"   );
	quint16 port = configuratorReader.getLineInput( "network/server/port" ).toInt();

	_communicationCenterServerClient->setIpPort( ip, port );
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- central widget
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void CommunicationCenter::setupCentralWidget()
{
	// this widget
	QPixmap mainSkinPixmap( "/home/nicu/Pictures/send-button/main.png" );
	setFixedSize( mainSkinPixmap.size() );

	SkinWindow* 	mainSkin = new SkinWindow( this );
				mainSkin->setPixmaps( mainSkinPixmap, mainSkinPixmap );

	const int c_shiftLeftX = 5;
	const int c_shiftTopY  = 5;


	// top bar
	QPixmap topBarSkinPixmap( "/home/nicu/Pictures/send-button/topBar.png" );

	QWidget* _topBar	= new QWidget( this );
	_topBar->setFixedSize( topBarSkinPixmap.size() );

	SkinWidget* 	topBarSkin = new SkinWidget( _topBar );
				topBarSkin->setPixmaps( topBarSkinPixmap, topBarSkinPixmap );

	_topBar->move( c_shiftLeftX, c_shiftTopY );

			// add some additional widgets on top
			QPixmap networkStatusSkinPixmap( "/home/nicu/Pictures/send-button/online.png" );
			QWidget*	networkStatusWidget = new QWidget( _topBar );
					networkStatusWidget->setFixedSize( networkStatusSkinPixmap.size() );

			_networkStatus = new SkinWidget( networkStatusWidget );
			_networkStatus->setPixmaps( networkStatusSkinPixmap, networkStatusSkinPixmap );

			networkStatusWidget->move( 5, 5 );


	// messages
	QPixmap messagesSkinPixmap( "/home/nicu/Pictures/send-button/messages.png" );

	_chatMessages	= new QTextEdit( this );
	_chatMessages->setFixedSize( messagesSkinPixmap.size() );
	_chatMessages->setReadOnly( true );

	SkinWidget* 	messagesSkin = new SkinWidget( _chatMessages );
				messagesSkin->setPixmaps( messagesSkinPixmap, messagesSkinPixmap );

	_chatMessages->move( c_shiftLeftX, c_shiftTopY + _topBar->height() );


	// message to send
	QPixmap messageToSendSkinPixmap( "/home/nicu/Pictures/send-button/messageToSend.png" );

	_messageToSend = new QTextEdit( this );
	_messageToSend->setFixedSize( messageToSendSkinPixmap.size() );

	SkinWidget* 	messageToSendSkin = new SkinWidget( _messageToSend );
				messageToSendSkin->setPixmaps( messageToSendSkinPixmap, messageToSendSkinPixmap );

	_messageToSend->move( c_shiftLeftX, c_shiftTopY + _topBar->height() + _chatMessages->height() );


	// send button
	QPixmap sendSkinRegularPixmap( "/home/nicu/Pictures/send-button/send-regular.png" );
	QPixmap sendSkinHoverPixmap  ( "/home/nicu/Pictures/send-button/send-hover.png"   );

	_sendButton = new QPushButton( "Send", this );
	_sendButton->setFixedSize( sendSkinRegularPixmap.size() );

	SkinWidget* 	sendSkin = new SkinWidget( _sendButton );
				sendSkin->setPixmaps( sendSkinRegularPixmap, sendSkinRegularPixmap );

	_sendButton->move( c_shiftLeftX + _messageToSend->width(), c_shiftTopY + _topBar->height() + _chatMessages->height() );


	// user list
	QPixmap userListSkinPixmap( "/home/nicu/Pictures/send-button/userList.png" );

	_listView = new QListView( this );
	_listView->setFixedSize( userListSkinPixmap.size() );
	_listView->setModel    ( &_userList                );

	SkinWidget* 	userListSkin = new SkinWidget( _listView );
				userListSkin->setPixmaps( userListSkinPixmap, userListSkinPixmap );

	_listView->move( c_shiftLeftX + _chatMessages->width(), 57 );

	connect( _sendButton, SIGNAL(released()), this, SLOT(sendPresed()) );
}

void CommunicationCenter::sendPresed()
{
	QString message = _messageToSend->toPlainText();

	if( false == message.isEmpty() )
	{
		_communicationCenterServerClient->sendTextMessage( message );
		_messageToSend->clear();

		//addMessageToLog( message, 0 );

		//_chatMessages->reload();
		//QWebFrame* webFrame = _chatMessages->page()->mainFrame();
		
		//webFrame->setScrollBarValue( Qt::Vertical, webFrame->scrollBarMaximum(Qt::Vertical) );
	}
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- network
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void CommunicationCenter::newSystemMessage( QString sender, QString message )
{
	addMessageToLog( sender, message, 0 );
}

void CommunicationCenter::newTextMessage( QString sender, QString message )
{
	addMessageToLog( sender, message, 1 );
}

void CommunicationCenter::newFileMessage( QString sender, QString fileName )
{}

void CommunicationCenter::userListAvailable( CommunicationCenterServerClient::ShortUserInfoList* shortUserInfoList )
{
	QStringList userNicks;
	for( int index=0; index < shortUserInfoList->size(); index++ )
	{
		_userIds .append( shortUserInfoList->value(index)._id   );
		userNicks.append( shortUserInfoList->value(index)._nick );
	}

	_userList.setStringList( userNicks );

	userNicks.clear();
	shortUserInfoList->clear();
	delete shortUserInfoList;
}

void CommunicationCenter::userInfoAvailable( CommunicationCenterServerClient::FullUserInfo* fullUserInfo )
{}

void CommunicationCenter::netowrkStatusUpdate( CommunicationCenterServerClient::NetworkStatus networkStatus )
{
	if( CommunicationCenterServerClient::eOn == networkStatus )
	{
		QPixmap pixmap( "/home/nicu/Pictures/send-button/online.png" );
		_networkStatus->setPixmaps( pixmap, pixmap );

		_messageToSend->setDisabled( false );
	}

	else
	if( CommunicationCenterServerClient::eOff == networkStatus )
	{
		QPixmap pixmap( "/home/nicu/Pictures/send-button/offline.png" );
		_networkStatus->setPixmaps( pixmap, pixmap );

		_messageToSend->setDisabled( true );
	}

	else
	if( CommunicationCenterServerClient::eConnecting == networkStatus )
	{
		QPixmap pixmap( "/home/nicu/Pictures/send-button/connecting.png" );
		_networkStatus->setPixmaps( pixmap, pixmap );

		_messageToSend->setDisabled( true );
	}
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- logs
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void CommunicationCenter::addMessageToLog( QString& sender, QString& message, int logType )
{
	QFile file( _dataFolder + "/" + _communicationCenterServerClient->getId() );

	file.open( QIODevice::Append );
		if( 0 == logType )
		{
			file.write( "<span style=\"color: gray; text-align: right;\">" );
		}

		else
		if( 1 == logType )
			file.write( "<span style=\"color: black;\">" );

		else
		if( 2 == logType )
			file.write( "<span style=\"color: blue; text-align: right;\">" );

		file.write( "<b>" );
		file.write( sender.toAscii() );
		file.write( "</b><br/>" );
		file.write( message.toAscii() );
		file.write( "</span><br/><br/>" );
	file.close();

	file.open( QFile::ReadOnly );

		_chatMessages->setHtml( file.readAll() );

		if(	_chatMessages->verticalScrollBar() )
			_chatMessages->verticalScrollBar()->setValue( _chatMessages->verticalScrollBar()->maximum() );

	file.close();
}
