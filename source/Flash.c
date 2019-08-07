/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    Uart.c
*   Author:
*   Description:    Uart API. 
*   
*   Author:  Huang Yu Qi
*   Date: 07/21/2012
*
******************************************************************************/

#include <intrins.h>
#include "C8051F350.h"
#include "Flash.h"

//unsigned char code gCalibrationParam[512] _at_ FLASH_ADDR;

void Flash_Eraser()
{  //只擦除DataAddr所在扇区的数据，实际应当根据用的数据量来擦除需要的扇区。
    bit SaveEA;
    SaveEA = EA;   
    EA = 0;
    PSCTL = 0x03;//写允许、擦出允许
    FLKEY = 0xa5;//写关键字
    FLKEY = 0xf1;
    //写入任意数据，flash将擦除该页512字节
    (*((unsigned char xdata*) FLASH_ADDR)) = 0xff;
    PSCTL = 0x00;//禁止写和擦除

	EA = SaveEA;
}
#if 0
void Flash_Write(unsigned int DataLength,unsigned char xdata *pData)
{
    bit SaveEA;
	unsigned int DataAddr = FLASH_ADDR;
    SaveEA = EA;   
    EA = 0;
    PSCTL = 0x01;//写允许
    while(DataLength--)
    {
        FLKEY = 0xa5;//写关键字
        FLKEY = 0xf1;
        (*((unsigned char xdata*)(DataAddr++))) = (*(pData++));
    }
    PSCTL = 0x00;//禁止写和擦除
    EA = SaveEA;
}

void Flash_Read(unsigned int DataLength,unsigned char xdata *pData)
{
    bit SaveEA;
	unsigned int DataAddr = FLASH_ADDR;
    SaveEA = EA;   
    EA = 0;             
    while(DataLength--)
    {
        (*(pData++)) = (*((unsigned char  code*)(DataAddr++)));
    }
    EA = SaveEA;
}

void Flash_WriteMulti(char count, unsigned int xdata *pDataLength,unsigned char xdata **pData)
{
	unsigned char i;
	unsigned int  aLen;
	unsigned char *apData;
	unsigned int DataAddr = FLASH_ADDR;

    bit SaveEA;

    SaveEA = EA;   
    EA = 0;

    PSCTL = 0x01;//写允许
	for ( i = 0; i < count; i++ )          
   {
   		aLen = pDataLength[i];
		apData = pData[i];

	    while(aLen--)
	    {
	        FLKEY = 0xa5;//写关键字
	        FLKEY = 0xf1;
	        (*((unsigned char xdata*)(DataAddr++))) = (*(apData++));
	    }
	}

    PSCTL = 0x00;//禁止写和擦除
    EA = SaveEA;
}

void Flash_ReadMulti(char count, unsigned int xdata *pDataLength,unsigned char xdata **pData)
{
	unsigned char i;
	unsigned int  aLen;
	unsigned char *apData;
	unsigned int DataAddr = FLASH_ADDR;
    bit SaveEA;
   
    SaveEA = EA;   
    EA = 0;   
	
	for ( i = 0; i < count; i++ )          
   {
   		aLen = pDataLength[i];
		apData = pData[i];
	    while(aLen--)
	    {
	        (*(apData++)) = (*((unsigned char  code*)(DataAddr++)));
	    }
	}

    EA = SaveEA;
}
#endif

#if 0
void FLASH_ByteWrite (FLADDR addr, charbyte);
unsigned char FLASH_ByteRead (FLADDR addr);
void FLASH_PageErase (FLADDR addr);

//-----------------------------------------------------------------------------
// FLASH_ByteWrite
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 
//  1)  FLADDR addr - target addressto write to
//                     range is 0 to(FLASH_TEMP-1)
//  2)  char byte   - byte to write
//
// This routine writes <byte> to thelinear FLASH address <addr>.
//
//-----------------------------------------------------------------------------
void FLASH_ByteWrite (FLADDR addr, charbyte)
{
  bit EA_SAVE = EA;                  // Preserve EA
  char xdata * data pwrite;          // FLASH write pointer

   EA= 0;                             //Disable interrupts
   //change clock speed to slow, then restore later
  VDM0CN = 0x80;                     // Enable VDD monitor
  RSTSRC = 0x02;                     // Enable VDD monitor as a reset source
  pwrite = (char xdata *) addr;

  FLKEY  = 0xA5;                      // Key Sequence 1
  FLKEY  = 0xF1;                      // Key Sequence 2
  PSCTL |= 0x01;                     // PSWE = 1

  VDM0CN = 0x80;                     // Enable VDD monitor
  RSTSRC = 0x02;                     // Enable VDD monitor as a reset source

  *pwrite = byte;                    // Write the byte
  PSCTL &= ~0x01;                    // PSWE = 0
  
  EA= EA_SAVE;                       //Restore interrupts
}
 
//-----------------------------------------------------------------------------
// FLASH_ByteRead
//-----------------------------------------------------------------------------
//
// Return Value : 
//  1)  unsigned char  - byte that was read from Flash
// Parameters   : 
//  1)  FLADDR addr    - target address to write to
//                       range is 0 to(FLASH_TEMP-1)
//
// This routine reads a <byte> fromthe linear FLASH address <addr>.
//
//-----------------------------------------------------------------------------
unsigned char FLASH_ByteRead (FLADDR addr)
{
  bit EA_SAVE = EA;                  // Preserve EA
  char code * data pread;            // FLASH read pointer
  unsigned char byte;

  EA= 0;                             //Disable interrupts

  pread = (char code *) addr;
  byte = *pread;                     // Read the byte
  EA= EA_SAVE;                       //Restore interrupts
  return byte;
}

 

//-----------------------------------------------------------------------------
// FLASH_PageErase
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 
//  1)  FLADDR addr    - target address to write to
//                        range is 0 to(FLASH_TEMP-1)
//
// This routine erases the FLASH page containingthe linear FLASH address
// <addr>.
//
//-----------------------------------------------------------------------------
void FLASH_PageErase (FLADDR addr)
{
  bit EA_SAVE = EA;                  // Preserve EA
  char xdata * data pwrite;           // FLASH write pointer
   EA= 0;                             //Disable interrupts
   //change clock speed to slow, then restore later
  VDM0CN = 0x80;                     // Enable VDD monitor
  RSTSRC = 0x02;                     // enable VDD monitor as a reset source
  pwrite = (char xdata *) addr;
  FLKEY  = 0xA5;                      // Key Sequence 1
  FLKEY  = 0xF1;                      // Key Sequence 2
  PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1
  VDM0CN = 0x80;                     // Enable VDD monitor
  RSTSRC = 0x02;                     // Enable VDD monitor as a reset source
  *pwrite = 0;                       // Initiate page erase
  PSCTL &= ~0x03;                    // PSWE = 0; PSEE = 0
  EA= EA_SAVE;                       //Restore interrupts
}

#endif
