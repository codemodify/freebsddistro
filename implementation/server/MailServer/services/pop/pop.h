#ifndef _POP_SERVICE_H_
#define _POP_SERVICE_H_

#include <QtNetwork/QTcpServer>
#include <QtCore/QMap>

class QStringList;


class PopService : public QTcpServer
{

public:
	PopService();
	~PopService();

private:
    void incomingConnection( int socketDescriptor );

};

#endif
