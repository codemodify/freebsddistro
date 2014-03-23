#ifndef DELERIUM_H
#define DELERIUM_H

// qt headers
#include <QtGui/QMainWindow>
#include <QtCore/QPointer>
#include <QtNetwork/QTcpServer>

class TwoDRenderer;

class Dellirium : public QMainWindow
{
	Q_OBJECT
	
public:
	Dellirium();
	~Dellirium();

	
private slots:
	void slot_HelpMe();
	void slot_PlayClient();
	void slot_PlayServer();
	void slot_RestartGame();
	void slot_Quit();
	
	void slot_CreateOpenMap();
	void slot_SendMapToClient();
	void slot_ChooseMapToPlay();

	
private:
	void initMenu();


private:
	QPointer<TwoDRenderer> _renderer;


private: // networking layer
	QPointer<QTcpServer> _tcpReadServer;
	QPointer<QTcpServer> _tcpSendServer;
	
	QPointer<QTcpSocket> _tcpSendSocket;
	QPointer<QTcpSocket> _tcpReadSocket;
	
	void destroyNetworkingLayer();

private slots:
	void slot_SenderConnected();
	void slot_ReaderConnected();
	void slot_DataReceivedAboutMoves();

};

#endif