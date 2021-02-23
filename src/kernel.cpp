#include <iostream.h>
#include <stdio.h>
#include <dos.h>

#include "thread.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "list.h"
#include "kernsem.h"
#include "idleTh.h"
#include "mainTh.h"

unsigned volatile int tss, tsp, tbp;
unsigned oldTimerSEG, oldTimerOFF;

Kernel* Kernel::system = 0;



void tick();


Kernel::Kernel(){
	this->made = 1;
	this->pcbHead = 0;
	this->pcbTail = 0;
	this->cntID = 0;
	this->mainThread = 0;
	this->running = 0;
	this->idlePCB = 0;
	this->idleThread = 0;
	this->makingIdle = 0;
	this->context_switch_on_demand = 0;
	Kernel::system = this;
}

Kernel::~Kernel(){
	delete	mainThread;
	deleteIdle();
	delete running;
}

void Kernel::makeMainThread(int argc, char* argv[]){
	running = new PCB(0,defaultStackSize, defaultTimeSlice);
	running->state = Kernel::markReady();
	mainThread=new MainThread(argc,argv);
	mainThread->start();
}

void Kernel::makeIdleThread(){
	makeIdle();
}

int Kernel::getAndIncId(){
	int ret = Kernel::cntID;
	Kernel::cntID = Kernel::cntID + 1;
	return ret;
}

void Kernel::addPCB(PCB *pcb){
	if(!pcbHead){
		pcbHead = pcb;
		pcbHead->next = 0;
		pcbHead->prev = 0;
		pcbTail = pcbHead;
		pcbTail->next = 0;
		pcbTail->prev = 0;
	}
	else{
		pcb->prev = pcbTail;
		pcb->next = 0;
		if(pcbTail)
			pcbTail->next = pcb;
		pcbTail = pcb;
		pcbTail->next = 0;
	}
}

void Kernel::deletePCB(PCB *pcb){
	pcb->state = Kernel::markDone();

		if(pcb->prev)
			pcb->prev->next = pcb->next;
		else
			pcbHead = pcb->next;

		if(pcb->next)
			pcb->next->prev = pcb->prev;
		else
			pcbTail = pcb->prev;
}

ID Kernel::getRunningId(){
	return running->getId();
}

Thread *Kernel::getThreadById(ID id){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif


	PCB *tmp = getPcbById(id);
	Thread *th;
	if(tmp != 0)
		th = tmp->me;
	else th = 0;


#ifndef BCC_BLOCK_IGNORE
	unlock
#endif

	return th;
}

PCB *Kernel::getPcbById(ID id){
	PCB *tmp = this->pcbHead;
	while(tmp){
		if(tmp->myID == id) return tmp;
		tmp = tmp->next;
	}
	return 0;
}

void Kernel::makeIdle(){
	makingIdle = 1;
	idleThread = new IdleThread();
}

void Kernel::deleteIdle(){
	delete idleThread;
	idlePCB = 0;
	idleThread = 0;
}

void Kernel::inicSystem(){
#ifndef BCC_BLOCK_IGNORE
	lock
	asm{
			push es
			push ax

			mov ax,0
			mov es,ax // es = 0

			// pamti staru rutinu
			mov ax, word ptr es:0022h
			mov word ptr oldTimerSEG, ax
			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFF, ax


			// postavlja novu rutinu
			mov word ptr es:0022h, seg timer
			mov word ptr es:0020h, offset timer


			// postavlja staru rutinu na int 60h
			mov ax, oldTimerSEG
			mov word ptr es:0182h, ax
			mov ax, oldTimerOFF

			mov word ptr es:0180h, ax

			pop ax
			pop es
		}
	unlock
#endif
}

void Kernel::restoreSystem(){
#ifndef BCC_BLOCK_IGNORE
	lock
	asm {
			push es
			push ax

			mov ax,0
			mov es,ax

			mov ax, word ptr oldTimerSEG
			mov word ptr es:0022h, ax
			mov ax, word ptr oldTimerOFF
			mov word ptr es:0020h, ax

			pop ax
			pop es
		}
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Kernel::deblockThreads(){
	PCB *tmp = this->pcbHead;

	while(tmp != 0){
		if(tmp->state == BLOCKED && tmp->blockedOnSem == 1 && tmp->hasMaxWaitingLimit == 1){
			if(--tmp->maxWaitingLimit == 0){
				tmp->blockedOn->val++;
				tmp->blockedOn->blocked->deleteElemById(tmp->myID);
				tmp->blockedOn = 0;
				tmp->hasMaxWaitingLimit = 0;
				tmp->state = Kernel::markReady();
				Scheduler::put(tmp);
			}
		}
		tmp = tmp->next;
	}
}



void dispatch_(){
	lock
#endif
	//cout << "Nit " << PCB::running->me->getId() << " je trazila DISPATCH\n";
	Kernel::system->context_switch_on_demand = 1;
	timer();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}


void interrupt timer(...){
	if(Kernel::system->context_switch_on_demand != 1){
		Kernel::system->running->cntr--;
		Kernel::system->deblockThreads();
		tick();
		}


	if((Kernel::system->running->cntr==0 && Kernel::system->running->noTimeLimit == 0) || Kernel::system->context_switch_on_demand){

		#ifndef BCC_BLOCK_IGNORE
			asm{
				mov tss, ss
				mov tsp, sp
				mov tbp, bp
			}
		#endif

			Kernel::system->running->ss = tss;
			Kernel::system->running->sp = tsp;
			Kernel::system->running->bp = tbp;


			if(Kernel::system->running->state != BLOCKED && Kernel::system->running->state != DONE && Kernel::system->running != Kernel::system->idlePCB){
				//cout << "Nit " << PCB::running->me->getId() << " se stavila u SCHEDULER!\n";
				Scheduler::put(Kernel::system->running);
			}

			Kernel::system->running = Scheduler::get();

			if(Kernel::system->running == 0)
				Kernel::system->running = Kernel::system->idlePCB;

			tss = Kernel::system->running->ss;
			tsp = Kernel::system->running->sp;
			tbp = Kernel::system->running->bp;

		#ifndef BCC_BLOCK_IGNORE
			asm {
				mov ss, tss
				mov sp, tsp
				mov bp, tbp
			}
		#endif

			Kernel::system->running->cntr = Kernel::system->running->timeSlice;
	}

	if(!Kernel::system->context_switch_on_demand) {
#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
#endif
	}
	Kernel::system->context_switch_on_demand = 0;

}

















