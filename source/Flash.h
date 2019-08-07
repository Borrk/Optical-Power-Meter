/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    Uart.h
*   Author:
*   Description:    Uart API declaration. 
*   
*   Author:  Huang Yu Qi
*   Date: 07/21/2012
*
******************************************************************************/

#ifndef FLASH_H
#define FLASH_H

#include "Typedefs.h"

#define FLASH_ADDR 0x200

/// Application APIs
void Flash_Eraser();
void Flash_Write(unsigned int DataLength,unsigned char xdata *pData);
void Flash_Read(unsigned int DataLength,unsigned char xdata *pData);
//void Flash_WriteMulti(char count, unsigned int xdata *pDataLength,unsigned char xdata **pData);
//void Flash_ReadMulti(char count, unsigned int xdata *pDataLength,unsigned char xdata **pData);


#endif  // end of FLASH_H