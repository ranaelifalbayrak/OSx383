#ifndef TIMER_H
#define TIMER_H

#include "common.h"
#include "isr.h"
typedef void (*task_func_t)(void*);
typedef struct task {
    task_func_t task_func;
    void *arg;
    u32int stack_size;
    u32int period; // in microseconds
    u8int priority;
    int countdown;
    int ready;
    registers_t regs;
} task_t;


void init_timer(u32int frequency);
extern u32int tick;


#endif
