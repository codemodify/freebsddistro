#ifndef collaborativeschema_h
#define collaborativeschema_h

#include <QtGui/QMainWindow>
#include <QtCore/QPointer>

class QTcpSocket;
class GraphicsScene;

class CollaborativeSchema : public QMainWindow
{
	Q_OBJECT
	
	public:
		CollaborativeSchema();
		~CollaborativeSchema();
		
	private:
		GraphicsScene* getScene();

	signals:
		void status( QString statusString );
		void objectAvailable( QByteArray byteArray );		
	private slots:
		void slot_StatusChanged( QString statusString );		

	private:
		QPointer<QTcpSocket> _connection;
	private slots:
		void slot_Connect();		
		void slot_ConnectionConnected();
		void slot_ConnectionDisconnected();
		void slot_ConnectionReadyRead();
		
	private slots:
		void slot_SendSchemaToPeers( QByteArray objectToSend );		
}; 

#endif
