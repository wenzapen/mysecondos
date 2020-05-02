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

extern void  isr0(void);
extern void  isr1(void);
extern void  isr2(void);
extern void  isr3(void);
extern void  isr4(void);
extern void  isr5(void);
extern void  isr6(void);
extern void  isr7(void);
extern void  isr8(void);
extern void  isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void  irq0(void);
extern void  irq1(void);
extern void  irq2(void);
extern void  irq3(void);
extern void  irq4(void);
extern void  irq5(void);
extern void  irq6(void);
extern void  irq7(void);
extern void  irq8(void);
extern void  irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);


#endif
