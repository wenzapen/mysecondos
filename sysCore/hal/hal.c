#include <hal.h>
#include "cpu.h"

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
    asm volatile("int $3");
}
