#include <iostream.h>
#include <stdlib.h>
#include <dos.h>
#include "pcb.h"
#include "kernel.h"
#include "idleTh.h"
#include "mainTh.h"


int main(int argc, char* argv[]){
	int ret = 0;

	Kernel* system = new Kernel();

	system->makeMainThread(argc, argv);
	system->makeIdleThread();
	system->idleThread->start();


	system->inicSystem();

	system->mainThread->waitToComplete();

	system->restoreSystem();

	ret = system->mainThread->getRetVal();

	delete system;

	return ret;
}

