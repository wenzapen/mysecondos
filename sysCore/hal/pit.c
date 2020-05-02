#include <hal.h>
#include "idt.h"
#include "pit.h"
#include "pic.h"

#ifdef _DEBUG
#include "../kernel/debugDisplay.h"
#endif


#define I86_PIT_REG_COUNTER_0 0x40
#define I86_PIT_REG_COUNTER_1 0x41
#define I86_PIT_REG_COUNTER_2 0x42
#define I86_PIT_REG_COMMAND 0x43

volatile uint32_t _pit_ticks=0;
static int _pit_bIsInit=0;

void i86_pit_irq();

void i86_pit_irq() {

//    asm volatile("add $12,%esp");
    asm volatile("cli");
    asm volatile ("pusha");
    _pit_ticks++;
    debugPrintf("*** [i86 Hal] PIT_Ticks: %d\n ", _pit_ticks);
    interruptdone(0);
    asm volatile ("popa");
//    asm volatile("add $4,%esp");
    asm volatile("sti");
    asm volatile("iret");

}

uint32_t i86_pit_set_tick_count(uint32_t i) {
    uint32_t ret = _pit_ticks;
    _pit_ticks = i;
    return ret;

}

uint32_t i86_pit_get_tick_count() {
    return _pit_ticks;
}

void i86_pit_send_command(uint8_t cmd) {
    outportb(I86_PIT_REG_COMMAND, cmd);
}

void i86_pit_send_data(uint16_t data, uint8_t counter) {
    uint8_t port = (counter==I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER_0:((counter==I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER_1:I86_PIT_REG_COUNTER_2);
    outportb(port, (uint8_t)data);
}

uint8_t i86_pit_read_data(uint8_t counter) {
    uint8_t port = (counter==I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER_0:((counter==I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER_1:I86_PIT_REG_COUNTER_2);
    return inportb(port);
}

void i86_pit_start_counter(uint32_t freq, uint8_t counter, uint8_t mode) {
    if(freq==0) return;
    uint16_t divisor = (uint16_t)(1193181 / (uint16_t)freq);
    uint8_t ocw=0;
    ocw = (ocw &~I86_PIT_OCW_MASK_MODE) | mode;
    ocw = (ocw &~I86_PIT_OCW_MASK_RL) | I86_PIT_OCW_RL_DATA;
    ocw = (ocw &~I86_PIT_OCW_MASK_COUNTER) | counter;
    i86_pit_send_command(ocw);

    i86_pit_send_data(divisor & 0xff, 0);
    i86_pit_send_data((divisor >> 8) & 0xff, 0);

    _pit_ticks = 0;

}

void i86_pit_initialize() {
//    setvect(32, i86_pit_irq);
    _pit_bIsInit = 1;
}

int i86_pit_is_initialized() {
    return _pit_bIsInit;
}
