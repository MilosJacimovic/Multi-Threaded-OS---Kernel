#ifndef KERNEV_H_
#define KERNEV_H_

#include "event.h"
#include "ivtEntry.h"
#include "kernel.h"

class PCB;
class Event;

typedef unsigned char IVTNo;


class KernelEv{
	public:
		KernelEv(Event *myEvent, IVTNo ivtNo);
		~KernelEv();

		void wait();
		void signal();

	private:
		PCB *myPCB;
		Event *myEvent;
		IVTNo ivtNo;
		int val;

};




#endif /* KERNEV_H_ */
