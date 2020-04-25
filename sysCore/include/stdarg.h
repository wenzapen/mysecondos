#ifndef _STDARG_H
#define _STDARG_H

#include <va_list.h>

#define STACKITEM int

//round up width of objects(parameters) pushed on stack
#define VA_SIZE(TYPE) ((sizeof(TYPE)+sizeof(STACKITEM)-1)&~(sizeof(STACKITEM)-1))

//&LASTARG points to the last fixed parameter
//AP points to the first variable parameter
#define va_start(AP,LASTARG) (AP=((va_list)&(LASTARG)+VA_SIZE(LASTARG)))

#define va_end(AP)

#define va_arg(AP,TYPE) ((AP+=VA_SIZE(TYPE)),*((TYPE *)(AP-VA_SIZE(TYPE))))

#endif
