#ifndef KERNEL_H_
#define KERNEL_H_

#include <string.h>
#include <stdio.h>
#include <iostream.h>

#include "thread.h"
#include "pcb.h"
#include "semaphor.h"

class Thread;
class PCB;
class IdleThread;
class List;
class MainThread;
class KernelSem;

#define BLOCKED -1
#define MADE 0
#define READY 1
#define DONE 2

#define lock asm{\
		pushf;\
		cli;\
}

#define unlock asm popf

typedef void interrupt(*pInterrupt)(...);
void dispatch_();
void interrupt timer(...);

class Kernel{
	public:
				Kernel();
				~Kernel();

				void makeMainThread(int argc, char* argv[]);
				void makeIdleThread();

				void addPCB(PCB *pcb);
				void deletePCB(PCB *pcb);

				int made;
				static Kernel *system;

				PCB *pcbHead;
				PCB *pcbTail;

				ID getRunningId();
				Thread* getThreadById(ID id);
				PCB* getPcbById(ID id);

				int markBlocked() { return -1; }
				int markMade() { return 0; }
				int markReady() { return 1; }
				int markDone(){ return 2; }

				ID cntID;
				ID getAndIncId();

				void deblockThreads();

				MainThread *mainThread;
				PCB *running;

				PCB *idlePCB;
				IdleThread *idleThread;
				int makingIdle;
				void makeIdle();
				void deleteIdle();

				friend class Thread;
				friend class PCB;
				friend class List;
				friend class KernelSem;
				friend class MainThread;

				void inicSystem();
				void restoreSystem();

				volatile int context_switch_on_demand;
				friend void dispatch_();
				friend void interrupt timer(...);



};



#endif /* KERNEL_H_ */
