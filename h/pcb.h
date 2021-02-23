#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "kernel.h"

class Kernel;
class MainThread;
class Thread;
class IdleThread;
class List;
class KernelSem;


class PCB{
	public:
			PCB(Thread *me, StackSize stackSize, Time timeSlice);
			~PCB();

			Kernel* system;

			PCB *next, *prev;


			void initThread();
			static void wrapper(Thread *me);
			void start();
			void waitToComplete();

			ID getId();

			ID myID;
			Thread *me;
			volatile int cntr;
			volatile int noTimeLimit;
			List *blockedThreads;
			StackSize stackSize;
			Time timeSlice;
			unsigned int *stack;
			unsigned int ss, sp;
			unsigned int bp;
			int state;

			KernelSem *blockedOn;
			int blockedOnSem;
			int hasMaxWaitingLimit;
			Time maxWaitingLimit;
			int semVal;

			friend class Thread;
			friend class List;
			friend class KernelSem;
			friend class Kernel;


};



#endif /* PCB_H_ */
