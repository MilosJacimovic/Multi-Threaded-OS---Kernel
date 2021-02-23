#include "event.h"
#include "kernEv.h"
#include "kernel.h"

Event::Event(IVTNo ivtNo){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myImpl = new KernelEv(this, ivtNo);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

Event::~Event(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Event::wait(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myImpl->wait();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Event::signal(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}
