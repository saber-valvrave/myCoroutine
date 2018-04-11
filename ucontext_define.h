#ifndef _UCONTEXT_DEFINE_H__
#define _UCONTEXT_DEFINE_H__

#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>


extern ucontext_t context,contextA,contextB,contextC;

 #define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


extern void ucontext_test();
extern void coroutineA();
extern void coroutineB();
extern void coroutineC();
extern void coroutine_run();

#endif
