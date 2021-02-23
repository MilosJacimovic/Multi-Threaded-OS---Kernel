#include "SCHEDULE.h"
#include "thread.h"
#include "kernel.h"
#include "pcb.h"



Thread::~Thread(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	delete myPCB;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::start(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->start();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::waitToComplete(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB->waitToComplete();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}


ID Thread::getId(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	ID myId = myPCB->getId();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return myId;
}


ID Thread::getRunningId(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	ID myId = Kernel::system->getRunningId();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return myId;
}


Thread* Thread::getThreadById(ID id){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	Thread *th = Kernel::system->getThreadById(id);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
	return th;
}

Thread::Thread(StackSize stackSize, Time timeSlice){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myPCB = new PCB(this, stackSize, timeSlice);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}


void dispatch(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	dispatch_();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}










