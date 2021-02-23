#ifndef MAINTH_H_
#define MAINTH_H_

#include "thread.h"

int userMain(int argc, char* argv[]);


class MainThread:public Thread{
public:
	MainThread(int _argc,char* _argv[]);
	virtual ~MainThread();

	void run();
	int getRetVal()const;
	int retVal;

private:
	int argc;
	char** argv;
};



#endif /* MAINTH_H_ */
