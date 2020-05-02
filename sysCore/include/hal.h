#ifndef _HAL_H
#define _HAL_H

int hal_initialize();
int hal_shutdown();
void geninterrupt(int n);
void interruptdone(unsigned int intno);
unsigned char inportb(unsigned short portid);
void outportb(unsigned short portid, unsigned char value);
void enable();
void disable();
void setvect(int intno, void (*vect)());
const char* get_cpu_vender();
int get_tick_count();

#endif
