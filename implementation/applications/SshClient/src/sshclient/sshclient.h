#ifndef ssclient_h
#define ssclient_h

#include <QtGui/QWidget>
#include <QtCore/QPointer>
#include <QtCore/QList>

#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslError>

class QTextEdit;
class QLineEdit;
class QPushButton;

class SshClient : public QWidget
{
	Q_OBJECT
	
	public:
		SshClient();
		~SshClient();
		
	private:
		QPointer<QTextEdit> _outputData;
		QPointer<QLineEdit> _inputData;
		QPointer<QPushButton> _sendButton;
	private slots:
		void slot_Send();

	private:
		QPointer<QSslSocket> _connection;
	private slots:
		void slot_ConnectionEcrypted();
		void slot_ConnectionSateChanged( QSslSocket::SslMode );
		void slot_ConnectionSslErrors( const QList<QSslError>& );
		
		void slot_ConnectionConnected();
		void slot_ConnectionDisconnected();
		void slot_ConnectionError( QAbstractSocket::SocketError );
		void slot_ConnectionHostFound();
		void slot_ConnectionStateChanged( QAbstractSocket::SocketState );
		void slot_ConnectionStatereadyRead();
					
}; 

#endif
