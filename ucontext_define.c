#include "ucontext_define.h"
/***********************
说明：
	getcontext：获取当前上下文
	setcontext：设置当前上下文并运行当前上下文
	makecontext：创建当前上下文
	swapcontext：交换上下文，挂起当前上下文，调度到被交换的上下文。

*************************/


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
	if(swapcontext(&contextA, &contextB) == -1){
		handle_error("swapcontext a->b");
	}else{
		printf("swapcontextA\n");
	}
	printf("coroutine A over\n");
}
void coroutineB(){
	printf("coroutine B\n");
	if(swapcontext(&contextB, &contextC) == -1){
		handle_error("swapcontext a->b");
	}else{
		printf("swapcontextB\n");
	}
	printf("coroutine B over\n");
}
void coroutineC(){
	printf("coroutine C\n");
	//if(swapcontext(&contextC, &contextA) == -1)	handle_error("swapcontext c->b");
	printf("coroutine C over\n");
}
void coroutine_run(){
		char stackA[1024 * 128] = {'\0'};
		char stackB[1024 * 128] = {'\0'};
		char stackC[1024 * 128] = {'\0'};
		//获取当前上下文
		if(-1 == getcontext(&contextA))	handle_error("getcontextA");
		//设置上下文环境函数运行栈空间，根据需要设置当前上下文运行结束要恢复的上下文
		contextA.uc_stack.ss_sp = (void*)stackA;
		contextA.uc_stack.ss_size = sizeof(stackA);
		contextA.uc_link = &context;//在函数运行结束后恢复到其指向的上下文环境运行
		//创建并绑定欲执行的函数到该上下文环境中
		makecontext(&contextA, coroutineA, 0);

		if(-1 == getcontext(&contextB))	handle_error("getcontextB");

		contextB.uc_stack.ss_sp = (void*)stackB;
		contextB.uc_stack.ss_size = sizeof(stackB);
		contextB.uc_link = &contextA;
		makecontext(&contextB, coroutineB, 0);

		if(-1 == getcontext(&contextC))	handle_error("getcontextC");

		contextC.uc_stack.ss_sp = (void*)stackC;
		contextC.uc_stack.ss_size = sizeof(stackC);
		contextC.uc_link = &contextB;
		makecontext(&contextC, coroutineC, 0);
		//两个功能：挂起当前上下文，调度到指定的上下文运行
		if(swapcontext(&context, &contextA) == -1)	handle_error("swapcontext m->a");
		printf("over\n");
}
