#include "debugDisplay.h"

void main() {


    int i = 0x12;
    debugClrScr(0x18);
    debugGotoXY(4,4);
    debugSetColor(0x17);
    debugPrintf("+------------------------------------+\n");
    debugPrintf("|   MOS 32 Bit C Kernel Excuting!    |\n");
    debugPrintf("+------------------------------------+\n\n");
    debugSetColor(0x12);


    debugSetColor(0x12);
    debugPrintf("\ni as integer ................:");
    debugPrintf("\ni in hex ....................:");

    debugGotoXY(25,8);
    debugSetColor(0x1F);
    debugPrintf("\n[%i]", i);
    debugPrintf("\n[0x%x]", i);

    debugGotoXY(4,16);
    debugSetColor(0x1F);
    debugPrintf("\n\nI am preparing to load... Hold on, please... :)");

}
