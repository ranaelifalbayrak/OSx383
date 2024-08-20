#include "isr.h"
#include "monitor.h"
#include "shell.h"
#include "timer.h"
#include "common.h"
#include "shell.h"

extern cursor_x;
extern cursor_y;
u8int cursor_x_tmp;
u8int cursor_y_tmp;

void delay(int milliseconds)
{
    int count = 0;
    while (count < milliseconds * 100000)
    {
        count++;
    }
}

static void setTimeHelper(char *c)
{
    u8int h, m, s;
    h = (c[1] - 48) * 10 + (c[2] - 48);
    m = (c[4] - 48) * 10 + (c[5] - 48);
    s = (c[7] - 48) * 10 + (c[8] - 48);
    if (((c[0] != ' ') || (c[3] != ' ')) || (c[6] != ' '))
    {
        cursor_x_tmp = cursor_x;
        cursor_y_tmp = cursor_y;
        cursor_x = 0;
        cursor_y = cursor_y_tmp;
        monitor_write("Usage: settime hh mm ss\n");
        cursor_x = cursor_x_tmp;
        cursor_y = cursor_y_tmp + 1;
    }
    else if (h < 24 && (m < 60 && s < 60))
    {

        setTime(h, m, s);
        c = '\0';
    }
    else
    {
        cursor_x_tmp = cursor_x;
        cursor_y_tmp = cursor_y;
        cursor_x = 0;
        cursor_y = cursor_y_tmp;
        monitor_write("Usage: settime hh mm ss\n");
        cursor_x = cursor_x_tmp;
        cursor_y = cursor_y_tmp + 1;
    }
}

void command(char *c)
{
    u8int i = 0;
    u8int c1[7];
    u8int c2[9];
    u8int c3[3] = "";

    c3[0] = c[0];
    c3[1] = c[1];
    c3[2] = c[2];

    if (strcmp(c, "help") == 0)
    {
        cursor_x_tmp = cursor_x;
        cursor_y_tmp = cursor_y;
        cursor_x = 0;
        cursor_y = cursor_y_tmp;
        monitor_write("Available commands: help, settime, reboot\n");
        cursor_x = cursor_x_tmp;
        cursor_y = cursor_y_tmp + 1;
        move_cursor();
    }
    else if (strcmp(c, "reboot") == 0)
    {
        cursor_x_tmp = cursor_x;
        cursor_y_tmp = cursor_y;
        cursor_x = 0;
        cursor_y = cursor_y_tmp;
        monitor_write("Rebooting...\n");
        cursor_x = cursor_x_tmp;
        cursor_y = cursor_y_tmp + 1;
        delay(10000);

        asm volatile(
            "wait1:\n"
            "inb $0x64, %%al\n"   // in al, 0x64
            "testb $0x02, %%al\n" // test al, 00000010b
            "jne wait1\n"

            "movb $0xFE, %%al\n" // mov al, 0xFE
            "outb %%al, $0x64\n" // out 0x64, al
            :
            :
            : "al");
    }
    else
    {
        for (i = 0; i < 7; i++)
        {
            c1[i] = c[i];
            c2[i] = c[i + 7];
        }
        c1[i] = 0;
        c2[7] = c[14];
        c2[8] = c[15];
        if (strcmp(c1, "settime") == 0)
        {
            setTimeHelper(c2);
        }
        else
        {
            cursor_x_tmp = cursor_x;
            cursor_y_tmp = cursor_y;
            cursor_x = 0;
            cursor_y = cursor_y_tmp;
            int size = 0;
            for(int i = 0; c[i] != '\0'; i++){
                size++;
            }
            monitor_write(c);
            monitor_write(" is an invalid command. Type help for a list of avaliable commands.\n");
            cursor_x = cursor_x_tmp;
            cursor_y = cursor_y_tmp + ((int)size/80) +1;
            move_cursor();
        }
    }
}