#include "debugDisplay.h"
#include <hal.h>

int main() {


    debugClrScr(0x18);
    debugGotoXY(0,0);
    debugSetColor(0x70);
    debugPrintf(" Microcomputer Operating System(MOS) Preparing to load...   ");
    debugGotoXY(0,1);
    debugSetColor(0x19);
    debugPrintf(" MOS Starting Up...\n");

    debugSetColor(0x70);
    debugGotoXY(0,4);
    debugPrintf(" Initializing Hardware Abstraction Layer (HAL.lib)...       ");

    debugSetColor(0x19);
    debugGotoXY(0,2);
    hal_initialize();
    while(1);
    geninterrupt(0x15);
    return 0;

}
