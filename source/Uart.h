/*****************************************************************************
* 
*   Copyright(C) 上海福天科技有限公司 2012
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

#ifndef UART_H
#define UART_H

#include "Typedefs.h"

#define BAUD_9600  0x96
#define BAUD_19200  0xcb

#define DataSeg idata


/// HW APIs
void  UartInitHW();
void  InitUart( );
void  ResetUartParam();

/// Application APIs
void	UART_CmdProcess();


#endif  // end of UART_H