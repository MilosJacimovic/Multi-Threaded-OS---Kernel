#include "kernsem.h"
#include "pcb.h"
#include "SCHEDULE.H"

KernelSem *KernelSem::kernelSemHead = 0;

KernelSem::KernelSem(int init){
	if(init < 0){
		val = 0;
	}
	else {
		val = init;
	}

	blocked = new List();

	this->next = kernelSemHead;
	this->prev = 0;

	if(kernelSemHead)
		kernelSemHead->prev = this;

	kernelSemHead = this;
}

KernelSem::~KernelSem(){
	if(this->next)
		this->next->prev = this->prev;
	if(this->prev)
		this->prev->next = this->next;
	else
		kernelSemHead = this->next;

	while(val < 0)
		signal();

	delete blocked;
}

void KernelSem::standardWait(){
	if(--val<0){
				Kernel::system->running->state = Kernel::system->markBlocked();
				blocked->addElem(Kernel::system->running);
				if(Kernel::system->running->hasMaxWaitingLimit == 1){
					Kernel::system->running->blockedOnSem = 1;
					Kernel::system->running->blockedOn = this; //je l ovako
				}
				dispatch();
				}
}
int KernelSem::wait(Time maxTimeToWait){
	if(maxTimeToWait == 0){
		Kernel::system->running->hasMaxWaitingLimit = 0;
			standardWait();
			return 1;
			}
		else{
			Kernel::system->running->hasMaxWaitingLimit = 1;
			Kernel::system->running->maxWaitingLimit = maxTimeToWait;
			standardWait();
			return 0;
		}
}

void KernelSem::standardSignal(){
	if(val++<0){
		PCB* pcb = blocked->getElem();
		if(pcb!=0){
			pcb->blockedOnSem = 0;
			pcb->blockedOn = 0;
			pcb->hasMaxWaitingLimit = 0;
			pcb->maxWaitingLimit = 0;
			pcb->state = READY;
			Scheduler::put(pcb);
		}
	}
}
int KernelSem::signal(int n){
	int i, toUnblock = 0;
		if(n == 0){
			standardSignal();
			return 0;
			}
		else if(n > 0){
			if(val < 0){
				int numOfBlocked = (-1)*(val);
				if(numOfBlocked < n)
					toUnblock = numOfBlocked;
				else
					toUnblock = n;

				for(i = 0; i < toUnblock; i++)
					standardSignal();

				n = n - i;
			}
			val = val + n;
			return toUnblock;
		}
		else return n;

}







