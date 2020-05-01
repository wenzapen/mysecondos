#include "debugDisplay.h"
#include <hal.h>

char* logo =
"\
    __  _______  _____\n\
   /  |/  / __ \\/ ___/\n\
  / /|_/ / / / /\\__ \\ Microcomputer Operating System \n\
 / /  / / /_/ /___/ / -------------------------------\n\
/_/  /_/\\____//____/  \n\n";

int main() {


    hal_initialize();

//! clear and init display
	DebugClrScr (0x13);
	DebugGotoXY (0,0);
	DebugSetColor (0x19);

	//! render text and logo
	DebugPuts (logo);
	DebugSetColor (0x17);
	DebugPuts ("Weee.... The PIC, PIT, and exception handlers are installed!\n\n");
	DebugPuts ("Hitting any key will fire the default HAL handlers as we do\n");
	DebugPuts ("not have a keyboard driver yet to install one.\n\n");
	DebugPrintf ("Cpu vender: %s \n\n", get_cpu_vender ());

	// Go into a loop--constantly display the current tick count
	for(;;) {
		DebugGotoXY (0,14);
		DebugPrintf ("Current tick count: %i", get_tick_count());
	}

    return 0;

}
