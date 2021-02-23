#ifndef LIST_H_
#define LIST_H_

#include "thread.h"
class PCB;
class Kernel;
class KernelSem;

class List{
	public:
			List();
			~List();

			struct Elem{
				Elem(PCB *threadPCB){
					pcb = threadPCB;
					next = 0;
				}
				PCB *pcb;
				Elem *next;
			};

			Elem *first, *last;

			void addElem(PCB *pcb);
			void deleteElemById(ID id);
			PCB *getElem();
};

#endif /* LIST_H_ */
