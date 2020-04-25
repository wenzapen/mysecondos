#ifndef _DEBUG_DISPLAY_H
#define _DEBUG_DISPLAY_H


void debugClrScr(const unsigned short c);
void debugPuts(char* str);
int debugPrintf(const char* str,...);
unsigned debugSetColor(const unsigned c);
void debugGotoXY(unsigned x, unsigned y);


#endif
