#ifndef _POP_CONNECTION_HANDLER_H_
#define _POP_CONNECTION_HANDLER_H_

#include <QtCore/QThread>
#include <QtCore/QPointer>

class QTcpSocket;

class PopConnectionHandler : public QThread
{

	Q_OBJECT

public:
	PopConnectionHandler( int socketDescription );
	~PopConnectionHandler();


public:
	void run();


private:
	int _socketDescription;
	QPointer<QTcpSocket> _tcpSocket;
};

#endif
