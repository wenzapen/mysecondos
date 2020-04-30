#include "debugDisplay.h"
#include <string.h>
#include <stdarg.h>

#define VID_MEMORY 0xB8000
static unsigned int _xPos=0, _yPos=0;
static unsigned _startX=0, _startY=0;
static unsigned _color=0;

void debugPutc(unsigned char c) {
    if(c==0) 
	return;
    if(c=='\n' || c=='\r') {
	_yPos++;
	_xPos = _startX;
	return;
    }
    if(_xPos > 79) {
	_yPos++;
	_xPos = _startX;
	return;
    }
    unsigned char* p = (unsigned char*)VID_MEMORY + 2*80*_yPos + (2*_xPos++);
    *p++ = c;
    *p = _color;

}

char tbuf[32];
char bchars[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

//convert unsigned integer to string, store in buf
void itoa(unsigned i, unsigned base, char *buf) {
    int pos=0;
    int opos=0;
    int top=0;
    if(i==0 || base>16) {
	*buf++ = '0';
	*buf = '\0';
	return;
    }
    while(i!=0) {
	tbuf[pos++] = bchars[i%base];
	i /= base;	
    }
    top = pos--;
    for(opos=0; opos<top; opos++,pos--) {
	buf[opos] = tbuf[pos];
    }
    buf[opos] = 0;

}

void itoa_s(int i, unsigned base, char* buf) {
    if(base > 16) return;
    if(i<0) {
	i *= -1;
	*buf++ = '-';
    }
    itoa(i,base,buf);
}

unsigned debugSetColor(const unsigned c) {
    unsigned t=_color;
    _color=c;
    return t;
}

void debugGotoXY(unsigned x, unsigned y) {
    _xPos = x*2;
    _yPos = y;
    _startX = _xPos;
    _startY = _yPos;

}

void debugClrScr(const unsigned short c) {
    unsigned char* p = (unsigned char*)VID_MEMORY;
    for(int i=0; i<2*80*25; i+=2) {
	p[i] = ' ';
	p[i+1] = c;
    }
    _xPos = _startX; _yPos = _startY;
}

void debugPuts(char *str) {
    if(!str) return;
    for(size_t i=0; i<strlen(str); i++) 
	debugPutc(str[i]);
}

int debugPrintf(const char* str, ...) {
    if(!str) return 0;
    va_list args;
    va_start(args, str);

    for(size_t i=0; i<strlen(str);i++) {
	switch(str[i]) {
	    case '%':
		switch(str[i+1]) {
		    case 'c': {
			char c = va_arg(args, char);
			debugPutc(c);
			i++;
			break;
		    }
		    case 's':

		    case 'p':

		    case 'd':
		    case 'i': {
			int c = va_arg(args, int);
			char tstr[32] = {0};
			itoa_s(c, 10, tstr);
			debugPuts(tstr);
			i++;
			break;
		    }
		    case 'x':
		    case 'X': {
			int c = va_arg(args, int);
			char tstr[32] = {0};
			itoa_s(c, 16, tstr);
			debugPuts(tstr);
			i++;
			break;
		    }

		    default:
			va_end(args);
			return 1;
		}
		break;

	    default:
		debugPutc(str[i]);
		break;
	}
    }
    va_end(args);
}
