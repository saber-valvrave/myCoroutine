#include "ucontext_define.h"

ucontext_t context,contextA,contextB,contextC;

void ucontext_test(){
	ucontext_t context;
	getcontext(&context);
	printf("ucontext_test\n");
	sleep(1);
	setcontext(&context);
}


void coroutineA(){
	printf("coroutine A\n");
	if(swapcontext(&contextA, &contextB) == -1)	handle_error("swapcontext a->b");
	printf("coroutine A over\n");
}
void coroutineB(){
	printf("coroutine B\n");
	if(swapcontext(&contextB, &contextC) == -1)	handle_error("swapcontext a->b");
}
void coroutineC(){
	printf("coroutine C\n");
	//if(swapcontext(&contextC, &contextA) == -1)	handle_error("swapcontext c->b");
}
void coroutine_run(){
		char stackA[1024 * 128] = {'\0'};
		char stackB[1024 * 128] = {'\0'};
		char stackC[1024 * 128] = {'\0'};
		if(-1 == getcontext(&contextA))	handle_error("getcontextA");

		contextA.uc_stack.ss_sp = (void*)stackA;
		contextA.uc_stack.ss_size = sizeof(stackA);
		contextA.uc_link = &context;
		makecontext(&contextA, coroutineA, 0);

		if(-1 == getcontext(&contextB))	handle_error("getcontextB");

		contextB.uc_stack.ss_sp = (void*)stackB;
		contextB.uc_stack.ss_size = sizeof(stackB);
		//contextB.uc_link = &contextC;
		makecontext(&contextB, coroutineB, 0);

		if(-1 == getcontext(&contextC))	handle_error("getcontextC");

		contextC.uc_stack.ss_sp = (void*)stackC;
		contextC.uc_stack.ss_size = sizeof(stackC);
		//contextC.uc_link = &contextA;
		makecontext(&contextC, coroutineC, 0);

		if(swapcontext(&context, &contextA) == -1)	handle_error("swapcontext m->a");
		printf("over\n");
}
