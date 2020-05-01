#include <hal.h>
#include "cpu.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"

extern void _geninterrupt(int n);

int hal_initialize() {
    i86_cpu_initialize();
    i86_pic_initialize(0x20, 0x28);
    i86_pit_initialize();
    i86_pit_start_counter(100, I86_PIT_OCW_COUNTER_0, I86_PIT_OCW_MODE_SQUAREWAVEGEN);
    enable();
    return 0;
}

int hal_shutdown() {
    i86_cpu_shutdown();
    return 0;
}
/*
void geninterrupt(int n) {
    asm volatile ("mov %0, %%eax;int %%al" : : "r"(n) : "%eax");
}
*/

void geninterrupt(int n) {
//   asm volatile("int $3");
/*    asm volatile("movb %0, %%al": : "m"(n)); 
    asm volatile("mov
    asm volatile("mov %al, [genint+1]"); 
    asm volatile("jmp genint"); 
    asm volatile("genint:int $0");
*/
    _geninterrupt(n);

}

void interruptdone(unsigned int intno) {
    if(intno > 16) return;
    if(intno >=8)
	i86_pic_send_command(I86_PIC_OCW2_MASK_EOI,1);
    i86_pic_send_command(I86_PIC_OCW2_MASK_EOI,0);
}

unsigned char inportb(unsigned short portid) {
    unsigned char ret;
    asm volatile("in %%dx, %%al" : "=a"(ret) : "d"(portid));
    return ret;
     
}
 void outportb(unsigned short portid, unsigned char value) {
    asm volatile("out %%al, %%dx" : : "=a"(value) , "d"(portid));
}

void enable() {
    asm("sti");
}

void disable() {
    asm("cli");
}

void setvect(int intno, (void &vect)()) {
    i86_install_ir(intno, 0x8e, 0x8, vect);
} 

const char* get_cpu_vender() {
    return i86_cpu_get_vender();
}

int get_tick_count() {
    return i86_pit_get_tick_count();
}
