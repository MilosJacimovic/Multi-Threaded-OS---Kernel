#include "list.h"
#include "pcb.h"
#include "kernsem.h"
#include "SCHEDULE.H"

List::List():first(0), last(0){}

List::~List(){
	Elem *tmp = first;

	while(tmp != 0){
		first = first->next;
		delete tmp;
		tmp = first;
	}

	last = 0;
}

void List::addElem(PCB *pcb){
	Elem *newElem = new Elem(pcb);

	if(first == 0)
		first = newElem;
	else
		last->next = newElem;

	last = newElem;
}

void List::deleteElemById(ID id){
	Elem *tmp = first;
	Elem *prev = 0;

	while(tmp!=0 && tmp->pcb->myID!=id){
		prev = tmp;
		tmp = tmp->next;
	}

	if(tmp!=0){
		if(prev ==0){
			first = tmp->next;
			if(first == 0)
				last = 0;
		}
		else
			prev->next = tmp->next;

		if(tmp == last)
			last = prev;
		delete tmp;
	}
}



PCB *List::getElem(){
	Elem *tmp = first;
	if(tmp == 0) return 0;
		first = first->next;

	if(first == 0) last = 0;
		PCB *pcb = tmp->pcb;

	delete tmp;
	return pcb;
}







