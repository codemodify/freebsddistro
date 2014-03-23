#ifndef DELERIUM_H
#define DELERIUM_H

// qt headers
#include <QtGui/QMainWindow>
#include <QtCore/QPointer>

class TwoDRenderer;
class QTcpSocket;
class QTcpServer;
class QStatusBar;
class QLabel;

class Dellirium : public QMainWindow
{
	Q_OBJECT
	
public:
	Dellirium();
	~Dellirium();

	
private slots:
	void slot_HelpMe();
	void slot_ConnectToServer();
	void slot_RestartGame();
	void slot_Quit();
	
	void slot_CreateOpenMap();
	void slot_SendMapToClient();
	void slot_ChooseMapToPlay();



private: // the GUI layer
	QPointer<TwoDRenderer> _renderer;
	
	QPointer<QStatusBar> _statusBar;
	QPointer<QLabel> _peerOnPort;
	QPointer<QLabel> _weOnPort;

private:
	void initMenu();
	void initStatusBar();
	void initRenderer();
	


private: // the Networking layer
	QPointer<QTcpServer> _tcpServer;
	QPointer<QTcpSocket> _tcpSendSocket;
	QPointer<QTcpSocket> _tcpReadSocket;

private slots:
	void slot_PeerConnected();
	void slot_ConectedToPeer();
	void slot_DataReceivedAboutMoves();
	void slot_DataAboutMovesMustBeSent( QByteArray moveContent );

private:
	void initNetworkLayer();

};

#endif
