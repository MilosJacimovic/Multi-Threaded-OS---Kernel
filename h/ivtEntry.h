#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "kernel.h"
#include "kernEv.h"
#include "event.h"

class KernelEv;
class Kernel;
class Event;

typedef unsigned char IVTNo;
const int numOfEntries=256;
typedef void interrupt (*pInterrupt)(...);

class IVTEntry{
	public:
		static IVTEntry *IVTEntries[numOfEntries];

		IVTEntry(IVTNo ivtNum, pInterrupt Isr);
		~IVTEntry();



		IVTNo ivtNumber;
		pInterrupt oldIsr, newIsr;
		KernelEv* myEvent;
};




#endif /* IVTENTRY_H_ */
