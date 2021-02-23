#include "kernEv.h"
#include "kernel.h"
#include "event.h"
#include "SCHEDULE.H"


KernelEv::KernelEv(Event *event, IVTNo ivtNmbr){
	this->myEvent = event;
	this->ivtNo = ivtNmbr;
	this->myPCB = Kernel::system->running;
	val = 0;
	IVTEntry::IVTEntries[this->ivtNo]->myEvent = this;
}

KernelEv::~KernelEv(){
	if(IVTEntry::IVTEntries[this->ivtNo] != 0)
		IVTEntry::IVTEntries[this->ivtNo]->myEvent = 0;
	this->signal();
}

void KernelEv::wait(){
	if(this->myPCB != Kernel::system->running)
		return;

	if(val==0){
			myPCB->state= Kernel::system->markBlocked();
			val--;
			dispatch();
		}else if(val==1 ){
			val--;
		}
}

void KernelEv::signal(){
	if(val==1) return;

		if(val==0)
			val++;
		else{
			myPCB->state= Kernel::system->markReady();
			Scheduler::put(myPCB);
			val++;
		}
}




