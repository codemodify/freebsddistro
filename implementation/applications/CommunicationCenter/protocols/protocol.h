#ifndef PROTOCOL_H_
#define PROTOCOL_H_

// Qt's headers
#include <QtCore/QObject>
#include <QtCore/QStringList>

// supported protocols
typedef enum{ ICQ, unknown } ProtocolID; 


class QString;

class Protocol : public QObject
{

	Q_OBJECT

public:
	Protocol();
	virtual ~Protocol();


virtual ProtocolID getProtocolID() = 0;


signals: // connection related
	virtual void statusChanged( QString status );



signals: // message related
	virtual void messageReceived( QString contactID, QString message );
	virtual void messageSent( QString contactID );
	virtual void messageNotSent( QString contactID );



signals: // file transfer related
	virtual void fileTransferRequested( QString contactID );
	virtual void fileTransferFinished( QString contactID );
	virtual void fileTransferNotFinished( QString contactID );



signals: // contacts related
	virtual void contactChangedStatus( QString contactID, int statusID );
	virtual void contactListReceived( QStringList contactList );



public slots: // connection related
	virtual void conect( QString user, QString password );



public slots: // message related
	virtual void sendMessage( QString contactID, QString message );



public slots: // file transfer realated
	virtual void sendFile( QString contactID, QString file );



public slots:
	virtual void changeStatus( int statusID );


};

#endif
