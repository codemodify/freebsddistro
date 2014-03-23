#ifndef threaded_operation_h
#define threaded_operation_h

// Qt headers
#include <QtCore/QThread>

// some forwards
class FileIoEngine;
class Operation;

class ThreadedOperation : public QThread
{
    Q_OBJECT

	public:
		ThreadedOperation( FileIoEngine* io );
		
		void start();

        Operation* getParameter1(){ return _parameter1; }

	public:
		typedef void (FileIoEngine::*ThreadedCode1)(Operation*,Operation*);
		typedef void (FileIoEngine::*ThreadedCode2)(Operation*,Operation*,bool);

		void setThreadedCode( ThreadedCode1 threadedCode1, Operation* parameter1, Operation* parameter2 );
		void setThreadedCode( ThreadedCode2 threadedCode2, Operation* parameter1, Operation* parameter2, bool parameter3 );

	protected:
		void run();

	private:
		FileIoEngine* _io;

		ThreadedCode1   _threadedCode1;
		ThreadedCode2   _threadedCode2;
		Operation*      _parameter1;
		Operation*      _parameter2;
		bool            _parameter3;
};

#endif

