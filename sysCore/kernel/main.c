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
//    geninterrupt(32);

//! clear and init display
	debugClrScr (0x13);
	debugGotoXY (0,0);
	debugSetColor (0x19);

	//! render text and logo
	debugPuts (logo);
	debugSetColor (0x17);
	debugPuts ("Weee.... The PIC, PIT, and exception handlers are installed!\n\n");
	debugPuts ("Hitting any key will fire the default HAL handlers as we do\n");
	debugPuts ("not have a keyboard driver yet to install one.\n\n");
	debugPrintf ("Cpu vender: %s \n\n", get_cpu_vender ());

	// Go into a loop--constantly display the current tick count
	for(;;) {
		debugGotoXY (0,14);
//		debugPrintf ("Current tick count: i");
		debugPrintf ("Current tick count: %i", get_tick_count());
	}

    return 0;

}
