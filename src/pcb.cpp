#include <iostream.h>
#include <stdio.h>
#include <dos.h>

#include "thread.h"
#include "pcb.h"
#include "list.h"
#include "kernsem.h"
#include "idleTh.h"
#include "kernel.h"

#include "SCHEDULE.H"


PCB::PCB(Thread *me, StackSize stackSize, Time timeSlice){
	this->next = 0;
	this->prev = 0;
	this->system = 0;
	this->system = Kernel::system;
	system->addPCB(this);


	this->myID = system->getAndIncId();
	this->me = me;
	blockedThreads = new List();

	this->state = system->markMade();
		if(system->makingIdle){
			system->idlePCB = this;
			system->makingIdle = 0;
		}

	if(stackSize > maxStackSize)
		stackSize = maxStackSize;
	this->stackSize = stackSize/sizeof(unsigned int);
	this->timeSlice = timeSlice;
	if(this->timeSlice == 0)
		this->noTimeLimit = 1;
	else
		this->noTimeLimit = 0;
	this->cntr = timeSlice;
	this->stack = 0;

	this->ss = 0;
	this->sp = 0;
	this->bp = 0;
	this->semVal = 0;
	this->hasMaxWaitingLimit = 0;
	this->maxWaitingLimit = 0;
	this->blockedOnSem = 0;
	this->blockedOn = 0;
}


PCB::~PCB(){
	system->deletePCB(this);
	delete blockedThreads;
	delete[] stack;
}


void PCB::initThread(){
	stack = new unsigned[stackSize];

#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 1] = FP_SEG(this->me);
	stack[stackSize - 2] = FP_OFF(this->me);
#endif

	stack[stackSize - 5] = 0x200;

	//(&wrapper) || PCB::wrapper
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 6] = FP_SEG(PCB::wrapper);
	stack[stackSize - 7] = FP_OFF(PCB::wrapper);
#endif

	/*
	this->sp = FP_OFF(&stack[stackSize - 16]);
	this->ss = FP_SEG(&stack[stackSize - 16]);
	 */

#ifndef BCC_BLOCK_IGNORE
	this->ss = FP_SEG(stack + stackSize - 16);
	this->sp = FP_OFF(stack + stackSize - 16);
#endif

	this->bp = sp;
}

void PCB::wrapper(Thread *me){
	me->run();


#ifndef BCC_BLOCK_IGNORE
	lock
#endif

	List::Elem *tmp = me->myPCB->blockedThreads->first;

	while(tmp != 0){
		PCB *th = tmp->pcb;
		th->state = Kernel::system->markReady();
		Scheduler::put(th);
		tmp = tmp->next;
	}

	me->myPCB->state = Kernel::system->markDone();
	delete 	me->myPCB->blockedThreads;
	me->myPCB->blockedThreads = 0;


#ifndef BCC_BLOCK_IGNORE
	unlock
#endif

	dispatch_();
}


void PCB::start(){
	if(this->state != MADE) return;
	this->state = system->markReady();
	this->initThread();

	if(this != system->idlePCB)
		Scheduler::put(this);
}


void PCB::waitToComplete(){
	if(this->state == MADE) return;
	if(this->state == DONE) return;
	if(this == system->running) return;
	if(this == system->idlePCB) return;

	system->running->state = system->markBlocked();
	blockedThreads->addElem(system->running);
	dispatch_();
}

ID PCB::getId(){
	return myID;
}



