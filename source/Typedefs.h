#ifndef TYPEDEFS_H
#define TYPEDEFS_H

/*****************************************************************************
* 
*   Copyright(C) 上海福天科技有限公司 2012
*   All rights reserved.
*
*   Module Name:    Display.h
*   Author:
*   Description:    Type definition. 
*   
*   Author:  Huang Yu Qi
*   Date: 06/22/2012
*
******************************************************************************/


#ifndef ASM_INCLUDE // assembler can't handle typedef

//-----------------------------------------------------------------------------
// Standard types for 8, 16 & 32-bit variables
//-----------------------------------------------------------------------------

typedef   signed char tINT8;
typedef unsigned char tUINT8;

typedef   signed int  tINT16;
typedef unsigned int  tUINT16;

typedef   signed long tINT32;
typedef unsigned long tUINT32;

//-----------------------------------------------------------------------------
// a byte-wide or register-wide representation of a boolean value,
// can be useful for minimizing code.
//-----------------------------------------------------------------------------

typedef enum
{
    FALSE = 0,
    TRUE = ~FALSE
} eBOOL;

//-----------------------------------------------------------------------------
// Mechanism for fractionally accessing parts of larger words
// (we currently have a big-endian processor)
//-----------------------------------------------------------------------------

typedef struct
{
    tUINT8  hi;
    tUINT8  lo;
} tUBYTES;

typedef union
{
    tUINT16  word;
    tUBYTES  byte;
} uUCOMBO16;

typedef struct
{
    uUCOMBO16  hi;
    uUCOMBO16  lo;
} tUHALVES;

typedef union
{
    tUINT32  lword;
    tUHALVES  half;
    tUINT8  byte[sizeof(tUINT32)];
} uUCOMBO32;

typedef struct
{
    tINT8  hi;
    tUINT8  lo;
} tSBYTES;

typedef union
{
    tINT16  word;
    tSBYTES  byte;
} uSCOMBO16;

typedef struct
{
    uSCOMBO16  hi;
    uUCOMBO16  lo;
} tSHALVES;

typedef union
{
    tINT32  lword;
    tSHALVES  half;
} uSCOMBO32;

//-----------------------------------------------------------------------------
// We have a special shortcut for portable optimization, representing
// "whatever is most efficient for this processor".
// Be aware that this type can change between signed and unsigned,
// and can vary from 8-bit wide up. This type is typically useful for
// function return values, indexing, loop counters etc.
//-----------------------------------------------------------------------------

typedef  tUINT8  tMP_INT; // for 8051 type processors (using Keil compiler)



#ifndef NULL
#define NULL    0
#endif
  #endif // ndef ASM_INCLUDE  assembler can't handle typedef

#endif // ndef TYPEDEFS_H