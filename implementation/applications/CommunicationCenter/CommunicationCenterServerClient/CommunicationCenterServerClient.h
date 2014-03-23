#ifndef __CommunicationCenterServerClient_H__
#define __CommunicationCenterServerClient_H__

// Qt headers
#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>


class CommunicationCenterServerClient : public QObject
{
	Q_OBJECT

	public:
		CommunicationCenterServerClient();

	public:
		void setIpPort( QString ip, quint16 port );
		QString getId();

	// data structures
	private:
		typedef enum
		{
			eMessageTypeBegin = 0,

			eSystemYourId,            // <-
			eSystemText,              // <-
			eSystemUpdateMyShortInfo, // ->
			eSystemShortInfoUpdated,  // <-
			eSystemFilePort,		   // <-
			eSystemUserList,		   // <-

			eText,
			eMail,

			eAudio,
			eVideo,

			eFile,

			eUserInfo,

			eMessageTypeEnd

		} MessageType;

	public:
		typedef enum
		{
			eOn,
			eOff,
			eConnecting

		} NetworkStatus;

	public:
		typedef struct
		{
			QString		_id;
			QString		_nick;
		} ShortUserInfo;
		typedef QList< ShortUserInfo > ShortUserInfoList;

		typedef struct
		{
			QString		_id;
			QString		_nick;
			QString		_name;
			QString		_contactInfo;
			QByteArray	_picture;
		} FullUserInfo;


	// api 
	signals:
		void newSystemMessage( QString sender, QString message  );
		void newTextMessage  ( QString sender, QString message  );
		void newFileMessage  ( QString sender, QString fileName );

		void userListAvailable( CommunicationCenterServerClient::ShortUserInfoList* shortUserInfoList );
		void userInfoAvailable( CommunicationCenterServerClient::FullUserInfo*      fullUserInfo      );

		void netowrkStatusUpdate( CommunicationCenterServerClient::NetworkStatus networkStatus );

	public:
		void sendTextMessage( QString& message  );
		void sendFileMessage( QString& filePath );
		void getUserInfo    ( QString& userId   );


	// network delegates
	private slots:
		void mainChannelConnected();
		void mainChannelDisconnected();
		void mainChannelError( QAbstractSocket::SocketError );
		void mainChannelReadData();

//		void fileChannelConnected();
//		void fileChannelDisconnected();
//		void fileChannelError( QAbstractSocket::SocketError );
//		void fileChannelReadData();


	// internal
	private:
		QTcpSocket* _mainChannel;
		QTcpSocket* _fileChannel;
		QString     _id;

		QString     _ip;
		quint16     _port;

};

#endif
