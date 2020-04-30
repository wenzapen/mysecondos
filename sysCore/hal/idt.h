#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

#define I86_MAX_INTERRUPTS  256

typedef void (*I86_IRQ_HANDLER)(void);

struct idt_descriptor {
    uint16_t baseLo;
    uint16_t sel;
    uint8_t reserved;
    uint8_t flags;
    uint16_t baseHi;
}__attribute__((packed));

struct idt_descriptor* i86_get_ir(uint32_t i);
int i86_install_ir(uint32_t i,uint16_t flags,uint16_t sel,I86_IRQ_HANDLER);
int i86_idt_initialize(uint16_t codeSel);

#endif
