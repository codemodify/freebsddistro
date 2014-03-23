#ifndef __CommunicationCenter_H__
#define __CommunicationCenter_H__

// Qt headers
#include <QtGui/QWidget>
#include <QtGui/QStringListModel>

// local headers
#include "CommunicationCenterServerClient/CommunicationCenterServerClient.h"

// forwards
class QTextEdit;
class QWebView;
class QPushButton;
class QMenu;
class QListView;
class SkinWidget;


class CommunicationCenter : public QWidget
{
	Q_OBJECT

	public:
		CommunicationCenter();
		~CommunicationCenter();

	// override some inherited stuff
	protected:
		void closeEvent( QCloseEvent* event );


	// sys tray
	private:
		void setupSysTray();
	private slots:
		void helpMe();
		void readConfiguration();


	// central widget
	private:
		void setupCentralWidget();
	private slots:
		void sendPresed();


	// communication logs
	private:
		void addMessageToLog( QString& sender, QString& message, int logType );


	// communication data
	private slots:
		void newSystemMessage   ( QString sender, QString message  );
		void newTextMessage     ( QString sender, QString message  );
		void newFileMessage     ( QString sender, QString fileName );
		void userListAvailable  ( CommunicationCenterServerClient::ShortUserInfoList* shortUserInfoList );
		void userInfoAvailable  ( CommunicationCenterServerClient::FullUserInfo*      fullUserInfo      );
		void netowrkStatusUpdate( CommunicationCenterServerClient::NetworkStatus      networkStatus     );
	

	// widgets
	private:
		SkinWidget*		_networkStatus;

		QTextEdit*		_chatMessages;
		QTextEdit*		_messageToSend;

		QPushButton*		_sendButton;

		QListView*		_listView;
		QStringListModel	_userList;
		QStringList		_userIds;

	// internal stuff
	private:
		QMenu*			_sysTrayMenu;
		QString			_dataFolder;
		CommunicationCenterServerClient* _communicationCenterServerClient;
};

#endif
