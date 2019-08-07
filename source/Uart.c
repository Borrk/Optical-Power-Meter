/*****************************************************************************
* 
*   Copyright(C) 上海福天科技有限公司 2012
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
#include <string.h>
#include <math.h>
#include "C8051F350.h"
#include "Uart.h"
#include "Task.h"

#include "Gdata.h"


// UART Buffer Defines
//unsigned char DataSeg UART0_Tx_Buff[UART0_BUF_LENGTH];		//暂存要发送的数据包

void  UartInitHW()
{
	XBR0 |= 0x01; // Enable UART
	P0SKIP &= 0xcf; 
	//SCON0 = 0x10;  // 8-bit ,MCE0=0,REN0=1,
	SCON0 = 0x00;  // 8-bit ,MCE0=0,REN0=0. only send data.
	ES0   = 1;  // Enable UART0 INT
}

void 	InitUart( )
{
	ResetUartParam();
}

void ResetUartParam()
{	
	UART0_Tx_Flag		= 0;
	UART0_Tx_Count		= 0;
	UART0_Tx_Bytes		= 0;
}

void short_delay(unsigned int z)//延时程序
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=120;y>0;y--);
} 

void UART0_Tx_Start(void)			// 启动1包数据的发送
{
	UART0_Tx_Flag 	= 1;			// 有数据要发送
	UART0_Tx_Count 	= 0;
	UART0_Tx_Bytes--;			//-1;
	
	ES0 = 0;
	short_delay(20);
	ES0 = 1;

	SBUF0 = UART0_Tx_Buff[0];		//发送第1个字节
}


void	UART_CmdProcess()
{
	if(UART0_Tx_Flag==1) 
		return;	//防止上一包数据未发送完就发送下一包
	
	UART0_Tx_Start();
}


void UART_ISR(void) interrupt 4
{
	if( TI0 ==1 )							// send data
	{
		if (UART0_Tx_Count < UART0_Tx_Bytes)	// 有数据要发送
		{
			UART0_Tx_Count++;
			SBUF0 = UART0_Tx_Buff[UART0_Tx_Count];
		}
		else										// 1包数据发送完成,结束
		{
			UART0_Tx_Count	= UART0_Tx_Bytes = UART0_Tx_Flag	= 0;

			short_delay(40);
		}

		TI0 = 0;
	}

	if( RI0 == 1 )								// receive data
	{
		RI0 = 0;
	}
}
