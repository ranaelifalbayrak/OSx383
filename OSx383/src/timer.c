#include "timer.h"
#include "monitor.h"
#include "clock.h"

u32int tick = 0;
#define MAX_TASKS 10
task_t task_queue[MAX_TASKS];
static int task_count = 0;
int running_task = 0;
u8int stacks[MAX_TASKS][1024];

void task_create(task_t *task)
{
    if (task_count < MAX_TASKS)
    {
        // task_t *task = &task_queue[i];
        task->regs.eip = task->task_func;
        task->regs.ebp = stacks[task_count] + 1024;
        task->regs.esp = stacks[task_count] + 1024;
        int i = task_count - 1;
        while (i >= 0 && task_queue[i].priority > task->priority)
        {
            task_queue[i + 1] = task_queue[i];
            i--;
        }
        task_queue[i + 1] = *task;

        task_count++;
    }
}
void schedule()
{
    if (task_count == 0)
        return;

    for (int i = 0; i < task_count; i++)
    {
        task_t task = task_queue[i];
        if (task.ready)
        {
            u32int esp = task.regs.esp;
            u32int ebp = task.regs.ebp;
            u32int eip = task.regs.eip;

            // task_func_t func = (task_func_t)(&task_queue[i])->task_func;
            task.ready = 0;
            running_task = i;
            // func((&task_queue[i])->arg);
            context_switch(eip, ebp, esp);
        }
    }
}

void wait_timer_event(int task_id)
{
    task_t *task = &task_queue[task_id];
    task->ready = 0;
    u32int esp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    u32int ebp;
    asm volatile("mov %%ebp, %0" : "=r"(ebp));
    u32int eip = read_eip();
    // monitor_put('\n');
    // monitor_write_dec(esp);
    if (eip == 0x12345)
    {
        return;
    }
    task->regs.esp = esp;
    task->regs.ebp = ebp;
    task->regs.eip = eip;
    while (task->ready == 0)
    {
        schedule();
    }
}

static void timer_callback(registers_t regs)
{
    tick++;
    write_clock();

    for (int i = 0; i < task_count; i++)
    {
        task_queue[i].countdown--;
        if (task_queue[i].countdown <= 0)
        {
            task_queue[i].countdown = task_queue[i].period;
            task_queue[i].ready = 1;
        }
    }
    if (tick % 10 == 0)
    {
        if (task_count == 0)
        {
            return;
        }
        task_t runningTask = task_queue[running_task];
        runningTask.regs.eip = regs.eip;
        runningTask.regs.esp = regs.esp;
        runningTask.regs.ebp = regs.ebp;
        schedule();
    }
}

void init_timer(u32int frequency)
{
    register_interrupt_handler(IRQ0, &timer_callback);

    u32int divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    u8int l = (u8int)(divisor & 0xFF);
    u8int h = (u8int)((divisor >> 8) & 0xFF);

    outb(0x40, l);
    outb(0x40, h);
}
