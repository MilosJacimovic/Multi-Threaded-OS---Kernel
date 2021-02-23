#include "semaphor.h"
#include "kernsem.h"
#include "pcb.h"


Semaphore::Semaphore(int initVal){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myImpl = new KernelSem(initVal);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}


Semaphore::~Semaphore(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

int Semaphore::wait(Time maxTimeToWait){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	int ret = myImpl->wait(maxTimeToWait);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return ret;
}

int Semaphore::signal(int n){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	int ret = myImpl->signal(n);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return ret;
}

int Semaphore::val() const{
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	int ret = myImpl->val;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return ret;
}







