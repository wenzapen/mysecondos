#include "debugDisplay.h"
#include <hal.h>
#include <bootinfo.h>
#include "mmngr_phys.h"
#include "mmngr_virtual.h"
#include "../keyboard/kybrd.h"

struct memory_region {
    uint32_t startLo;
    uint32_t startHi;
    uint32_t sizeLo;
    uint32_t sizeHi;
    uint32_t type;
    uint32_t acpi_3_0;
};

//! different memory regions(in memory_region.type)
char* strMemoryTypes[] = {
    "Available",    //memory_region.type==0
    "Reserved",
    "ACPI Reclaim",
    "ACPI NVS Memory"  //memory_region.type==3
};

uint32_t kernelSize = 0;
uint32_t size = 0;

void init(struct multiboot_info* bootinfo) {

//! get kernel size passed from boot loader by register edx
    asm volatile("xor %eax, %eax");
    asm volatile("mov %%dx, %%ax":"=a"(kernelSize));

    //! make demo look nice :)
    debugClrScr (0x13);
    debugGotoXY (0,0);
    debugSetColor (0x17);
    debugPrintf ("M.O.S Kernel is starting up...\n ");

    hal_initialize();

//! get memory size in KB
    uint32_t memSize = 1024+bootinfo->m_memoryLo+bootinfo->m_memoryHi*64;

// ! place memory bit map used by PMM at the end of the kernel in memory
    pmmngr_init(memSize, 0xc0000000+kernelSize*512);

//    debugPrintf("pmm initialized with %d KB physical memory; memLo: %d memHi: %d\n\n", memSize, bootinfo->m_memoryLo, bootinfo->m_memoryHi);

//    debugSetColor(0x19);
//    debugPrintf("Physical Memory Map:\n");
    struct memory_region *region = (struct memory_region*)0x1000;

    for (int i=0; i<15; ++i) {
    
    	//! sanity check; if type is > 4 mark it reserved
    	if (region[i].type>4)
    		region[i].type=1;
    
    	//! if start address is 0, there is no more entries, break out
    	if (i>0 && region[i].startLo==0)
    		break;
    
    	//! display entry
//    	debugPrintf ("region %i: start: 0x%x%x length (bytes): 0x%x%x type: %i (%s)\n", i, 
    		region[i].startHi, region[i].startLo,
    		region[i].sizeHi,region[i].sizeLo,
    		region[i].type, strMemoryTypes[region[i].type-1]);
    
    	//! if region is avilable memory, initialize the region for use
    	if (region[i].type==1)
    		pmmngr_init_region (region[i].startLo, region[i].sizeLo);
    }
    
//    debugSetColor (0x17);
    
//    debugPrintf ("\npmm regions initialized: %i allocation blocks; used or reserved blocks: %i\nfree blocks: %i\n",
    	pmmngr_get_block_count (),  pmmngr_get_use_block_count (), pmmngr_get_free_block_count () );
    
//! deinit the region the kernel is in as its in use; deinit 0~1Mb
    pmmngr_deinit_region (0x0, 0x1000);
    pmmngr_deinit_region (0x100000, kernelSize*512);
    
//    debugSetColor (0x17);
    
//    debugPrintf ("\npmm regions initialized: %i allocation blocks; block size: %i bytes\n",
    	pmmngr_get_block_count (), pmmngr_get_block_size () );
    
    vmmngr_initialize();
    kkybrd_install();

}

void sleep(int ms) {
    static int ticks = ms + get_tick_count();
    while(ticks > get_tick_count()) 
	;
}

int getch() {
    int key = KEY_UNKNOWN;
    //! wait for a keypress
    while(key==KEY_UNKNWON)
	key = kkybrd_get_last_key();
    kkybrd_discard_last_key();   // reset _scancode to KEY_UNKNOWN
    return key;

}

void cmd() {
    debugPrintf("\nCommand> ");
}

void get_cmd(char* buf, int n) {
    cmd();
    int key = KEY_UNKNOWN;
    int i = 0;
    int bufChar = 1;
    while(i < n) {
	bufChar = 1;
	key = getch();
	if(key==KEY_ENTER) break;
	if(key==KEY_BACKSPACE) {
	    bufChar = 0;
	    if(i>0) {
		unsigned x, y;
		debugGetXY(&x, &y);
		debugGotoXY(--x, y);
		debugPutc(' ');
		debugGotoXY(x, y);
		i--;
	    }
	}
	
	if(bufChar) {
	    char c = kkybrd_key_to_ascii(key);
	    if(c!=0) {
		debugPutc(c);
		buf[i++] = c;
	    }
	}
	sleep(10);

    }

    buf[i] = '\0';


}

bool run_cmd(char* cmd_buf) {
    if(strcmp(cmd_buf, "exit")==0) 
	return true;
    else if(strcmp(cmd_buf, "cls")==0)
	debugClrScr(0x17);
    else if(strcmp(cmd_buf, "help")==0) {
	debugPuts ("\nOS Development Series Keyboard Programming Demo");
	debugPuts ("\nwith a basic Command Line Interface (CLI)\n\n");
	debugPuts ("Supported commands:\n");
	debugPuts (" - exit: quits and halts the system\n");
	debugPuts (" - cls: clears the display\n");
	debugPuts (" - help: displays this message\n");
    }
    else
	debugPuts("\nUnknown command");

    return false;
}

void run() {
    char cmd_buf[100];
    while(1) {
	get_cmd(cmd_buf, 98);
	if(run_cmd(cmd_buf)==true)
	    break;
    }
}


int main(multiboot_info *bootinfo) {
    init(bootinfo);
	debugGotoXY (0,0);
	debugPuts ("OSDev Series Keyboard Demo");
	debugPuts ("\nType \"exit\" to quit, \"help\" for a list of commands\n");
	debugPuts ("+-------------------------------------------------------+\n");

	run ();

	debugPrintf ("\nExit command recieved; demo halted");
	for (;;);
	return 0;

}
