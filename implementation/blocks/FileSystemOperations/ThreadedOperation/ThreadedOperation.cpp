
// local headers
#include "ThreadedOperation.h"
#include "FileSystemOperations/FileIoEngine/FileIoEngine.h"

ThreadedOperation::ThreadedOperation( FileIoEngine* io ) :
	QThread()
{
	_threadedCode1 = 0;
	_threadedCode2 = 0;
	_io = io;
}

void ThreadedOperation::start()
{
    QThread::start( QThread::LowestPriority );
}

void ThreadedOperation::setThreadedCode( ThreadedCode1 threadedCode1, Operation* parameter1, Operation* parameter2 )
{
	_threadedCode1 = threadedCode1;

	_parameter1 = parameter1;
	_parameter2 = parameter2;
}

void ThreadedOperation::setThreadedCode( ThreadedCode2 threadedCode2, Operation* parameter1, Operation* parameter2, bool parameter3 )
{
	_threadedCode2 = threadedCode2;

	_parameter1 = parameter1;
	_parameter2 = parameter2;
	_parameter3 = parameter3;
}

void ThreadedOperation::run()
{
	if( 0 != _threadedCode1 )
		(_io->*_threadedCode1)( _parameter1, _parameter2 );

	else if( 0 != _threadedCode2 )
		(_io->*_threadedCode2)( _parameter1, _parameter2, _parameter3 );	
}

