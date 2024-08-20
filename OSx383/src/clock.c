#include "common.h"
#include "clock.h"

#define RTCaddress 0x70
#define RTCdata 0x71
// http://docs.huihoo.com/help-pc/hw-CMOS_RAM.html
//https://wiki.osdev.org/CMOS

u8int currentTime[6];
unsigned char get_RTC_register(u8int reg) {
      outb(RTCaddress, reg);
      return inb(RTCdata);
}
static void timer_getTime()
{
  u8int tvalue, regB;
  outb(RTCaddress, 0x0B);
  regB = inb(RTCdata);
  regB = regB | 0x06;
  outb(RTCaddress, 0x0B);
  outb(RTCdata, regB);
 
    tvalue = get_RTC_register(0);
    currentTime[0] = (tvalue % 10) + 48;
    currentTime[1] = (tvalue / 10) + 48;
    tvalue = get_RTC_register(2);
    currentTime[2] = (tvalue % 10) + 48;
    currentTime[3] = (tvalue / 10) + 48;
    tvalue = get_RTC_register(4);
    currentTime[4] = (tvalue % 10) + 48;
    currentTime[5] = (tvalue / 10) + 48;
  }

extern cursor_x;
extern cursor_y;
u8int cursor_x_temp;
u8int cursor_y_temp;

void write_clock()
{
  int index;
  timer_getTime();
  cursor_x_temp = cursor_x;
  cursor_y_temp = cursor_y;
  cursor_x = 72;
  for (index = 5; index >= 0; index--)
  {
    monitor_put_with_colors(currentTime[index], 0x7, 0xE);
    if (index % 2 == 0 & index != 0)
    {
      monitor_put_with_colors(':', 0x7, 0xE);
    }
  }
  cursor_x = cursor_x_temp;
  cursor_y = cursor_y_temp;
  move_cursor();
}

void setTime(u8int h, u8int m, u8int s)
{
  u8int index, regB;
  u32int i;

  outb(RTCaddress, 0xb);
  regB = inb(RTCdata);
  regB = regB | 0x06;
  outb(RTCaddress, 0x0B);
  outb(RTCdata, regB);
  outb(RTCaddress, 0);
  outb(RTCdata, s);
  outb(RTCaddress, 2);
  outb(RTCdata, m);
  outb(RTCaddress, 4);
  outb(RTCdata, h);
  cursor_x_temp = cursor_x;
  cursor_y_temp = cursor_y;
  cursor_x = 0;
  cursor_y = cursor_y_temp;
  monitor_write("Time has been set to: ");
  monitor_put(h / 10 + 48);
  monitor_put(h % 10 + 48);
  monitor_put(':');
  monitor_put(m / 10 + 48);
  monitor_put(m % 10 + 48);
  monitor_put(':');
  monitor_put(s / 10 + 48);
  monitor_put(s % 10 + 48);
  monitor_put('\n');
  cursor_x = cursor_x_temp;
  cursor_y = cursor_y_temp + 1;
}
