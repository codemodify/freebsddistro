
#include <QtGui/QApplication> 

#include "sshclient/sshclient.h"

int main( int argc, char** argv )
{
	QApplication application( argc, argv );
	
	SshClient sshClient;
	sshClient.show();
		
	return application.exec();
}
