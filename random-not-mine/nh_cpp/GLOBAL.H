#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream.h>
#include "color.h"
#include <strings.h>

#define uchar unsigned char             // 8 bits unsigned  0 - 255
#define schar signed char               // 8 bits signed    -128 - 127
#define ushort unsigned short int       // 16 bits unsigned 0 - 65536
#define sshort short int                // 16 bits signed   -32768 - 32767
#define uint unsigned int               // 32 bits unsigned ~4G
#define sint signed int                 // 32 bit signed    ~-2G - 2G
// Long int it still 32 bits...

#define Bitfields(n,s) uchar n:s
#define TRUE 1
#define FALSE 0

#include "random.h"
#include "display.h"


#include "event.h"
static eventlist timer;

#endif
