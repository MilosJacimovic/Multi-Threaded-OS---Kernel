#ifndef KERNSEM_H_
#define KERNSEM_H_

#include "list.h"
#include "semaphor.h"

class KernelSem{
	public:
		KernelSem(int init = 1);
		~KernelSem();
		int wait(Time maxTimeToWait);
		void standardWait();
		int signal(int n = 0);
		void standardSignal();

		int val; //napravi da bude private sa geterima i seterima
		List *blocked;
		static KernelSem* kernelSemHead;
		KernelSem *next, *prev;
};




#endif /* KERNSEM_H_ */
