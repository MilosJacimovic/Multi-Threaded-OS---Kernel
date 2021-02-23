#include <dos.h>

#include "ivtEntry.h"
#include "kernEv.h"
#include "kernel.h"

IVTEntry* IVTEntry::IVTEntries[numOfEntries] = {0};

IVTEntry::IVTEntry(IVTNo ivtNum, pInterrupt Isr){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	this->ivtNumber = ivtNum;
	IVTEntry::IVTEntries[ivtNumber] = this;
	this->myEvent = 0;
	this->newIsr = 0;
	this->oldIsr = 0;


#ifndef BCC_BLOCK_IGNORE
	this->oldIsr=getvect(ivtNum);
	this->newIsr = Isr;
	setvect(ivtNum,Isr);
	unlock
#endif
}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	lock
	setvect(this->ivtNumber, this->oldIsr);
#endif
	IVTEntry::IVTEntries[this->ivtNumber] = 0;
	this->myEvent = 0;
	if(this->oldIsr)
		(*oldIsr)();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}




