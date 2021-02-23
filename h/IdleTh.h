#ifndef IDLETH_H_
#define IDLETH_H_

class IdleThread:public Thread{
	public:
		void run(){
			while(1) dispatch();
		}
};




#endif /* IDLETH_H_ */
