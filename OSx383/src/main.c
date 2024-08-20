#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "common.h"


void Task1(void *arg)
{
    // set_timer_periodic(2000);
    // u32int tickk = tick;
    int i = 0;
    while (1)
    {
        //char *i = (char *)arg;
        asm volatile("cli");
        // monitor_put(*i);
        monitor_write_dec(i++);
        monitor_write("deneme1  ");
        asm volatile("sti"); 
        // if(tick >= tickk+10){
        wait_timer_event(2);
        //}
    }
}
void Task2(void *arg)
{
    // set_timer_periodic();
    while (1)
    {
        asm volatile("cli");
        monitor_write("deneme2  ");
        asm volatile("sti");
        wait_timer_event(0);
    }
}
void Task3(void *arg)
{
    while (1)
    {
        asm volatile("cli");
        monitor_write("deneme3  ");
        asm volatile("sti");
        wait_timer_event(1);
    }
}
int main(struct multiboot *mboot_ptr)
{
    init_descriptor_tables();
    monitor_clear();
    monitor_write_with_colors("Welcome to OSx383.\n", 0x0, 0xA);
    keyboard_install();

    char arg = 'a';
    task_t task1 = {Task1, &arg, 1024, 1, 3, 0, 1};
    task_create(&task1);
    task_t task2 = {Task2, &arg, 1024, 2, 1, 0, 1};
    task_create(&task2);
    task_t task3 = {Task3, &arg, 1024, 7, 2, 0, 1};
    task_create(&task3);

    // initialize_stacks();
    // monitor_put('\n');
    // monitor_write_dec(task2.regs.esp);
    asm volatile("sti");
    init_timer(1000);
    context_switch(task2.regs.eip, task2.regs.ebp, task2.regs.esp);
    
    // while(1){
    //     schedule();
    // }

    return 0xDEADBABA;
}
