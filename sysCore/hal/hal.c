#include <hal.h>
#include "cpu.h"

extern void _geninterrupt(int n);

int hal_initialize() {
    i86_cpu_initialize();
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
