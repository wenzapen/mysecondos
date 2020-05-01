#include <hal.h>
#include "pic.h"

// controller registers

//! PIC 1 register port address
#define I86_PIC1_REG_COMMAND 0x20
#define I86_PIC1_REG_STATUS 0x20
#define I86_PIC1_REG_DATA 0x21
#define I86_PIC1_REG_IMR 0x21

//! PIC 2 register port address
#define I86_PIC1_REG_COMMAND 0xA0
#define I86_PIC1_REG_STATUS 0xA0
#define I86_PIC1_REG_DATA 0xA1
#define I86_PIC1_REG_IMR 0xA1

// initialization command bit masks

//! initialization control word 1 bit masks
#define I86_ICW1_MASK_IC4 0x1
#define I86_ICW1_MASK_SNGL 0x2
#define I86_ICW1_MASK_ADI 0x4
#define I86_ICW1_MASK_LTIM 0x8
#define I86_ICW1_MASK_INIT 0x10

//! initialization control word 4 bit masks
#define I86_ICW4_MASK_UPM 0x1
#define I86_ICW4_MASK_AEOI 0x2
#define I86_ICW4_MASK_MS 0x4
#define I86_ICW4_MASK_BUF 0x8
#define I86_ICW4_MASK_SFNM 0x10

//-----------------------------------------------
//	Initialization Command 1 control bits
//-----------------------------------------------

#define I86_PIC_ICW1_IC4_EXPECT				1			//1
#define I86_PIC_ICW1_IC4_NO					0			//0
#define I86_PIC_ICW1_SNGL_YES				2			//10
#define I86_PIC_ICW1_SNGL_NO				0			//00
#define I86_PIC_ICW1_ADI_CALLINTERVAL4		4			//100
#define I86_PIC_ICW1_ADI_CALLINTERVAL8		0			//000
#define I86_PIC_ICW1_LTIM_LEVELTRIGGERED	8			//1000
#define I86_PIC_ICW1_LTIM_EDGETRIGGERED		0			//0000
#define I86_PIC_ICW1_INIT_YES				0x10		//10000
#define I86_PIC_ICW1_INIT_NO				0			//00000

//-----------------------------------------------
//	Initialization Command 4 control bits
//-----------------------------------------------

#define I86_PIC_ICW4_UPM_86MODE			1			//1
#define I86_PIC_ICW4_UPM_MCSMODE		0			//0
#define I86_PIC_ICW4_AEOI_AUTOEOI		2			//10
#define I86_PIC_ICW4_AEOI_NOAUTOEOI		0			//0
#define I86_PIC_ICW4_MS_BUFFERMASTER	4			//100
#define I86_PIC_ICW4_MS_BUFFERSLAVE		0			//0
#define I86_PIC_ICW4_BUF_MODEYES		8			//1000
#define I86_PIC_ICW4_BUF_MODENO			0			//0
#define I86_PIC_ICW4_SFNM_NESTEDMODE	0x10		//10000
#define I86_PIC_ICW4_SFNM_NOTNESTED		0			//a binary 2 (futurama joke hehe ;)

void i86_pic_send_command(uint8_t cmd, uint8_t picNum) {
    if(picNum > 1) return;
    uint8_t reg = (picNum==1) ? I86_PIC2_REG_COMMAND : I86_PIC1_REG_COMMAND;
    outportb(reg, cmd);

}
void i86_pic_send_data(uint8_t data, uint8_t picNum) {
    if(picNum > 1) return;
    uint8_t reg = (picNum==1) ? I86_PIC2_REG_DATA : I86_PIC1_REG_DATA;
    outportb(reg, data);

}
uint8_t i86_pic_read_data( uint8_t picNum) {
    if(picNum > 1) return;
    uint8_t reg = (picNum==1) ? I86_PIC2_REG_DATA : I86_PIC1_REG_DATA;
    return intportb(reg);

}

void i86_pic_initialize(uint8_t base0, uint8_t base1) {
    uint8_t icw = 0;
    disable();
    icw = (icw & ~I86_PIC_ICW1_MASK_INIT) | I86_PIC_ICW1_INIT_YES;
    icw = (icw & ~I86_PIC_ICW1_MASK_IC4) | I86_PIC_ICW1_IC4_EXPECT;

    i86_pic_send_command(icw, 0);
    i86_pic_send_command(icw, 1);
  
    i86_pic_send_data(base0, 0);
    i86_pic_send_data(base1, 1);
    i86_pic_send_data(0x04, 0);
    i86_pic_send_data(0x02, 1);

    icw = (icw & ~I86_PIC_ICW4_MASK_UPM) | I86_PIC_ICW4_UPM_86MODE;
    i86_pic_send_data(icw, 0);
    i86_pic_send_data(icw, 1);


}
