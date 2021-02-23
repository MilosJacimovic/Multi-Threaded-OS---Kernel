#ifndef EVENT_H_
#define EVENT_H_

#include "ivtEntry.h"

#define PREPAREENTRY(ivtNo, callOld) \
void interrupt inter##ivtNo(...); \
IVTEntry newEntry##ivtNo(ivtNo, inter##ivtNo); \
void interrupt inter##ivtNo(...) { \
	if (callOld == 1 && newEntry##ivtNo.oldIsr != 0) \
		(*newEntry##ivtNo.oldIsr)(); \
	if (newEntry##ivtNo.myEvent)\
		newEntry##ivtNo.myEvent->signal(); \
	dispatch(); \
}

typedef unsigned char IVTNo;

class KernelEv;

class Event {
	public:
		Event (IVTNo ivtNo);
		~Event ();

		void wait ();

	protected:
		friend class KernelEv;
		void signal(); // can call KernelEv

	private:
		 KernelEv* myImpl;
};


#endif /* EVENT_H_ */
