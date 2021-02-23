#include "mainTh.h"

int userMain(int argc, char* argv[]);

MainThread::MainThread(int _argc,char* _argv[]):argc(_argc),argv(_argv){
	retVal = 0;
}

MainThread::~MainThread(){
	waitToComplete();
}

void MainThread::run(){
	retVal =userMain(argc,argv);
}

int MainThread::getRetVal()const{
	return retVal;
}
